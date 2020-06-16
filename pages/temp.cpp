#include "temp.h"
#include "ui_temp.h"
#include <QApplication>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QStandardItemModel>
#include <QTableView>

#include <iostream>


temp::temp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::temp)
{
    ui->setupUi(this);

    QStandardItemModel* model = new QStandardItemModel();
    QStringList labels = QObject::trUtf8("频率,功率,误差").simplified().split(",");
    model->setHorizontalHeaderLabels(labels);

    QStandardItem* item = 0;

    for(int i = 0;i < 10;i++){
        item = new QStandardItem(QString("%1").arg(i));
        model->setItem(i,0,item);
        item = new QStandardItem(QString("%1").arg(i*2));
        model->setItem(i,1,item);
        item = new QStandardItem(QString("%1").arg(i*3));
        model->setItem(i,2,item);
    }
    ui->tableView->setModel(model);
    ui->tableView->show();

//    model.setItem(0, 2, new QStandardItem("男"));

//    ComboBoxDelegate* delegate = new ComboBoxDelegate;
//    //tableView.setItemDelegate(&delegate);
//    ui->tableView->setItemDelegateForColumn(1, delegate); // Column 0 can take any value, column 1 can only take values up to 8.

//    for (int row = 0; row < 4; ++row)
//      {
//      for (int column = 0; column < 2; ++column)
//        {
//        QModelIndex index = model.index(row, column, QModelIndex());
//        int value = (row+1) * (column+1);
//        std::cout << "Setting (" << row << ", " << column << ") to " << value << std::endl;
//        model.setData(index, QVariant(value));
//        //std::cout << index.row() << "," << index.column() << "," << value << std::endl;
//        }
//      }

    // Make the combo boxes always displayed.

//    for ( int i = 0; i < model.rowCount(); ++i )
//      {
//      ui->tableView->openPersistentEditor( model.index(i, 1) );
//      }

   // ui->tableView->show();
}

temp::~temp()
{
    delete ui;
}
