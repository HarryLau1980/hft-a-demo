#include "deltastgworker.h"
#include "deltastrategy.h"
#include "../localindexstream.h"
#include "../../utils/common.h"

IndexStream *DeltaStgWorker::createIndexStream(quint32 tickerSymbol, IndexSynchronizer *sync)
{
    LocalIndexStream *stream = new LocalIndexStream(tickerSymbol, DEF_DBFILE, sync);
    QDateTime startTime = STG_TEST_TIME[0];
    QDateTime endTime = STG_TEST_TIME[1];
    DeltaStrategy::streamSetQuery(stream, startTime, endTime);
    return stream;
}

StrategyAnalyzer *DeltaStgWorker::createAnalyzer()
{
    return new DeltaStrategy();
}

