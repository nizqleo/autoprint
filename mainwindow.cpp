#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pixmap(".\\data\\logo.png");
    //QPixmap pixmap("C:\\Users\\nzq82\\source\\QtRepos\\data\\logo.png");
    ui->logo_label->setPixmap(pixmap);
    ui->logo_label->show();


    loadPrintersInfo();
//    DM = new dataMaintainance(this);
//    SW = new startWorking(this);

    defaultDir = ".\\";
}


void MainWindow::loadPrintersInfo(){
    ifstream inFile(".\\data\\printers.csv", ios::in);
    //ifstream inFile("C:\\Users\\nzq82\\source\\QtRepos\\data\\printers.csv", ios::in);
    if(!inFile){
        cout<<"failed to open printer file."<<endl;
    }

    string lineStr;

    while (getline(inFile, lineStr))
    {
        std::cout<<lineStr<<std::endl;
        printer temp(lineStr);
        printers.push_back(temp);
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Login_button_clicked(){
    this->hide();
    open_datamaintainance();
}

//void MainWindow::on_SW_button_clicked(){
//    this->hide();
//    SW->show();
//}


void MainWindow::on_Exit_button_clicked(){
    this->close();
}


void MainWindow::open_printersetting(){
    PS = new printerSetting(this);
    PS->show();
    this->hide();
}


void MainWindow::open_printtaskmanagement(){
    PTM = new printTaskManagement(this);
    PTM->show();
    this->hide();
}


void MainWindow::open_datamaintainance(){
    DM = new dataMaintainance(this);
    DM->show();
    this->hide();
}
