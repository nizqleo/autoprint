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
void printerSetting::on_Exit_button_clicked(){
    MW->show();
    this->hide();
}



void printerSetting::on_Delete_button_clicked(){
//    int row = ui->tableView->currentIndex().row();
//    std::cout<<row<<std::endl;
//    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "删除", "确定删除这一条项目?", QMessageBox::No | QMessageBox::Yes, QMessageBox::No);
//    if (reply == QMessageBox::No)
//        return ;
//    ui->Edit_button->setEnabled(false);
//    ui->Delete_button->setEnabled(false);

//    MW->api->deletePrinter(row);
//    ui->totalNum_label->setNum(DModel->getTotalPatternNum());
}

void printerSetting::on_Edit_button_clicked(){
//    int row = ui->tableView->currentIndex().row();
//    dialog = new Dialog(this, MW->api);
//    std::string pattern = DModel->getItem(row);
//    dialog->init(pattern);
//    connect(dialog, SIGNAL(confirmEditing(QString, QString, QString, QString, QString, QString, QString, bool,bool)),
//            this, SLOT(save_files(QString, QString, QString, QString, QString, QString, QString, bool,bool)));
//    connect(dialog, SIGNAL(PatternNameChanged(QString)), this, SLOT(delete_files(QString)));
//    dialog->show();
//    ui->Edit_button->setEnabled(false);
//    ui->Delete_button->setEnabled(false);
}

void printerSetting::on_Add_button_clicked(){
//    dialog = new Dialog(this, MW->api);
//    connect(dialog, SIGNAL(confirmEditing(QString, QString, QString, QString, QString, QString, QString, bool,bool)),
//            this, SLOT(save_files(QString, QString, QString, QString, QString, QString, QString, bool,bool)));
//    dialog->show();
}


void printerSetting::save_files(QString pattern, QString DFAR4Address, QString DBAR4Address,
                                  QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                                  QString MimageAddress, bool hasBack, bool hasFront){

//    DModel->save_files( pattern.toStdString(),  DFAR4Address, DBAR4Address,
//                        LFAR4Address, LBAR4Address, PimageAddress,
//                        MimageAddress, hasBack, hasFront);
//    ui->totalNum_label->setNum(DModel->getTotalPatternNum());
}

void printerSetting::on_tableView_clicked(){
//    ui->Edit_button->setEnabled(true);
//    ui->Delete_button->setEnabled(true);
}

void printerSetting::delete_files(QString pattern){

//    DModel->deleteItem(DModel->getIndex(pattern));
//    ui->totalNum_label->setNum(DModel->getTotalPatternNum());
}



void printerSetting::on_search_lineEdit_textEdited(QString s){

//    DModel->searching(s.toStdString());
//    ui->Edit_button->setEnabled(false);
//    ui->Delete_button->setEnabled(false);
}
