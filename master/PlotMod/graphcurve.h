#ifndef GRAPHCURVE_H
#define GRAPHCURVE_H

#include "plotmod_global.h"
#include "../utils/indexsequenceset.h"
#include <QMap>
#include <QVector>
#include <QString>

#define XBOTTOM 1
#define XTOP    2
#define YLEFT   1
#define YRIGHT  2

#define CURVE_AXIS(x,y) (((x) << 8) | (y))

class QCPGraph;
class PlotDialog;
class PM_EXPORT GraphCurveWindow
{
public:
    GraphCurveWindow(const QString& title);
    ~GraphCurveWindow();

private:

    QVector<double> mXBottom;
    QVector<double> mXTop;
    PlotDialog* mDialog;
    QMap<int, IndexSequenceSet* > mSeqs;

private:
    void plot();
    void createGraph(int axis, const QString& name, Sequence* s);

    int  getSequenceSetAxis(IndexSequenceSet* seqSet);

public:
    void show(bool bShow = true);

    bool addCurveFamily(int axis, IndexSequenceSet* seqSet, Range* rge = NULL);
    bool updateCurveFamily(IndexSequenceSet* seqSet);
    bool updateRealtimeFltCurveFamily(IndexSequenceSet* seqSet, int nFlrWinSize);
    bool updateHeartbeatCurveFamily(IndexSequenceSet* seqset);

    bool removeCurveFamily(IndexSequenceSet* seqSet);
    void removeAll();
};

#endif // GRAPHCURVE_H
