#include "tradeexecmod.h"
#include <QDebug>

#define DEF_LEVERAGE_RATIO 10.0

FuturesContract::FuturesContract(quint32 symbol)
    : mSymbol(symbol)
    , mLeverRatio(DEF_LEVERAGE_RATIO)
{
    initVars();
}

FuturesContract::~FuturesContract()
{

}

void FuturesContract::initVars()
{
    mLeverRatio = DEF_LEVERAGE_RATIO;
    mTotalGuaranteePriz = mUnitGuaranteePriz = 0.0;
    mOpenTotalPriz = mOpenUnitPriz = 0.0;
    mCloseTotalPriz = mCloseUnitPriz = 0.0;
    mLots = 0;
    mType = InvalidTransaction;
    mGain = 0.0;
}

double FuturesContract::guranteePrice(double priz)
{
    return priz/DEF_LEVERAGE_RATIO;
}

double FuturesContract::factorage() const
{
    return (mOpenTotalPriz/10000.0)*0.25;
}

bool FuturesContract::openPosition(FuturesTransactType ty, double openUnitPrice, quint32 lots)
{
    if(lots <= 0 || ty <= InvalidTransaction || ty >= TransactionMax) {
        qCritical() << "Invalid openPosition args";
        return false;
    }

    mType = ty;
    mLots = lots;

    mOpenUnitPriz = openUnitPrice;
    mOpenTotalPriz = openUnitPrice*mLots;

    mTotalGuaranteePriz = mOpenTotalPriz/mLeverRatio;
    mUnitGuaranteePriz  = mOpenUnitPriz/mLeverRatio;

    mOpenTime = QDateTime::currentDateTime();

    return true;
}

bool FuturesContract::closePosition(double closeUnitPrice)
{
    if(mType <= InvalidTransaction || mType >= TransactionMax ) {
        qCritical() << "Invaid closePostion Args";
        return false;
    }

    mCloseUnitPriz = closeUnitPrice;
    mCloseTotalPriz = closeUnitPrice*mLots;

    if(mType == BullMarket) {
        mGain = mCloseTotalPriz - mOpenTotalPriz;
    } else if(mType == BearMarket) {
        mGain = mOpenTotalPriz - mCloseTotalPriz;
    }

    if(mGain < 0 ) {
        qDebug() << mSymbol << " gain lost: $" << mGain;
    }

    mCloseTime = QDateTime::currentDateTime();
    return true;
}

// ============ Arbitrager =================
Arbitrager::Arbitrager(double totalMoney)
    : mMoneyPool(totalMoney)
    , mStop(false)
{

}

Arbitrager::~Arbitrager()
{
    if(mContracts.size() > 0 ) {
        qCritical() << "There is still some contracts, couldn't shutdown the arbitrager!";
    }
}

AbtError Arbitrager::openPosition(quint32 symbol, double openUnitPrice, FuturesTransactType tty, quint32 lots)
{
    FuturesContract* fc = new FuturesContract(symbol);
    if(!fc->openPosition(tty, openUnitPrice, lots)) {
        delete fc;
        return InvalidArgs;
    }
    if(!mMoneyPool.pull(fc->totalGuaranteePrice())) {
        delete fc;
        return NotEnoughMoney;
    }
    mContracts.insertMulti(symbol, fc);
    return NoErr;
}

AbtError Arbitrager::closePosition(quint32 symbol, double closeUnitPrice)
{
    if(!mContracts.contains(symbol))
        return NotExists;
    QList<FuturesContract*> conts = mContracts.values(symbol);
    for( int i = 0; i < conts.size(); i ++ ) {
        FuturesContract* fc = conts.at(i);
        if(!fc->closePosition(closeUnitPrice)) {
            qCritical() << "Close a NOT opened contract!";
            delete fc;
            return NotOpenErr;
        } else {
            double g = fc->gain();
            g += fc->totalGuaranteePrice();
            g -= fc->factorage();
            if(g < 0.0 ) {
                qCritical() << "oops!!!!!!";
            }
            mMoneyPool.push(g);
        }
        delete fc;
    }
    mContracts.remove(symbol);
    return NoErr;
}

FuturesTransactType Arbitrager::transactType(quint32 symbol)
{
    FuturesContract* fc = mContracts.value(symbol, NULL);
    if( fc == NULL) {
        return InvalidTransaction;
    }
    return fc->trasactType();
}

// MoneyPool
MoneyPool::MoneyPool(double total)
    : mTotalInit(total)
    , mCurrent(total)
{
    qDebug() << "Money Pool Initialized: $" << total;
}

MoneyPool::~MoneyPool()
{

}

bool MoneyPool::pull(double amount)
{
    if(mCurrent - amount < 0.0) {
        qCritical() << "Not enough money!";
        return false;
    }
    if(amount < 0 ) {
        qCritical() << "MoneyPool::pull bad arg";
        return false;
    }
    mCurrent -= amount;
    return true;
}

bool MoneyPool::push(double amount)
{
    if(amount < 0 ) {
        qCritical("MoneyPool:push, bad arg");
        return false;
    }
    mCurrent += amount;
    return true;
}
