#include "dashboard.h"
#include "ui_dashboard.h"
#include "pages/printtaskmanagement.h"
#include "mainwindow.h"

DashBoard::DashBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DashBoard)
{
    ui->setupUi(this);
}


DashBoard::DashBoard(printTaskManagement* _PTM, QWidget *parent):PTM(_PTM), ui(new Ui::DashBoard){

    ui->setupUi(this);

//    QPixmap pixmap(".\\data\\logo.png");
//    ui->logoLabel->setPixmap(pixmap);
//    ui->logoLabel->show();

    updateTableViews();

    ui->printer_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->printer_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->task_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->task_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

DashBoard::~DashBoard()
{
    delete ui;
}


void DashBoard::updateTableViews(){
    ui->task_tableView->setModel(PTM->TaskLeftModel);
    ui->printer_tableView->setModel(PTM->PrinterModel);
}
