#ifndef TASKMODEL_H
#define TASKMODEL_H


#include <QAbstractTableModel>
#include <QMap>
#include <vector>
#include <iostream>

#include "task.h"


/* TaskModel -- the class especially for task presenting in QtableView.
 *
 *
 */
class TaskModel : public QAbstractTableModel
{
public:
    TaskModel(QObject *parent = 0);
    TaskModel(Task t, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;


    int totalJobNumber;
    QString currentPattern;
    friend class Task;

private:
    Task task;



};


#endif // TASKMODEL_H
