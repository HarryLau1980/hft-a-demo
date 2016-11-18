#include "indexsynchronizer.h"
#include "strategyanalyzer.h"
#include "../DataAccInterf/dataaccif.h"
#include "../utils/common.h"
#include <QDebug>

// --------------------- IndexSynchronizer -------------------------
static const QDateTime InvalidLastExecTime = QDateTime::currentDateTime().addDays(1);
IndexSynchronizer::IndexSynchronizer(quint32 *chCodes, quint32 chCount, quint32 refClock, StrategyAnalyzer* analyzer)
    : mStreamCount(chCount)
    , mTickInterval(refClock)
    , mAnalyzer(analyzer)
    , mNetLatency(0)
    , mTimelineZoomoutFactor(1)
    , mDeltaRelay(50)
{
    mLastExecTimestamp = InvalidLastExecTime;
}

IndexSynchronizer::~IndexSynchronizer()
{
}

void IndexSynchronizer::calcLatency(const QDateTime &timestamp)
{

}

void IndexSynchronizer::updateIndexTimestamp(const QDateTime &timestamp)
{
    QMutexLocker alk(&mMtxCurrTimestamp);
    mCurrTimestamp = timestamp;
    if(mMode == SystemClock) {
        if(!QThread::isRunning()) {
            start();
        }
    } else {
        updateTickTimestamp(timestamp);
    }
}

void IndexSynchronizer::addIndexStream(IndexStream *stream)
{
    if(!stream)
        return;
    QObject::connect(stream, SIGNAL(updateIndexTimestamp(const QDateTime&)),
                     this, SLOT(updateIndexTimestamp(const QDateTime&)));
    QObject::connect(stream, SIGNAL(indexStreamError(IndexStream*)), this,
                     SLOT(indexStreamError(IndexStream*)));
    mIndexStreams.append(stream);
}

IndexStream *IndexSynchronizer::indexStream(const quint32 symbol)
{
    for( int i = 0; i < mIndexStreams.size(); i ++ ) {
        if(mIndexStreams[i]->symbol() == symbol)
            return mIndexStreams[i];
    }
    return NULL;
}

void IndexSynchronizer::indexStreamError(IndexStream *stream)
{
    qDebug() << stream->symbol() << " streaming terminated!";
    mAnalyzer->indexStreamError(stream);
}

void IndexSynchronizer::start(QThread::Priority priority)
{
    if(QThread::isRunning()) {
        return;
    }
    mTimer.start();
    mStopSig = false;
    QThread::start(priority);
}

void IndexSynchronizer::printIndexTick(const QVector<IndexTick> &tick)
{
    int siz = tick.size();
    if(siz == 0 )
        return;
    QString msg = QString("%1\t").arg(DATETIME2STR(tick.first().timeStamp()));
    for( int i = 0; i < siz; i ++ ) {
        QVector<double>& tv = tick[i].index();
        for( int j = 0; j < tv.size(); j ++ ) {
            msg += QString("%1\t").arg(tv[j]);
        }
    }
    qDebug() << msg;
}

void IndexSynchronizer::updateTickTimestamp(const QDateTime &timestamp)
{
    TickRelayInfo* tri;
    if(mRelayTickInfo.contains(timestamp)) {
        tri = &(mRelayTickInfo[timestamp]);
        tri->mCounter ++;
        if(tri->mCounter >= mStreamCount) {
           QVector<IndexTick> ticks;
           getIndexStreamTicks(timestamp, ticks);
        }
    } else {
        // in this case, there are 2 possibles: 1) have been executed; 2) not plained yet.
        if(timestamp > mLastExecTimestamp || mLastExecTimestamp == InvalidLastExecTime) {
            TickRelayInfo xtri;
            xtri.mCounter = 1;
            xtri.mExecTime = mTimer.elapsed() + mDeltaRelay;

            // qDebug() << "EXEC TIME" << xtri.mExecTime;
            mRelayTickInfo[timestamp] = xtri;
        } else {
            if(mLastExecTimestamp != InvalidLastExecTime)
                qDebug() << "going to add a relay tick, but has executed: " << timestamp << " EXEC TIME: " << mTimer.elapsed() + mDeltaRelay;
        }
    }
}

bool IndexSynchronizer::getIndexStreamTicks(const QDateTime &timestamp, QVector<IndexTick> &ticks)
{
    ticks.clear();
    mRelayTickInfo.remove(timestamp);

    for( int i = 0; i < mStreamCount; i ++ ) {
       IndexTick& t = mIndexStreams[i]->pop(timestamp);
       if(t.isFinIndex()) {
           mAnalyzer->indexStreamFinished();
           return false;
       }
       ticks.append(t);
    }

    mLastExecTimestamp = timestamp;
    // printIndexTick(ticks);
    mAnalyzer->incomingIndexTicks(timestamp, ticks);
    return true;
}

void IndexSynchronizer::terminate()
{
    mStopSig = true;

    if(!QThread::isRunning()) {
        return;
    }

    QThread::terminate();
    QThread::wait();
}

void IndexSynchronizer::run()
{
    if(mMode == SystemClock) {
        SysClockRun();
    } else {
        RefEachClockRun();
    }
    qDebug() << "[Warning]: IndexSynchronizer stoped!";
}

void IndexSynchronizer::stopIndexStreams()
{
    for( int i = 0; i < mIndexStreams.size(); i ++ ) {
        mIndexStreams[i]->terminate();
    }
}

void IndexSynchronizer::startIndexStreams()
{
    for( int i = 0; i < mIndexStreams.size(); i ++ ) {
        mIndexStreams[i]->terminate();
        mIndexStreams[i]->start();
    }
}

void IndexSynchronizer::zoomOutTimeline(double factor)
{
    if(factor <= 0.0 || factor > 1.0)
        factor = 1.0;
    mTimelineZoomoutFactor = 1.0/factor;
}

void IndexSynchronizer::RefEachClockRun()
{
}

void IndexSynchronizer::SysClockRun()
{
    QDateTime timestamp = mCurrTimestamp;
    QVector<IndexTick> ticks;
    QTime timer;
    int sleept = 0, i = 0;
    const int intervalMs = mTickInterval*1000;
    timer.start();

    bool streamsReady = false;
    qDebug() << "SystemClockRun called, in SystemClockMode";
    while(!streamsReady) {
        streamsReady = true;
        for( i = 0; i < mStreamCount; i ++ ) {
            if(mIndexStreams[i]->emptyTicks()) {
                streamsReady = false;
                break;
            }
        }
        if(!streamsReady) {
            QThread::msleep(10);
        }
    }

    while(!mStopSig) {
        timer.restart();
        if(!getIndexStreamTicks(timestamp, ticks)) {
            break;
        }
        ticks.clear();
        sleept = intervalMs - timer.elapsed();
        if(sleept < 0 )
            sleept = 0;

        timer.restart();
        QThread::msleep(sleept);
        timestamp = timestamp.addSecs(mTickInterval);
    }
}

