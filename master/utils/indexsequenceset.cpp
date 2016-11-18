#include "indexsequenceset.h"

// ============ Sequence ==================
Sequence::Sequence(int reservedSize, Qt::PenStyle ps, int penWidth)
    : mDrawable(false)
    , mCurvePenStyle(ps)
    , mCurvePenWidth(penWidth)
    , mIsFltCurve(false)
{
    if(reservedSize > 0 ) {
        mData = QVector<double>(reservedSize);
    }
}

Sequence::~Sequence()
{

}

void Sequence::setExtData(const QString &name, const QVariant &var)
{
    if(name.isEmpty())
        return;
    mExtData[name] = var;
}

QVariant Sequence::extData(const QString& name) const
{
    QVariant var = mExtData.value(name, mEmptyVar);
    return var;
}


IndexSequenceSet::IndexSequenceSet()
{

}

IndexSequenceSet::~IndexSequenceSet()
{

}

void IndexSequenceSet::clear()
{
    QMap<QString, Sequence*>::iterator itr;
    for ( itr = mSeqs.begin(); itr != mSeqs.end(); itr ++ ) {
        Sequence* s = itr.value();
        delete s;
    }
    mSeqs.clear();
}

bool IndexSequenceSet::addSeq(const QString &name, int reservedSize, bool drawable, int penWidth, Qt::PenStyle ps)
{
    if(name.isEmpty()) {
        return false;
    }
    if(mSeqs.contains(name))
        return false;

    Sequence* s = new Sequence(reservedSize, ps, penWidth);
    s->setDrawable(drawable);
    mSeqs[name] = s;
    return false;
}

QVector<double> &IndexSequenceSet::seqData(const QString &name, bool *ok)
{
    if(ok) *ok = false;
    Sequence* s = mSeqs.value(name, NULL);
    if(s == NULL) {
        return mEmpty;
    }
    if(ok) *ok = true;
    return s->data();
}

void IndexSequenceSet::setSeqExtData(const QString &seqName, const QString &xDataName, const QVariant &var)
{
    Sequence* s = mSeqs.value(seqName, NULL);
    if(s == NULL)
        return;
    s->setExtData(xDataName, var);
}

QVariant IndexSequenceSet::seqExtData(const QString &seqName, const QString &xDataName)
{
    Sequence* s = mSeqs.value(seqName, NULL);
    if(s == NULL)
        return QVariant();
    return s->extData(xDataName);
}

Sequence *IndexSequenceSet::seq(const QString &name)
{
    return mSeqs.value(name, NULL);
}

QMap<QString, Sequence *> IndexSequenceSet::allSeqs() const
{
    return mSeqs;
}

bool IndexSequenceSet::appendSeq(const QString &name, double v)
{
    if(name.isEmpty())
        return false;
    Sequence* s = mSeqs.value(name, NULL);
    if(s == NULL)
        return false;
    s->data().append(v);
    return true;
}

int IndexSequenceSet::size() const
{
    Sequence* s = mSeqs.first();
    if( s )
        return s->data().size();
    return 0;
}

bool IndexSequenceSet::trimSeqSize(int size, bool removeHeadOrTail)
{
    if(size <= 0 ) {
        return false;
    }
    int xsize = 0;
    bool ok = true;
    QMap<QString, Sequence* >::iterator itr;
    for( itr = mSeqs.begin(); itr != mSeqs.end(); itr ++ ) {
        Sequence* s = itr.value();
        QVector<double>& vect = s->data();
        xsize = vect.size() - size;
        if(xsize < 0 ) {
            ok = false;
        } else {
            if(removeHeadOrTail) {
                // from head
                while(xsize-- > 0 ) {
                    vect.removeFirst();
                }
            } else {
                // from tail
                while(xsize-- > 0 ) {
                    vect.removeLast();
                }
            }
        }
    }
    return ok;
}




