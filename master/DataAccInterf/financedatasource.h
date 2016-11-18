#ifndef FINANCEDATASOURCE_H
#define FINANCEDATASOURCE_H

#include "dataaccessinterf_global.h"
#include <QVector>
#include <QDateTime>
#include <QMap>

class QSqlDatabase;
class QSqlQuery;
class DAI_EXPORT FinanceRow
{
public:
    FinanceRow(const QDateTime& timestamp, const QVector<double>& data)
        : mTimestamp(timestamp), mIndexData(data) {}
    FinanceRow() {}
    ~FinanceRow() {}

private:
    QDateTime mTimestamp;
    QVector<double> mIndexData;

public:
    void setTimestamp(const QDateTime& timestamp) { mTimestamp = timestamp; }
    QDateTime& timestamp() const { return (QDateTime&)mTimestamp; }
    void setData(const QVector<double>& data);
    void appendData(double v) { mIndexData.append(v); }
    void clearData() { mIndexData.clear(); }
    QVector<double>& data() const { return (QVector<double>&)mIndexData; }

    static void dumpRow(const FinanceRow& row);
};

class DAI_EXPORT FinanceDataSource
{
public:
    FinanceDataSource(const QString& query)
        : mQuerySql(query) {}
    FinanceDataSource() {}
    virtual ~FinanceDataSource() {}

protected:
    QString mQuerySql;

public:
    virtual bool setQuery(const QString& q) {
        mQuerySql = q;
        return true;
    }
    QString querySql() const { return mQuerySql; }

    virtual bool isOpen() const = 0;
    virtual bool open() = 0;
    virtual void close() = 0;

    virtual bool read(FinanceRow& row) = 0;
    virtual bool write(FinanceRow& row) { return false; }
};

class DAI_EXPORT FinanceSqliteDataSource
        : public FinanceDataSource
{
public:
    FinanceSqliteDataSource(const QString& dbFile, const QString& querySql);
    FinanceSqliteDataSource(const QString& querySql);
    FinanceSqliteDataSource();
    virtual ~FinanceSqliteDataSource();

protected:
    QSqlDatabase* mDb;
    QSqlQuery* mQuery;
    QString mDbFile;
    bool mTransactionStarted;
public:
    void setDbFile(const QString& dbFile);
    virtual bool setQuery(const QString& q);
    void resetQuery();

    bool transaction();
    bool commit();
    bool transactionStarted() const;

    virtual bool isOpen() const;
    virtual bool open();
    virtual void close();
    virtual bool read(FinanceRow &row);
};

enum IndexCacheDir {
    INTO_DB,
    FROM_MEM
};

class DAI_EXPORT FinanceIndexCache
        : public FinanceSqliteDataSource
{
public:
    FinanceIndexCache(const QString& dbFile);
    virtual ~FinanceIndexCache();

private:
    QMap<QDateTime, QVector<double> > mIndexCacheMem;
    QString mSqlIntoDb;
    QSqlQuery* mQueryIntoDb;
    bool mCacheLoaded;

public:
    void setFromDbQuerySql(const QString& sql);
    void setIntoDbQuerySql(const QString& sql);

    bool cache(IndexCacheDir dir, const QDateTime& timestamp, QVector<double>& v);
};

#endif // FINANCEDATASOURCE_H
