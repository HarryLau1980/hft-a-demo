#include "graphcurve.h"
#include "plotdialog.h"

#define AXIS_X(axis) (((axis)&0xff00) >> 8)
#define AXIS_Y(axis) ((axis&0xff))


GraphCurveWindow::GraphCurveWindow(const QString& title)
    : mDialog(NULL)
{
    mDialog = new PlotDialog();
    mDialog->setWindowTitle(title);
}

GraphCurveWindow::~GraphCurveWindow()
{
    if(mDialog) {
        removeAll();
        mDialog->hide();
        mDialog->deleteLater();
    }
}

void GraphCurveWindow::plot()
{

}

void GraphCurveWindow::createGraph(int axis, const QString &name, Sequence *s)
{
    if(!s->drawable())
        return;
    s->setExtData("axis", QVariant(axis));
    s->setExtData("name", QVariant(name));
    int xAxis = AXIS_X(axis);
    int yAxis = AXIS_Y(axis);
    QVector<double>* xkey = NULL;
    QCPAxis* xa, *ya;
    QCustomPlot* plt = mDialog->plot();
    xa = ((xAxis == XBOTTOM) ? plt->xAxis : plt->xAxis2);
    ya = ((yAxis == YLEFT)   ? plt->yAxis : plt->yAxis2);
    xkey = ((xAxis == XBOTTOM) ? &mXBottom : &mXTop);
    xa->setVisible(true);
    ya->setVisible(true);

    QCPGraph* g = new QCPGraph(xa, ya);
    g->setName(name);
    s->setExtData("graph", QVariant((qlonglong)g));
    g->setData(*xkey, s->data());
    QPen pen(PlotDialog::getRandColor());
    pen.setStyle(s->curvePenStyle());
    pen.setWidth(s->curvePenWidth());
    g->setPen(pen);
}

int GraphCurveWindow::getSequenceSetAxis(IndexSequenceSet *seqSet)
{
    QMap<int, IndexSequenceSet*>::const_iterator itr;
    int axis = -1;
    for( itr = mSeqs.constBegin(); itr != mSeqs.constEnd(); itr ++ ) {
        IndexSequenceSet* ss = itr.value();
        if(ss == seqSet) {
            axis = itr.key();
            break;
        }
    }
    return axis;
}


void GraphCurveWindow::show(bool bShow)
{
    if(bShow) {
        mDialog->show();
    } else {
        mDialog->hide();
    }
}


bool GraphCurveWindow::addCurveFamily(int axis, IndexSequenceSet *seqSet, Range* rge)
{
    int xAxis = AXIS_X(axis);
    int yAxis = AXIS_Y(axis);
    if(xAxis < 1 || xAxis > 2 || yAxis < 1 || yAxis > 2 || !seqSet)
        return false;

    QVector<double>* xkey = NULL;
    if(xAxis == XBOTTOM) {
        xkey = &mXBottom;
    } else {
        xkey = &mXTop;
    }
    QMap<QString, Sequence* > seqMap = seqSet->allSeqs();
    QMap<QString, Sequence* >::const_iterator itr;

    int ySize = 0;
    for( itr = seqMap.constBegin(); itr != seqMap.constEnd(); itr ++ ) {
        Sequence* s = itr.value();
        if(ySize < s->dataSize() && s->drawable()) {
            ySize = s->dataSize();
        }
    }
    xkey->clear();
    for( register int i = 0; i < ySize; i ++ ) {
        xkey->append(i);
    }

    mSeqs[axis] = seqSet;
    for( itr = seqMap.constBegin(); itr != seqMap.constEnd(); itr ++ ) {
        QString name = itr.key();
        Sequence* s = itr.value();
        if(s->drawable()) {
            createGraph(axis, name, s);
        }
    }

    if(rge) {
        QCPAxis* ay = ((yAxis == YLEFT) ? mDialog->plot()->yAxis : mDialog->plot()->yAxis2);
        ay->setRange(rge->low(), rge->high());
    }
    QCPAxis* ax = ((xAxis == XBOTTOM) ? mDialog->plot()->xAxis : mDialog->plot()->xAxis2);
    ax->setRange(xkey->first(), xkey->last());
    mDialog->plot()->rescaleAxes(true);
    mDialog->plot()->replot();
    return true;
}

bool GraphCurveWindow::updateCurveFamily(IndexSequenceSet* seqSet)
{
   int axis = getSequenceSetAxis(seqSet);
   if(axis < 0 )
       return false;

   int xAxis = AXIS_X(axis);
   QVector<double>* xkey = ((xAxis == XBOTTOM) ? &mXBottom : &mXTop);

   QMap<QString, Sequence*> seqMap = seqSet->allSeqs();
   QMap<QString, Sequence*>::const_iterator itr;
   for( itr = seqMap.constBegin(); itr != seqMap.constEnd(); itr ++ ) {
        Sequence* s = itr.value();
        if(s->drawable()) {
            QCPGraph* g = (QCPGraph*)s->extData("graph").toULongLong();
            g->setData(*xkey, s->data());
        }
   }

   mDialog->plot()->replot();
   return true;
}

bool GraphCurveWindow::updateRealtimeFltCurveFamily(IndexSequenceSet *seqSet, int nFlrWinSize)
{
    if(seqSet == NULL || nFlrWinSize <= 0 )
        return false;
    int axis = getSequenceSetAxis(seqSet);
    if(axis < 0 )
        return false;
    int xAxis = AXIS_X(axis);
    int yAxis = AXIS_Y(axis);
    QVector<double>* xkey = ((xAxis == XBOTTOM) ? &mXBottom : &mXTop);
    const double xkeyLast = xkey->last();
    const double xkeyFirst = xkey->first();

    QMap<QString, Sequence*> seqMap = seqSet->allSeqs();
    QMap<QString, Sequence*>::ConstIterator itr;

    int ySize = 0, count = 0;
    double tXMax;
    QCPDataMap* dp = NULL;
    for( itr = seqMap.constBegin(); itr != seqMap.constEnd(); itr ++ ) {
        Sequence* s = itr.value();
        QVector<double>& y = s->data();
        if(!s->drawable() || y.size() < nFlrWinSize) {
            continue;
        }

        QCPGraph* g = (QCPGraph*)(s->extData("graph").toULongLong());
        dp = g->data();
        dp->remove(xkeyFirst);

        count = nFlrWinSize/2;
        tXMax = xkeyLast;
        // remove tail
        while(count--) {
            dp->remove(tXMax);
            tXMax -= 1.0;
        }
        tXMax += 1.0;
        ySize = y.size() - 1;
        count = nFlrWinSize/2 + 1;
        // append tails
        while(count ) {
            dp->insertMulti(tXMax, QCPData(tXMax, y[ySize - count]));
            count --;
            tXMax += 1.0;
        }
    }

    QCPAxis* ax = ((xAxis == XBOTTOM) ? mDialog->plot()->xAxis : mDialog->plot()->xAxis2);
    xkey->removeFirst();
    xkey->append(xkeyLast + 1.0);
    ax->setRange(xkey->first(), xkey->last());
    mDialog->plot()->replot(QCustomPlot::rpQueued);
    return true;
}

bool GraphCurveWindow::updateHeartbeatCurveFamily(IndexSequenceSet *seqSet)
{
    if(seqSet == NULL)
        return false;
    int axis = getSequenceSetAxis(seqSet);
    if(axis < 0 )
        return false;
    int xAxis = AXIS_X(axis);
    int yAxis = AXIS_Y(axis);
    QVector<double>* xkey = ((xAxis == XBOTTOM) ? &mXBottom : &mXTop);
    const double xkeyLast = xkey->last();
    const double xkeyFirst = xkey->first();

    QMap<QString, Sequence*> seqMap = seqSet->allSeqs();
    QMap<QString, Sequence*>::ConstIterator itr;

    QCPDataMap* dp = NULL;
    for( itr = seqMap.constBegin(); itr != seqMap.constEnd(); itr ++ ) {
        Sequence* s = itr.value();
        QVector<double>& y = s->data();
        if(!s->drawable()) {
            continue;
        }

        QCPGraph* g = (QCPGraph*)(s->extData("graph").toULongLong());
        dp = g->data();
        dp->remove(xkeyFirst);
        dp->insert(xkeyLast + 1.0, QCPData(xkeyLast+1.0, y.last()));
    }

    QCPAxis* ax = ((xAxis == XBOTTOM) ? mDialog->plot()->xAxis : mDialog->plot()->xAxis2);
    xkey->removeFirst();
    xkey->append(xkeyLast + 1.0);
    ax->setRange(xkey->first(), xkey->last());
    mDialog->plot()->replot(QCustomPlot::rpQueued);
    return true;
}

void GraphCurveWindow::removeAll()
{
   mSeqs.clear();
   mXBottom.clear();
   mXTop.clear();
}
