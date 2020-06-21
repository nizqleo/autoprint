#ifndef STARTWORKING_H
#define STARTWORKING_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


#include <QWidget>
#include <QMainWindow>
#include <QMessageBox>
#include "models\taskmodel.h"
#include "pickuplist.h"
#include "auxiliary.h"
#include "API/api.h"

using namespace std;

enum modes {WORKING, SHOWING};

namespace Ui {
class startWorking;
}

class startWorking : public QWidget
{
    Q_OBJECT

public:
    explicit startWorking(QWidget *parent = nullptr);
    explicit startWorking(Task* t, modes m, int i, QString name, QWidget *parent = nullptr);
    ~startWorking();

    static API* api;

private slots:
    void on_Return_button_clicked();

    void on_lastPattern_button_clicked();
    void on_nextPattern_button_clicked();
    void on_frontPage_button_clicked();
    void on_backPage_button_clicked();

    void receiving_new_task(Task* t, int i, int ID);

signals:
    void asking_for_adjacent(int row, bool working, int ID);
    void update(int ID);

private:
    void init();

    Ui::startWorking *ui;

    QMap<QString, int> totalNum;

    Task* task;
    TaskModel* TM;
    int printerIndex;
    QString printerName;
    int index;// index in the tasklist

    modes mode;
};

#endif // STARTWORKING_H
