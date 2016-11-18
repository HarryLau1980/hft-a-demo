#ifndef STRATEGYANALYZER_H
#define STRATEGYANALYZER_H

#include "indexsynchronizer.h"
#include "indexstream.h"
#include <QDateTime>
#include <QVector>
#include <QTimer>

enum StrategyMode
{
    SM_LIVE_MODE,
    SM_HISTORICAL
};

#define DELAY_CALL(member) \
    QTimer::singleShot(msTimeout, this, member)

class StrategyAnalyzer
        : public QObject
{
    Q_OBJECT
public:
    StrategyAnalyzer();
    virtual ~StrategyAnalyzer();

protected:
    quint64 mTicksCount;
    QString mName;
    StrategyMode mMode;
    IndexSynchronizer* mSynchronizer;

public:
    void setSynchronizer(IndexSynchronizer* sync);
    virtual void setMode(StrategyMode mode) { mMode = mode; }
    StrategyMode mode() const { return mMode; }

    void setName(const QString& name) { mName = name; }
    QString name() const { return mName; }

    void printTick(const QDateTime& timestamp, const QVector<double>& ticks);

    void startIndexStreams();
    void stopIndexStreams();

public:
    quint64 ticksCount() const { return mTicksCount; }

public:
    virtual int incomingIndexTicks(const QDateTime& timestamp, const QVector<IndexTick>& ticks);
    virtual void indexStreamError(IndexStream* stream) = 0;
    virtual void indexStreamFinished() = 0;
};

#endif // STRATEGYANALYZER_H
