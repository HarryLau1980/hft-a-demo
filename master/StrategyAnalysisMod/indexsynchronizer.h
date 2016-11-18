#ifndef INDEXSYNCHRONIZER_H
#define INDEXSYNCHRONIZER_H

#include "indexstream.h"
#include <QDateTime>
#include <QVector>
#include <QList>
#include <QMap>
#include <QString>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSemaphore>

enum SynchronizerMode
{
    SystemClock, // if there is ONLY one stocks there
    RefEachClock // if there is multiple stocks, select in RefEachClock, IndexSynchronizer will lookup each other stock's timestamp...
};

class StrategyAnalyzer;
class IndexStream;
// minimun resolution of IndexSynchronizer is of 1 second.
// todo: improve the IndexSynchronizer to milli-sec level.
class IndexSynchronizer
        : public QThread
{
    Q_OBJECT
public:
    IndexSynchronizer(quint32* chCodes, quint32 chCount, quint32 tickInterval /*in second*/, StrategyAnalyzer* analyzer);
    ~IndexSynchronizer();

private:
    quint32 mTickInterval;
    quint32 mStreamCount;
    bool mStopSig;
    quint64 mNetLatency;

    StrategyAnalyzer* mAnalyzer;
    QList<IndexStream*> mIndexStreams;

    int mTimelineZoomoutFactor;
    QDateTime mCurrTimestamp;

    typedef struct TickRelayInfo_t {
        int mExecTime;
        int mCounter;
    } TickRelayInfo;
    QDateTime mLastExecTimestamp;
    QMap<QDateTime, TickRelayInfo> mRelayTickInfo;
    quint32 mDeltaRelay;
    SynchronizerMode mMode;
    QMutex mMtxCurrTimestamp;
    QTime mTimer;

private:
    void calcLatency(const QDateTime& timestamp);
    void start(Priority priority = InheritPriority);
    void printIndexTick(const QVector<IndexTick>& tick);

    void RefEachClockRun();
    void SysClockRun();

    void updateTickTimestamp(const QDateTime& timestamp);

    bool getIndexStreamTicks(const QDateTime& timestamp, QVector<IndexTick>& ticks);

public:
    void setMode(SynchronizerMode mode) { mMode = mode; }
    SynchronizerMode mode() const { return mMode; }

    inline quint32 tickInterval() const { return mTickInterval; }

    void setRefEachClockMode(quint32 deltaMs) { mDeltaRelay = deltaMs; }

    Q_SLOT void updateIndexTimestamp(const QDateTime& timestamp);
    Q_SLOT void indexStreamError(IndexStream* stream);

    void addIndexStream(IndexStream* stream);
    IndexStream *indexStream(const quint32 symbol);

    void terminate();
    virtual void run();

    void stopIndexStreams();
    void startIndexStreams();

    QList<IndexStream*> indexStreams() const { return (QList<IndexStream*>&)mIndexStreams; }

    // zooming-out timeline can make the Synchronizer works faster. e.g: if tick-interval is 5 seconds, and zooming out the time line to factor=0.001
    // then the IndexSynchronizer will treat 5ms as 5s. the feature is important for test purpose especially if the tick-stream is from localstorage.
    void zoomOutTimeline(double factor/*(0, 1)*/);
};

#endif // INDEXSYNCHRONIZER_H
