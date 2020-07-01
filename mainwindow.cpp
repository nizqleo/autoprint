#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include "API\api.h"
#include "API\localapi.h"
#include "auxiliary.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTextCodec *gbk = QTextCodec::codecForName("gb18030");
    QTextCodec *utf8 = QTextCodec::codecForName("utf-8");

    QTextCodec::setCodecForLocale(utf8);


//    QPixmap pixmap(".\\data\\logo.png");
//    //QPixmap pixmap("C:\\Users\\nzq82\\source\\QtRepos\\data\\logo.png");
//    ui->logo_label->setPixmap(pixmap);
//    ui->logo_label->show();


    defaultDir = ".\\";


    /*************************************/
    // new a API based on progress.
    api = new localAPI(this, ".\\data");

    /*************************************/

    api->readPrinterData(printers);

//    QDir qdir(".\\data\\ARX4_files");
//    if (!qdir.exists()){
//        qdir.mkpath(".");
//    }
//    vector<QString> patternNames;
//    if(!get_files(".\\data\\AR4", patternNames, 1)){
//        std::cout<<"failed to find any file of this type."<<std::endl;
//    }
//    qDebug()<<"pattern size:"<<patternNames.size()<<endl;
//    for(auto p  = patternNames.begin(); p < patternNames.end(); p++){
//        qdir.setPath(".\\data\\ARX4_files\\"+*p);
//        if (!qdir.exists()){
//            qdir.mkpath(".");
//        }
//        vector<QString> fileNames;
//        if(!get_files(".\\data\\image\\"+*p, fileNames, 0)){
//            std::cout<<"failed to find any file of this type."<<std::endl;
//        }
//        if(!fileNames.empty()){
//            // Pimage
//            qDebug()<<"no file"<<endl;
//            QImageReader Qimagereader;
//            Qimagereader.setDecideFormatFromContent(true);
//            int ind = 0;
//            if(fileNames[0].size() != 12 && fileNames.size()>1){
//                ind = 1;
//            }

//            Qimagereader.setFileName(QString(".\\data\\image\\"+*p + "\\" + fileNames[ind]));

//            QString format = ".png";
//            if(Qimagereader.canRead()){

//                QString format = Qimagereader.format();
//                qDebug()<<0<<"format"<<format<<endl;
//                QPixmap Pimage = QPixmap::fromImageReader(&Qimagereader);
//                Pimage.save(".\\data\\ARX4_files\\"+*p +"\\生产版单."+ format);
//            }
//        }


//        // info file & ARX4 files

//        fileNames.clear();
//        if(!get_files(".\\data\\AR4\\"+*p, fileNames, 0)){
//            std::cout<<"failed to find any file of this type."<<std::endl;
//        }
//        qDebug()<<"file number ： "<<fileNames.size()<<".\\data\\AR4\\"+*p<<endl;

//        QFile file(".\\data\\ARX4_files\\"+*p+"\\info.csv");
//        if (!file.open(QIODevice::ReadWrite)){
//            cout<<"failed to write in a new file."<<endl;
//        }
//        else{
//            qDebug()<<file.fileName()<<endl;
//            QTextStream txtOut(&file);
//            txtOut.setCodec(QTextCodec::codecForName("utf-8"));
//            txtOut<<QString("备注,类型")<<endl;
//            txtOut<<",";
//            if(p->contains("ZPT"))
//                txtOut<<"ZPT"<<endl;
//            else if(p->contains("ZPWY"))
//                txtOut<<"ZPWY"<<endl;
//            else if(p->contains("ZPC"))
//                txtOut<<"ZPC"<<endl;
//            else if(p->contains("ZPLMWY"))
//                txtOut<<"ZPLMWY"<<endl;
//            txtOut<<QString("颜色,位置,油墨,版大小")<<endl;
//            for(auto n = fileNames.begin(); n < fileNames.end(); n++){
//                if(*n == "info.txt") continue;
//                qDebug()<<*n<<endl;
//                vector<QString> temp;
//                SplitString(*n, temp, "-");
//                qDebug()<<temp.size()<<endl;
//                ARX4 ar4;

//                if(temp[0] == QString("浅") || temp[0] == QString("Ã│"))
//                    ar4.applyColor = -1;
//                else ar4.applyColor = 1;

//                if(temp[1] == QString("前") || temp[1] == QString("Ã░"))
//                    ar4.position = 0;
//                else ar4.position = 1;
//                qDebug()<<"begin extraction"<<endl;
//                vector<QPixmap> block;
//                api->extractARX4file(".\\data\\AR4\\"+*p+"\\" + *n, ar4, block);
//                qDebug()<<"end extraction"<<endl;
//                txtOut<<ar4.color2QString()<<","
//                     <<positionString[ar4.position]<<",";
//                if(ar4.whiteInk)
//                    txtOut<<QString("彩色+白色")<<",";
//                else txtOut<<QString("仅彩色")<<",";
//                txtOut<<platenSizeString[ar4.platenSize]<<endl;

//                qdir.setPath(".\\data\\ARX4_files\\"+*p +"\\"+ar4.color2QString());
//                if (!qdir.exists()){
//                    qdir.mkpath(".");
//                }

//                // copy file

//                coverFileCopy(".\\data\\ARX4_files\\"+*p +"\\"+ar4.color2QString()+"\\"+positionString[ar4.position]+".arx4",
//                              ".\\data\\AR4\\"+*p + "\\"+*n);
//                block[0].save(".\\data\\ARX4_files\\"+*p +"\\"+ar4.color2QString()+"\\"+positionString[ar4.position]+".png");

//            }
//            file.close();
//        }
//    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Login_button_clicked(){
    this->hide();
    DM = new dataMaintainance(this);
    open_datamaintainance();
    PTM = new printTaskManagement(this);
}


void MainWindow::on_Exit_button_clicked(){
    this->close();
}


void MainWindow::open_printersetting(){
    PS = new printerSetting(this);
    PS->show();
    this->hide();
}


void MainWindow::open_printtaskmanagement(){
    PTM->show();
    this->hide();
}


void MainWindow::open_datamaintainance(){
    DM->show();
    this->hide();
}
