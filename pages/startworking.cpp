#include "startworking.h"
#include "ui_startworking.h"


startWorking::startWorking(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::startWorking)
{
    ui->setupUi(this);
}

startWorking::startWorking(Task* t, modes m, int i, QString name,  QWidget *parent) :
    QWidget(parent),
    ui(new Ui::startWorking), task(t),index(i),mode(m), printerName(name)
{
    setAttribute( Qt::WA_DeleteOnClose, true );
    ui->setupUi(this);

    printerIndex = task->printerIdx;
    cout<<"SW page: "<<printerIndex<<endl;
    if(mode == WORKING)
        ui->lastPattern_button->setEnabled(false);

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
    if(mode == WORKING){
        QMessageBox::StandardButton reply = QMessageBox::question(NULL, "暂停", "是否暂停任务？", QMessageBox::Yes | QMessageBox::No );
        if(reply == QMessageBox::No)
            return ;
        else{
            emit asking_for_adjacent(-1, true, printerIndex);
        }
    }
    else{
        emit asking_for_adjacent(index-1, false, printerIndex);
    }
}

void startWorking::on_nextPattern_button_clicked(){
    if(mode == WORKING){
        QMessageBox::StandardButton reply = QMessageBox::question(NULL, "确认完成", "是否确认本单已经完成？", QMessageBox::Yes | QMessageBox::No );
        if(reply == QMessageBox::No)
            return ;
        else{
            emit asking_for_adjacent(index+1, true, printerIndex);
        }
    }
    else{
        emit asking_for_adjacent(index+1, false, printerIndex);
    }
}

void startWorking::on_frontPage_button_clicked(){
    int code = api->sendPrintingFile(task->name, task->isDark, printerName, 1);
    if(code != 1){
        if(code == 3)
            QMessageBox::StandardButton reply = QMessageBox::information(NULL, "上传失败", "文件拷贝错误", QMessageBox::Yes);
        else
            QMessageBox::StandardButton reply = QMessageBox::information(NULL, "上传失败", "错误代码： "+QString::number(code), QMessageBox::Yes);
    }
    //ui->frontPage_button->setText("再次上传前片文件");
}
void startWorking::on_backPage_button_clicked(){
    int code = api->sendPrintingFile(task->name, task->isDark, printerName, 0);
    if(code != 1){
        if(code == 3)
            QMessageBox::StandardButton reply = QMessageBox::information(NULL, "上传失败", "文件拷贝错误", QMessageBox::Yes);
        else
            QMessageBox::StandardButton reply = QMessageBox::information(NULL, "上传失败", "错误代码： "+QString::number(code), QMessageBox::Yes);
    }

    //ui->backPage_button->setText("再次上传后片文件");
}


void startWorking::receiving_new_task(Task* t, int i, int ID){
    if(ID != printerIndex) return;
    if(t == NULL && mode == SHOWING){
        if(i < 0){
            QMessageBox::StandardButton reply = QMessageBox::information(NULL, "无更多任务", "已到任务列表顶端！", QMessageBox::Yes );
            return ;
        }else {
            QMessageBox::StandardButton reply = QMessageBox::information(NULL, "无更多任务", "已到任务列表底端！", QMessageBox::Yes );
            return ;
        }
    }
    if(t == NULL && mode == WORKING){
        this->close();
        return;
    }
    index = i;
    task = t;
    init();
}

// init operations based on task specifing.
void startWorking::init(){
    TM = new TaskModel(*task);

    ui->tableView->setModel(TM);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch );
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch );

    ui->printerIDLabel->setText(printerName);
    ui->patternIDLabel->setText(task->name);
    ui->type_label->setText(printTypeString[task->type]);

    QSize picSize(473, 300);
    if(task->pattern->hasMimages){
        ui->M_label->setPixmap(api->loadPics(task->pattern->name, 1).scaled(picSize,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->M_label->show();
    }else{
        ui->M_label->setText("效果图/模特图");
    }
    if(task->pattern->hasPimages){
        ui->P_label->setPixmap(api->loadPics(task->pattern->name, 0).scaled(picSize,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->P_label->show();

    }
    else{
            ui->P_label->setText("生产版单");
    }

    if(mode == WORKING){

        ui->lastPattern_button->setText("暂停");
        ui->lastPattern_button->setEnabled(true);
        ui->nextPattern_button->setText("打印完毕");

        ui->backPage_button->setHidden(false);
        ui->frontPage_button->setHidden(false);
        if(task->pattern->hasBack){
            ui->backPage_button->setEnabled(true);
            ui->backPage_button->setText("上传后片文件");
        }else{
            ui->backPage_button->setEnabled(false);
            ui->backPage_button->setText("本款无后片");
        }
        if(task->pattern->hasFront){
            ui->frontPage_button->setEnabled(true);
            ui->frontPage_button->setText("上传前片文件");
        }else{
            ui->frontPage_button->setEnabled(false);
            ui->frontPage_button->setText("本款无前片");
        }
        ui->file_label->setText("文件齐全");

    }else{
        ui->backPage_button->setHidden(true);
        ui->frontPage_button->setHidden(true);
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

    emit update(printerIndex);
}


API* startWorking::api = NULL;
