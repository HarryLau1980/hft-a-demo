#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include "qcustomplot.h"
#include "graphcurve.h"
#include <QDialog>
#include <QVector>
#include <QMap>

namespace Ui {
class PlotDialog;
}

class PlotDialog
     : public QDialog
{
    Q_OBJECT

public:
    explicit PlotDialog(QWidget *parent = 0);
    ~PlotDialog();

public:
    QCustomPlot* plot() const;
    static QColor getRandColor();

private:
    Ui::PlotDialog *ui;
};

#endif // PLOTDIALOG_H
