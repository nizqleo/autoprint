#ifndef DATAMAINTAINANCE_H
#define DATAMAINTAINANCE_H

#include <QWidget>
#include <QMainWindow>
#include <QStandardItem>
#include "models\ordermodel.h"
#include <QTableView>
#include "models\datasetmodel.h"
#include "dialog.h"
#include <QDebug>
#include "pages\printtaskmanagement.h"
#include "pages\printersetting.h"
#include "models\task.h"

class MainWindow;
namespace Ui {
class dataMaintainance;
}

class dataMaintainance : public QWidget
{
    Q_OBJECT

public:
    explicit dataMaintainance(MainWindow *mainwindow, QWidget *parent = nullptr);
    ~dataMaintainance();

    QString searchDir;
    datasetModel* DModel;
    bool patternNameOverlapCheck(QString s);

private slots:
    void on_Add_button_clicked();
    void on_Delete_button_clicked();
    void on_Edit_button_clicked();
    void on_Exit_button_clicked();

    void save_files(QString pattern, QString DFAR4Address, QString DBAR4Address,
                    QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                    QString MimageAddress, bool hasBack, bool hasFront);
    void delete_files(QString pattern);

    void on_tableView_clicked();

    void on_taskPage_button_clicked();
    void on_settingPage_button_clicked();

    void on_search_lineEdit_textEdited(QString s);

signals:
    void taskpageopen();
    void settingpageopen();


private:
    Ui::dataMaintainance *ui;
    MainWindow *MW;
    QStandardItemModel* model;

    Dialog* dialog;


    printTaskManagement* PTM;
    printerSetting* PS;


};


#endif // DATAMAINTAINANCE_H
