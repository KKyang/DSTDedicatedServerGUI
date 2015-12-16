#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    QLocale locale;
    translator.load("translations/dstdsgui_" + locale.name() + ".ts");
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
