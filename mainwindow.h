#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pages\datamaintainance.h"
#include "pages\printersetting.h"
#include "pages\printtaskmanagement.h"
#include "pages\startworking.h"

#include "models\printer.h"

#include "API\api.h"
#include "API\localapi.h"
#include "auxiliary.h"
#include <QTextCodec>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    string defaultDir;


    vector<Printer> printers;
    void loadPrintersInfo();
    API* api;
    dataMaintainance* DM;

public slots:
    void on_Exit_button_clicked();
    void on_Login_button_clicked();
    //void on_SW_button_clicked();

    void open_printersetting();
    void open_printtaskmanagement();
    void open_datamaintainance();


private:
    Ui::MainWindow *ui;
    printerSetting* PS;
    printTaskManagement* PTM;


    //startWorking* SW;

};

#endif // MAINWINDOW_H
