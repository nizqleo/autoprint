#include "printing.h"
#include "ui_printing.h"

Printing::Printing(OrderModel* OM,QWidget* SW,QWidget *parent) :
    QWidget(parent),SW(SW),OM(OM),
    ui(new Ui::Printing)
{
    ui->setupUi(this);
    currentOrderIndex = 0;

    TotalJobs = OM->totalJobNumber;
    finishedJobs = 0;
    lastPattern = "";
    connect(this, SIGNAL(finish_sending()),this, SLOT(slotfor_finish_uploading()));
    connect(this, SIGNAL(start_printing()),this, SLOT(slotfor_starting_printing()));
    connect(this, SIGNAL(finish_printing()),this, SLOT(slotfor_finishing_printing()));
    slotfor_state_update();
    time.start();
    ui->PS_label->setText("上传图样中");
    ui->NF_label->setText("0");
    ui->ETA_label->setText("/");
    ui->TN_label->setText(QString::number(TotalJobs));
}

Printing::~Printing()
{
    delete ui;
}


void Printing::on_Stop_button_clicked(){
    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "中止打印", "确定要中止打印?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No)
        return ;
    else{// Actually stop printing
        SW->show();
        this->close();
    }
}

void Printing::slotfor_state_update(){

    std::cout<<"updating"<<std::endl;
    std::pair<QString, QString> info = OM->OrderInfo(currentOrderIndex);

    pattern = info.first;
    cloth = info.second;

    ui->CJ_Info_label->setText(pattern+"_"+cloth);
    ui->progressBar->setValue(finishedJobs*100.0/(TotalJobs*1.0));
    int time_Diff = time.elapsed();
    if(finishedJobs != 0){
        int time_left = time_Diff*(TotalJobs-finishedJobs)/(finishedJobs*1.0);
        time_left /=1000.0;
        ui->ETA_label->setText(QString::number(time_left));
    }
}


void Printing::slotfor_finish_uploading(){
    ui->PS_label->setText("打印机就绪， 请在放置衣物后操作打印机开始打印。");
    QEventLoop eventloop;
    QTimer::singleShot(5000, &eventloop, SLOT(quit()));
    eventloop.exec();

    emit start_printing();

}

void Printing::slotfor_starting_printing(){
    ui->PS_label->setText("正在打印...");
    QEventLoop eventloop;
    QTimer::singleShot(5000, &eventloop, SLOT(quit()));
    eventloop.exec();
    emit finish_printing();
}

void Printing::slotfor_finishing_printing(){
    ui->PS_label->setText("打印完毕！请取下成衣。");
    lastPattern = pattern;
    OM->orderMap[pattern][cloth]--;
    finishedJobs++;

    if(OM->orderMap[pattern][cloth] == 0){
        //OM->orderMap[pattern].remove(cloth);
        currentOrderIndex++;
        if(currentOrderIndex > OM->orderMap.size()){

            QMessageBox::StandardButton reply = QMessageBox::information(NULL, "完成打印", "打印完毕！", QMessageBox::Yes);

            return;
        }
    }

    QEventLoop eventloop;
    QTimer::singleShot(5000, &eventloop, SLOT(quit()));
    eventloop.exec();

    slotfor_state_update();

    if(pattern != lastPattern){
        ui->PS_label->setText("上传图样中");
        send_AR4_to_printer();
    }
    else {
        emit finish_sending();
    }
}


void Printing::send_AR4_to_printer(){

    // use the pattern to load file and send to the printer.
    // TO DO
    std::cout<<"start sending..."<<std::endl;
    QEventLoop eventloop;
    QTimer::singleShot(5000, &eventloop, SLOT(quit()));
    eventloop.exec();
    std::cout<<"finish sending..."<<std::endl;
    emit finish_sending();
}
