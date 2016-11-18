#ifndef INDEXFILTER_H
#define INDEXFILTER_H

#include <QVector>
#include <QDateTime>
#include "../Eigen/Dense"

/* signal seq          o----------------------------------o
 * filter window   o___^___o                          o___^___o
 *                 1st output                         last output
 * in real-time Mode, when the latest signal arrived[call IndexFilter.realTimeSigIn(..)],
 * size of (WinSize/2 + 1) tail sequence will be updated.
 * WinSize/2 elements of tail-updated-seq are old updated value, and the 1 new generated filtered-value.
 *
 * in static Mode, after calling IndexFilter.historicalSigIn(...), please call historicalTailOut(...);
 *  a size of (WinSize/2) tail seq will return.
 */
class IndexFilter
{
public:
    IndexFilter(Eigen::MatrixXd* kernel, int winSize/*size of filter, winSize X winSize Matrix*/);
    ~IndexFilter();
private:
    int  mWinSize;
    int  mHalfWinSize;
    QVector<double> mSigWin; // signal under the filter window

    Eigen::MatrixXd* mKernel;
    double mCurSigVal;
    double mCurFltrVal; // filtered value
    int mSigCount;
    int mOutputCount;
    double* mTmpBuff;

private:
    double CalcFilteredValue(double* vect, int cSamples);
    // updateOrAppend: true, update; false, append
    void   signalFixTail(QVector<double>& entrieFltredSig, bool updateOrAppend);

public:
    static IndexFilter* createSmoothFilter(int matSize);
    static Eigen::MatrixXd* getSmoothKernel(int matSize);

    int  winSize() const { return mWinSize; }

    void reset();

    // append without filter calculations
    bool   appendSignal(double v);

    // static signal in.
    // true: has output; false: dosen't have any output
    bool staticSignal(double v, double *out, int* outCounter);
    void staticSignalFixTail(QVector<double>& fltredSig);

    void staticSignal(const QVector<double>& sig, QVector<double>& out);

    // dynamical signal in.
    // true if there is output, otherwise false return.
    bool dynamicSignal(double v, QVector<double>& entireFltredSig);

    double out() const { return mCurFltrVal; }
    double in() const { return mCurSigVal; }
    int  outCount() const { return mOutputCount; }
};

#endif // INDEXFILTER_H
