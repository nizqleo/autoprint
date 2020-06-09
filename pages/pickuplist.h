#ifndef PICKUPLIST_H
#define PICKUPLIST_H

#include <QDialog>
#include <QStandardItem>
#include <QCheckBox>
#include <vector>
#include <string>
#include <iostream>
using namespace std;
namespace Ui {
class PickUpList;
}

class PickUpList : public QDialog
{
    Q_OBJECT

public:
    explicit PickUpList(QMap<QString, int> PUT, QWidget *parent = nullptr);
    ~PickUpList();

private:
    Ui::PickUpList *ui;
    QStandardItemModel* model;
    std::vector<QStandardItem*> cboxs;
    QMap<QString, int> PUTable;

};

#endif // PICKUPLIST_H
