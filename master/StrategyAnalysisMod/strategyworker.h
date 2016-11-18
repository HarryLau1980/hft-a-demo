#ifndef STRATEGYWORKER_H
#define STRATEGYWORKER_H

#include <QList>
#include <QVector>
#include <QDateTime>

class IndexStream;
class IndexSynchronizer;
class StrategyAnalyzer;

#define STRATEGY_MAX_INPUT 32
class StrategyWorker
{
public:
    StrategyWorker(quint32* tickerSymbols, int cCh, quint32 tickInterval);
    virtual ~StrategyWorker();

protected:
    QList<IndexStream*> mIndexStreams;
    IndexSynchronizer*  mSynchronizer;
    StrategyAnalyzer*   mAnalyzer;

    int mIndexCount;
    quint32 mTickerSymbols[STRATEGY_MAX_INPUT];
    quint32 mTickInterval;

public:
    bool create();
    void destroy();

    bool start();
    void stop();

protected:
    virtual IndexStream* createIndexStream(quint32 tickerSymbol, IndexSynchronizer* sync) = 0;
    virtual StrategyAnalyzer* createAnalyzer() = 0;
};



#endif // STRATEGYWORKER_H
