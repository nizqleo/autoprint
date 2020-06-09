#include <QtCore>

#include "datasetmodel.h"

datasetModel::datasetModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    readDataset();
}

void datasetModel::readDataset()
{
    beginResetModel();

    if(!get_files(dir+"\\AR4", PatternNames)){
        std::cout<<"failed to find any file of this type."<<std::endl;
    }

    for (int i = 0; i < PatternNames.size(); i++) {
        QPixmap image;
        std::string name = dir+"\\image\\"+PatternNames[i]+"\\P.jpg";
        if(image.load(QString::fromStdString(name))){
            Pimages[PatternNames[i]] = image;
        }
        name = dir+"\\image\\"+PatternNames[i]+"\\M.jpg";
        if(image.load(QString::fromStdString(name))){
            Mimages[PatternNames[i]] = image;
        }

        int status = 0;
        std::ifstream inFile(dir+"\\AR4\\"+PatternNames[i]+"\\info.txt", ios::out);
        if(!inFile){
            cout<<"failed to open order file."<<endl;
        }
        string lineStr;
        getline(inFile, lineStr);
        if(lineStr[0]== '3') status = 48;
        if(lineStr[0]== '2') status = 32;
        if(lineStr[0]== '1') status = 16;
        inFile.close();

        //AR4
        QFileInfo fileInfo;

        fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+PatternNames[i]+"\\LF.txt"));
        if(fileInfo.isFile())  status += 8;
        fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+PatternNames[i]+"\\DF.txt"));
        if(fileInfo.isFile())  status += 4;
        fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+PatternNames[i]+"\\LB.txt"));
        if(fileInfo.isFile())  status += 2;
        fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+PatternNames[i]+"\\DB.txt"));
        if(fileInfo.isFile())  status += 1;

        fileStatus[PatternNames[i]] = status;
        cout<<status<<endl;
    }
    endResetModel();
}

//返回行数
int datasetModel::rowCount(const QModelIndex & /* parent */) const
{
    return PatternNames.size();
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
    if(index.column() == 1){
        if(Mimages.find(PatternNames[index.row()]) != Mimages.end()){
            if(role == Qt::DecorationRole)
                return Mimages[PatternNames[index.row()]].scaled(QSize(280,280));
        }
        else {
            if(role == Qt::DisplayRole)
                return QVariant("暂无图片");
        }
    }
    if(index.column() == 2){
        if(Pimages.find(PatternNames[index.row()]) != Pimages.end()){
            if(role == Qt::DecorationRole)
                return Pimages[PatternNames[index.row()]].scaled(QSize(280,280));
        }
        else {
            if(role == Qt::DisplayRole)
                return QVariant("暂无图片");
        }
    }

    if (role == Qt::DisplayRole) {
        if(index.column() == 0){//pattern name
            return QString::fromStdString(PatternNames[index.row()]);
        }

        int status = fileStatus[PatternNames[index.row()]];
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
            value.chop(1);

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
        return section;
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
    std::string pattern = PatternNames[index];
    Pimages.erase(Pimages.find(pattern));
    Mimages.erase(Mimages.find(pattern));
    PatternNames.erase(PatternNames.begin()+index);
    std::cout<<pattern<<std::endl;

    std::remove((dir+"\\image\\"+pattern+"\\P.jpg").data());
    std::remove((dir+"\\image\\"+pattern+"\\M.jpg").data());
    std::remove((dir+"\\AR4\\"+pattern+"\\LB.txt").data());
    std::remove((dir+"\\AR4\\"+pattern+"\\LF.txt").data());
    std::remove((dir+"\\AR4\\"+pattern+"\\DB.txt").data());
    std::remove((dir+"\\AR4\\"+pattern+"\\DF.txt").data());
    std::remove((dir+"\\AR4\\"+pattern+"\\info.txt").data());


    std::cout<<rmdir((dir+"\\AR4\\"+pattern+"\\").c_str())<<std::endl;
    std::cout<<rmdir((dir+"\\image\\"+pattern+"\\").c_str())<<std::endl;
//    QMessageBox::StandardButton reply = QMessageBox::information(NULL, "删除成功", "删除成功！", QMessageBox::Yes);

    endResetModel();
}


std::string datasetModel::getItem(int index){
    return PatternNames[index];
}


void datasetModel::save_files(std::string pattern, QString DFAR4Address, QString DBAR4Address,
                              QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                              QString MimageAddress, bool hasBack, bool hasFront){
    std::cout<<"saving"<<std::endl;
    beginResetModel();

    if( std::find(PatternNames.begin(), PatternNames.end(), pattern) == PatternNames.end()){
        PatternNames.push_back(pattern);
    }

    string folderPath = dir+"\\AR4\\"+pattern;
    if (0 != access(folderPath.c_str(), 0))
        mkdir(folderPath.c_str());   // 返回 0 表示创建成功，-1 表示失败
    int status = 0;
    if(hasBack) status += 32;
    if(hasFront) status += 16;
    //AR4

    if(LFAR4Address != ""){
        if(!coverFileCopy(dir+"\\AR4\\"+pattern+"\\LF.txt", LFAR4Address))
            cout<<"failed saving LF AR4 file!"<<endl;
    }
    if(DFAR4Address != ""){
        if(!coverFileCopy(dir+"\\AR4\\"+pattern+"\\DF.txt", DFAR4Address))
            cout<<"failed saving DF AR4 file!"<<endl;
    }
    if(LBAR4Address != ""){
        if(!coverFileCopy(dir+"\\AR4\\"+pattern+"\\LB.txt", LBAR4Address))
            cout<<"failed saving LB AR4 file!"<<endl;
    }
    if(DBAR4Address != ""){
        if(!coverFileCopy(dir+"\\AR4\\"+pattern+"\\DB.txt", DBAR4Address))
            cout<<"failed saving DB AR4 file!"<<endl;
    }

    QFileInfo fileInfo;

    fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+pattern+"\\LF.txt"));
    if(fileInfo.isFile())  status += 8;
    fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+pattern+"\\DF.txt"));
    if(fileInfo.isFile())  status += 4;
    fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+pattern+"\\LB.txt"));
    if(fileInfo.isFile())  status += 2;
    fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+pattern+"\\DB.txt"));
    if(fileInfo.isFile())  status += 1;


    folderPath = dir+"\\image\\"+pattern;
    if (0 != access(folderPath.c_str(), 0))
        mkdir(folderPath.c_str());

    QSize picSize(250, 250);
    //image
    if(PimageAddress == "" ||!coverFileCopy(dir+"\\image\\"+pattern+"\\P.jpg", PimageAddress)){
        cout<<"failed saving P image!"<<endl;
    }else{
        QPixmap pixmapP(PimageAddress);
        pixmapP = pixmapP.scaled(picSize);
        Pimages[pattern] = pixmapP;
    }
    if(MimageAddress == "" ||!coverFileCopy(dir+"\\image\\"+pattern+"\\M.jpg", MimageAddress)){
        cout<<"failed saving M image!"<<endl;
    }else{
        QPixmap pixmapM(MimageAddress);
        pixmapM = pixmapM.scaled(picSize);
        Mimages[pattern] = pixmapM;
    }

    fileStatus[pattern] = status;

    std::ofstream outFile(dir+"\\AR4\\"+pattern+"\\info.txt", ios::out);
    if(!outFile){
        cout<<"failed to write in a new file."<<endl;
    }
    outFile<<hasBack*2+hasFront<<endl;
    outFile.close();

    cout<<status<<endl;
    endResetModel();
}


int datasetModel::getIndex(QString pattern){
    return std::find(PatternNames.begin(), PatternNames.end(), pattern.toStdString()) - PatternNames.begin();
}


bool datasetModel::patternNameOverlapCheck(QString s){
    string s1 = s.toStdString();
    return (find(PatternNames.begin(), PatternNames.end(), s1) != PatternNames.end());//1: overlapped!
}
