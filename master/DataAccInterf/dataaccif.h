#ifndef DATAACCIF_H
#define DATAACCIF_H

#include "dataaccessinterf_global.h"

#include <QVector>
#include <QDateTime>
#include <QString>

#define DAI_DATETIME_FMT "yyyy-MM-dd hh:mm:ss"
#define DAI_DATE_FMT "yyyy-MM-dd"
#define DAI_TIME_FMT "hh:mm:ss"


typedef enum DAIType_e
{
    DAI_PERSEC,
    DAI_PER5SEC,
    DAI_PER10SEC,
    DAI_PER15SEC,
    DAI_PER20SEC,
    DAI_PER30SEC,
    DAI_PERMIN,
    DAI_PER5MIN,
    DAI_PER10MIN,
    DAI_PER15MIN,
    DAI_PER30MIN,
    DAI_PERHOUR,
    DAI_PERDAY,
    DAI_PERWEEK,
    DAI_PERMONTH,
    DAI_PERYEAR
} DAIType;

typedef struct DAIContext_t
{
    QString mDBName;
    QString mSqlVeriyTradeDate; // sql statement to verify the date is a trade-date. should be counter of date in records returned

    QTime   mAMOpenTime;
    QTime   mAMCloseTime;
    QTime   mPMOpenTime;
    QTime   mPMCloseTime;

    //private:
    void*   mDbInst;
    void*   mQuery;
} DAIContext;


DAIContext* DAI_EXPORT DAICreate(QString dbName, QString sqlVerifyTradeDate,
     QTime amOpenTime, QTime amCloseTime, QTime pmOpenTime, QTime pmCloseTime );
void DAI_EXPORT DAIDestroy(DAIContext* ctx);

QString DAI_EXPORT DAILastError(DAIContext* ctx);

bool DAI_EXPORT DAIIsTradeDate(DAIContext* ctx, QDate date);
bool DAI_EXPORT DAIIsFuturesSettlement(const QDateTime& datetime);

int DAI_EXPORT DAIGetTimeSequence(DAIContext* ctx, QDateTime start, QDateTime end, DAIType ty, QVector<QDateTime>* v);

// utils
QString DAIFormatQuery(const QString& templat, const char **args, int argc);

#endif // DATAACCIF_H
