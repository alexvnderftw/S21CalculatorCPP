#include "calculator.h"

#include "ui_calculator.h"

Calculator::Calculator(QWidget *parent)
    : QWidget(parent), ui(new Ui::Calculator) {
  ui->setupUi(this);
  ctrl.setRadian();
  initializeGraph();
  connectSignals();
}

Calculator::~Calculator() { delete ui; }

void Calculator::setDefaultFocus() { ui->lineEditExpression->setFocus(); }

void Calculator::setDefaultSizes() {
  ui->widgetCalculator->setMinimumWidth(335);
  ui->widgetCalculator->setMinimumHeight(426);
  ui->widgetPlot->setMinimumWidth(400);
  ui->widgetPlot->setMinimumHeight(400);
}

void Calculator::nullDefaultSizes() {
  ui->widgetCalculator->setMinimumWidth(0);
  ui->widgetCalculator->setMinimumHeight(0);
  ui->widgetPlot->setMinimumWidth(0);
  ui->widgetPlot->setMinimumHeight(0);
}

void Calculator::initializeGraph() {
  size = ui->widgetPlot->width();
  x.resize(size);
  y.resize(size);
  ui->widgetPlot->setBackground(QColor(238, 237, 247, 255));
  ui->widgetPlot->xAxis->setLabelFont(QFont("Anta", 14));
  ui->widgetPlot->yAxis->setLabelFont(QFont("Anta", 14));
  ui->widgetPlot->xAxis->setTickLabelFont(QFont("Kode Mono"));
  ui->widgetPlot->yAxis->setTickLabelFont(QFont("Kode Mono"));
  ui->widgetPlot->xAxis->setNumberFormat("gb");
  ui->widgetPlot->yAxis->setNumberFormat("gb");
  //  ui->widgetPlot->xAxis->setTickLength(1, 1);
  //  ui->widgetPlot->yAxis->setTickLength(1, 1);
  ui->widgetPlot->xAxis->setSubTicks(true);
  ui->widgetPlot->yAxis->setSubTicks(true);
  //  ui->widgetPlot->xAxis->setNumberPrecision(2);
  //  ui->widgetPlot->yAxis->setNumberPrecision(2);
  ui->widgetPlot->setAttribute(Qt::WA_OpaquePaintEvent);
  ui->widgetPlot->setNoAntialiasingOnDrag(true);
  ui->widgetPlot->setInteraction(QCP::iRangeDrag, true);
  ui->widgetPlot->setInteraction(QCP::iRangeZoom, true);
  ui->widgetPlot->addGraph();
  ui->widgetPlot->graph(0)->setPen(QPen(Qt::black, 1));
  ui->widgetPlot->xAxis->setLabel("X axis");
  ui->widgetPlot->yAxis->setLabel("Y axis");
  setRange();
}

void Calculator::connectSignals() {
  connect(ui->pushButtonDelete, SIGNAL(clicked()), this,
          SLOT(deleteLineText()));
  connect(ui->pushButtonBackspace, SIGNAL(clicked()), this,
          SLOT(backspaceLineText()));
  connect(ui->pushButtonResult, SIGNAL(clicked()), this,
          SLOT(calculateResult()));
  connect(ui->lineEditExpression, SIGNAL(returnPressed()), this,
          SLOT(calculateResult()));
  connect(ui->lineEditX, SIGNAL(returnPressed()), this,
          SLOT(calculateResult()));
  connect(ui->radioButtonRad, SIGNAL(clicked()), this, SLOT(setRadian()));
  connect(ui->radioButtonDeg, SIGNAL(clicked()), this, SLOT(setDegree()));
  connect(ui->pushButtonPlot, SIGNAL(clicked()), this, SLOT(setRange()));
  connect(ui->doubleSpinBoxMinX, SIGNAL(editingFinished()), this,
          SLOT(setRange()));
  connect(ui->doubleSpinBoxMaxX, SIGNAL(editingFinished()), this,
          SLOT(setRange()));
  connect(ui->doubleSpinBoxMinY, SIGNAL(editingFinished()), this,
          SLOT(setRange()));
  connect(ui->doubleSpinBoxMaxY, SIGNAL(editingFinished()), this,
          SLOT(setRange()));

  connect(ui->widgetPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this,
          SLOT(changeRangeX(QCPRange)));
  connect(ui->widgetPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this,
          SLOT(changeRangeY(QCPRange)));

  connect(ui->pushButton0, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButton1, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButton2, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButton3, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButton4, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButton5, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButton6, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButton7, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButton8, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButton9, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButtonX, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButtonDot, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButtonLeftPar, SIGNAL(clicked()), this,
          SLOT(inputLineText()));
  connect(ui->pushButtonRightPar, SIGNAL(clicked()), this,
          SLOT(inputLineText()));
  connect(ui->pushButtonPlus, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButtonMinus, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButtonAltMinus, SIGNAL(clicked()), this,
          SLOT(inputLineText()));
  connect(ui->pushButtonMult, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButtonDiv, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButtonPow, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButtonMod, SIGNAL(clicked()), this, SLOT(inputLineText()));
  connect(ui->pushButtonLn, SIGNAL(clicked()), this,
          SLOT(inputLineTextFunction()));
  connect(ui->pushButtonLog, SIGNAL(clicked()), this,
          SLOT(inputLineTextFunction()));
  connect(ui->pushButtonSqrt, SIGNAL(clicked()), this,
          SLOT(inputLineTextFunction()));
  connect(ui->pushButtonSin, SIGNAL(clicked()), this,
          SLOT(inputLineTextFunction()));
  connect(ui->pushButtonCos, SIGNAL(clicked()), this,
          SLOT(inputLineTextFunction()));
  connect(ui->pushButtonTan, SIGNAL(clicked()), this,
          SLOT(inputLineTextFunction()));
  connect(ui->pushButtonAsin, SIGNAL(clicked()), this,
          SLOT(inputLineTextFunction()));
  connect(ui->pushButtonAcos, SIGNAL(clicked()), this,
          SLOT(inputLineTextFunction()));
  connect(ui->pushButtonAtan, SIGNAL(clicked()), this,
          SLOT(inputLineTextFunction()));
}

/* All slots */

/* Produce result and call plot graph */
void Calculator::calculateResult() {
  QString result = doubleToQString(
      ctrl.calculate(ui->lineEditExpression->text().toStdString(),
                     ui->lineEditX->text().toStdString()));
  if (ctrl.isEmpty() == true)
    result.clear();
  else if (ctrl.isSuccessful() == false)
    result = "Error";
  ui->lineEditResult->setText(result);
  setRange();
}

/* When called set limits from ui. If limits is changed plot graph by
 * "changeRange" signal, otherwise directly plot graph. */
void Calculator::setRange() {
  if (isLimitsChanged() == true) {
    min_x = ui->doubleSpinBoxMinX->value(),
    max_x = ui->doubleSpinBoxMaxX->value(),
    min_y = ui->doubleSpinBoxMinY->value(),
    max_y = ui->doubleSpinBoxMaxY->value();
    ui->widgetPlot->xAxis->setRange(min_x, max_x);
    ui->widgetPlot->yAxis->setRange(min_y, max_y);
  } else {
    plotGraph();
  }
}

/* Checks for limit boundaries on X axis and replot graph. */
void Calculator::changeRangeX(const QCPRange &range) {
  QCPRange new_range = range;
  if (new_range.upper - new_range.lower < 2 * MIN_PLOT_DECIMAL) {
    new_range.lower -= MIN_PLOT_DECIMAL;
    new_range.upper += MIN_PLOT_DECIMAL;
  } else if (new_range.lower < MIN_PLOT_RANGE) {
    new_range.lower = MIN_PLOT_RANGE;
    new_range.upper = MIN_PLOT_RANGE + new_range.size();
  } else if (new_range.upper > MAX_PLOT_RANGE) {
    new_range.upper = MAX_PLOT_RANGE;
    new_range.lower = MAX_PLOT_RANGE - new_range.size();
  }
  ui->widgetPlot->xAxis->setRange(new_range);
  min_x = new_range.lower;
  max_x = new_range.upper;
  plotGraph();
}

/* Checks for limit boundaries on Y axis and replot graph. */
void Calculator::changeRangeY(const QCPRange &range) {
  QCPRange new_range = range;
  if (new_range.upper - new_range.lower < 2 * MIN_PLOT_DECIMAL) {
    new_range.lower -= MIN_PLOT_DECIMAL;
    new_range.upper += MIN_PLOT_DECIMAL;
  } else if (new_range.lower < MIN_PLOT_RANGE) {
    new_range.lower = MIN_PLOT_RANGE;
    new_range.upper = MIN_PLOT_RANGE + new_range.size();
  } else if (new_range.upper > MAX_PLOT_RANGE) {
    new_range.upper = MAX_PLOT_RANGE;
    new_range.lower = MAX_PLOT_RANGE - new_range.size();
  }
  ui->widgetPlot->yAxis->setRange(new_range);
  min_y = new_range.lower;
  max_y = new_range.upper;
  plotGraph();
}

/* Slot to replot graph. */
void Calculator::plotGraph() {
  double step = (max_x - min_x) / size;
  for (int i = 0; i < size; ++i) {
    x[i] = min_x + i * step;
    y[i] = ctrl.calculate(x[i]);
  }
  ui->widgetPlot->graph(0)->setData(x, y, true);
  ui->widgetPlot->replot();
}

/* Set radian as measure */
void Calculator::setRadian() { ctrl.setRadian(); }

void Calculator::setDegree() { ctrl.setDegree(); }

void Calculator::inputLineText() {
  QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
  if (ui->lineEditX->hasFocus() == true) {
    ui->lineEditX->insert(buttonSender->text());
  } else {
    ui->lineEditExpression->insert(buttonSender->text());
  }
}

void Calculator::inputLineTextFunction() {
  QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
  if (ui->lineEditX->hasFocus() == true) {
    ui->lineEditX->insert(appendLeftPar(buttonSender->text()));
  } else {
    ui->lineEditExpression->insert(appendLeftPar(buttonSender->text()));
  }
}

void Calculator::deleteLineText() {
  if (ui->lineEditX->hasFocus() == true) {
    ui->lineEditX->clear();
  } else {
    ui->lineEditExpression->clear();
    ui->lineEditResult->clear();
  }
}

void Calculator::backspaceLineText() {
  if (ui->lineEditX->hasFocus() == true) {
    ui->lineEditX->backspace();
  } else {
    ui->lineEditExpression->backspace();
  }
}

/* Misc */

/* Convert double to QString with formatting.
Trails all useless zeros, up to 16 digits. */
QString Calculator::doubleToQString(double value) {
  QString str = QString::number(value, 'g', 16);
  if (hasDot(str) && !hasExp(str)) {
    while (str.back() == '0') {
      str.chop(1);
    }
    if (str.back() == '.') str.chop(1);
  } else if (hasDot(str) && hasExp(str)) {
    int exp_pos = findExp(str);
    while (str[exp_pos - 1] == '0') {
      str.remove(exp_pos - 1, 1);
      exp_pos--;
    }
  }
  return str;
}

int Calculator::findExp(const QString str) {
  for (int i = 0; i < str.size(); ++i) {
    if (str[i] == 'e' || str[i] == 'E') return i;
  }
  return -1;
}

bool Calculator::hasDot(const QString str) {
  for (int i = 0; i < str.size(); ++i) {
    if (str[i] == '.') return true;
  }
  return false;
}

bool Calculator::hasExp(const QString str) {
  for (int i = 0; i < str.size(); ++i) {
    if (str[i] == 'e' || str[i] == 'E') return true;
  }
  return false;
}

bool Calculator::isLimitsChanged() {
  return min_x != ui->doubleSpinBoxMinX->value() ||
         max_x != ui->doubleSpinBoxMaxX->value() ||
         min_y != ui->doubleSpinBoxMinY->value() ||
         max_y != ui->doubleSpinBoxMaxY->value();
}

/* Append '(' to function names in QLineEdit */
QString Calculator::appendLeftPar(const QString str) {
  QString ret = str;
  ret.push_back('(');
  return ret;
}
