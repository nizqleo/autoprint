#ifndef ADDPRINTTASK_H
#define ADDPRINTTASK_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include <QWidget>
#include <QMainWindow>

#include "pickuplist.h"

#include <QDirModel>
#include <QCompleter>
#include "auxiliary.h"
#include "models/pattern.h"
#include "models/datasetmodel.h"

#include <QStandardItemModel>

using namespace std;

class MainWindow;
namespace Ui {
class addPrintTask;
}

class addPrintTask : public QWidget
{
    Q_OBJECT

public:

    explicit addPrintTask(MainWindow *MW, QWidget *parent = nullptr);
    ~addPrintTask();
    MainWindow *MW;

private slots:
    void on_Return_button_clicked();
    void on_comfirm_button_clicked();

    void on_tableView_clicked();
    void on_pattern_lineEdit_textChanged(QString s);

signals:
    void send_orders(int**, QString, int);

private:
    Ui::addPrintTask *ui;
    QStandardItemModel* model;
    QModelIndex lastManuallyModifiedIndex;
    Pattern* pattern;

    int printerIndex;
    int** numbers;

    int last_r;
    int last_c;

    int printerNumber;
};

#endif // ADDPRINTTASK_H
