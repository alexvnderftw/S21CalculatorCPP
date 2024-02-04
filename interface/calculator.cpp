#include "calculator.h"
#include "ui_calculator.h"

Calculator::Calculator(QWidget *parent)
    : QWidget(parent), ui(new Ui::Calculator) {
    ui->setupUi(this);
    plotGraph();
    ctrl.setRadian();

    connect(ui->widgetPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui->widgetPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));

    connect(ui->pushButtonDelete, SIGNAL(clicked()), this, SLOT(deleteLineText()));
    connect(ui->pushButtonBackspace, SIGNAL(clicked()), this, SLOT(backspaceLineText()));
    connect(ui->pushButtonResult, SIGNAL(clicked()), this, SLOT(calculateResult()));
    connect(ui->lineEditExpression, SIGNAL(returnPressed()), this, SLOT(calculateResult()));
    connect(ui->lineEditX, SIGNAL(returnPressed()), this, SLOT(calculateResult()));
    connect(ui->radioButtonRad, SIGNAL(clicked()), this, SLOT(setRadian()));
    connect(ui->radioButtonDeg, SIGNAL(clicked()), this, SLOT(setDegree()));

    QSignalMapper* signalMapper = new QSignalMapper(this);
    connect(ui->pushButton0, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButton1, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButton2, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButton3, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButton4, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButton5, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButton6, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButton7, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButton8, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButton9, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonX, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonDot, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonLeftPar, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonRightPar, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonPlus, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonMinus, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonAltMinus, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonMult, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonDiv, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonPow, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonMod, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonLn, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonLog, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonSqrt, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonSin, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonCos, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonTan, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonAsin, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonAcos, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonAtan, SIGNAL(clicked()), signalMapper, SLOT(map()));

    signalMapper->setMapping(ui->pushButton0, ui->pushButton0->text());
    signalMapper->setMapping(ui->pushButton1, ui->pushButton1->text());
    signalMapper->setMapping(ui->pushButton2, ui->pushButton2->text());
    signalMapper->setMapping(ui->pushButton3, ui->pushButton3->text());
    signalMapper->setMapping(ui->pushButton4, ui->pushButton4->text());
    signalMapper->setMapping(ui->pushButton5, ui->pushButton5->text());
    signalMapper->setMapping(ui->pushButton6, ui->pushButton6->text());
    signalMapper->setMapping(ui->pushButton7, ui->pushButton7->text());
    signalMapper->setMapping(ui->pushButton8, ui->pushButton8->text());
    signalMapper->setMapping(ui->pushButton9, ui->pushButton9->text());
    signalMapper->setMapping(ui->pushButtonX, ui->pushButtonX->text());
    signalMapper->setMapping(ui->pushButtonDot, ui->pushButtonDot->text());
    signalMapper->setMapping(ui->pushButtonLeftPar, ui->pushButtonLeftPar->text());
    signalMapper->setMapping(ui->pushButtonRightPar, ui->pushButtonRightPar->text());
    signalMapper->setMapping(ui->pushButtonPlus, ui->pushButtonPlus->text());
    signalMapper->setMapping(ui->pushButtonMinus, ui->pushButtonMinus->text());
    signalMapper->setMapping(ui->pushButtonAltMinus, ui->pushButtonAltMinus->text());
    signalMapper->setMapping(ui->pushButtonMult, ui->pushButtonMult->text());
    signalMapper->setMapping(ui->pushButtonDiv, ui->pushButtonDiv->text());
    signalMapper->setMapping(ui->pushButtonPow, ui->pushButtonPow->text());
    signalMapper->setMapping(ui->pushButtonMod, ui->pushButtonMod->text());
    signalMapper->setMapping(ui->pushButtonLn, appendLeftPar(ui->pushButtonLn->text()));
    signalMapper->setMapping(ui->pushButtonLog, appendLeftPar(ui->pushButtonLog->text()));
    signalMapper->setMapping(ui->pushButtonSqrt, appendLeftPar(ui->pushButtonSqrt->text()));
    signalMapper->setMapping(ui->pushButtonSin, appendLeftPar(ui->pushButtonSin->text()));
    signalMapper->setMapping(ui->pushButtonCos, appendLeftPar(ui->pushButtonCos->text()));
    signalMapper->setMapping(ui->pushButtonTan, appendLeftPar(ui->pushButtonTan->text()));
    signalMapper->setMapping(ui->pushButtonAsin, appendLeftPar(ui->pushButtonAsin->text()));
    signalMapper->setMapping(ui->pushButtonAcos, appendLeftPar(ui->pushButtonAcos->text()));
    signalMapper->setMapping(ui->pushButtonAtan, appendLeftPar(ui->pushButtonAtan->text()));

    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(inputLineText(QString)));
}

Calculator::~Calculator()
{
    delete ui;
}

const QString Calculator::appendLeftPar(const QString str) {
    QString ret = str;
    ret.push_back('(');
    return ret;
}

void Calculator::inputLineText(const QString str) {
    if (ui->lineEditX->hasFocus() == true) {
        ui->lineEditX->insert(str);
    } else {
        ui->lineEditExpression->insert(str);
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

QString Calculator::doubleToQString(double value) {
        std::ostringstream os;
        os << value;
        return QString::fromLocal8Bit(os.str().c_str());
}

void Calculator::calculateResult() {
     QString result = doubleToQString(ctrl.calculate(ui->lineEditExpression->text().toStdString(), ui->lineEditX->text().toStdString()));
     if (ctrl.isEmpty() == true)
        result.clear();
      else if (ctrl.isSuccessful() == false)
        result = "Error";
    ui->lineEditResult->setText(result);
    plotGraph();
}

void Calculator::plotGraph() {
    int size = ui->widgetPlot->width();
    double min_x = ui->doubleSpinBoxMinX->value(), max_x = ui->doubleSpinBoxMaxX->value(),
        min_y = ui->doubleSpinBoxMinY->value(), max_y = ui->doubleSpinBoxMaxY->value();
    QVector<double> x(size), y(size);
    double step = (max_x - min_x) / size;
    for (int i = 0; i < size; ++i) {
        x[i] = min_x + i * step;
        y[i] = ctrl.calculate(x[i]);
    }
    ui->widgetPlot->setInteraction(QCP::iRangeDrag, true);
    ui->widgetPlot->setInteraction(QCP::iRangeZoom, true);
    ui->widgetPlot->addGraph();
    ui->widgetPlot->graph(0)->setData(x, y);
    ui->widgetPlot->graph(0)->setPen(QPen(Qt::black, 2.0));
    ui->widgetPlot->xAxis->setLabel("X axis");
    ui->widgetPlot->yAxis->setLabel("Y axis");
    ui->widgetPlot->graph(0)->setData(x, y);
    ui->widgetPlot->xAxis->setRange(min_x, max_x);
    ui->widgetPlot->yAxis->setRange(min_y, max_y);
    ui->widgetPlot->replot();
}

void Calculator::setDefaultFocus() {
    ui->lineEditExpression->setFocus();
}

void Calculator::setRadian() {
    ctrl.setRadian();
}

void Calculator::setDegree() {
    ctrl.setDegree();
}

//void Calculator::mousePressEvent(QMouseEvent* move) {}

//void Calculator::mouseMoveEvent(QMouseEvent* move) {

//}
//void Calculator::wheelEvent(QWheelEvent* move) {

//}
