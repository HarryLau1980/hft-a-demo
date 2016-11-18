#include "plotdialog.h"
#include "ui_plotdialog.h"
#include <QPalette>
#include <QDebug>

PlotDialog::PlotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotDialog)
{
    ui->setupUi(this);

    setWindowOpacity(0.8);
    const QColor crBkgnd = QColor(0x33, 0x33, 0x33);
    setAutoFillBackground(true);
    QPalette pal;
    pal.setColor(QPalette::Background, crBkgnd);
    setPalette(pal);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |QCP::iSelectPlottables);
    ui->plot->setBackground(QBrush(crBkgnd));
}

PlotDialog::~PlotDialog()
{
    delete ui;
}

QColor PlotDialog::getRandColor()
{
    static QColor *cr_table = NULL;
    static int nColors = 50;
    static int cr_index = 0;
    if(cr_table == NULL) {
        cr_table = new QColor[nColors];
        srand(time(NULL));
        int h, s, l;
        int ls = 0, lh = 0, ll = 0;
        int idx = 0;
        h = rand()%360;
        for( int i = 0;;i += 40 ) {
            if( i >= 360) {
                i = 0;
            }
            while(abs(h - lh) < 20 && abs(s - ls) < 15 && abs(l - ll) < 15) {
                h = (i+h)%360;
                s = 100 + (rand() + i)%155;
                l = 100 + (rand() + i)%155;
            }
            lh = h; ls = s; ll = l;
            QColor cr = QColor::fromHsl(h, s, l);
            if(cr.red() < 20 && cr.green() < 20 && cr.blue() < 20) {
                continue;
            } else {
                cr_table[idx++] = cr;
            }
            if(idx >= nColors)
                break;
        }
    }
    QColor cr = cr_table[cr_index];
    ++ cr_index;
    if(cr_index >= nColors) {
        cr_index = 0;
    }
    return cr;
}

QCustomPlot *PlotDialog::plot() const
{
    return ui->plot;
}
