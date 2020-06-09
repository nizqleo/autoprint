#ifndef DATASETMODEL_H
#define DATASETMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QPixmap>
#include "auxiliary.h"
#include <cstdio>
#include <QMessageBox>
#include <algorithm>
#include <QFileInfo>
#include <fstream>
#include <direct.h>

class datasetModel : public QAbstractTableModel
{
public:
    datasetModel(QObject *parent = 0);

    void readDataset();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;

    void deleteItem(int index);
    std::string getItem(int index);
    int getIndex(QString pattern);
    //std::string dir = "C:\\Users\\nzq82\\source\\QtRepos\\data";
    std::string dir = ".\\data";


    void save_files(std::string pattern, QString DFAR4Address, QString DBAR4Address,
                    QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                    QString MimageAddress, bool hasBack, bool hasFront);
    bool patternNameOverlapCheck(QString s);

    /* 111111 == 63
     * 1     : has back 32
     *  1    : has front 16
     *   1   : has LF 8
     *    1  : has DF 4
     *     1 : has LB 2
     *      1: has DB 1
     */
    QMap<std::string, int> fileStatus;
private:

    std::vector<std::string> PatternNames;
    QMap<std::string, QPixmap> Pimages;
    QMap<std::string, QPixmap> Mimages;





};

#endif
