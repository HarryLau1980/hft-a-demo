#include "dataaccif.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDate>
#include <QDebug>
#include <math.h>

DAIContext *DAICreate(QString dbName, QString sqlVerifyTradeDate,
   QTime amOpenTime, QTime amCloseTime, QTime pmOpenTime, QTime pmCloseTime)
{
    DAIContext* ctx = new DAIContext;
    ctx->mDBName = dbName;
    ctx->mSqlVeriyTradeDate = sqlVerifyTradeDate;
    ctx->mDbInst = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", QString("Connection%1").arg(rand())));
    ctx->mQuery = new QSqlQuery(*(QSqlDatabase*)ctx->mDbInst);

    ctx->mAMOpenTime = amOpenTime;
    ctx->mAMCloseTime = amCloseTime;
    ctx->mPMOpenTime = pmOpenTime;
    ctx->mPMCloseTime = pmCloseTime;

    QSqlDatabase* db = (QSqlDatabase*)ctx->mDbInst;
    db->setDatabaseName(dbName);
    if(!db->open()) {
        DAIDestroy(ctx);
        return NULL;
    }

    return ctx;
}

void DAIDestroy(DAIContext *ctx)
{
    if(ctx) {
        if(ctx->mDbInst) {
            ((QSqlDatabase*)ctx->mDbInst)->close();
            delete (QSqlDatabase*)(ctx->mDbInst);
        }
        if(ctx->mQuery) {
            delete (QSqlQuery*)(ctx->mQuery);
        }
        delete ctx;
    }
}

QString DAILastError(DAIContext* ctx)
{
    QSqlDatabase* db = (QSqlDatabase*)ctx->mDbInst;
    return db->lastError().text();
}

int DAIGetTimeSequence(DAIContext *ctx, QDateTime start, QDateTime end, DAIType ty, QVector<QDateTime> *v)
{
    int cRows = 0;
    if(!ctx || !v)
        return -1;

    if(start >= end)
        return -1;

    v->clear();
    QDate lastTradeDate = start.date();
    // move to trade date
    while(!DAIIsTradeDate(ctx, lastTradeDate)) {
        lastTradeDate = lastTradeDate.addDays(1);
        start.setDate(lastTradeDate);
        start.setTime(ctx->mAMOpenTime);
        if(start > end)
            break;
    }

    QTime tm;
    while(start <= end) {
        tm = start.time();
        if(tm > ctx->mPMCloseTime) {
            start = start.addDays(1);
            start.setTime(ctx->mAMOpenTime);
            if(start > end )
                break;
        } else if((tm > ctx->mAMCloseTime) && (tm < ctx->mPMOpenTime)) {
            start.setTime(ctx->mPMOpenTime);
            if(start > end)
                break;
        } else if(tm < ctx->mAMOpenTime) {
            start.setTime(ctx->mAMOpenTime);
        }

        if(start.date() != lastTradeDate) {
            while(!DAIIsTradeDate(ctx, start.date())) {
                start = start.addDays(1);
                if(start > end)
                    break;
            }
            lastTradeDate = start.date();
            start.setTime(ctx->mAMOpenTime);
            if(start > end )
                break;
        }

        v->append(start);

        switch(ty)
        {
        case DAI_PERSEC:
            start = start.addSecs(1);
            break;
        case DAI_PER5SEC:
            start = start.addSecs(5);
            break;
        case DAI_PER10SEC:
            start = start.addSecs(10);
            break;
        case DAI_PER15SEC:
            start = start.addSecs(15);
            break;
        case DAI_PER20SEC:
            start = start.addSecs(20);
            break;
        case DAI_PER30SEC:
            start = start.addSecs(30);
            break;
        case DAI_PERMIN:
            start = start.addSecs(60);
            break;
        case DAI_PER5MIN:
            start = start.addSecs(60*5);
            break;
        case DAI_PER10MIN:
            start = start.addSecs(60*10);
            break;
        case DAI_PER15MIN:
            start = start.addSecs(60*15);
            break;
        case DAI_PER30MIN:
            start = start.addSecs(60*30);
            break;
        case DAI_PERHOUR:
            start = start.addSecs(60*60);
            break;
        case DAI_PERDAY:
            start = start.addDays(1);
            break;
        case DAI_PERWEEK:
            start = start.addDays(7);
            break;
        case DAI_PERMONTH:
            start = start.addMonths(1);
            break;
        case DAI_PERYEAR:
            start = start.addYears(1);
            break;
        default:
            break;
        }
    }
    return cRows;
}

bool DAIIsTradeDate(DAIContext* ctx, QDate date)
{
    if(!ctx)
        return false;
    QSqlDatabase* db = (QSqlDatabase*)ctx->mDbInst;
    QString sql = QString(ctx->mSqlVeriyTradeDate).arg(date.toString(DAI_DATE_FMT));
    QSqlQuery q = db->exec(sql);
    if(q.next()) {
        return (q.value(0).toInt() > 0 );
    }

    return false;
}

QString DAIFormatQuery(const QString &templat, const char **args, int argc)
{
    QString q = QString(templat).arg(args[0]);
    for( int i = 1; i < argc; i ++ ) {
        QString argPlaceHolder = QString("%%1").arg(i+1);
        q = q.replace(argPlaceHolder, QString("%1"));
        q = q.arg(args[i]);
    }
    return q;
}


bool DAIIsFuturesSettlement(const QDateTime &datetime)
{
    QDate date = datetime.date();
    if(date.dayOfWeek() == 5) {
        /*3rd Friday in each month*/
        int mon = date.month();
        date = date.addDays(-7*2);
        return (date.month() == mon);
    }
    return false;
}
