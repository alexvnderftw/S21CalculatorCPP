#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLineEdit>

#include <sstream>

#include "../controller/s21_controller.h"

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

private:
    Ui::Calculator *ui;
    s21::Controller ctrl;

    QString doubleToQString(double value);
    const QString appendLeftPar(const QString str);
};

#endif // CALCULATOR_H
