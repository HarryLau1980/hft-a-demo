#include "localindexstream.h"
#include "indexsynchronizer.h"
#include "../utils/common.h"

LocalIndexStream::LocalIndexStream(quint32 symbol, const QString &url, IndexSynchronizer *syn)
    : IndexStream(symbol, url, syn)
    , mDb(NULL)
{

}

LocalIndexStream::~LocalIndexStream()
{
    if(mDb) {
        delete mDb;
    }
}

void LocalIndexStream::query(const QString& query, QDateTime startTime, QDateTime endTime)
{
    mSqlQuery = query;
    if(mDb == NULL) {
        mDb = new FinanceSqliteDataSource(url(), mSqlQuery);
    }
    if(!mDb->isOpen()) {
        if(!mDb->open())
            return;
    }

    mDb->setQuery(mSqlQuery);
    mIndexTicksLocal.clear();
    FinanceRow row;
    IndexTick lastTick;
    while (mDb->read(row)) {
        IndexTick tick;
        tick.setSymbol(mSymbol);
        tick.setTimeStamp(row.timestamp());
        tick.setIndex(row.data());
        mIndexTicksLocal.append(tick);
        lastTick = tick;
    }
    mDb->close();

    if(!endTime.isNull()) {
        if(endTime.time() > CN_PMCLOSE_TIME) {
            endTime.setTime(CN_PMCLOSE_TIME);
        } else if((endTime.time() > CN_AMCLOSE_TIME) && (endTime.time() < CN_PMOPEN_TIME)) {
            endTime .setTime(CN_AMCLOSE_TIME);
        }
    }

    //* append to the end time
    QDateTime lastTimestamp = lastTick.timeStamp();
    QTime lastTime = lastTimestamp.time();
    lastTime = lastTime.addSecs(mSynchronizer->tickInterval());
    while( lastTime <= endTime.time() ) {
        lastTimestamp.setTime(lastTime);
        lastTick.setTimeStamp(lastTimestamp);
        mIndexTicksLocal.append(lastTick);
        lastTime = lastTime.addSecs(mSynchronizer->tickInterval());
    }
    // */
    mIndexTicksLocal.append(IndexTick::FinIndex());
}


bool LocalIndexStream::read(IndexTick &tick)
{
    if(mIndexTicksLocal.isEmpty())
        return false;
    tick = mIndexTicksLocal.first();
    mIndexTicksLocal.removeFirst();
    return true;
}

