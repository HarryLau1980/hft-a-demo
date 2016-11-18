#ifndef TRADEEXECMOD_H
#define TRADEEXECMOD_H

#include "tradeexecmod_global.h"
#include <QDateTime>
#include <QVector>
#include <QMap>

enum FuturesTransactType
{
    InvalidTransaction,
    BullMarket,
    BearMarket,
    TransactionMax
};

class TEM_EXPORT FuturesContract
{
public:
    FuturesContract(quint32 symbol);
    ~FuturesContract();

private:
    quint32 mSymbol;
    FuturesTransactType mType;
    quint32  mLots;

    double  mOpenTotalPriz; // price of contract
    double  mOpenUnitPriz;
    double  mCloseTotalPriz; // price at time of closure
    double  mCloseUnitPriz;
    double  mTotalGuaranteePriz;
    double  mUnitGuaranteePriz;

    QDateTime mOpenTime;
    QDateTime mCloseTime;
    QDateTime mDeliveryTime; // delivery time of futures

    double  mLeverRatio;
    double  mGain;

private:
    void initVars();
public:
    quint32 symbol() const { return mSymbol; }
    FuturesTransactType trasactType() const { return mType; }

    double openTotalPrice() const { return mOpenTotalPriz; }
    double openUnitPrice() const { return mOpenUnitPriz; }

    double closeTotalPrice() const { return mCloseTotalPriz; }
    double closeUnitPrice() const { return mCloseUnitPriz; }

    quint32 lots() const { return mLots; }
    QDateTime& openTime() const { return (QDateTime&)mOpenTime; }
    QDateTime& closeTime() const { return (QDateTime&)mCloseTime; }

    static double guranteePrice(double priz);

    double totalGuaranteePrice() const { return mTotalGuaranteePriz; }
    double leverRatio() const { return mLeverRatio; }
    double gain() const { return mGain; }
    double factorage() const;
    bool isOpen() const { return mOpenTime.isValid(); }
    bool isClose() const { return mCloseTime.isValid(); }

public:
    void setDeliveryTime(const QDateTime& time) { mDeliveryTime = time; }

public:
    bool openPosition(FuturesTransactType ty, double openUnitPrice, quint32 lots);
    bool closePosition(double closeUnitPrice);
};

class TEM_EXPORT MoneyPool
{
public:
    MoneyPool(double total);
    ~MoneyPool();

public:
    double mTotalInit;
    double mCurrent;

public:
    double total() const { return mTotalInit; }
    double current() const { return mCurrent; }
    bool   pull(double amount);
    bool   push(double amount);
    double gain() const { return mCurrent - mTotalInit; }
    double gainPct() const { return ((mCurrent - mTotalInit)/mTotalInit)*100; }
};

enum AbtError
{
    NoErr = 0,
    NotEnoughMoney = -1,
    NetworkErr = -2,
    NotOpenErr = -3,
    NotExists = -4,
    InvalidArgs = -5,
    UnknownErr = -100
};

class TEM_EXPORT Arbitrager
{
public:
    Arbitrager(double totalMoney);
    ~Arbitrager();

private:
    MoneyPool mMoneyPool;
    QMap<quint32, FuturesContract*> mContracts;
    bool mStop;

public:
    AbtError openPosition(quint32 symbol, double openUnitPrice, FuturesTransactType tty, quint32 lots);
    AbtError closePosition(quint32 symbol, double closeUnitPrice);

    FuturesTransactType transactType(quint32 symbol);

    bool stoped() const { return mStop; }
    int  contractsCount() const { return mContracts.size(); }
    double gain() const { return mMoneyPool.gain(); }

    MoneyPool& moneyPool() const { return (MoneyPool&)mMoneyPool; }
};



#endif // TRADEEXECMOD_H
