#ifndef PRINTTASKMANAGEMENT_H
#define PRINTTASKMANAGEMENT_H

#include <QWidget>
#include <QMainWindow>

#include "pages\startworking.h"
#include "auxiliary.h"

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


private slots:
    void on_dataPage_button_clicked();
    void on_settingPage_button_clicked();
    void on_ERP_button_clicked();


signals:
    void datapageopen();
    void settingpageopen();


private:
    void readERPOriginalData();
    void jobAssignment();

    Ui::printTaskManagement *ui;
    MainWindow *MW;
    startWorking* SW;

    std::vector<QString> Patterns;
    QMap<QString, std::vector<std::vector<int>>> orderArray;

    QMap<int, vector<QString>> assignment;
};

#endif // PRINTTASKMANAGEMENT_H
