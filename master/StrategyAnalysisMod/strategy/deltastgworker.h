#ifndef DELTASTGWORKER_H
#define DELTASTGWORKER_H

#include "../strategyworker.h"

// ======== DeltaStrategyWorker ===========
class DeltaStgWorker
        : public StrategyWorker
{
public:
    DeltaStgWorker(quint32* tickerSymbols, int cCh, quint32 tickInterval)
        : StrategyWorker(tickerSymbols, cCh, tickInterval) {}
    virtual ~DeltaStgWorker() {}
public:
    virtual IndexStream *createIndexStream(quint32 tickerSymbol, IndexSynchronizer *sync);
    virtual StrategyAnalyzer *createAnalyzer();

};


#endif // DELTASTGWORKER_H
