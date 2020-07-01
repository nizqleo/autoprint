#include "dialog.h"
#include "ui_dialog.h"
#include "pages\datamaintainance.h"
#include <API/api.h>

Dialog::Dialog(dataMaintainance* DM,Pattern* _pattern, QWidget *parent) :
    QDialog(parent),DM(DM),pattern(_pattern),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setAttribute( Qt::WA_DeleteOnClose, true );


    ui->color_comboBox->addItem("默认");
    for(int i = 0; i < colorString.size(); i++){
        ui->color_comboBox->addItem(colorString[i]);
    }
    for(int i = 0; i < 4; i++){
        ui->position_comboBox->addItem(positionString[i]);
    }
    for(int i = 0; i < 4; i++){
        ui->type_comboBox->addItem(printTypeString[i]);
    }

    ui->type_comboBox->setCurrentIndex(0);
    ui->color_comboBox->setCurrentIndex(0);
    ui->position_comboBox->setCurrentIndex(0);

    ui->delete_button->setEnabled(false);

    if(_pattern == NULL){
        setWindowTitle(tr("新增条目"));
        isEdit = false;
        fileModel = new AR4FileModel();
        pattern = new Pattern();
        ui->type_comboBox->setCurrentIndex(0);
    }

    else{
        setWindowTitle(tr("修改条目"));
        isEdit=true;
        patternName = pattern->name;

        ui->Pattern_lineEdit->setText(patternName);
        ui->Note_lineEdit->setText(pattern->Notes);

        originalPatternName = pattern->name;

        //Pimage
        if(pattern->hasPimages)
            PimageAddress = api->showFileDirinDatabase(pattern->name, 4);
        else PimageAddress = "";
        ui->Pimage_lineEdit->setText(PimageAddress);

        // create tableView element
        fileModel = new AR4FileModel(pattern->AR4Files);
        api->fillImagesAddresses(pattern->name, fileModel->images, AR4FileAddresses);

        ui->type_comboBox->setCurrentIndex(pattern->type);

    }

    ui->tableView->setModel(fileModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents );
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );

    connect(ui->type_comboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(type_comboBox_changed(const QString &)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_AR4_tool_button_clicked(){
    QString fileNames = QFileDialog::getOpenFileName(this,tr("选择新AR4文件"), DM->searchDir, tr("全部文件 （*.*）, *.*;; ARX4文件 (*.arx4),*.arx4;; ARXP文件 (*.arxp),*.arxp"));
    ui->AR4_lineEdit->setText(fileNames);
}

void Dialog::on_Pimage_toolButton_clicked(){
    QString fileNames = QFileDialog::getOpenFileName(this,tr("选择效果图/模特图文件"), DM->searchDir, tr("全部文件 （*.*）, *.*;;图片文件(*png *jpg)"));
    ui->Pimage_lineEdit->setText(fileNames);
}

void Dialog::update(){
//    patternName = ui->Pattern_lineEdit->text();

//    ui->AR4_lineEdit->setText(AR4Address);

//    ui->Pimage_lineEdit->setText(PimageAddress);

//    QSize picSize(450, 280);
//    //image

//    QImageReader Qimagereader(PimageAddress);
//    Qimagereader.setDecideFormatFromContent(true);
//    if(Qimagereader.canRead()){
//        QPixmap pixmapM = api->loadPics(PimageAddress).
//                scaled(picSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
//        ui->Pimage_label->setPixmap(pixmapM);
//        ui->Pimage_label->show();
//    }
}

void Dialog::on_ConfirmButton_clicked(){
    try{
        api->savePatternData(patternName, PimageAddress, ui->Note_lineEdit->text(),
                             ui->type_comboBox->currentIndex(), AR4FileAddresses, fileModel);
        delete pattern->AR4Files;

        pattern->AR4Files = new AR4FileModel(fileModel);
        pattern->type = ui->type_comboBox->currentIndex();
        pattern->Notes = ui->Note_lineEdit->text();
        pattern->hasPimages = hasPimages;
        pattern->name = ui->Pattern_lineEdit->text();

        emit confirmEditing(pattern);
        if(pattern->name != patternName){
            emit PatternNameChanged(originalPatternName);
        }
        this->close();
    }
    catch( exception& e ){
        QMessageBox::StandardButton reply = QMessageBox::information(NULL, "发生错误", e.what(), QMessageBox::Yes );
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

    patternName = s;

    // check
    if(DM->patternNameOverlapCheck(s)){
        ui->label_error->setText("款号和已有款号冲突！");
        QPalette pe;
        pe.setColor(QPalette::WindowText,Qt::red);
        ui->label_error->setPalette(pe);

        ui->ConfirmButton->setEnabled(false);
    }
    else{
        ui->label_error->setText("");
        ui->ConfirmButton->setEnabled(true);
    }

}

void Dialog::on_AR4_lineEdit_textChanged(QString s){
    AR4Address = s;
}

void Dialog::on_Pimage_lineEdit_textChanged(QString s){
    PimageAddress = s;

    QImageReader Qimagereader(PimageAddress);
    Qimagereader.setDecideFormatFromContent(true);
    if(Qimagereader.canRead()){
        QPixmap pixmapM = api->loadPics(PimageAddress).
                scaled(QSize(790,300),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->Pimage_label->setPixmap(pixmapM);
        ui->Pimage_label->show();
        hasPimages = true;
    }
}

int Dialog::fileReadyCheck(){
    return 1;
    /*
    if(ui->Pattern_lineEdit->text() == ""){
        QMessageBox::StandardButton reply = QMessageBox::information(NULL, "无款号", "请输入款号！", QMessageBox::Yes);
        return 0;
    }
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

    QImageReader Qimagereader(MimageAddress);
    Qimagereader.setDecideFormatFromContent(true);
    if(!Qimagereader.canRead())  errorList += "\t效果图/模特图文件\n";

    Qimagereader.setFileName(PimageAddress);
    Qimagereader.setDecideFormatFromContent(true);
    if(!Qimagereader.canRead())  errorList += "\t生产版单文件\n";


    if(errorList != ""){
        if(fileErrorNum == 4 || (fileErrorNum == 2 && (!hasBack || !hasFront))){
            QMessageBox::StandardButton reply = QMessageBox::information(NULL, "信息错误", "无正确的AR4文件信息输入，无法正常储存。", QMessageBox::Yes);
            return 0;
        }
        QMessageBox::StandardButton reply = QMessageBox::question(NULL, "信息错误", "以下信息内容错误（错误的文件路径或未填写），是否继续保存？\n\n"+errorList+"\n对以上文件的修改将不会被保存。", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
            return 0;
        else{
            QString pattern = patternName;
            if(hasFront){
                fileInfo.setFile(DFAR4Address);
                if(!fileInfo.isFile())  DFAR4Address = "";
                else {
                    hasFileCover |= api->checkFileOverlap(pattern, 0, DFAR4Address);
                }

                fileInfo.setFile(LFAR4Address);
                if(!fileInfo.isFile())  LFAR4Address = "";
                else {
                    hasFileCover |= api->checkFileOverlap(pattern, 1, LFAR4Address);
                }
            }

            if(hasBack){
                fileInfo.setFile(LBAR4Address);
                if(!fileInfo.isFile())  LBAR4Address = "";
                else {
                    hasFileCover |= api->checkFileOverlap(pattern, 2, LBAR4Address);
                }
                fileInfo.setFile(DBAR4Address);
                if(!fileInfo.isFile())  DBAR4Address = "";
                else {
                    hasFileCover |= api->checkFileOverlap(pattern, 3, DBAR4Address);
                }
            }

            fileInfo.setFile(PimageAddress);
            if(!fileInfo.isFile())  PimageAddress = "";
            else {
                hasFileCover |= api->checkFileOverlap(pattern, 4, PimageAddress);
            }
            fileInfo.setFile(MimageAddress);
            if(!fileInfo.isFile())  MimageAddress = "";
            else {
                hasFileCover |= api->checkFileOverlap(pattern, 5, MimageAddress);
            }

            if(hasFileCover)
                return 2;
            else
                return 1;
        }
    }*/
}

void Dialog::on_add_button_clicked(){
    // check input error:
    ui->delete_button->setEnabled(false);

    vector<int> colorList;
    // fill colorList
    colorList.push_back(ui->color_comboBox->currentIndex()-1);

    int posi = ui->position_comboBox->currentIndex();
    qDebug()<<colorList[0]<< ' '<<posi<<endl;
    ARX4 temp;
    QPixmap image;
    int result = api->extractARX4file(ui->AR4_lineEdit->text(), temp, image);

    // input dir error
    if(result != 0){
        if(result == -2201)
            QMessageBox::information(NULL, "文件错误", "解析ARX4文件失败，请确认文件类型是否正确。",
                                                                         QMessageBox::Yes );
        else
            QMessageBox::information(NULL, "文件错误", "新增ARX4文件失败，错误代码："+QString::number(result),
                                                                         QMessageBox::Yes );
        return;
    }

    temp.position = posi;

    for(auto p = colorList.begin(); p < colorList.end(); p++){

        auto f = fileModel->searchFile(*p, posi);
        if(f != fileModel->files.end()){// if (color, position) exists
            QString colorStr;
            if(*p == -1) colorStr = API::defaultColor;
            else colorStr = colorString[*p];

            QMessageBox::StandardButton reply = QMessageBox::question(NULL, "文件重复",
                "已存在"+colorStr+positionString[posi]+"的ARX4文件，是否继续覆盖保存？", QMessageBox::Yes |QMessageBox::No);
            if(reply == QMessageBox::No) continue;
            // else: cover the original one

            AR4FileAddresses[f-fileModel->files.begin()] = ui->AR4_lineEdit->text();
            temp.applyColor = *p;
            fileModel->addItem(temp, image, f-fileModel->files.begin());
        }
        else{
            AR4FileAddresses.push_back(ui->AR4_lineEdit->text());
            temp.applyColor = *p;
            if(*p != -1){
                pattern->specialColor.push_back(*p);
            }

            fileModel->addItem(temp, image, -1);
        }
    }
}

void Dialog::on_delete_button_clicked(){
    if(!ui->tableView->currentIndex().isValid())
        return;

    int index = ui->tableView->currentIndex().row();
    fileModel->deleteItem(index);
    AR4FileAddresses.erase(index + AR4FileAddresses.begin());
}

void Dialog::on_Note_lineEdit_textChanged(QString s){
    pattern->Notes = s;
}

void Dialog::on_tableView_clicked(){
    ui->delete_button->setEnabled(true);
}

void Dialog::type_comboBox_changed(const QString & s){

}
