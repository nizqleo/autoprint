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
    explicit startWorking(Task* t, modes m, int idx, QString name, QWidget *parent = nullptr);
    ~startWorking();


private slots:

    void on_lastPattern_button_clicked();
    void on_nextPattern_button_clicked();

    void on_upload_button_clicked();

    void on_file_listView_clicked();

    void receiving_new_task(Task* t, int i, int ID);

signals:
    void asking_for_adjacent(int row, bool working, int ID);
    void updatePrinterTaskStatus(int ID);

private:
    void specialization();

    Ui::startWorking *ui;

    QMap<QString, int> totalNum;

    Task* task;
    TaskModel* TM;
    int printerIndex;
    QString printerName;
    int index; // index in the tasklist
    vector<int> fileIndexMap;
    modes mode;

    QStandardItemModel* fileModel;
};

#endif // STARTWORKING_H
