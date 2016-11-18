#ifndef LEDCTRL_H
#define LEDCTRL_H

#include <QWidget>

namespace Ui {
class LedCtrl;
}

class LedCtrl : public QWidget
{
    Q_OBJECT

public:
    explicit LedCtrl(QWidget *parent = 0);
    ~LedCtrl();

private:
    Ui::LedCtrl *ui;
};

#endif // LEDCTRL_H
