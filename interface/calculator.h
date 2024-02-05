#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLineEdit>

//#include <sstream>
//#include <iomanip>

#include "../controller/s21_controller.h"
#include "qcustomplot.h"

#define MIN_PLOT_RANGE -1000000.0
#define MAX_PLOT_RANGE 1000000.0

namespace Ui {
class Calculator;
}

class Calculator : public QWidget {
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = nullptr);
    ~Calculator();

    void setDefaultFocus();

private slots:
    void inputLineText(const QString str);
    void deleteLineText();
    void backspaceLineText();
    void calculateResult();
    void plotGraph();
    void setDegree();
    void setRadian();
    void setRange();
    void changeRangeX(const QCPRange &range);
    void changeRangeY(const QCPRange &range);

private:
    Ui::Calculator *ui;
    s21::Controller ctrl;
    double min_x = 0.0, max_x = 0.0,
        min_y = 0.0, max_y = 0.0;
    int size;
    QVector<double> x, y;


    bool hasDot(const QString str);
    bool hasExp(const QString str);
    int findExp(const QString str);
    void initializeGraph();
    bool isLimitsChanged();
    QString doubleToQString(double value);
    const QString appendLeftPar(const QString str);
};

#endif // CALCULATOR_H
