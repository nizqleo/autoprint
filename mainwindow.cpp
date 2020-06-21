#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pages/startworking.h"
#include <QTextCodec>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTextCodec *gbk = QTextCodec::codecForName("gb18030");
    QTextCodec *utf8 = QTextCodec::codecForName("utf-8");


    QTextCodec::setCodecForLocale(utf8);


    QPixmap pixmap(".\\data\\logo.png");
    //QPixmap pixmap("C:\\Users\\nzq82\\source\\QtRepos\\data\\logo.png");
    ui->logo_label->setPixmap(pixmap);
    ui->logo_label->show();



//    DM = new dataMaintainance(this);
//    SW = new startWorking(this);

    defaultDir = ".\\";


    /*************************************/
    // new a API based on progress.
    api = new localAPI(this, ".\\data");

    startWorking::api = api;
    /*************************************/

    api->readPrinterData(printers);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Login_button_clicked(){
    this->hide();
    DM = new dataMaintainance(this);
    open_datamaintainance();
    PTM = new printTaskManagement(this);
}


void MainWindow::on_Exit_button_clicked(){
    this->close();
}


void MainWindow::open_printersetting(){
    PS = new printerSetting(this);
    PS->show();
    this->hide();
}


void MainWindow::open_printtaskmanagement(){
    PTM->show();
    this->hide();
}


void MainWindow::open_datamaintainance(){
    DM->show();
    this->hide();
}
