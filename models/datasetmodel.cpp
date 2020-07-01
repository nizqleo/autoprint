#include <QtCore>

#include "datasetmodel.h"
#include "auxiliary.h"
#include <direct.h>

#include "models/task.h"

#include <QMap>
#include <QPixmap>

#include <cstdio>
#include <QMessageBox>
#include <algorithm>
#include <QFileInfo>
#include <fstream>
#include "API/api.h"

datasetModel::datasetModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    readDataset();
    Task::DModel = this;
    searchingMode = false;

}

/* Only called once when the program starts.
 * read the file system and build the patterns.
 */
void datasetModel::readDataset()
{
    beginResetModel();

    api->getPatternNameList(patternNames);
    int cnt = 0;
    for (int i = 0; i < patternNames.size(); i++) {
        patterns.push_back(Pattern(patternNames[i]));
        api->readPatternData(patterns[cnt]);
//        patterns[cnt].AR4Files->api = api;
        cnt++;
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
    //return 5;
    return 4;
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

    unsigned long long patternIndex = foundIndexMap(index.row());
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

    if (role == Qt::DisplayRole) {
        if(index.column() == 0){//pattern name
            return patternNames[patternIndex];
        }

        if(index.column() == 2){//文件
            QString content;
            for (auto p = patterns[patternIndex].AR4Files->files.begin();
                  p < patterns[patternIndex].AR4Files->files.end(); p ++) {
                content += p->color2QString()+positionString[p->position]+"文件\n";
            }
            content.chop(1);
            return content;
        }

        if(index.column() == 3){//备注
            return patterns[patternIndex].Notes;
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
//    case 2:
//        return QString("生产版单");
    case 2:
        return QString("已保存文件");
    case 3:
        return QString("备注");
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


void datasetModel::patternSetUpdate(Pattern* pattern){
    beginResetModel();
    unsigned long long patternIndex;
    patternIndex = std::find(patternNames.begin(), patternNames.end(), pattern->name) - patternNames.begin();
    if(patternIndex == patternNames.size()){

        patternNames.push_back(pattern->name);
        Pattern temp(*pattern);
//        temp.AR4Files->api = api;
        patterns.push_back(temp);
    }

    patterns[patternIndex].name = pattern->name;
    patterns[patternIndex].Mimages = api->loadPics(pattern->name, 1);
    if(!patterns[patternIndex].Mimages.isNull())
        patterns[patternIndex].hasMimages = true;

    endResetModel();
}


void datasetModel::patternSetUpdate(QString pattern, int type){
    beginResetModel();
    unsigned long long patternIndex;
    patternIndex = std::find(patternNames.begin(), patternNames.end(), pattern) - patternNames.begin();
    if(patternIndex == patternNames.size()){
        patternNames.push_back(pattern);
        Pattern temp(pattern);
        temp.type = type;
//        temp.AR4Files->api = api;
        patterns.push_back(temp);
    }

    patterns[patternIndex].name = pattern;
    endResetModel();
}
\
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



