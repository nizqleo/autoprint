#include "addprinttask.h"
#include "ui_addprinttask.h"
#include "mainwindow.h"


addPrintTask::addPrintTask(MainWindow *MW, QWidget *parent) :
    QWidget(parent),MW(MW),
    ui(new Ui::addPrintTask)
{
    ui->setupUi(this);

    QStringList word_list;

    for(int i = 0; i < MW->DM->DModel->patternNames.size(); i++){
        word_list<<MW->DM->DModel->patternNames[i];
    }

    QCompleter *completer = new QCompleter(word_list, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->pattern_lineEdit->setCompleter(completer);

    model = new QStandardItemModel(9,6);

    QStringList labels = QObject::trUtf8("S,M,L,XL,XXL,总计").simplified().split(",");
    model->setHorizontalHeaderLabels(labels);
    labels = QObject::trUtf8("白色,黑色,粉色,蓝色,绿色,黄色,紫色,红色,总计").simplified().split(",");
    model->setVerticalHeaderLabels(labels);

    for (int row = 0; row < 9; ++row){
        for (int column = 0; column < 6; ++column){
            QModelIndex index = model->index(row, column, QModelIndex());
            model->setData(index, QVariant(0));
        }
    }

    ui->tableView->setModel(model);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView->show();

    ui->comboBox->addItem("自动分配");
    printerNumber = MW->printers.size();
    for (int i = 0; i < printerNumber;i++) {
        ui->comboBox->addItem(MW->printers[i].name);
    }
    last_r = last_c = 0;


    for(int i = 0; i < 13; i++){
        ui->type_comboBox->addItem(printTypeString[i]);
    }
}


void addPrintTask::on_pattern_lineEdit_textChanged(QString s){
    //cout<<"edited"<<endl;
    if(MW->DM->DModel->hasPatternName(s)){
        pattern = MW->DM->DModel->patternPointer(s);
        QString file = "";
        if(pattern->DarkReady)  file+="深色文件\n";
        if(pattern->LightReady)  file+="浅色文件\n";
        if(file == "") file = "文件不齐全";
        else file.chop(1);

        ui->file_label->setText(file);

        if(pattern->hasMimages){
            ui->M_label->setPixmap(MW->api->loadPics(pattern->name, 1).scaled(QSize(200, 200),Qt::KeepAspectRatio,Qt::SmoothTransformation));
            ui->M_label->show();
        }
        if(pattern->hasPimages){
            ui->P_label->setPixmap(MW->api->loadPics(pattern->name, 0).scaled(QSize(200, 200),Qt::KeepAspectRatio,Qt::SmoothTransformation));
            ui->P_label->show();
        }
    }
}


addPrintTask::~addPrintTask()
{
    delete ui;
}

void addPrintTask::on_Return_button_clicked(){
    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "返回", "返回将不会保存当前已编辑信息，是否确认返回？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No)
        return ;
    else{
        this->close();
    }
}


void addPrintTask::on_comfirm_button_clicked(){
    if(ui->pattern_lineEdit->text() == ""){
        QMessageBox::StandardButton reply = QMessageBox::information(NULL, "无款号", "请输入款号！", QMessageBox::Yes );
        return;
    }

    numbers = new int*[8];

    for(int i = 0; i < 8; i++){
        numbers[i] = new int[5];
        for(int j = 0; j < 5; j++){
            QModelIndex index = model->index(i, j, QModelIndex());
            numbers[i][j] = model->data(index).toInt();
        }
    }
    emit send_orders(numbers, ui->pattern_lineEdit->text(), ui->type_comboBox->currentIndex(), ui->comboBox->currentIndex());
    this->close();
}


void addPrintTask::on_tableView_clicked(){

    QModelIndex index = model->index(last_r, last_c, QModelIndex());
    if(model->data(index).toInt() < 0){
        model->setData(index, 0);
        QMessageBox::StandardButton reply = QMessageBox::information(NULL, "值错误", "请输入正值。", QMessageBox::Yes );
        return ;
    }
    // update column end
    int sum = 0;
    for(int i = 0; i < 8; i++){
        QModelIndex index = model->index(i, last_c, QModelIndex());
        sum += model->data(index).toInt();
    }
    model->setData(model->index(8, last_c, QModelIndex()), QVariant(sum));

    // update row end
    sum = 0;
    for(int i = 0; i < 5; i++){
        QModelIndex index = model->index(last_r, i, QModelIndex());
        sum += model->data(index).toInt();
    }
    model->setData(model->index(last_r, 5, QModelIndex()), QVariant(sum));


    int r = ui->tableView->currentIndex().row();
    int c = ui->tableView->currentIndex().column();

    sum = 0;
    for(int i = 0; i < 8; i++){
        QModelIndex index = model->index(i, c, QModelIndex());
        sum += model->data(index).toInt();
    }
    model->setData(model->index(8, c, QModelIndex()), QVariant(sum));

    // update row end
    sum = 0;
    for(int i = 0; i < 5; i++){
        QModelIndex index = model->index(r, i, QModelIndex());
        sum += model->data(index).toInt();
    }
    model->setData(model->index(r, 5, QModelIndex()), QVariant(sum));

    sum = 0;
    for(int i = 0; i < 5; i++){
        QModelIndex index = model->index(8, i, QModelIndex());
        sum += model->data(index).toInt();
    }
    model->setData(model->index(8, 5, QModelIndex()), QVariant(sum));

    last_r = r;
    last_c = c;
}
