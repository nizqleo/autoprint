#ifndef AR4FILEMODEL_H
#define AR4FILEMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "models/arx4.h"

class Dialog;
//using namespace std;


class AR4FileModel : public QAbstractTableModel
{
public:
    AR4FileModel(QObject *parent = 0);
    AR4FileModel(AR4FileModel* fileModel);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;

    int addItem(QString fileName, int posi, int color);
    void addItem(ARX4 newFile, QPixmap image, int index);
    void deleteItem(int index);

    vector<ARX4>::iterator searchFile(QString color, QString position);
    vector<ARX4>::iterator searchFile(int color, int position);

    vector<ARX4> files;
    vector<QPixmap> images;
};

#endif // AR4FILEMODEL_H
