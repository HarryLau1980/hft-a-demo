#ifndef LOCALINDEXSTREAM_H
#define LOCALINDEXSTREAM_H

#include "indexstream.h"
#include "../DataAccInterf/financedatasource.h"
#include "../utils/indexsequenceset.h"

// ============ LocalDbIndexStream ==================
class LocalIndexStream
        : public IndexStream
{
public:
    LocalIndexStream(quint32 symbol, const QString& url, IndexSynchronizer* syn);
    virtual ~LocalIndexStream();
private:
    FinanceSqliteDataSource* mDb;
    QString mSqlQuery;

    QVector<IndexTick> mIndexTicksLocal;
protected:
    virtual bool read(IndexTick& tick);

public:
    void query(const QString& query, QDateTime startTime = QDateTime(), QDateTime endTime = QDateTime());
};



#endif // LOCALINDEXSTREAM_H
