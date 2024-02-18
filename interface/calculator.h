#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QLineEdit>
#include <QWidget>

#include "../controller/s21_controller.h"
#include "qcustomplot.h"

namespace Ui {
class Calculator;
}

class Calculator : public QWidget {
  Q_OBJECT

 public:
  explicit Calculator(QWidget *parent = nullptr);
  ~Calculator();

  void setDefaultFocus();
  void setDefaultSizes();
  void nullDefaultSizes();

 private slots:
  void calculateResult();
  void setRange();
  void changeRangeX(const QCPRange &range);
  void changeRangeY(const QCPRange &range);
  void plotGraph();

  void setDegree();
  void setRadian();
  void inputLineText(const QString str);
  void deleteLineText();
  void backspaceLineText();

 private:
  Ui::Calculator *ui;
  s21::Controller ctrl;

  const double MIN_PLOT_RANGE = -1000000.0;
  const double MAX_PLOT_RANGE = 1000000.0;
  const double MIN_PLOT_DECIMAL = 1e-08;

  double min_x = 0.0, max_x = 0.0, min_y = 0.0, max_y = 0.0;
  int size;
  QVector<double> x{NAN}, y{NAN};

  /* Initial set */
  void initializeGraph();
  void connectSignals();

  /* Misc */
  QString doubleToQString(double value);
  bool hasDot(const QString str);
  bool hasExp(const QString str);
  int findExp(const QString str);
  bool isLimitsChanged();
  QString appendLeftPar(const QString str);
};

#endif  // CALCULATOR_H
