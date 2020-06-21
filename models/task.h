#ifndef TASK_H
#define TASK_H

#include <string>
#include "order.h"
#include "pattern.h"

#include <QDateTime>

using namespace std;


/* Task -- class for a piece of printing task
 *
 * An task contains multiple orders, but they share only one pattern.
 *
 * A task can be arranged to only one printer, and is not dividable at present.
 *
 * Task are constructed after the orders are collected, just sum up the orders with same pattern
 *
 * A OrderModel(Tasklist) contains multiple orders.
 *
 *
 */
class datasetModel;

enum TaskStatus {FINISHED, PRINTING, PENDING, SUSPENDED};

class Task
{
public:
    Task();
    Task( Order* o);
    Task( vector<QString>& temp);
    ~Task();
    // Assigned
    int printerIdx;
    int taskID;
    static datasetModel* DModel;

    Task operator +(Task t);
    Task operator +(Order t);

    bool operator<(const Task & right)const;   //重载<运算符
    void finishOrders();
    friend class TaskModel;
    friend class OrderModel;
    friend class startWorking;
    friend class printTaskManagement;
    friend class localAPI;
    void Add(Order* o);

    void update();
    QString sizeNumberString();
    void sizeNumberStringAnalysis(QString s);
private:
    Pattern* pattern; // might be NULL!
    QString name;
    int numbers[9][6];
    int totalNum;

    vector<Order*> orders;
    // information
    TaskStatus Tstatus;
    bool fileReady;
    printType type;
    QString createTime;
    QString modifiedTime;
    bool fromERP;
    int colorInfo;
    bool isDark;
    int topped;

    void init();
};


#endif // TASK_H
