#ifndef PRINTTASKMANAGEMENT_H
#define PRINTTASKMANAGEMENT_H

#include <QWidget>
#include <QMainWindow>

#include "pages\startworking.h"
#include "pages\dialog.h"
#include "pages\addprinttask.h"
#include "auxiliary.h"

#include "models/order.h"
#include "models/datasetmodel.h"
#include "models/ordermodel.h"
#include "pages/temp.h"

class MainWindow;
namespace Ui {
class printTaskManagement;
}

class printTaskManagement : public QWidget
{
    Q_OBJECT

public:
    explicit printTaskManagement(MainWindow *mainwindow, QWidget *parent = nullptr);
    ~printTaskManagement();

    void saveOrders();

private slots:
    void on_dataPage_button_clicked();
    void on_settingPage_button_clicked();

    void on_tableView_clicked();

    void on_ERP_button_clicked();
    void on_add_button_clicked();

    void on_up_button_clicked();
    //void on_down_button_clicked();
    void on_detail_button_clicked();
    void on_edit_button_clicked();

    void on_startPrint_button_clicked();

    void on_comboBox_changed(const QString & s);
    void update(QString, QString, QString, QString, QString, QString, QString, bool,bool);

    void receiving_asking(int row, bool working, int ID);

    void receive_orders(int** numbers, QString name, int printerID);
    void updateTable(int i);

    void topTaskFinished(int printerID);

signals:
    void datapageopen();
    void settingpageopen();
    void createNewPattern(QString, QString, QString, QString, QString, QString, QString, bool,bool);
    void sending_new_task(Task*, int, int);

private:
    void orderAssignment(int begin, int end, int printerID = 0);
    void updateDatasetWithOrder();
    void readSaveOrders();


    Ui::printTaskManagement *ui;
    MainWindow *MW;

    API* api;
    vector<Order> orderList;

    vector<OrderModel*> ordermodels;

    int printerNumber;
    unsigned long long currentPrinterIndex;
    vector<string> timeMap;
    int timeStamp;

};

#endif // PRINTTASKMANAGEMENT_H
