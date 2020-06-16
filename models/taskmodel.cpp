#include <QtCore>

#include "taskmodel.h"


TaskModel::TaskModel(QObject *parent)
    : QAbstractTableModel(parent){

}


TaskModel::TaskModel(Task t, QObject *parent)
    : QAbstractTableModel(parent), task(t){

    totalJobNumber = 0;

}

//返回行数
int TaskModel::rowCount(const QModelIndex & /* parent */) const
{
    return 1;
}
//返回列数
int TaskModel::columnCount(const QModelIndex & /* parent */) const
{
    return 6;
}

//返回一个项的任意角色的值，这个项被指定为QModelIndex
QVariant TaskModel::data(const QModelIndex &index, int role) const
{
   // std::cout<<index.row()<<' '<<index.column()<<std::endl;
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignRight | Qt::AlignVCenter);
    } else if (role == Qt::DisplayRole) {

/*        if(index.column() == 5 && index.row() == colorNum){
//            return task.totalNum;
//        }
//        if(index.column() == 5){
//            int sum = 0;
//            for(int i = 0; i < 5; i++){
//                sum+=task.numbers[task.colorInfo[index.row()]][i];
//            }
//            return sum;
//        }
//        if(index.row() == colorNum){
//            int sum = 0;
//            for(int i = 0; i < task.colorInfo.size(); i++){
//                sum+=task.numbers[task.colorInfo[i]][index.column()];
//            }
//            return sum;
        }
*/
        if(index.column() == 5){// && index.row() == colorNum){
            return task.totalNum;
        }
        return task.numbers[task.colorInfo][index.column()];

    }
    return QVariant();
}


//返回表头名称,(行号或列号，水平或垂直，角色)
QVariant TaskModel::headerData(int section,
                                   Qt::Orientation orientation ,
                                   int role) const
{
//    return QVariant();
    if (role != Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Vertical){//垂直
        /*
        switch (section) {
        case 0:
            return QString("白色");
        case 1:
            return QString("黑色");
        case 2:
            return QString("粉色");
        case 3:
            return QString("蓝色");
        case 4:
            return QString("绿色");
        case 5:
            return QString("黄色");
        case 6:
            return QString("紫色");
        case 7:
            return QString("红色");
        case 8:
            return QString("总计");
        }
        */


        return colorString[task.colorInfo];
    }

    switch (section) {//水平
    case 0:
        return QString("S");
    case 1:
        return QString("M");
    case 2:
        return QString("L");
    case 3:
        return QString("XL");
    case 4:
        return QString("XXL");
    case 5:
        return QString("总计");
    }
}



