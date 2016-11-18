#include "strategyworker.h"
#include "indexsynchronizer.h"
#include "strategyanalyzer.h"
#include "indexstream.h"

StrategyWorker::StrategyWorker(quint32 *tickerSymbols, int cCh, quint32 tickInterval)
    : mSynchronizer(NULL)
    , mAnalyzer(NULL)
    , mIndexCount(cCh)
    , mTickInterval(tickInterval)
{
    for( int i = 0; i < STRATEGY_MAX_INPUT; i ++ ) {
        if(i < cCh) {
            mTickerSymbols[i] = tickerSymbols[i];
        } else {
            mTickerSymbols[i] = 0;
        }
    }
}

StrategyWorker::~StrategyWorker()
{
    destroy();
}

bool StrategyWorker::create()
{
    if(!mAnalyzer) {
        mAnalyzer = createAnalyzer();
    }
    if(!mSynchronizer) {
        mSynchronizer = new IndexSynchronizer(mTickerSymbols, mIndexCount, mTickInterval, mAnalyzer);
        if(mIndexCount >= 2) {
            mSynchronizer->setMode(RefEachClock);
        }
    }
    mAnalyzer->setSynchronizer(mSynchronizer);

    for( int i = 0; i < mIndexCount; i ++ ) {
        IndexStream* stream = createIndexStream(mTickerSymbols[i], mSynchronizer);
        mIndexStreams.append(stream);
        mSynchronizer->addIndexStream(stream);
    }
}

void StrategyWorker::destroy()
{
    stop();

    if(mAnalyzer) {
        delete mAnalyzer;
    }
    if(mSynchronizer) {
        delete mSynchronizer;
    }
    for( int i = 0; i < mIndexCount; i ++ ) {
        delete mIndexStreams[i];
    }
}

bool StrategyWorker::start()
{
    mSynchronizer->startIndexStreams();
}

void StrategyWorker::stop()
{
    mSynchronizer->stopIndexStreams();
    mSynchronizer->terminate();
}

