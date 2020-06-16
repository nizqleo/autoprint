#include "startworking.h"
#include "ui_startworking.h"


startWorking::startWorking(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::startWorking)
{
    ui->setupUi(this);
}



startWorking::startWorking(Task* t, modes m, int i, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::startWorking), task(t),index(i),mode(m)
{
    ui->setupUi(this);

    printerIndex = task->printerIdx;
    init();

}


startWorking::~startWorking()
{
    delete ui;
}


void startWorking::on_Return_button_clicked(){
    this->close();
}

void startWorking::on_lastPattern_button_clicked(){
    emit asking_for_adjacent(index-1);
}

void startWorking::on_nextPattern_button_clicked(){
    if(mode == WORKING){
        QMessageBox::StandardButton reply = QMessageBox::question(NULL, "确认完成", "是否确认本单已经完成？", QMessageBox::Yes | QMessageBox::No );
        if(reply == QMessageBox::No)
            return ;
        else{
            cout<<"working next pattern"<<endl;
            task->Tstatus = FINISHED;
            emit asking_for_adjacent(index+1);
        }
    }
    else{
        cout<<"showing next pattern"<<endl;
        emit asking_for_adjacent(index+1);
    }
}

void startWorking::receiving_new_task(Task* t, int i){
    if(t == NULL){
        QMessageBox::StandardButton reply = QMessageBox::information(NULL, "无任务", "无更多任务！", QMessageBox::Yes );
        return ;
    }
    index = i;
    task = t;
    init();
}

// init operations based on task specifing.
void startWorking::init(){
    //cout<<"init called"<<endl;
    TM = new TaskModel(*task);

    ui->tableView->setModel(TM);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch );
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch );

    ui->printerIDLabel->setNum(printerIndex);
    ui->patternIDLabel->setText(QString::fromStdString(task->name));

    if(task->pattern->hasMimages){
        ui->M_label->setPixmap(task->pattern->Mimages.scaled(QSize(200, 200)));
        ui->M_label->show();
    }
    if(task->pattern->hasPimages){
        ui->P_label->setPixmap(task->pattern->Pimages.scaled(QSize(200, 200)));
        ui->P_label->show();
    }

    if(mode == WORKING){
        // send files!!!!
        cout<<"sending files"<<endl;

        ui->file_label->setText("上传中");
        QPalette red;
        red.setColor(QPalette::WindowText,Qt::red);
        ui->file_label->setPalette(red);
        // when finished loading

        ui->file_label->setText("上传完毕");
        QPalette green;
        green.setColor(QPalette::WindowText,Qt::darkGreen);
        ui->file_label->setPalette(green);

        task->Tstatus = PRINTING;

    }else{
        if(task->fileReady){
            ui->file_label->setText("文件齐全");
            QPalette green;
            green.setColor(QPalette::WindowText,Qt::darkGreen);
            ui->file_label->setPalette(green);
        }
        else{
            ui->file_label->setText("文件不齐");
            QPalette red;
            red.setColor(QPalette::WindowText,Qt::red);
            ui->file_label->setPalette(red);
        }
    }
}
