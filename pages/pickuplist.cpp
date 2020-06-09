#include "pickuplist.h"
#include "ui_pickuplist.h"

PickUpList::PickUpList(QMap<QString, int> PUT, QWidget *parent) :
    QDialog(parent),PUTable(PUT),
    ui(new Ui::PickUpList)
{
    ui->setupUi(this);

    cboxs.resize(PUTable.size());
    /*setItem设置条目栏中的一个格子的信息*/
    for (auto p = cboxs.begin();p != cboxs.end();p++) {
        *p = new QStandardItem;
        (*p)->setCheckable(true);
    }




    model = new QStandardItemModel;
    model->setRowCount(PUTable.size());
    model->setColumnCount(4);

    model->setHorizontalHeaderItem(0, new QStandardItem("颜色") );
    model->setHorizontalHeaderItem(1, new QStandardItem("尺码"));
    model->setHorizontalHeaderItem(2, new QStandardItem("数量"));
    model->setHorizontalHeaderItem(3, new QStandardItem("确认"));


    for (int i = 0; i < PUTable.size();i++) {
        string str = (PUTable.begin()+i).key().toStdString();
        model->setItem(i, 0, new QStandardItem(QString::fromStdString( str.substr(0,1))));
        model->setItem(i, 1, new QStandardItem(QString::fromStdString( str.substr(1))));
        model->setItem(i, 2, new QStandardItem(QString::number((PUTable.begin()+i).value())));
        model->setItem(i, 3, cboxs[i]);
    }


    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch );;
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );

}

PickUpList::~PickUpList()
{
    delete ui;
}
