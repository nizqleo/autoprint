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
#include "API/api.h"
#include "models/pattern.h"
#include "models/task.h"
#include <map>


/* datasetModel -- a class of pattern dataset.
 *
 * Constructed by when the program start running.
 *
 * Should be able to connect to the most complete dataset.(might be online
 * or local)
 *
 *
 */
class datasetModel : public QAbstractTableModel
{
public:
    datasetModel(API* api ,QObject *parent = 0);

    void readDataset();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;

    void deleteItem(int index);
    string getItem(int index);
    int getIndex(QString pattern);
    //string dir = "C:\\Users\\nzq82\\source\\QtRepos\\data";
    string dir = ".\\data";

    // do file savings
    // and update informations
    void save_files(string pattern, QString DFAR4Address, QString DBAR4Address,
                    QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                    QString MimageAddress, bool hasBack, bool hasFront);
    bool patternNameOverlapCheck(QString s);

    Pattern* patternPointer(string name);

    void searching(string s);

    int foundIndexMap(int originalInd) const;
    Pattern* getTheNewOne();

    int getPatternSize();
    bool hasPatternName(string name);

    int getTotalPatternNum();

    friend class addPrintTask;

private:

    vector<int> indexMap;

    vector<string> patternNames;
    vector<Pattern> patterns;

    bool searchingMode;

    API* api;
};

#endif
