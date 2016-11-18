#ifndef INDEXSEQUENCESET_H
#define INDEXSEQUENCESET_H

#include <QVector>
#include <QString>
#include <QMap>
#include <QVariant>

#define Q_APPEND(_vect, _val, _q_size, _q_full) { \
        if(_vect.size() < _q_size) \
            _vect.append(_val); \
        else { \
            _vect.append(_val); \
            _vect.removeFirst(); \
        } \
        _q_full = (_vect.size >= _q.size()); \
    }
class Range
{
public:
    Range(double low, double high)
        : mLow(low), mHigh(high) {}
    ~Range() {}
private:
    double mLow;
    double mHigh;
public:
    double low() const { return mLow; }
    double high() const { return mHigh; }
};

class Sequence
{
public:
    Sequence(int reservedSize = 0, Qt::PenStyle ps = Qt::SolidLine, int penWidth = 1);
    ~Sequence();

private:
    QVector<double> mData;
    QMap<QString, QVariant> mExtData;
    QVariant mEmptyVar;

    bool mDrawable;
    int  mCurvePenWidth;
    Qt::PenStyle mCurvePenStyle;

    bool mIsFltCurve;

public:
    QVector<double>& data() const { return (QVector<double>&)mData; }
    void setData(const QVector<double>& data) { mData = data; }

    int  dataSize() const { return mData.size(); }

    bool drawable() const { return mDrawable; }
    void setDrawable(bool drawable = true) { mDrawable = drawable; }

    bool isFltCurve() const { return mIsFltCurve; }
    void setFltCurve(bool flt = true) { mIsFltCurve = flt; }

    Qt::PenStyle  curvePenStyle() const { return mCurvePenStyle; }
    int curvePenWidth() const { return mCurvePenWidth; }

    void setExtData(const QString& name, const QVariant& var);
    QVariant extData(const QString& name) const;
};

// =========== IndexSequenceSet =============
class IndexSequenceSet
{
public:
    IndexSequenceSet();
    ~IndexSequenceSet();

private:
    QMap<QString, Sequence* > mSeqs;
    QVector<double> mEmpty;

public:
    void clear();
    bool addSeq(const QString& name, int reservedSize = 0, bool drawable = false, int penWidth=1, Qt::PenStyle ps = Qt::SolidLine);

    QVector<double>& seqData(const QString& name, bool *ok = NULL) ;
    void setSeqExtData(const QString& seqName, const QString& xDataName, const QVariant& var);
    QVariant seqExtData(const QString& seqName, const QString& xDataName);
    Sequence* seq(const QString& name);
    QMap<QString, Sequence*> allSeqs() const;

    bool isValid(const QVector<double>& v) { return v.size() > 0; }
    bool contains(const QString& key) const { return mSeqs.contains(key); }

    bool appendSeq(const QString& name, double v);
    int  size() const;

    // removeHeadOrTail: true, remove head; false tail; if size of seqs > size
    // return false, if one of size of seq < {size}
    bool trimSeqSize(int size, bool removeHeadOrTail = true);
};

#endif // INDEXSEQUENCESET_H
