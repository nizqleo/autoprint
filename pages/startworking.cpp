#include "startworking.h"
#include "ui_startworking.h"

startWorking::startWorking(QMainWindow* MW, QWidget *parent) :
    QWidget(parent),
    MW(MW),
    ui(new Ui::startWorking)
{
    ui->setupUi(this);


    ifstream inFile(".\\data\\order.csv", ios::in);
//    ifstream inFile("C:\\Users\\nzq82\\source\\QtRepos\\data\\order.csv", ios::in);
    if(!inFile){
        cout<<"failed to open order file."<<endl;
    }
    string lineStr;
    QMap<QString, QMap<QString, int>> OrderMap;
    QMap<QString, std::vector<std::vector<int>>> orderArray;
    PUTable.clear();
//    totalNum.clear();
    std::vector<int> temp1;
    temp1.resize(5, 0);
    std::vector<std::vector<int>> temp;
    temp.resize(8, temp1);
    while (getline(inFile, lineStr))
    {
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
        totalNum[Pattern] += d;
        PUTable[cloth]+=d;
    }
    cout<<"file reading ready."<<endl;

    OM = new OrderModel;
    OM->setCurrentPattern(Patterns[currentPatternIndex]);
    OM->setOrderMap(OrderMap);
    OM->setOrderArray(orderArray);

    ui->tableView->setModel(OM);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch );
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );

    ui->printerIDLabel->setNum(1);
    ui->patternIDLabel->setText(Patterns[currentPatternIndex]);
    ui->TotalNumLabel->setNum(totalNum[Patterns[currentPatternIndex]]);
}

int startWorking::SizeMap(string size){
    if(size == "S")
        return 0;
    if(size == "M")
        return 1;
    if(size == "L")
        return 2;
    if(size == "XL")
        return 3;
    if(size == "XXL")
        return 4;
}

int startWorking::ColorMap(string size){
    if(size == "W")
        return 0;
    if(size == "B")
        return 1;
    if(size == "P")
        return 2;
    if(size == "B")
        return 3;
    if(size == "G")
        return 4;
    if(size == "Y")
        return 5;
    if(size == "U")
        return 6;
    if(size == "R")
        return 7;
}


startWorking::~startWorking()
{
    delete ui;
}


void startWorking::on_Return_button_clicked(){
    MW->show();
    this->hide();
}


void startWorking::on_PL_button_clicked(){

    PickUpList* PUL = new PickUpList(PUTable);
//    cout<<PUTable["BXL"]<<endl;

    PUL->show();
}


void startWorking::on_CP_button_clicked(){
    Printing* PT = new Printing(OM, this);

    PT->show();
    PT->send_AR4_to_printer();
}

void startWorking::on_lastPattern_button_clicked(){
    if(currentPatternIndex == 0)
        return;
    currentPatternIndex--;
    OM->setCurrentPattern(Patterns[currentPatternIndex]);
    ui->patternIDLabel->setText(Patterns[currentPatternIndex]);
    ui->TotalNumLabel->setNum(totalNum[Patterns[currentPatternIndex]]);
}

void startWorking::on_nextPattern_button_clicked(){
    if(currentPatternIndex == Patterns.size()-1)
        return;
    currentPatternIndex++;
    OM->setCurrentPattern(Patterns[currentPatternIndex]);
    ui->patternIDLabel->setText(Patterns[currentPatternIndex]);
    ui->TotalNumLabel->setNum(totalNum[Patterns[currentPatternIndex]]);
}

