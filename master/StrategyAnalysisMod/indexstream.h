#ifndef INDEXSTREAM_H
#define INDEXSTREAM_H

#include "indextick.h"
#include <QVector>
#include <QString>
#include <QThread>
#include <QList>
#include <QDateTime>
#include <QMutex>

class IndexSynchronizer;
struct DAIContext_t;

class IndexStream
        : public QThread
{
    Q_OBJECT
public:
    IndexStream(quint32 symbol, const QString& url, IndexSynchronizer* syn);
    virtual ~IndexStream();

protected:
    QString mUrl;
    IndexSynchronizer* mSynchronizer;
    quint32 mSymbol;

    bool mIsOpen;
    bool mStopSig;

    QString mName;

    QList<IndexTick> mTicks;
    QMutex mMtx;
    IndexTick mLastPopTick;
    QDateTime mLastPushTickTm;
    IndexTick mLastPushTick;

    int mSleepTime;

protected:
    virtual void run();
    virtual bool open();
    virtual void close();
    virtual bool read(IndexTick& tick) = 0;

    inline void push(const IndexTick& tick);
    inline void pushFinIndex(); // in case of at the end of stream

    inline void appendIndexTick(const IndexTick& tick) {
        mMtx.lock();
        mTicks.append(tick);
        mMtx.unlock();
    }

    Q_SIGNAL void updateIndexTimestamp(const QDateTime& timestamp);
    Q_SIGNAL void indexStreamError(IndexStream* stream);

public:
    void start(Priority prio = InheritPriority);
    void terminate();

    void setSymbol(quint32 symbol) { mSymbol = symbol; }
    inline quint32 symbol() const { return mSymbol; }

    inline QString name() const { return mName; }
    inline QString url() const { return mUrl; }

    void setSleep(int sl/*ms*/) { mSleepTime = sl; }

    IndexTick& pop(const QDateTime& timestamp);
    bool emptyTicks();

    inline bool isOpen() const { return mIsOpen; }

};


#endif // INDEXSTREAM_H
