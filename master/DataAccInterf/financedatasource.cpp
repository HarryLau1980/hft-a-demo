#include "financedatasource.h"
#include "../utils/common.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

void FinanceRow::dumpRow(const FinanceRow &row)
{
    QString xrow = row.timestamp().toString("yyyy-MM-dd hh:mm:ss\t");
    QVector<double> xrowData = row.data();
    for( int i = 0; i < xrowData.size(); i ++ ) {
        xrow += QString("%1\t").arg(xrowData[i]);
    }
    qDebug() << xrow;
}

// ========== FinanceSqliteDataSource ===============
FinanceSqliteDataSource::FinanceSqliteDataSource(const QString &dbFile, const QString &query)
    : FinanceDataSource(query)
    , mDb(NULL) , mQuery(NULL), mDbFile(dbFile), mTransactionStarted(false)
{
}

FinanceSqliteDataSource::FinanceSqliteDataSource(const QString& query)
    : FinanceDataSource(query)
    , mDb(NULL), mQuery(NULL), mTransactionStarted(false)
{

}

FinanceSqliteDataSource::FinanceSqliteDataSource()
    : mDb(NULL), mQuery(NULL), mTransactionStarted(false)
{

}

FinanceSqliteDataSource::~FinanceSqliteDataSource()
{
    close();
}

void FinanceSqliteDataSource::setDbFile(const QString &dbFile)
{
    mDbFile = dbFile;
}

bool FinanceSqliteDataSource::setQuery(const QString &q)
{
    FinanceDataSource::setQuery(q);
    return true;
}

void FinanceSqliteDataSource::resetQuery()
{
    if(isOpen()) {
        mQuery->finish();
    }
}

bool FinanceSqliteDataSource::transaction()
{
    if(!isOpen())
        return false;
    if(mDb->transaction()) {
        mTransactionStarted = true;
    }
    return mTransactionStarted;
}

bool FinanceSqliteDataSource::commit()
{
    if(!isOpen())
        return false;
    if(mTransactionStarted) {
        mTransactionStarted = false;
        return mDb->commit();
    }
    return false;
}

bool FinanceSqliteDataSource::transactionStarted() const
{
    if(!isOpen())
        return false;
    return mTransactionStarted;
}

bool FinanceSqliteDataSource::isOpen() const
{
    if(mDb == NULL)
        return false;
    return mDb->isOpen();
}

bool FinanceSqliteDataSource::open()
{
    if(mDb != NULL) {
        if( mDb->databaseName() == mDbFile) {
            return true;
        } else {
            close();
            mDb = NULL;
        }
    }

    mDb = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", QString("Connection%1").arg((qulonglong)(void*)this)));
    mDb->setDatabaseName(mDbFile);
    if(!mDb->open()) {
        delete mDb;
        mDb = NULL;
        return false;
    }
    mQuery = new QSqlQuery(*mDb);

    return true;
}

void FinanceSqliteDataSource::close()
{
    if(!isOpen())
        return;

    if(mDb) {
        if(mTransactionStarted) {
            commit();
            mTransactionStarted = false;
        }
        mDb->close();
        if(mQuery) {
            delete mQuery;
        }
        delete mDb;
    }
    mDb = NULL;
    mQuery = NULL;
}

bool FinanceSqliteDataSource::read(FinanceRow &row)
{
    if(!isOpen()) {
        if(!open())
            return false;
    }

    if(mQuery->lastQuery() != querySql()) {
        mQuery->finish();
        if(!mQuery->exec(querySql())) {
            qWarning() << "Failed to execute sql: " << querySql();
            return false;
        }
    }
    row.clearData();
    if(!mQuery->next())
        return false;

    row.setTimestamp(mQuery->value(0).toDateTime());
    for( int i = 1; ; i ++ ) {
        QVariant var = mQuery->value(i);
        if(!var.isValid()) {
            break;
        }
        row.appendData(var.toDouble());
    }

    return true;
}


// ============ FinanceIndexCache =============
FinanceIndexCache::FinanceIndexCache(const QString &dbFile)
    : mCacheLoaded(false)
    , mQueryIntoDb(NULL)
{
    FinanceSqliteDataSource::setDbFile(dbFile);
}

FinanceIndexCache::~FinanceIndexCache()
{
    if(mQueryIntoDb) {
        mQueryIntoDb->finish();
        delete mQueryIntoDb;
    }
}

void FinanceIndexCache::setFromDbQuerySql(const QString &sql)
{
    FinanceSqliteDataSource::setQuery(sql);
}

void FinanceIndexCache::setIntoDbQuerySql(const QString &sql)
{
    mSqlIntoDb = sql;
}

bool FinanceIndexCache::cache(IndexCacheDir dir, const QDateTime &timestamp, QVector<double>& v)
{
    if(mSqlIntoDb.isEmpty() || querySql().isEmpty()) {
        qDebug() << "FinanceIndexCache: I don't know how do I cache elements!";
        return false;
    }
    if(!isOpen()) {
        if(!open())
            return false;
    }

    if(dir == FROM_MEM) {
        if(!mCacheLoaded) {
            FinanceRow row;
            mIndexCacheMem.clear();
            while(read(row)) {
                mIndexCacheMem.insert(row.timestamp(), row.data());
            }
            mCacheLoaded = true;
        }
        v = mIndexCacheMem.value(timestamp);
        return !v.isEmpty();
    } else if(dir == INTO_DB) {
        if(mQueryIntoDb == NULL) {
            mQueryIntoDb = new QSqlQuery(*mDb);
        }
        mQueryIntoDb->prepare(mSqlIntoDb);
        mQueryIntoDb->addBindValue(QVariant(DATETIME2STR(timestamp)));
        for( int i = 0; i < v.size(); i ++ ) {
            mQueryIntoDb->addBindValue(QVariant(v.at(i)));
        }
        return mQueryIntoDb->exec();
    }
    return false;
}
