#ifndef ORDERMODEL_H
#define ORDERMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <vector>
#include <iostream>

class OrderModel : public QAbstractTableModel
{
public:
    OrderModel(QObject *parent = 0);

    void setOrderMap(const QMap<QString, QMap<QString, int>> &map);
    void setOrderArray(const QMap<QString, std::vector<std::vector<int>>> &array);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;

    void PI_init();
    std::pair<QString, QString> OrderInfo(int index) const;
    void setCurrentPattern(QString pattern);

    int totalJobNumber;
    QString currentPattern;


private:

    std::vector<int> patternIndex;
    int totalOrderNumber;



public:
    QMap<QString, QMap<QString, int>> orderMap;
    QMap<QString, std::vector<std::vector<int>>> orderArray;
};

#endif
