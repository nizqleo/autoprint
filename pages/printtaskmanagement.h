#ifndef PRINTTASKMANAGEMENT_H
#define PRINTTASKMANAGEMENT_H

#include <QWidget>
#include <QMainWindow>

#include "pages\startworking.h"
#include "pages\dialog.h"
#include "pages\addprinttask.h"
#include "pages/dashboard.h"

#include "auxiliary.h"

#include "models/order.h"
#include "models/datasetmodel.h"
#include "models/ordermodel.h"

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
    void totalUpdate();

    QStandardItemModel* PrinterModel;
    QStandardItemModel* TaskLeftModel;

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

    void on_dashboard_botton_clicked();
    void on_yesterday_botton_clicked();

    void on_startPrint_button_clicked();

    void comboBox_changed(const QString & s);
    void type_comboBox_changed(const QString & s);

    void updatePatternInfo(Pattern* p);

    void receiving_asking(int row, bool working, int ID);

    void receive_orders(int** numbers, QString name, int printerID);
    void updateTable(int i);

    void topTaskFinished(unsigned long long printerID);

signals:
    void datapageopen();
    void settingpageopen();
    void createNewPattern(QString, int);
    void sending_new_task(Task*, int, int);
    void filterType(int printerID, int type);
    void DBmodelsReady();

private:
    bool orderAssignment(int begin, int end, int printerID = 0);
    void updateDatasetWithOrder();
    void readSaveOrders();

    void computeDashBoardModels();

    int totalUnfinishedNumber(int color, int size);
    Ui::printTaskManagement *ui;
    MainWindow *MW;

    vector<Order> orderList;

    vector<OrderModel*> ordermodels;

    int printerNumber;
    unsigned long long currentPrinterIndex;
    vector<string> timeMap;

    int timeStamp;

    DashBoard* DB;


    int** LeftNumbers;

    friend class DashBoard;

};

#endif // PRINTTASKMANAGEMENT_H
