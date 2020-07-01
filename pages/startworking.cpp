#include "startworking.h"
#include "ui_startworking.h"
#include "API/api.h"

startWorking::startWorking(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::startWorking)
{
    ui->setupUi(this);
}

startWorking::startWorking(Task* t, modes m, int idx, QString ptrname,  QWidget *parent) :
    QWidget(parent),
    ui(new Ui::startWorking), task(t),index(idx),mode(m), printerName(ptrname)
{
    setAttribute( Qt::WA_DeleteOnClose, true );
    ui->setupUi(this);

    printerIndex = task->printerIdx;

    if(mode == WORKING){
        ui->lastPattern_button->setText("暂停");
        ui->lastPattern_button->setEnabled(true);
        ui->nextPattern_button->setText("打印完毕");

        ui->upload_button->setHidden(false);
        ui->upload_button->setEnabled(false);
    }else{

        ui->upload_button->setHidden(true);
    }
    QPalette darkRed;
    darkRed.setColor(QPalette::WindowText,Qt::darkRed);
    ui->position_label->setPalette(darkRed);
    ui->platenSize_label->setPalette(darkRed);
    ui->ink_label->setPalette(darkRed);

    specialization();
}
// init operations based on task specifing.
void startWorking::specialization(){

    vector<QString> block;
\
    api->fillImagesAddresses(task->pattern->name, task->pattern->AR4Files->images, block, false);

    TM = new TaskModel(*task);

    ui->tableView->setModel(TM);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch );
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch );



    // ARX4 files process
    int fileColorUsed;
    if(std::find(task->pattern->specialColor.begin(), task->pattern->specialColor.end(), task->colorInfo)
            == task->pattern->specialColor.end())// use default color
        fileColorUsed = -1;
    else
        fileColorUsed = task->colorInfo;

    fileModel = new QStandardItemModel(this);
    qDebug()<<"fileColorUsed"<<fileColorUsed<<endl;

    for(auto i = task->pattern->AR4Files->files.begin(); i < task->pattern->AR4Files->files.end(); i++){
        if(i->applyColor == fileColorUsed){
            fileIndexMap.push_back(i - task->pattern->AR4Files->files.begin());
            QString string = positionString[i->position] + "文件";
            QStandardItem *item = new QStandardItem(string);

            fileModel->appendRow(item);
        }
    }


    ui->file_listView->setModel(fileModel);
//    ui->file_listView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->file_listView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
//    ui->file_listView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->printerIDLabel->setText(printerName);
    ui->patternIDLabel->setText(task->name);
    ui->type_label->setText(printTypeString[task->type]);
    ui->note_label->setText(task->pattern->Notes);

    QSize picSize(700, 300);

    if(task->pattern->hasPimages){
        ui->P_label->setPixmap(api->loadPics(task->pattern->name, 0).scaled(picSize,Qt::KeepAspectRatio,
                                                                            Qt::SmoothTransformation));
        ui->P_label->show();
    }
    else{
        ui->P_label->setText("生产版单");
    }



    emit updatePrinterTaskStatus(printerIndex);
}

startWorking::~startWorking()
{
    delete ui;
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
            task->pattern->AR4Files->images.clear();
            emit asking_for_adjacent(index+1, true, printerIndex);

        }
    }
    else{
        emit asking_for_adjacent(index+1, false, printerIndex);
    }
}

void startWorking::on_upload_button_clicked(){
    if(!ui->file_listView->currentIndex().isValid())
        return;

    int fileIdx = fileIndexMap[ui->file_listView->currentIndex().row()];
    ARX4& file = task->pattern->AR4Files->files[fileIdx];
    int code = api->sendPrintingFile(task->name, file, printerName, 1);
    if(code != 0){
        if(code == 3)
            QMessageBox::StandardButton reply = QMessageBox::information(NULL, "上传失败", "文件拷贝错误", QMessageBox::Yes);
        else
            QMessageBox::StandardButton reply = QMessageBox::information(NULL, "上传失败", "错误代码： "+QString::number(code), QMessageBox::Yes);
    }

    ui->position_label->setText(positionString[file.position]);
    ui->platenSize_label->setText(platenSizeString[file.platenSize]);
    ui->ink_label->setText(InkString[file.whiteInk]);

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
    specialization();
}

void startWorking::on_file_listView_clicked(){
    ui->upload_button->setEnabled(true);
}

