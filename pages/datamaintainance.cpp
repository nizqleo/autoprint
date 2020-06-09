#include "datamaintainance.h"
#include "ui_datamaintainance.h"
#include "mainwindow.h"
dataMaintainance::dataMaintainance(MainWindow *mainwindow, QWidget *parent) :
    QWidget(parent),MW(mainwindow),
    ui(new Ui::dataMaintainance)
{
    ui->setupUi(this);

    //自定义表模型
    DModel = new datasetModel;
    ui->tableView->setModel(DModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch );
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents );
    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents );
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );

    ui->Edit_button->setEnabled(false);
    ui->Delete_button->setEnabled(false);

    connect(this, SIGNAL(taskpageopen()), MW, SLOT(open_printtaskmanagement()));
    connect(this, SIGNAL(settingpageopen()), MW, SLOT(open_printersetting()));

    //QPixmap pixmap("C:\\Users\\nzq82\\source\\QtRepos\\data\\logo.png");
    QPixmap pixmap(".\\data\\logo.png");
    ui->logoLabel->setPixmap(pixmap);
    ui->logoLabel->show();
}

dataMaintainance::~dataMaintainance()
{
    delete ui;
}

void dataMaintainance::on_Return_button_clicked(){
    MW->show();
    this->hide();
}



void dataMaintainance::on_Delete_button_clicked(){
    int row = ui->tableView->currentIndex().row();
    std::cout<<row<<std::endl;
    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "删除", "确定删除这一条项目?", QMessageBox::No | QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::No)
        return ;
    ui->Edit_button->setEnabled(false);
    ui->Delete_button->setEnabled(false);
    DModel->deleteItem(row);
}

void dataMaintainance::on_Edit_button_clicked(){
    int row = ui->tableView->currentIndex().row();
    dialog = new Dialog(this);
    std::string pattern = DModel->getItem(row);
    dialog->init(pattern, DModel->dir, DModel->fileStatus[pattern]);
    connect(dialog, SIGNAL(confirmEditing(QString, QString, QString, QString, QString, QString, QString, bool,bool)),
            this, SLOT(save_files(QString, QString, QString, QString, QString, QString, QString, bool,bool)));
    connect(dialog, SIGNAL(PatternNameChanged(QString)), this, SLOT(delete_files(QString)));
    dialog->show();
}

void dataMaintainance::on_Add_button_clicked(){
    dialog = new Dialog(this);
    connect(dialog, SIGNAL(confirmEditing(QString, QString, QString, QString, QString, QString, QString, bool,bool)),
            this, SLOT(save_files(QString, QString, QString, QString, QString, QString, QString, bool,bool)));
    dialog->show();
}
void dataMaintainance::on_Search_button_clicked(){
    MW->show();
    this->hide();
}

void dataMaintainance::save_files(QString pattern, QString DFAR4Address, QString DBAR4Address,
                                  QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                                  QString MimageAddress, bool hasBack, bool hasFront){

    DModel->save_files( pattern.toStdString(),  DFAR4Address, DBAR4Address,
                        LFAR4Address, LBAR4Address, PimageAddress,
                        MimageAddress, hasBack, hasFront);
}

void dataMaintainance::on_tableView_clicked(){
    ui->Edit_button->setEnabled(true);
    ui->Delete_button->setEnabled(true);
}

void dataMaintainance::delete_files(QString pattern){

    DModel->deleteItem(DModel->getIndex(pattern));
}


void dataMaintainance::on_taskPage_button_clicked(){
    emit taskpageopen();
    this->close();
}


void dataMaintainance::on_settingPage_button_clicked(){
    emit settingpageopen();
    this->close();
}

bool dataMaintainance::patternNameOverlapCheck(QString s){
    return DModel->patternNameOverlapCheck(s);
}
