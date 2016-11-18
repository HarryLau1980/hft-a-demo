#include "indexstream.h"
#include "indexsynchronizer.h"
#include "../utils/common.h"
#include "../DataAccInterf/dataaccif.h"
#include <QDebug>


IndexStream::IndexStream(quint32 symbol, const QString &url, IndexSynchronizer *syn)
    : QThread(NULL)
    , mSymbol(symbol)
    , mUrl(url)
    , mSynchronizer(syn)
    , mIsOpen(false)
    , mStopSig(true)
    , mSleepTime(0)

{
    mLastPopTick.setSymbol(INVALID_INDEX);
}

IndexStream::~IndexStream()
{
    terminate();
}

void IndexStream::run()
{
    if(!open()) {
        qDebug() << "Couldn't open the stream: " << name();
       return;
    }

    IndexTick tick;
    qDebug() << "IndexStream: " << mSymbol << " Started!";
    mTicks.clear();
    while(!mStopSig) {
        if(!read(tick)) {
            emit indexStreamError(this);
            pushFinIndex();
            break;
        }
        if(tick.isFinIndex()) {
            pushFinIndex();
            break;
        } else {
            push(tick);
        }
        if(mSleepTime > 0 ) {
            msleep(mSleepTime);
        }
    }

    close();
    qDebug() << "IndexStream quit: " << this->mSymbol;
}

void IndexStream::terminate()
{
    mStopSig = true;
    QThread::terminate();
    QThread::wait();
}

IndexTick& IndexStream::pop(const QDateTime &timestamp)
{
    if(timestamp == mLastPopTick.timeStamp()) {
        return mLastPopTick;
    }
    static int RefClockDelayCount = 0;
    {
        QMutexLocker lk(&mMtx);
        if(!mTicks.isEmpty()) {
            IndexTick& tick = mTicks.first();
            if(tick.isFinIndex()) {
                qDebug() << "oops";
            }
            QDateTime& ts = tick.timeStamp();
            if(timestamp == ts ) {
                RefClockDelayCount = 0;
                mLastPopTick = tick;
                mTicks.removeFirst();
            } else if(timestamp > ts) {
                RefClockDelayCount = 0;
                qDebug() << this->mSymbol << ":In Queue: " << DATETIME2STR(ts) << "Wanted: " << DATETIME2STR(timestamp);
                QThread::sleep(1);
                return mLastPopTick;
            } else {
                // timestamp < ts
                ++ RefClockDelayCount;
                if(RefClockDelayCount > 5) {
                    qDebug() << "todo: make ref-clock more slower";
                    QThread::sleep(1);
                    RefClockDelayCount = 0;
                }
            }
        } else {
            qDebug() << "[Empty Record]Wanted: " << DATETIME2STR(timestamp);
            QThread::sleep(1);
        }
    }

    mLastPopTick.setTimeStamp(timestamp);
    if(mLastPopTick.isInvalid() ) {
        mLastPopTick.setSymbol(mSymbol);
        QVector<double> v;
        mLastPopTick.setIndex(v);
    }

    return (IndexTick&)mLastPopTick;
}

bool IndexStream::emptyTicks()
{
    int c = 0;
    mMtx.lock();
    c = mTicks.size();
    mMtx.unlock();
    return (c <= 0);
}

void IndexStream::start(Priority prio)
{
    mStopSig = false;
    QThread::start();
}

bool IndexStream::open()
{
    mIsOpen = true;
    return true;
}

void IndexStream::close()
{
    mIsOpen = false;
}

void IndexStream::push(const IndexTick& tick)
{
    QDateTime& curTickTim = tick.timeStamp();
    QTime tim = curTickTim.time();
    if( (tim < CN_AMOPEN_TIME) ||
        ((tim > CN_AMCLOSE_TIME) && (tim < CN_PMOPEN_TIME)) ||
        (tim > CN_PMCLOSE_TIME))
        return;

    if(mLastPushTickTm.isNull()) {
        appendIndexTick(tick);
    } else {
        QDateTime minCurTime = mLastPushTickTm.addSecs(mSynchronizer->tickInterval());
        if( curTickTim == minCurTime || curTickTim == mLastPushTickTm ) {
            appendIndexTick(tick);
        } else {
            while(curTickTim > minCurTime) {
                tim = minCurTime.time();
                if( (tim >= CN_AMOPEN_TIME && tim <= CN_AMCLOSE_TIME) ||
                    (tim >= CN_PMOPEN_TIME && tim <= CN_PMCLOSE_TIME)) {
                    IndexTick xt(tick.symbol(), mLastPushTick.index(), minCurTime);
                    appendIndexTick(xt);
                    emit updateIndexTimestamp(minCurTime);
                }
                minCurTime = minCurTime.addSecs(mSynchronizer->tickInterval());
            }
            appendIndexTick(tick);
        }
    }

    mLastPushTickTm = curTickTim;
    mLastPushTick = tick;
    emit updateIndexTimestamp(tick.timeStamp());
}

void IndexStream::pushFinIndex()
{
    IndexTick finIndex = IndexTick::FinIndex();
    appendIndexTick(finIndex);
    emit updateIndexTimestamp(finIndex.timeStamp());
    // qDebug() << "FIN";
}


