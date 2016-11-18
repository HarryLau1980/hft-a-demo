#include "strategyanalyzer.h"
#include "indexfilter.h"
#include <QDebug>

StrategyAnalyzer::StrategyAnalyzer()
    : QObject(NULL)
    , mTicksCount(0)
    , mName("UnknowStrategy")
    , mSynchronizer(NULL)
{
    mMode = SM_HISTORICAL;
}

StrategyAnalyzer::~StrategyAnalyzer()
{

}

void StrategyAnalyzer::setSynchronizer(IndexSynchronizer *sync)
{
    mSynchronizer = sync;
}

void StrategyAnalyzer::printTick(const QDateTime &timestamp, const QVector<double> &ticks)
{
    QString msg = timestamp.toString("yyyy-MM-dd hh:mm:ss");
    for( int i = 0; i < ticks.size(); i ++ ) {
        msg += "\t" + QString("%1").arg(ticks[i]);
    }
    qDebug() << msg;
}

void StrategyAnalyzer::startIndexStreams()
{
    mSynchronizer->startIndexStreams();
}

void StrategyAnalyzer::stopIndexStreams()
{
    mSynchronizer->stopIndexStreams();
}

int StrategyAnalyzer::incomingIndexTicks(const QDateTime &timestamp, const QVector<IndexTick> &ticks)
{
    ++ mTicksCount;
    return 0;
}

