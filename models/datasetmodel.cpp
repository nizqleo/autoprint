#include <QtCore>

#include "datasetmodel.h"

datasetModel::datasetModel(API* api ,QObject *parent)
    : QAbstractTableModel(parent),api(api)
{
    readDataset();
    Task::DModel = this;
    searchingMode = false;
}

void datasetModel::readDataset()
{
    beginResetModel();

    api->getPatternNameList(patternNames);

    for (int i = 0; i < patternNames.size(); i++) {
        Pattern temp = api->readPatternData(patternNames[i]);
        patterns.push_back(temp);
    }

    endResetModel();
}

//返回行数
int datasetModel::rowCount(const QModelIndex & /* parent */) const
{
    if(searchingMode)
        return indexMap.size();
    else
        return patternNames.size();
}

//返回列数
int datasetModel::columnCount(const QModelIndex & /* parent */) const
{
    return 5;
}

//返回一个项的任意角色的值，这个项被指定为QModelIndex
QVariant datasetModel::data(const QModelIndex &index, int role) const
{
    //std::cout<<index.row()<<' '<<index.column()<<std::endl;
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignHCenter | Qt::AlignVCenter);
    }

    int patternIndex = foundIndexMap(index.row());
    if(index.column() == 1){
        if(patterns[patternIndex].hasMimages){
            if(role == Qt::DecorationRole)
                return patterns[patternIndex].Mimages.scaled(QSize(150,150),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        }
        else {
            if(role == Qt::DisplayRole)
                return QVariant("暂无图片");
        }
    }
    if(index.column() == 2){
        if(patterns[patternIndex].hasPimages){
            if(role == Qt::DecorationRole)
                return patterns[patternIndex].Pimages.scaled(QSize(150,150), Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
        }
        else {
            if(role == Qt::DisplayRole)
                return QVariant("暂无图片");
        }
    }

    if (role == Qt::DisplayRole) {
        if(index.column() == 0){//pattern name
            return patternNames[patternIndex];
        }

        int status = patterns[patternIndex].status;
        if(index.column() == 3){//印花位置
            if (status>= 48) //11
                return QVariant("前后皆有");
            else if(((status>>5) & 1) == 1) //10
                return QVariant("只有后片");
            else //01
                return QVariant("只有前片");
        }

        if(index.column() == 4){//深片文件
            QString value = "";
            if(((status>>3) & 1) == 1)
                value += "浅色前片文件\n";
            if(((status>>2) & 1) == 1)
                value += "深色前片文件\n";
            if(((status>>1) & 1) == 1)
                value += "浅色后片文件\n";
            if((status & 1) == 1)
                value += "深色后片文件\n";

            if(value == QString(""))
                value = "无";
            else value.chop(1);

            return QVariant(value);
        }
        return QVariant();
    }
    return QVariant();
}


//返回表头名称,(行号或列号，水平或垂直，角色)
QVariant datasetModel::headerData(int section,
                                   Qt::Orientation orientation ,
                                   int role) const
{
//    return QVariant();
    if (role != Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Vertical)
        return section+1;
    switch (section) {
    case 0:
        return QString("款号");
    case 1:
        return QString("效果图/模特图");
    case 2:
        return QString("生产版单");
    case 3:
        return QString("印花位置");
    case 4:
        return QString("已保存文件");
    }
}


void datasetModel::deleteItem(int index){
    beginResetModel();
    unsigned long long realIndex = foundIndexMap(index);
    QString pattern = patternNames[realIndex];

    patternNames.erase(patternNames.begin()+realIndex);
    patterns.erase(patterns.begin()+realIndex);
    if(find(indexMap.begin(), indexMap.end(), realIndex) != indexMap.end())
        indexMap.erase(indexMap.begin() + index);

    api->deletePatternData(pattern);

    endResetModel();
}


QString datasetModel::getItem(int index){

    return patternNames[foundIndexMap(index)];
}


void datasetModel::save_files(QString pattern, QString DFAR4Address, QString DBAR4Address,
                              QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                              QString MimageAddress, bool hasBack, bool hasFront){
    std::cout<<"saving"<<std::endl;
    beginResetModel();

    int patternIndex;
    patternIndex = std::find(patternNames.begin(), patternNames.end(), pattern) - patternNames.begin();
    if(patternIndex == patternNames.size()){
        patternNames.push_back(pattern);
        Pattern temp;
        patterns.push_back(temp);
    }
    patterns[patternIndex].name = pattern;
    patterns[patternIndex].hasBack = hasBack;
    patterns[patternIndex].hasFront = hasFront;

    api->savePatternData(pattern, DFAR4Address, DBAR4Address,
                        LFAR4Address, LBAR4Address, PimageAddress,
                         MimageAddress, hasBack, hasFront);
    api->examPatternData(&patterns[patternIndex]);

    endResetModel();
}


int datasetModel::getIndex(QString pattern){
    return std::find(patternNames.begin(), patternNames.end(), pattern) - patternNames.begin();
}


bool datasetModel::patternNameOverlapCheck(QString s){

    return (find(patternNames.begin(), patternNames.end(), s) != patternNames.end());//1: overlapped!
}


Pattern* datasetModel::patternPointer(QString name){
    vector<QString>::iterator p = find(patternNames.begin(), patternNames.end(), name);
    if(p != patternNames.end()){
        return &patterns[p-patternNames.begin()];
    }
    else return NULL;
}


void datasetModel::searching(QString s){
    beginResetModel();
    if(s == QString("")){
        searchingMode = false;
        endResetModel();
        return;
    }
    indexMap.clear();
    for(int i = 0; i < patternNames.size(); i++){
        if(patternNames[i].contains(s,Qt::CaseInsensitive)){
            indexMap.push_back(i);
        }
    }
    searchingMode = true;
    endResetModel();
}


int datasetModel::foundIndexMap(int originalInd) const{
    if(!searchingMode) return originalInd;
    if(originalInd < indexMap.size())
        return indexMap[originalInd];
    else {
        cout<<"larger than the indexmap size!"<<endl;
        return 0;
    }
}



Pattern* datasetModel::getTheNewOne(){
    return &patterns[patterns.size()];
}


int datasetModel::getPatternSize(){
    return patternNames.size();
}


bool datasetModel::hasPatternName(QString name){
    return find(patternNames.begin(), patternNames.end(), name) != patternNames.end();
}


int datasetModel::getTotalPatternNum(){
    return patterns.size();
}



