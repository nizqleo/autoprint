#include <QtCore>

#include "ordermodel.h"

#include <algorithm>
OrderModel::OrderModel(QObject *parent)
    : QAbstractTableModel(parent){
    totalNum = 0;
    currentToppingNum = 0;
}

OrderModel::OrderModel(int ptrIdx, QObject *parent)
    : QAbstractTableModel(parent), printerIndex(ptrIdx){
    totalNum = 0;
    currentToppingNum = 0;
    finishedNum = 0;
}

//返回行数
int OrderModel::rowCount(const QModelIndex & /* parent */) const
{
    return tasklist.size();
}

//返回列数
int OrderModel::columnCount(const QModelIndex & /* parent */) const
{
    return 9;
}

//返回一个项的任意角色的值，这个项被指定为QModelIndex
QVariant OrderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0:
                return QVariant(tasklist[index.row()].pattern->name);
            case 1:
                if(!tasklist[index.row()].pattern->hasMimages)
                    return QVariant("暂无效果图");
                else return QVariant();
            case 2:
                return QVariant(printTypeString[tasklist[index.row()].type]);
            case 3:
                return QVariant(tasklist[index.row()].totalNum);
            case 4:
                if(tasklist[index.row()].fileReady)
                    return QVariant("文件齐全");
                else return QVariant("文件不全");
            case 5:
                switch (tasklist[index.row()].Tstatus) {
                    case FINISHED:
                        return QVariant("打印完毕");
                    case PRINTING:
                        return QVariant("正在打印");
                    case PENDING:
                        return QVariant("等待打印");
                    case SUSPENDED:
                        return QVariant("暂停中");
                 }
            case 6:
                return QVariant(tasklist[index.row()].createTime);
            case 7:
                if(tasklist[index.row()].fromERP)
                    return QVariant("ERP订单");
                else return QVariant("备货");
            case 8:
                return QVariant(colorString[tasklist[index.row()].colorInfo]);
        }

    }
    else if(role == Qt::DecorationRole){
        if(index.column() == 1 &&  tasklist[index.row()].pattern->hasMimages){
            return tasklist[index.row()].pattern->Mimages.scaled(QSize(100,100),Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
    }
    else if(role == Qt::BackgroundRole){
        if(!tasklist[index.row()].fileReady){
            return QColor(Qt::lightGray);
        }
        if(tasklist[index.row()].topped){
            return QColor(Qt::darkGreen);
        }
    }
    return QVariant();
}


//返回表头名称,(行号或列号，水平或垂直，角色)
QVariant OrderModel::headerData(int section,
                                   Qt::Orientation orientation ,
                                   int role) const
{
//    return QVariant();
    if (role != Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Vertical){//垂直
        return QString();
    }

    switch (section) {//水平
    case 0:
        return QString("款号"); //
    case 1:
        return QString("效果图");
    case 2:
        return QString("打印类型"); //
    case 3:
        return QString("总数量"); //
    case 4:
        return QString("AR4文件情况");
    case 5:
        return QString("状态"); //
    case 6:
        return QString("添加时间"); //
    case 7:
        return QString("任务来源"); //
    case 8:
        return QString("任务颜色"); //
    }
}


void OrderModel::setCurrentPattern(QString pattern){
    beginResetModel();
    currentPattern = pattern;
    endResetModel();
}

void OrderModel::move(int index, int direction){
    beginResetModel();
    if(index+direction < 0 || index+direction >= tasklist.size())
        return;
    swap(tasklist[index], tasklist[index+direction]);
    endResetModel();
}

void OrderModel::update(QString name){
    beginResetModel();
    for(unsigned long long i = 0; i < tasklist.size(); i++){
        if(tasklist[i].name == name)
            tasklist[i].update();
    }
    endResetModel();
}

void OrderModel::addOrder(Order* o){
    totalNum += o->number;
    //search for tasks with same color, same pattern, (same timestamp)(later)
    for(int i = 0; i < tasklist.size(); i++){
        if(tasklist[i].name == o->pattern && tasklist[i].colorInfo == o->color && tasklist[i].fromERP == o->fromERP){
            tasklist[i].Add(o);
            return;
        }
    }

    Task temp(o);
    temp.printerIdx = printerIndex;
    currentTaskID ++ ;
    temp.taskID = currentTaskID;
    tasklist.push_back(temp);
}

void OrderModel::sortTable(){
    std::sort(tasklist.begin(), tasklist.end(), less<Task>());
}

void OrderModel::update(){
    beginResetModel();
    sortTable();

    endResetModel();
}


void OrderModel::numberCheck(){
    for (unsigned long long i = 0; i < tasklist.size(); i ++) {
        totalNum += tasklist[i].totalNum;
        if(tasklist[i].Tstatus == FINISHED)
            finishedNum+=tasklist[i].totalNum;
    }
}
