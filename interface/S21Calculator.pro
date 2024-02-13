QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RESOURCES = \
    ../misc/resource.qrc

SOURCES += \
    ../controller/s21_controller.cpp \
    ../controller/s21_controller_credit.cpp \
    ../controller/s21_controller_deposit.cpp \
    ../model/s21_calculation.cpp \
    ../model/s21_common.cpp \
    ../model/s21_credit.cpp \
    ../model/s21_deposit.cpp \
    calculator.cpp \
    creditcalculator.cpp \
    depositcalculator.cpp \
    qcustomplot.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../controller/s21_controller.h \
    ../controller/s21_controller_credit.h \
    ../controller/s21_controller_deposit.h \
    ../model/s21_calculation.h \
    ../model/s21_common.h \
    ../model/s21_credit.h \
    ../model/s21_deposit.h \
    calculator.h \
    creditcalculator.h \
    depositcalculator.h \
    qcustomplot.h \
    mainwindow.h

FORMS += \
    calculator.ui \
    creditcalculator.ui \
    depositcalculator.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
