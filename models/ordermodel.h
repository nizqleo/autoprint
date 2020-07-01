#ifndef ORDERMODEL_H
#define ORDERMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <vector>
#include <set>
#include <iostream>
#include "task.h"
#include <QColor>


/* OrderModel -- class for a task list.
 *
 * Collect the information while in charge of presenting it with a QtableView.
 *
 * Printer and OrderModels are 1-1 coresponded.
 *
 * A OrderModel has a list of OrderModel. It's constructed after the tasks are all collected and,
 * most importantly, arranged to a certain printer. The printTaskManagement page will do the
 * collect & arrange.
 *
 */
class OrderModel : public QAbstractTableModel
{
public:
    OrderModel(QObject *parent = 0);
    OrderModel(int ptrIdx, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;

//    void setCurrentPattern(QString pattern);

    //QString currentPattern;

    void update(QString name);
    void update();
    void addOrder(Order* o);

    void numberCheck();

    void filter(int type);

    Task* getTaskAtIndex(int index);

    friend class printTaskManagement;

    int printerIndex;
    vector<Task> tasklist;
    int totalNum;
    int finishedNum;
    int currentToppingNum;
    void sortTable();

    int currentTaskID;
    bool filterMode;
    int filteringType;

private:
    vector<int> indexMap;


};

#endif
