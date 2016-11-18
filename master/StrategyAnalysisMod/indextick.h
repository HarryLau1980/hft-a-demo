#ifndef INDEXTICK_H
#define INDEXTICK_H

#define FIN_INDEX 0
#define INVALID_INDEX -2

#include <QDateTime>
#include <QVector>

class IndexTick
{
public:
    IndexTick(quint32 symbol, const QVector<double>& v, const QDateTime& timestamp)
        : mSymbol(symbol)
        , mTimeStamp(timestamp)
        , mIndexVal(v) {
    }
    IndexTick() {}
    IndexTick(quint32 symbol)
        : mSymbol(symbol) {}
    ~IndexTick() {}

private:
    quint32  mSymbol;
    QDateTime mTimeStamp;
    QVector<double> mIndexVal;

public:
    inline void setSymbol(quint32 symbol) { mSymbol = symbol; }
    inline quint32 symbol() const { return mSymbol; }
    inline bool isFinIndex() const { return mSymbol == FIN_INDEX; }
    inline bool isInvalid() const { return mSymbol == INVALID_INDEX; }
    static IndexTick FinIndex() {
        QDateTime InvalidDateTime = QDateTime::fromString("2118-01-01 00:00:00", "yyyy-MM-dd hh:mm:ss");
        return IndexTick(FIN_INDEX, QVector<double>(), InvalidDateTime);
    }

    void reset() { mIndexVal.clear(); }
    inline void setTimeStamp(const QDateTime& ts) { mTimeStamp = ts; }
    inline QDateTime& timeStamp() const { return (QDateTime&)mTimeStamp; }

    inline void appendIndex(double v) { mIndexVal.append(v); }
    inline void setIndex(const QVector<double>& v) { mIndexVal = v; }
    inline QVector<double>& index() const { return (QVector<double>&)mIndexVal; }
};

#endif // INDEXTICK_H
