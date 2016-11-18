#ifndef DELTASTRATEGY_H
#define DELTASTRATEGY_H


#include "../strategyanalyzer.h"
#include "../strategyworker.h"

#include "../DataAccInterf/financedatasource.h"
#include "../utils/indexsequenceset.h"
#include "../TradeExecMod/tradeexecmod.h"
const static QDateTime STG_TEST_TIME[] = {
    QDateTime(QDate(2015, 4, 23), QTime(9, 30, 0))
    , QDateTime(QDate(2015, 4, 23), QTime(9,40, 0))
    , QDateTime(QDate(2015, 4, 23), QTime(15, 30, 0))
};
// ========== DeltaStrategy ==========
typedef struct ArbitrageArgs_t
{
    int mSymbol;
    double mSellPriz;
    double mBuyPriz;
    double mBidPriz;
    double mClosePriz;

    double mKurt;
    double mSkew;
    double mExpect;
    double mVar;
    double mStdDev;
    double mCurS;
    double mCurU;
    double mCurDelta;
    double mArbiUper;
    double mArbiLower;

    double mCurO;
    double mO5;
    double mO10;
    double mO30;
    double mO60;
    QVector<double>* mDeltaSeq;
} ArbitrageArgs;

class IndexFilter;
class  GraphCurveWindow;
class LocalIndexStream;
class DeltaStrategy
        : public StrategyAnalyzer
{
    Q_OBJECT
public:
    DeltaStrategy();
    virtual ~DeltaStrategy();

private:
    QVector<QDateTime> mTimeLine;
    IndexSequenceSet mSeqSet;
    IndexSequenceSet mSeqSetDelta;

    IndexFilter* mFltrSB1;
    IndexFilter* mFltrSB2;
    ArbitrageArgs mArgA;
    ArbitrageArgs mArgB;

    GraphCurveWindow* mCurveWin;
    GraphCurveWindow* mCurveDelta;

    Arbitrager* mArbitrager;
    FinanceIndexCache* mIndexCache;
private:
    void DrawCurvesSBR(bool update=false);
    void DrawCurvesDelta(bool update=false);
    Q_SLOT void enterLiveMode();

    void makeTradeArgs(QVector<double>& s, QVector<double>& u, QVector<double>& o, ArbitrageArgs* arg);
    void calcArbitrageArgs(QVector<double>& s, QVector<double>& u, ArbitrageArgs* arg);
    void makeTradeDecision(const QDateTime& timestamp);
    void makeOArgsSeq(QVector<double>& s, QVector<double>& u, QVector<double>& o);

public:
    static void streamSetQuery(LocalIndexStream* stream, QDateTime& startTm, QDateTime& endTm);

public:
    virtual int  incomingIndexTicks(const QDateTime& timestamp, const QVector<IndexTick> &ticks);
    virtual void indexStreamError(IndexStream* stream);
    virtual void indexStreamFinished();

};


#endif // DELTASTRATEGY_H
