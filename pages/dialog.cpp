#include "dialog.h"
#include "ui_dialog.h"
#include "pages\datamaintainance.h"


Dialog::Dialog(dataMaintainance* DM, QWidget *parent) :
    QDialog(parent),DM(DM),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    isEdit = false;
    PatternChanged = false;


    setWindowTitle(tr("新增条目"));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_comboBox_changed(const QString &)));
    DFAR4Address = "";
    DBAR4Address = "";
    LFAR4Address = "";
    LBAR4Address = "";

    //default state
    hasBack = true;
    hasFront = true;
    ui->DBtool_button->setEnabled(true);
    ui->LBtool_button->setEnabled(true);
    ui->DB_lineEdit->setEnabled(true);
    ui->LB_lineEdit->setEnabled(true);

    ui->DFtool_button->setEnabled(true);
    ui->LFtool_button->setEnabled(true);
    ui->DF_lineEdit->setEnabled(true);
    ui->LF_lineEdit->setEnabled(true);
    //dir = "C:\\Users\\nzq82\\source\\QtRepos";
//    dir = ".";
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::init(std::string pattern, std::string dir, int status){// happens only in editing
    setWindowTitle(tr("修改条目"));
    isEdit=true;
    Pattern = QString::fromStdString(pattern);
    ui->Pattern_lineEdit->setText(Pattern);
    originalPatternName = Pattern;
    Dir = QString::fromStdString(dir);

    if (status<48) {//
        if((status>>5) & 1 == 1){ //10 只有后
            ui->comboBox->setCurrentIndex(2);
            hasFront = false;

            ui->DFtool_button->setEnabled(false);
            ui->LFtool_button->setEnabled(false);
            ui->DF_lineEdit->setEnabled(false);
            ui->LF_lineEdit->setEnabled(false);
            ui->DF_lineEdit->setText("");
            ui->LF_lineEdit->setText("");
        }
        else {//01
            ui->comboBox->setCurrentIndex(1);
            hasBack = false;
            ui->DBtool_button->setEnabled(false);
            ui->LBtool_button->setEnabled(false);
            ui->DB_lineEdit->setEnabled(false);
            ui->LB_lineEdit->setEnabled(false);
            ui->DB_lineEdit->setText("");
            ui->LB_lineEdit->setText("");
        }
    }

    if(((status>>3) & 1) == 1)
        LFAR4Address = QString::fromStdString(dir + "\\AR4\\"+pattern+"\\LF.txt");
    if(((status>>2) & 1) == 1)
        DFAR4Address = QString::fromStdString(dir + "\\AR4\\"+pattern+"\\DF.txt");
    if(((status>>1) & 1) == 1)
        LBAR4Address = QString::fromStdString(dir + "\\AR4\\"+pattern+"\\LB.txt");
    if((status & 1) == 1)
        DBAR4Address = QString::fromStdString(dir + "\\AR4\\"+pattern+"\\DB.txt");


    QFileInfo fileInfo;
    PimageAddress = QString::fromStdString(dir + "\\image\\"+pattern+"\\P.jpg");
    fileInfo.setFile(PimageAddress);
    if(!fileInfo.isFile())  PimageAddress = "";

    MimageAddress = QString::fromStdString(dir + "\\image\\"+pattern+"\\M.jpg");
    fileInfo.setFile(MimageAddress);
    if(!fileInfo.isFile())  MimageAddress = "";



    update();
}

void Dialog::on_DFtool_button_clicked(){
    QString fileNames = QFileDialog::getOpenFileName(this,tr("选择深色前片AR4文件"), DM->searchDir, tr("文本文件 (*.txt),*.txt;; AR4文件 (*.ar4),*.ar4"));
    DFAR4Address = fileNames;
    update();
}
void Dialog::on_DBtool_button_clicked(){
    QString fileNames = QFileDialog::getOpenFileName(this,tr("选择深色后片AR4文件"), DM->searchDir,tr("文本文件 (*.txt),*.txt;; AR4文件 (*.ar4),*.ar4"));
    DBAR4Address = fileNames;
    update();
}
void Dialog::on_LFtool_button_clicked(){
    QString fileNames = QFileDialog::getOpenFileName(this,tr("选择浅色前片AR4文件"), DM->searchDir,tr("文本文件 (*.txt),*.txt;; AR4文件 (*.ar4),*.ar4"));
    LFAR4Address = fileNames;
    update();
}
void Dialog::on_LBtool_button_clicked(){
    QString fileNames = QFileDialog::getOpenFileName(this,tr("选择浅色后片AR4文件"), DM->searchDir,tr("文本文件 (*.txt),*.txt;; AR4文件 (*.ar4),*.ar4"));
    LBAR4Address = fileNames;
    update();
}

void Dialog::on_Pimage_toolButton_clicked(){
    QString fileNames = QFileDialog::getOpenFileName(this,tr("选择生产版单文件"), DM->searchDir,  tr("图片文件(*png *jpg)"));
    PimageAddress = fileNames;
    update();
}
void Dialog::on_Mimage_toolButton_clicked(){
    QString fileNames = QFileDialog::getOpenFileName(this,tr("选择效果图/模特图文件"), DM->searchDir, tr("图片文件(*png *jpg)"));
    MimageAddress = fileNames;
    update();
}

// swifting pattern picture distribution
void Dialog::on_comboBox_changed(const QString & s){
    switch(ui->comboBox->currentIndex()){
    case 0://front & back
        hasBack = true;
        hasFront = true;

        ui->DBtool_button->setEnabled(true);
        ui->LBtool_button->setEnabled(true);
        ui->DB_lineEdit->setEnabled(true);
        ui->LB_lineEdit->setEnabled(true);

        ui->DFtool_button->setEnabled(true);
        ui->LFtool_button->setEnabled(true);
        ui->DF_lineEdit->setEnabled(true);
        ui->LF_lineEdit->setEnabled(true);
        break;
    case 1:// front
        hasBack = false;
        hasFront = true;
        ui->DBtool_button->setEnabled(false);
        ui->LBtool_button->setEnabled(false);
        ui->DB_lineEdit->setEnabled(false);
        ui->LB_lineEdit->setEnabled(false);
        ui->DB_lineEdit->setText("");
        ui->LB_lineEdit->setText("");

        ui->DFtool_button->setEnabled(true);
        ui->LFtool_button->setEnabled(true);
        ui->DF_lineEdit->setEnabled(true);
        ui->LF_lineEdit->setEnabled(true);
        break;
    case 2:// back
        hasFront = false;
        hasBack = true;

        ui->DBtool_button->setEnabled(true);
        ui->LBtool_button->setEnabled(true);
        ui->DB_lineEdit->setEnabled(true);
        ui->LB_lineEdit->setEnabled(true);

        ui->DFtool_button->setEnabled(false);
        ui->LFtool_button->setEnabled(false);
        ui->DF_lineEdit->setEnabled(false);
        ui->LF_lineEdit->setEnabled(false);
        ui->DF_lineEdit->setText("");
        ui->LF_lineEdit->setText("");
        break;
    }

    update();
}

void Dialog::update(){
    Pattern = ui->Pattern_lineEdit->text();

    ui->DF_lineEdit->setText(DFAR4Address);
    ui->DB_lineEdit->setText(DBAR4Address);
    ui->LF_lineEdit->setText(LFAR4Address);
    ui->LB_lineEdit->setText(LBAR4Address);

    ui->Mimage_lineEdit->setText(MimageAddress);
    ui->Pimage_lineEdit->setText(PimageAddress);

    QSize picSize(280, 280);
    QFileInfo fileInfo(MimageAddress);
    if(fileInfo.isFile()){
        QPixmap pixmapM(MimageAddress);
        pixmapM = pixmapM.scaled(picSize);
        ui->Mimage_label->setPixmap(pixmapM);
        ui->Mimage_label->show();
    }

    fileInfo.setFile(PimageAddress);
    if(fileInfo.isFile()){
        QPixmap pixmapP(PimageAddress);
        pixmapP = pixmapP.scaled(picSize);
        ui->Pimage_label->setPixmap(pixmapP);
        ui->Pimage_label->show();
    }
}

void Dialog::on_ConfirmButton_clicked(){
    int fileStatus = fileReadyCheck();

    if(fileStatus == 2){// file covering exists happens only when editing.
        QMessageBox::StandardButton reply = QMessageBox::question(NULL, "确认修改", "本次修改将导致已有文件被覆盖，是否确认修改？", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
            return ;
        else{
            emit confirmEditing(Pattern, DFAR4Address,DBAR4Address,LFAR4Address,LBAR4Address, PimageAddress, MimageAddress, hasBack, hasFront);
            if(PatternChanged){
                emit PatternNameChanged(originalPatternName);
            }
            this->close();
        }
    }
    if(fileStatus == 1){ // no file covering
        emit confirmEditing(Pattern, DFAR4Address,DBAR4Address,LFAR4Address,LBAR4Address, PimageAddress, MimageAddress, hasBack, hasFront);
        if(isEdit && PatternChanged){
            emit PatternNameChanged(originalPatternName);
        }
        this->close();
    }
    else{ // 0
        return;
    }
}

void Dialog::on_CancelButton_clicked(){
    QMessageBox::StandardButton reply = QMessageBox::question(NULL, "返回", "返回将不会保存当前已编辑信息，是否确认返回？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No)
        return ;
    else{
        this->close();
    }
}

void Dialog::on_Pattern_lineEdit_textEdited(QString s){

    Pattern = s;
    // check
    if(DM->patternNameOverlapCheck(s)){
        ui->label_error->setText("款号和已有款号冲突！");
        ui->ConfirmButton->setEnabled(false);
    }
    else{
        ui->label_error->setText("");
        ui->ConfirmButton->setEnabled(true);
    }
    update();
    PatternChanged = true;
}


void Dialog::on_DF_lineEdit_textEdited(QString s){
    DFAR4Address = s;
    update();
}
void Dialog::on_DB_lineEdit_textEdited(QString s){
    DBAR4Address = s;
    update();
}void Dialog::on_LF_lineEdit_textEdited(QString s){
    LFAR4Address = s;
    update();
}void Dialog::on_LB_lineEdit_textEdited(QString s){
    LBAR4Address = s;
    update();
}

void Dialog::on_Pimage_lineEdit_textEdited(QString s){
    PimageAddress = s;
    update();
}
void Dialog::on_Mimage_lineEdit_textEdited(QString s){
    MimageAddress = s;
    update();
}

int Dialog::fileReadyCheck(){
    QString errorList = "";
    int fileErrorNum = 0;
    QFileInfo fileInfo;
    bool hasFileCover = 0;
    if(hasFront){
        fileInfo.setFile(DFAR4Address);
        if(!fileInfo.isFile())  { errorList += "\t深色前片文件\n"; fileErrorNum++;}
        fileInfo.setFile(LFAR4Address);
        if(!fileInfo.isFile())  { errorList += "\t浅色前片文件\n"; fileErrorNum++;}
    }

    if(hasBack){
        fileInfo.setFile(DBAR4Address);
        if(!fileInfo.isFile())  { errorList += "\t深色后片文件\n"; fileErrorNum++;}
        fileInfo.setFile(LBAR4Address);
        if(!fileInfo.isFile())  { errorList += "\t浅色后片文件\n"; fileErrorNum++;}
    }

    fileInfo.setFile(PimageAddress);
    if(!fileInfo.isFile())  errorList += "\t生产版单文件\n";
    fileInfo.setFile(MimageAddress);
    if(!fileInfo.isFile())  errorList += "\t效果图/模特图文件\n";

    if(errorList != ""){
        if(fileErrorNum == 4 || (fileErrorNum == 2 && (!hasBack || !hasFront))){
            QMessageBox::StandardButton reply = QMessageBox::information(NULL, "信息错误", "无正确的AR4文件信息输入，无法正常储存。", QMessageBox::Yes);
            return 0;
        }
        QMessageBox::StandardButton reply = QMessageBox::question(NULL, "信息错误", "以下信息内容错误（错误的文件路径或未填写），是否继续保存？\n\n"+errorList+"\n对以上文件的修改将不会被保存。", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
            return 0;
        else{
            if(hasFront){
                fileInfo.setFile(DFAR4Address);
                if(!fileInfo.isFile())  DFAR4Address = "";
                else {
                    fileInfo.setFile(Dir + "\\AR4\\"+Pattern+"\\DF.txt");
                    if(fileInfo.isFile())  hasFileCover = true;
                }

                fileInfo.setFile(LFAR4Address);
                if(!fileInfo.isFile())  LFAR4Address = "";
                else {
                    fileInfo.setFile(Dir + "\\AR4\\"+Pattern+"\\LF.txt");
                    if(fileInfo.isFile())  hasFileCover = true;
                }
            }

            if(hasBack){
                fileInfo.setFile(LBAR4Address);
                if(!fileInfo.isFile())  LBAR4Address = "";
                else {
                    fileInfo.setFile(Dir + "\\AR4\\"+Pattern+"\\LB.txt");
                    if(fileInfo.isFile())  hasFileCover = true;
                }
                fileInfo.setFile(DBAR4Address);
                if(!fileInfo.isFile())  DBAR4Address = "";
                else {
                    fileInfo.setFile(Dir + "\\AR4\\"+Pattern+"\\DB.txt");
                    if(fileInfo.isFile())  hasFileCover = true;
                }
            }

            fileInfo.setFile(PimageAddress);
            if(!fileInfo.isFile())  PimageAddress = "";
            else {
                fileInfo.setFile(Dir + "\\image\\"+Pattern+"\\P.jpg");
                if(fileInfo.isFile())  hasFileCover = true;
            }
            fileInfo.setFile(MimageAddress);
            if(!fileInfo.isFile())  MimageAddress = "";
            else {
                fileInfo.setFile(Dir + "\\image\\"+Pattern+"\\M.jpg");
                if(fileInfo.isFile())  hasFileCover = true;
            }

            if(hasFileCover)
                return 2;
            else
                return 1;
        }
    }
}
