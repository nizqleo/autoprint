#include "ar4filemodel.h"
#include "API/api.h"


AR4FileModel::AR4FileModel(QObject *parent ):QAbstractTableModel(parent){

}

AR4FileModel::AR4FileModel(AR4FileModel* fileModel){
    files.clear();
    for(auto p = fileModel->files.begin(); p < fileModel->files.end(); p++){
        files.push_back(*p);
    }
    //api = fileModel->api;
}


int AR4FileModel::rowCount(const QModelIndex &parent)const {
    return files.size();
}

int AR4FileModel::columnCount(const QModelIndex &parent) const{
    return 5;
}

QVariant AR4FileModel::data(const QModelIndex &index, int role)const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignHCenter | Qt::AlignVCenter);
    }

    if(index.column() == 0){
        if(role == Qt::DecorationRole)
            //qDebug()<<"images size"<< images.size()<<endl;
            if(index.row() < images.size() && !images[index.row()].isNull())
                return images[index.row()].scaled(QSize(100,100),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }

    if (role == Qt::DisplayRole) {
        if(index.column() == 1){// platen Size
            return platenSizeString[files[index.row()].platenSize];
        }

        if(index.column() == 2){// Ink type
            if(files[index.row()].whiteInk)
                return "彩墨+白墨";
            else return "仅彩墨";
        }

        if(index.column() == 3){// 颜色
            return files[index.row()].color2QString();
        }

        if(index.column() == 4){// 位置
            return positionString[files[index.row()].position];
        }
        return QVariant();
    }
    return QVariant();
}
QVariant AR4FileModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const{

        if (role != Qt::DisplayRole)
            return QVariant();
        if(orientation == Qt::Vertical)
            return section+1;
        switch (section) {
        case 0:
            return QString("插图");
        case 1:
            return QString("用版尺寸");
        case 2:
            return QString("油墨种类");
        case 3:
            return QString("适用颜色");
        case 4:
            return QString("位置");
        }

}


int AR4FileModel::addItem(QString fileName, int posi, int color){

    int result = 0;

    ARX4 temp;
    QPixmap image;
    result = api->extractARX4file(fileName, temp, image);

    if(result != 0){
        return result;
    }
    temp.position = posi;
    temp.applyColor = color;

    beginResetModel();
    auto f = searchFile(color, posi);
    if(f != files.end()){
        *f = temp;
        images[f-files.begin()] = image;
    }
    else {
        files.push_back(temp);
        images.push_back(image);
    }
    endResetModel();

    return result;
}


void AR4FileModel::addItem(ARX4 newFile, QPixmap image, int index){
    beginResetModel();
    if(index >= 0){
        files[index] = newFile;
        images[index] = image;
    }
    else {
        files.push_back(newFile);
        images.push_back(image);
    }
    endResetModel();
}

void AR4FileModel::deleteItem(int index){
    beginResetModel();
    files.erase(files.begin()+index);
    images.erase(images.begin()+index);
    qDebug()<<"delete Item"<<files.size()<<endl;
    endResetModel();
}


vector<ARX4>::iterator AR4FileModel::searchFile(QString color, QString position){
    int c;
    if(color == API::defaultColor)
        c = -1;
    else c = Order::color2Index(color);

    int p = ARX4::position2Index(position);

    qDebug()<<"checking hasFile"<<c<<' '<<p<<endl;

    vector<ARX4>::iterator f;
    for (f = files.begin(); f < files.end(); f++) {
        if(f->position == p && f ->applyColor == c)
            break;
    }
    return f;
}



vector<ARX4>::iterator AR4FileModel::searchFile(int color, int position){
    vector<ARX4>::iterator f;
    for (f = files.begin(); f < files.end(); f++) {
        if(f->position == position && f ->applyColor == color)
            break;
    }
    return f;
}
