#ifndef DATASETMODEL_H
#define DATASETMODEL_H

#include <QAbstractTableModel>
#include "models/pattern.h"


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
    datasetModel(QObject *parent = 0);

    void readDataset();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;

    void deleteItem(int index);
    QString getItem(int index);
    int getIndex(QString pattern);


    bool patternNameOverlapCheck(QString s);

    Pattern* patternPointer(QString name);

    void searching(QString s);

    int foundIndexMap(int originalInd) const;
    Pattern* getTheNewOne();

    int getPatternSize();
    bool hasPatternName(QString name);

    int getTotalPatternNum();

    friend class addPrintTask;
    friend class dataMaintainance;

    // add new item into the table
    void patternSetUpdate(Pattern* pattern);

    void patternSetUpdate(QString pattern, int type);

private:

    vector<int> indexMap;

    vector<QString> patternNames;
    vector<Pattern> patterns;

    bool searchingMode;
};

#endif
