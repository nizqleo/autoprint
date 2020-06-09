#include "printersetting.h"
#include "ui_printersetting.h"
#include "mainwindow.h"
printerSetting::printerSetting(MainWindow *mainwindow,QWidget *parent) :
    QWidget(parent),MW(mainwindow),
    ui(new Ui::printerSetting)
{
    ui->setupUi(this);

    connect(this, SIGNAL(taskpageopen()), MW, SLOT(open_printtaskmanagement()));
    connect(this, SIGNAL(datapageopen()), MW, SLOT(open_datamaintainance()));

    //QPixmap pixmap("C:\\Users\\nzq82\\source\\QtRepos\\data\\logo.png");
    QPixmap pixmap(".\\data\\logo.png");
    ui->logoLabel->setPixmap(pixmap);
    ui->logoLabel->show();
}

printerSetting::~printerSetting()
{
    delete ui;
}


void printerSetting::on_taskPage_button_clicked(){
    emit taskpageopen();
    this->close();
}


void printerSetting::on_dataPage_button_clicked(){
    emit datapageopen();
    this->close();
}
