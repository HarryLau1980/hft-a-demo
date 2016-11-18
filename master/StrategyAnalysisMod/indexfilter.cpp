#include "indexfilter.h"
#include <QMap>

IndexFilter::IndexFilter(Eigen::MatrixXd *kernel, int winSize)
    : mKernel(kernel)
    , mWinSize(winSize)
    , mHalfWinSize(winSize/2)
    , mTmpBuff(NULL)
{
    mTmpBuff = new double[mWinSize];
    reset();
}

IndexFilter::~IndexFilter()
{
    if(mTmpBuff) {
        delete[] mTmpBuff;
    }
}

bool IndexFilter::appendSignal(double v)
{
    if(mSigCount >= mWinSize) {
        mSigWin.removeFirst();
        mSigWin.append(v);
        ++ mSigCount;
        return true;
    } else if(mSigCount >= mHalfWinSize) {
        mSigWin[mSigCount] = v;
        ++ mSigCount;
        return true;
    } else {
        // mSigCount < mHalfWinSize;
        mSigWin[mSigCount] = v;
        ++ mSigCount;
        return (mSigCount >= mHalfWinSize);
    }
    return false;
}

void IndexFilter::reset()
{
    mCurFltrVal = 0;
    mCurSigVal = 0;
    mSigCount = 0;
    mOutputCount = 0;
    mSigWin = QVector<double>(mWinSize);
    memset(mTmpBuff, 0, sizeof(double)*mWinSize);
}

double IndexFilter::CalcFilteredValue(double* vect, int cSamples)
{
    if(vect == NULL)
        return 0.0;

    Eigen::VectorXd matRes = (*mKernel)*((Eigen::Map<Eigen::VectorXd>(vect, mWinSize)));
    double ret = 0;
    for( int i = 0; i < mWinSize; i ++ ) {
        ret += matRes[i];
    }
    ret /= cSamples;
    return ret;
}

IndexFilter *IndexFilter::createSmoothFilter(int matSize)
{
    return new IndexFilter(getSmoothKernel(matSize), matSize);
}

Eigen::MatrixXd *IndexFilter::getSmoothKernel(int MatSize)
{
    static QMap<int, Eigen::MatrixXd*> FILTER_KERNELS;
    QMap<int, Eigen::MatrixXd*>::iterator itr;
    if(MatSize < 4)
        return NULL;

    itr = FILTER_KERNELS.find(MatSize);
    if(itr != FILTER_KERNELS.end()) {
        return itr.value();
    }

    Eigen::MatrixXd matFilter(MatSize, MatSize);
    Eigen::MatrixXd identity(MatSize, MatSize);
    // Eigen::MatrixXf inverseMatrix(matrixSize, matrixSize);
    identity = Eigen::MatrixXd::Identity(MatSize, MatSize);
    matFilter = Eigen::MatrixXd::Zero(MatSize, MatSize);

    for( int i = 0; i < MatSize; i ++ ) {
        if(i == 0 ) {
            matFilter(0, 0) = 1;
            matFilter(0, 1) = -2;
            matFilter(0, 2) = 1;
        } else if(i == 1) {
            matFilter(1, 0) = -2;
            matFilter(1, 1) = 5;
            matFilter(1, 2) = -4;
            matFilter(1, 3) = 1;
        } else if( i == MatSize - 2) {
            matFilter(i, MatSize - 1) = -2;
            matFilter(i, MatSize - 2) = 5;
            matFilter(i, MatSize - 3) = -4;
            matFilter(i, MatSize - 4) = 1;
        } else if( i == MatSize - 1) {
            matFilter(i, MatSize - 1) = 1;
            matFilter(i, MatSize - 2) = -2;
            matFilter(i, MatSize - 3) = 1;
        } else {
            matFilter(i, i - 2) = 1;
            matFilter(i, i - 2 + 1) = -4;
            matFilter(i, i - 2 + 2) = 6;
            matFilter(i, i - 2 + 3) = -4;
            matFilter(i, i - 2 + 4) = 1;
        }
    }

    Eigen::MatrixXd *Filter = new Eigen::MatrixXd(MatSize, MatSize);
    *Filter = (identity + 100000*matFilter).inverse();
    FILTER_KERNELS[MatSize] = Filter;
    return Filter;
}

bool IndexFilter::staticSignal(double v, double *out, int *outCounter)
{
    double* vect = NULL;
    if(out) {
        *out = 0;
    }
    if(outCounter) {
        *outCounter = 0;
    }

    if(!appendSignal(v))
        return false;

    if(mSigCount < mWinSize) {
        memcpy(&(mTmpBuff[mWinSize - 1 - mSigCount]), mSigWin.data(), sizeof(double)*mSigCount);
        vect = mTmpBuff;
    } else {
        // mSigCount >= mWinSize
        vect = mSigWin.data();
    }

    mCurFltrVal = CalcFilteredValue(vect, (mSigCount < mWinSize) ? mSigCount : mWinSize);
    ++ mOutputCount;

    if(outCounter) {
        *outCounter = mOutputCount;
    }
    if(out) {
        *out = mCurFltrVal;
    }

    return true;
}
void IndexFilter::signalFixTail(QVector<double> &entireFltrdSig, bool updateOrAppend)
{
    if(mSigCount < mWinSize)
        return;

    int nSize = entireFltrdSig.size();
    double expand = 0;
    for( register int i = 1; i <= mHalfWinSize; i ++ ) {
        memcpy(&mTmpBuff[0], mSigWin.data(), sizeof(double)*(mWinSize-i));
        expand = mSigWin.last();
        for( register int x = 0; x < i; x ++ ) {
            mTmpBuff[mWinSize - i + x] = 0;
        }
        // memset(&mTmpBuff[mWinSize - i], expand, sizeof(double)*i);

        mCurFltrVal = CalcFilteredValue(mTmpBuff, mWinSize - i);
        if(updateOrAppend == false) {
            ++ mOutputCount;
            entireFltrdSig.append(mCurFltrVal);
        } else {
            // update
            entireFltrdSig[nSize - 1 - (mHalfWinSize - i)] = mCurFltrVal;
        }
    }
}
void IndexFilter::staticSignalFixTail(QVector<double> &entireFltrdSig)
{
    signalFixTail(entireFltrdSig, false);
}

void IndexFilter::staticSignal(const QVector<double> &sig, QVector<double> &out)
{
    int siz = sig.size();
    bool ok;
    double fout;
    int cCount;
    for ( int i = 0; i < siz; i ++ ) {
        ok = staticSignal(sig[i], &fout, &cCount);
        if(ok) {
            out.append(fout);
        }
    }
    staticSignalFixTail(out);
}

bool IndexFilter::dynamicSignal(double v, QVector<double> &entireFltredSig)
{
    if(!appendSignal(v))
        return false;

    double* vect;
    if(mSigCount < mWinSize) {
        memcpy(&(mTmpBuff[mWinSize - 1 - mSigCount]), mSigWin.data(), sizeof(double)*mSigCount);
        vect = mTmpBuff;
    } else {
        // mSigCount >= mWinSize
        vect = mSigWin.data();
    }

    mCurFltrVal = CalcFilteredValue(vect, (mSigCount < mWinSize) ? mSigCount : mWinSize);
    ++ mOutputCount;

    if(mSigCount > mWinSize) {
        entireFltredSig[entireFltredSig.size() - 1 - (mHalfWinSize-1)] = mCurFltrVal;
        entireFltredSig.append(0);
        signalFixTail(entireFltredSig, true);
    } else if(mSigCount < mWinSize) {
        entireFltredSig.append(mCurFltrVal);
    } else {
        // if(mSigCount == mWinSize)
        entireFltredSig.append(mCurFltrVal);
        signalFixTail(entireFltredSig, false);
    }
    return true;
}

