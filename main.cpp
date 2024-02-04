#include "mainwindow.h"

//#include "QtTheme.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QFile styleSheetFile(":/misc/Theme.qss");
//    styleSheetFile.open(QFile::ReadOnly);
//    QString styleSheet = QLatin1String(styleSheetFile.readAll());
//    a.setStyleSheet(styleSheet);
    //setFocusPolicy(Qt.NoFocus);
    MainWindow w;
    w.show();
    return a.exec();
}
