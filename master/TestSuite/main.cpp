#include <QApplication>

#include "../DataAccInterf/dataaccif.h"
#include "../DataAccInterf/financedatasource.h"
#include "../StrategyAnalysisMod/strategyanalysismod.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*
    FinanceSqliteDataSource db("/home/harry/localdb/fiona.db", "select * from ic1505_persec_view limit 5");
    FinanceRow row;
    while(db.read(row)) {
        FinanceRow::dumpRow(row);
    }
    db.setQuery("select * from ic1506_persec_view limit 5");
    while(db.read(row)) {
        FinanceRow::dumpRow(row);
    }

    FinanceSqliteDataSource db2("/home/harry/localdb/fiona.db", "select * from ic1506_persec_view limit 5");
    while(db2.read(row)) {
        FinanceRow::dumpRow(row);
    }
    // */
#if 0
    const QString verifyTradeDate = "select count(*) from ic1505_persec_view where DATE(DateTime)='%1'";
    DAIContext* ctx = DAICreate("/home/harry/localdb/fiona.db", verifyTradeDate, QTime(9, 30, 0), QTime(11, 30, 0), QTime(13, 30, 0), QTime(15, 30, 0));
    QVector<QDateTime> dtv;

    const QDateTime start = QDateTime::fromString("2015-04-22 09:30:00", "yyyy-MM-dd hh:mm:ss");
    const QDateTime end = QDateTime::fromString("2015-04-22 09:32:00", "yyyy-MM-dd hh:mm:ss");
    DAIGetTimeSequence(ctx, start, end, DAI_PERSEC, &dtv);

    QVector<QDateTime> emu_dtv;
    QDateTime xdtm = start;
    for( int i = 0; i < 10; i ++ ) {
        emu_dtv.append(xdtm);
        xdtm = xdtm.addSecs(1);
    }
    DAIDataTable tbl;
    tbl.mRowCount = 2;
    tbl.mColCount = 2;
    xdtm = start.addSecs(4);
    tbl.mData = new QVector<double>[tbl.mColCount];
    int x = 1;
    for( int i = 0; i < tbl.mColCount; i ++ ) {
        tbl.mDateTime.append(xdtm);
        xdtm = xdtm.addSecs(2);
        for( int j = 0; j < tbl.mRowCount; j ++ ) {
            tbl.mData[i].append(x++);
        }
    }
    DAIFillLostDataSequence(&tbl, emu_dtv);

    for( int i = 0 ; i < dtv.size(); i ++ ) {
        qDebug() << dtv[i].toString("yyyy-MM-dd hh:mm:ss");
    }

    DAIDestroy(ctx);
#endif

#if 1

#endif

#if 1
    SAMInit();
    // SAMStrategyIvy();
    SAMTestIndexSynchronizer();
#endif

    int x = a.exec();

    SAMUnInit();
    return x;
}
