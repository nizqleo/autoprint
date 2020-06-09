#include "printtaskmanagement.h"
#include "ui_printtaskmanagement.h"
#include "mainwindow.h"
printTaskManagement::printTaskManagement(MainWindow *mainwindow, QWidget *parent) :
    QWidget(parent),MW(mainwindow),
    ui(new Ui::printTaskManagement)
{
    ui->setupUi(this);


    connect(this, SIGNAL(datapageopen()), MW, SLOT(open_datamaintainance()));
    connect(this, SIGNAL(settingpageopen()), MW, SLOT(open_printersetting()));

    //QPixmap pixmap("C:\\Users\\nzq82\\source\\QtRepos\\data\\logo.png");
    QPixmap pixmap(".\\data\\logo.png");
    ui->logoLabel->setPixmap(pixmap);
    ui->logoLabel->show();


    for (int i = 0; i < MW->printers.size();i++) {
        ui->comboBox->addItem(QString::fromStdString(to_string(MW->printers[i].printerID)+"号打印机"));
    }

}

printTaskManagement::~printTaskManagement()
{
    delete ui;
}


void printTaskManagement::on_dataPage_button_clicked(){
    emit datapageopen();
    this->close();
}


void printTaskManagement::on_settingPage_button_clicked(){
    emit settingpageopen();
    this->close();
}


void printTaskManagement::on_ERP_button_clicked(){
    // read order file
    // renew tableview

    readERPOriginalData();
    jobAssignment();


}


void printTaskManagement::jobAssignment(){

    assignment[0] = Patterns;


}
void printTaskManagement::readERPOriginalData(){

    //ifstream inFile(".\\data\\order.csv", ios::in);
    ifstream inFile("C:\\Users\\nzq82\\source\\QtRepos\\data\\order.csv", ios::in);
    if(!inFile){
        cout<<"failed to open order file."<<endl;
    }
    string lineStr;
    QMap<QString, QMap<QString, int>> OrderMap;

    //totalNum.clear();
    std::vector<int> temp1;
    temp1.resize(5, 0);
    std::vector<std::vector<int>> temp;
    temp.resize(8, temp1);
    while (getline(inFile, lineStr))
    {
        //std::cout<<lineStr<<std::endl;
        stringstream ss(lineStr);
        string color,size, num;

        getline(ss, color, ',');
        QString Pattern = QString::fromStdString(color);
        getline(ss, color, ',');
        getline(ss, size, ',');
        QString cloth = QString::fromStdString(color+size);
        getline(ss, num, ',');
        int d = atoi(num.c_str());
        OrderMap[Pattern][cloth] += d;
        if(orderArray.find(Pattern) == orderArray.end()){
            orderArray[Pattern] = temp;
            Patterns.push_back(Pattern);
        }
        orderArray[Pattern][ColorMap(color)][SizeMap(size)] += d;
    }
    cout<<"order reading ready."<<endl;
}
