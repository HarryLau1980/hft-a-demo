#include "deltastrategy.h"
#include "../indexfilter.h"
#include "../localindexstream.h"
#include "../../utils/common.h"
#include "../../DataAccInterf/dataaccif.h"
#include "../../PlotMod/graphcurve.h"
#include "hftmath.h"
#include <QDebug>

// ------------------- DeltaStrategy -----------------
#define HISTORY_INDEX_COUNT 600
DeltaStrategy::DeltaStrategy()
    : StrategyAnalyzer()
    , mIndexCache(NULL)
{
    mName = "DeltaStrategy";
    const int matSize = 20;
    mFltrSB1 = IndexFilter::createSmoothFilter(matSize);
    mFltrSB2 = IndexFilter::createSmoothFilter(matSize);

    mSeqSet.addSeq("SBR_A", 0, true);
    mSeqSet.addSeq("SBR_B", 0, true);
    mSeqSet.addSeq("SBR_AVG_A", 0, true, 2);
    mSeqSet.addSeq("SBR_AVG_B", 0, true, 2);
    mSeqSet.seq("SBR_AVG_A")->setFltCurve();
    mSeqSet.seq("SBR_AVG_B")->setFltCurve();
    mSeqSet.addSeq("OMEGA_A");
    mSeqSet.addSeq("OMEGA_B");

    mSeqSetDelta.addSeq("DELTA_A", HISTORY_INDEX_COUNT, true, 1);
    mSeqSetDelta.addSeq("DELTA_B", HISTORY_INDEX_COUNT, true, 1);
    mSeqSetDelta.addSeq("DELTA_AVG_A", HISTORY_INDEX_COUNT, true, 1);
    mSeqSetDelta.addSeq("DELTA_AVG_B", HISTORY_INDEX_COUNT, true, 1);
    mArgA.mDeltaSeq = &(mSeqSetDelta.seq("DELTA_A")->data());
    mArgB.mDeltaSeq = &(mSeqSetDelta.seq("DELTA_B")->data());
    mSeqSetDelta.addSeq("DELTA_A_UPER", HISTORY_INDEX_COUNT, true, 1, Qt::SolidLine );
    mSeqSetDelta.addSeq("DELTA_A_LOWER", HISTORY_INDEX_COUNT, true, 2, Qt::SolidLine);
    mSeqSetDelta.addSeq("DELTA_B_UPER", HISTORY_INDEX_COUNT, true, 1, Qt::SolidLine );
    mSeqSetDelta.addSeq("DELTA_B_LOWER", HISTORY_INDEX_COUNT, true, 2, Qt::SolidLine);

    mCurveWin = new GraphCurveWindow("s/b curve");
    mCurveDelta = new GraphCurveWindow("Delta Curve");
    mArbitrager = new Arbitrager(400000.0);

    mIndexCache = new FinanceIndexCache(DEF_DBFILE);

    QString sql;
    sql = QString("SELECT * FROM icdelta_persec_fltr WHERE `DateTime`>='%1' AND `DateTime`<='%2'")
            .arg(DATETIME2STR(STG_TEST_TIME[0])).arg(DATETIME2STR(STG_TEST_TIME[2]));
    mIndexCache->setFromDbQuerySql(sql);
    sql = QString("INSERT INTO icdelta_persec_fltr(DateTime, U40_0, U40_1) VALUES(?,?,?)");
    mIndexCache->setIntoDbQuerySql(sql);
    if(!mIndexCache->open()) {
        qWarning() << "failed open indexCache from database";
    } else {
        mIndexCache->transaction();
    }
}

DeltaStrategy::~DeltaStrategy()
{
    if(mCurveWin) {
        delete mCurveWin;
    }
    if(mCurveDelta) {
        delete mCurveDelta;
    }
    if(mArbitrager) {
        delete mArbitrager;
    }
}

int DeltaStrategy::incomingIndexTicks(const QDateTime &timestamp, const QVector<IndexTick> &ticks)
{
    mTimeLine.append(timestamp);
    double s1, b1, s2, b2, sb1, sb2, vout1, vout2;
    s1 = ticks[0].index().at(1);
    b1 = ticks[0].index().at(0);
    s2 = ticks[1].index().at(1);
    b2 = ticks[1].index().at(0);

    sb1 = (s1/b2);
    sb2 = (b1/s2);

    mSeqSet.appendSeq("SBR_A", sb1);
    mSeqSet.appendSeq("SBR_B", sb2);

    if( mode() == SM_HISTORICAL) {
        QVector<double> v;
        if(0) {//mIndexCache->cache(FROM_MEM, timestamp, v)) {
            // it is neccessary to append signal to the filter system,
            // or the filter system won't works well later after.
            mFltrSB1->appendSignal(sb1);
            mFltrSB2->appendSignal(sb2);

            mSeqSet.appendSeq("SBR_AVG_A", v[0]);
            mSeqSet.appendSeq("SBR_AVG_B", v[1]);
        } else {
            bool ok = false;
            if(mFltrSB1->staticSignal(sb1, &vout1, NULL)) {
                mSeqSet.appendSeq("SBR_AVG_A", vout1);
                ok = true;
            }
            if(mFltrSB2->staticSignal(sb2, &vout2, NULL)) {
                mSeqSet.appendSeq("SBR_AVG_B", vout2);
            } else {
                ok = false;
            }
            if(ok) {
                v.append(vout1);
                v.append(vout2);
                // mIndexCache->cache(INTO_DB, timestamp, v);
            }
        }
    } else {
        mFltrSB1->dynamicSignal(sb1, mSeqSet.seqData("SBR_AVG_A"));
        mFltrSB2->dynamicSignal(sb2, mSeqSet.seqData("SBR_AVG_B"));

        mArgA.mSymbol = ticks[0].symbol();
        mArgA.mSellPriz = s1;
        mArgA.mBuyPriz = b1;

        mArgB.mSymbol = ticks[1].symbol();
        mArgB.mSellPriz = s2;
        mArgB.mBuyPriz = b2;

        makeTradeDecision(timestamp);
        DrawCurvesSBR(true);
    }

    StrategyAnalyzer::incomingIndexTicks(timestamp, ticks);
    return 0; // no error
}

void DeltaStrategy::indexStreamError(IndexStream* stream)
{
    qDebug() << "Stream error: " << stream->symbol();
}

void DeltaStrategy::DrawCurvesSBR(bool update)
{
    if(!update) {
        Range rge(0.99, 1.0);
        mCurveWin->addCurveFamily(CURVE_AXIS(XBOTTOM, YLEFT), &mSeqSet, &rge);
        mCurveWin->show();
    } else {
        mCurveWin->updateRealtimeFltCurveFamily(&mSeqSet, mFltrSB1->winSize());
    }
}

void DeltaStrategy::DrawCurvesDelta(bool update)
{
    if(!update) {
        Range rge(-0.004, +0.004);
        mCurveDelta->addCurveFamily(CURVE_AXIS(XBOTTOM, YLEFT), &mSeqSetDelta, &rge);
        mCurveDelta->show();
    } else {
        mCurveDelta->updateHeartbeatCurveFamily(&mSeqSetDelta);
    }
}

void DeltaStrategy::enterLiveMode()
{
    setMode(SM_LIVE_MODE);
    QList<IndexStream*> streams = mSynchronizer->indexStreams();
    for( int i = 0; i < streams.size(); i ++ ) {
        QDateTime startTime = STG_TEST_TIME[1].addSecs(1);
        QDateTime endTime = STG_TEST_TIME[2];
        streamSetQuery((LocalIndexStream*)streams[i], startTime, endTime);
        streams[i]->setSleep(50);
    }
    mIndexCache->commit();
    mIndexCache->transaction();
    mSynchronizer->startIndexStreams();
}

#define MAX_O_SIZE 60
void DeltaStrategy::makeOArgsSeq(QVector<double> &s, QVector<double> &u, QVector<double> &o)
{
    double cur_u, exp_s, cur_o;
    const int siz = s.size();
    if(o.size() < MAX_O_SIZE) {
        o.clear();
        for( int i = 0; i < MAX_O_SIZE; i ++ ) {
            exp_s = VectorExpect(s, siz - 2*MAX_O_SIZE + 1 + i, siz - MAX_O_SIZE + i);
            cur_u = u.at(siz - MAX_O_SIZE + i);
            cur_o = fabs(cur_u - exp_s);
            o.append(cur_o);
        }
    } else {
        exp_s = VectorExpect(s, siz - MAX_O_SIZE, siz - 1);
        cur_u = u.last();
        cur_o = fabs(cur_u - exp_s);
        o.append(cur_o);
    }
}

void DeltaStrategy::makeTradeArgs(QVector<double> &s, QVector<double> &u, QVector<double> &o, ArbitrageArgs* arg)
{
    makeOArgsSeq(s, u, o);
    arg->mCurS = s.last();
    arg->mCurU = u.last();
    arg->mCurDelta = arg->mCurS  - arg->mCurU;
    arg->mCurO = o.last();

    int osiz = o.size();
    arg->mO5  = VectorExpect(o, osiz - 1 - 5, osiz - 1);
    arg->mO10 = VectorExpect(o, osiz - 1 - 10, osiz - 1);
    arg->mO30 = VectorExpect(o, osiz - 1 - 30, osiz - 1);
    arg->mO60 = VectorExpect(o, osiz - 1 - 60, osiz - 1);

    calcArbitrageArgs(s, u, arg);
}

void DeltaStrategy::calcArbitrageArgs(QVector<double> &s, QVector<double> &u, ArbitrageArgs* arg)
{
    if(s.size() != u.size()) {
        return;
    }
    QVector<double>& delta = *(arg->mDeltaSeq);
    VectorSubstract(s, u, delta);
    double ex = VectorExpect(delta);
    double stdev = VectorStdDeviation(delta, ex);
    double skew = VectorSkew(delta, ex, stdev);
    double kurt = VectorKurt(delta, ex, stdev);

    arg->mExpect = ex;
    arg->mKurt = kurt;
    arg->mSkew = skew;
    arg->mStdDev = stdev;

    int size = s.size();
    int Nle, Neq, Ngt;
    Nle = Neq = Ngt = 0;
    double d;
    double sig1, sig2;
    sig1 = sig2 = 0;
    for( int i = 0; i < size; i ++ ) {
        d = delta[i];
        if(d < ex) {
            ++ Nle;
            sig1 += (d - ex)*(d-ex);
        } else if( d > ex) {
            ++ Ngt;
            sig2 += (d-ex)*(d-ex);
        } else {
            ++ Neq;
        }
    }

    sig1 = sqrt((2*sig1)/(2*Nle + Neq));
    sig2 = sqrt((2*sig2)/(2*Ngt + Neq));

    double uper, lower;
    uper = lower = 0;

    const double const_ampl = 1.5;
    if(kurt <= 4.0) {
        uper = ex + (3*const_ampl*sig2)/kurt;
        lower = ex - (3*const_ampl*sig1)/kurt;
    } else if(kurt < 150) {
        uper = ex + (3*const_ampl*sig2)/4;
        lower= ex - (3*const_ampl*sig1)/4;
    }else {
        // kurt >= 150
        uper = ex + 0.0006;
        lower = ex - 0.0006;
    }

    if(kurt <= 4 || kurt >= 150) {
        if(uper < ex + 0.0004)
            uper = ex + 0.0004;
        if(lower > ex - 0.0004)
            lower = ex - 0.0004;
    }

    arg->mArbiLower = lower;
    arg->mArbiUper = uper;
}

void DeltaStrategy::makeTradeDecision(const QDateTime& timestamp)
{
     mSeqSet.trimSeqSize(HISTORY_INDEX_COUNT);

     int siz_s1, siz_s2, siz_u1, siz_u2;
     siz_s1 = mSeqSet.seqData("SBR_A").size();
     siz_s2 = mSeqSet.seqData("SBR_B").size();
     siz_u1 = mSeqSet.seqData("SBR_AVG_A").size();
     siz_u2 = mSeqSet.seqData("SBR_AVG_B").size();
     if( siz_s1 != HISTORY_INDEX_COUNT || siz_s1 != HISTORY_INDEX_COUNT ||
         siz_u1 != HISTORY_INDEX_COUNT || siz_u2 != HISTORY_INDEX_COUNT) {
         qCritical() << "More index required for analysis, please expand the historical period!";
         return;
     }

     makeTradeArgs(mSeqSet.seqData("SBR_A"), mSeqSet.seqData("SBR_AVG_A"), mSeqSet.seqData("OMEGA_A"), &mArgA);
     makeTradeArgs(mSeqSet.seqData("SBR_B"), mSeqSet.seqData("SBR_AVG_B"), mSeqSet.seqData("OMEGA_B"), &mArgB);

     // qDebug() << "&&&^^%%% Args, lower: " << mArgA.mArbiLower << "uper: " << mArgA.mArbiUper << " current Delta: " << mArgA.mExpect;
     mSeqSetDelta.appendSeq("DELTA_A_UPER", mArgA.mArbiUper);
     mSeqSetDelta.appendSeq("DELTA_A_LOWER", mArgA.mArbiLower);
     mSeqSetDelta.appendSeq("DELTA_AVG_A", mArgA.mExpect);
     mSeqSetDelta.appendSeq("DELTA_AVG_B", mArgB.mExpect);
     mSeqSetDelta.appendSeq("DELTA_B_UPER", mArgB.mArbiUper);
     mSeqSetDelta.appendSeq("DELTA_B_LOWER", mArgB.mArbiLower);
     mSeqSetDelta.trimSeqSize(HISTORY_INDEX_COUNT);
     DrawCurvesDelta(true);


     bool open = false;
     const int Lots = 100;
     static double c0 = 0.00035;
     static double c1 = 0.0003;
     static double c2 = 0.00025;
     static double c3 = 0.0002;
     static double c4 = 0.00015;

     if( /*mArgA.mCurO < c0 && mArgB.mCurO < c4 &&
         mArgA.mO5 < c1 && mArgB.mO5 < c4 &&
         mArgA.mO10 < c2 && mArgB.mO10 < c4 &&
         mArgA.mO30 < c3 && mArgB.mO30 < c4 &&
         mArgA.mO60 < c4 && mArgB.mO60 < c4 */
             true
     ) {
         if( mArgA.mCurDelta >= mArgA.mArbiUper && mArgA.mCurDelta >= mArgB.mArbiUper &&
             mArgB.mCurDelta >= mArgA.mArbiUper && mArgB.mCurDelta >= mArgB.mArbiUper ) {
             if(mArbitrager->contractsCount() <= 0 ) {
                 mArbitrager->openPosition(mArgA.mSymbol, mArgA.mBuyPriz, BearMarket, Lots); // sell
                 mArbitrager->openPosition(mArgB.mSymbol, mArgB.mSellPriz, BullMarket, Lots); // buy
                 mArgA.mBidPriz = mArgA.mBuyPriz;
                 mArgB.mBidPriz = mArgB.mSellPriz;
                 open = true;
             }
         } else if(mArgA.mCurDelta <= mArgA.mArbiLower && mArgA.mCurDelta <= mArgB.mArbiLower &&
                   mArgB.mCurDelta <= mArgA.mArbiLower && mArgB.mCurDelta <= mArgB.mArbiLower ) {
             if(mArbitrager->contractsCount() <= 0 ) {
                 mArbitrager->openPosition(mArgA.mSymbol, mArgA.mSellPriz, BullMarket, Lots); // buy
                 mArbitrager->openPosition(mArgB.mSymbol, mArgB.mBuyPriz, BearMarket, Lots); // sell
                 mArgA.mBidPriz = mArgA.mSellPriz;
                 mArgB.mBidPriz = mArgB.mBuyPriz;
                 open = true;
             }
         }
     }

     if(open) {
         QString strMsg = QString("[%1] Open: %2(%3, %4), %5(%6,%7)")
                 .arg(DATETIME2STR(timestamp))
                 .arg(mArgA.mSymbol)
                 .arg((mArbitrager->transactType(mArgA.mSymbol) == BullMarket) ? "Bull" : "Bear")
                 .arg(mArgA.mBidPriz)
                 .arg(mArgB.mSymbol)
                 .arg((mArbitrager->transactType(mArgB.mSymbol) == BullMarket) ? "Bull" : "Bear")
                 .arg(mArgB.mBidPriz);
         qDebug() << strMsg;
     }

     bool close = false;
     if(!open && (mArbitrager->contractsCount() > 0)) {
        if(mArbitrager->transactType(mArgA.mSymbol) == BearMarket && mArbitrager->transactType(mArgB.mSymbol) == BullMarket) {
            if( mArgA.mCurDelta <= mArgA.mExpect && mArgA.mCurDelta <= mArgB.mExpect &&
                mArgB.mCurDelta <= mArgA.mExpect && mArgB.mCurDelta <= mArgB.mExpect) {
                mArbitrager->closePosition(mArgB.mSymbol, mArgB.mBuyPriz); // sell
                mArbitrager->closePosition(mArgA.mSymbol, mArgA.mSellPriz); // buy
                mArgA.mClosePriz = mArgA.mSellPriz;
                mArgB.mClosePriz = mArgB.mBuyPriz;
                close = true;
            }
        } else if(mArbitrager->transactType(mArgA.mSymbol) == BullMarket && mArbitrager->transactType(mArgB.mSymbol) == BearMarket) {
            if( mArgA.mCurDelta >= mArgA.mExpect && mArgA.mCurDelta >= mArgB.mExpect &&
                mArgB.mCurDelta >= mArgA.mExpect && mArgB.mCurDelta >= mArgB.mExpect) {
                mArbitrager->closePosition(mArgB.mSymbol, mArgB.mSellPriz); // buy
                mArbitrager->closePosition(mArgA.mSymbol, mArgA.mBuyPriz); // sell
                mArgA.mClosePriz = mArgA.mBuyPriz;
                mArgB.mClosePriz = mArgB.mSellPriz;
                close = true;
            }
        }
     }
     if(close) {
         QString strMsg = QString("[%1] @Close: %2(%3, %4), %5(%6, %7); Accumulative Gain:$%8")
                 .arg(DATETIME2STR(timestamp))
                 .arg(mArgA.mSymbol)
                 .arg((mArgA.mClosePriz > mArgA.mBidPriz) ? "Bull" : "Bear")
                 .arg(mArgA.mClosePriz)
                 .arg(mArgB.mSymbol)
                 .arg((mArgB.mClosePriz > mArgB.mBidPriz) ? "Bull" : "Bear")
                 .arg(mArgB.mClosePriz)
                 .arg(mArbitrager->gain());
         qDebug() << strMsg;
     }
}

void DeltaStrategy::indexStreamFinished()
{
    if(mode() == SM_HISTORICAL) {
        qDebug() << "Total Ticks: " << mTicksCount;
        mFltrSB1->staticSignalFixTail(mSeqSet.seqData("SBR_AVG_A"));
        mFltrSB2->staticSignalFixTail(mSeqSet.seqData("SBR_AVG_B"));

        mSeqSet.trimSeqSize(HISTORY_INDEX_COUNT);
        mSeqSetDelta.trimSeqSize(HISTORY_INDEX_COUNT);
        VectorSubstract(mSeqSet.seq("SBR_A")->data(), mSeqSet.seq("SBR_AVG_A")->data(), mSeqSetDelta.seq("DELTA_A")->data());
        VectorSubstract(mSeqSet.seq("SBR_B")->data(), mSeqSet.seq("SBR_AVG_B")->data(), mSeqSetDelta.seq("DELTA_B")->data());
        DrawCurvesSBR();
        DrawCurvesDelta();

        QTimer::singleShot(200, this, SLOT(enterLiveMode()));
    } else {
        qDebug() << "Live Mode finished";
    }
}

void DeltaStrategy::streamSetQuery(LocalIndexStream *stream, QDateTime &startTm, QDateTime &endTm)
{
    const QString sqlTemplate
      = QString("SELECT * FROM %1 WHERE DateTime>='")
            + DATETIME2STR(startTm) + "'" + " AND DateTime<='" + DATETIME2STR(endTm) + "'";

    QString tblName = QString("ic%1_persec_view").arg(stream->symbol());
    stream->query(QString(sqlTemplate).arg(tblName), startTm, endTm);
}




