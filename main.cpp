#include "mainwindow.h"
#include <QApplication>

#include <QtGui>

#include <QTableView>
#include "models\ordermodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();



    return a.exec();
}
