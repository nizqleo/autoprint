#include <QtCore>

#include "ordermodel.h"

OrderModel::OrderModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    totalOrderNumber = 0;
    totalJobNumber = 0;
}

void OrderModel::setOrderMap(const QMap<QString, QMap<QString, int>> &map)
{
    beginResetModel();
    orderMap = map;
    PI_init();

    for(int i = 0;i < patternIndex.size(); i++){
        std::cout<<patternIndex[i]<<' ';
    }
    std::cout<<std::endl;
    endResetModel();
}

void OrderModel::setOrderArray(const QMap<QString, std::vector<std::vector<int>>> &array)
{
    beginResetModel();
    orderArray = array;
    std::cout<<"setArray"<<std::endl;
    endResetModel();
}

//返回行数
int OrderModel::rowCount(const QModelIndex & /* parent */) const
{
    return 8;
}
//返回列数
int OrderModel::columnCount(const QModelIndex & /* parent */) const
{
    return 6;
}

//返回一个项的任意角色的值，这个项被指定为QModelIndex
QVariant OrderModel::data(const QModelIndex &index, int role) const
{
    std::cout<<index.row()<<' '<<index.column()<<std::endl;
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignRight | Qt::AlignVCenter);
    } else if (role == Qt::DisplayRole) {

        if(index.column() == 5){
            int sum = 0;
            for(int i = 0; i < 5; i++){
                sum+=orderArray[currentPattern][index.row()][i];
            }
            return sum;
        }
        return orderArray[currentPattern][index.row()][index.column()];
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
        }
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


void OrderModel::PI_init(){
    patternIndex.push_back(0);
    for (auto i = orderMap.begin(); i != orderMap.end(); i ++) {
        totalOrderNumber += i->size();
        for (auto j = i->begin(); j != i->end(); j ++) {
            totalJobNumber += j.value();
        }
        patternIndex.push_back(i->size()+*patternIndex.rbegin());
    }
    std::cout<<totalJobNumber<<std::endl;
}


void OrderModel::setCurrentPattern(QString pattern){
    beginResetModel();
    currentPattern = pattern;
    endResetModel();
}


std::pair<QString, QString> OrderModel::OrderInfo(int index)const {
    int patternNum = 0;
    while(patternNum < patternIndex.size() && index > patternIndex[patternNum]){
        patternNum++;
    }
    if(index < patternIndex[patternNum]){
        patternNum-=1;
    }
    if(patternNum >= orderMap.size()){
        std::cout<<"Larger than ordermap size"<<std::endl;
        std::cout<<index<<std::endl;
    }
    QString pattern = (orderMap.begin() + patternNum).key();

    if(patternNum >= patternIndex.size()){
        std::cout<<"Larger than ordermap size"<<std::endl;
    }
    int clothNum = index - patternIndex[patternNum];

    if(clothNum >= orderMap[pattern].size()){
        std::cout<<"Larger than orderMap[pattern] size"<<std::endl;
    }
    QString cloth = (orderMap[pattern].begin()+clothNum).key();

    return std::make_pair(pattern, cloth);
}


