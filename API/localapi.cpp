#include "localapi.h"

localAPI::localAPI()
{

}

localAPI::localAPI(MainWindow* MW, QString dir): API(MW), dir(dir){
    codec = QTextCodec::codecForName("gbk");
    }


//collect all of the names of the patterns in the database.
void localAPI::getPatternNameList(vector<QString>& patternNames){
    if(!get_files(dir+"\\ARX4_files", patternNames, 1)){
        std::cout<<"failed to find any file of this type."<<std::endl;
    }
}

// read a pattern named "pattern", fill the information based on the database
// called by datasetModels, only called once when the program starts
void localAPI::readPatternData(Pattern& pattern){

    QFileInfo fileInfo;

    //image
    // produce pattern file: only check existance
    QImageReader Qimagereader(dir+"\\ARX4_files\\"+pattern.name+"\\"+PimName);
    Qimagereader.setDecideFormatFromContent(true);
    if(Qimagereader.canRead()){
        pattern.hasPimages = true;
    }else pattern.hasPimages = false;


    Qimagereader.setFileName(dir+"\\ARX4_files\\"+pattern.name+"\\"+defaultColor +
                             "\\" + positionString[1]);
    if(Qimagereader.canRead()){
        pattern.Mimages = QPixmap::fromImageReader(&Qimagereader).scaled(QSize(150,150), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        pattern.hasMimages = true;
    }
    else {
        Qimagereader.setFileName(dir+"\\ARX4_files\\"+pattern.name+"\\"+defaultColor +
                                 "\\" + positionString[0]);
        if(Qimagereader.canRead()){
            pattern.Mimages = QPixmap::fromImageReader(&Qimagereader).scaled(QSize(150,150), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            pattern.hasMimages = true;
        }
        else pattern.hasMimages = false;
    }


    // reading info.txt
    QFile file(dir+"\\ARX4_files\\"+pattern.name+"\\info.csv");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"failed to open info file."<< file.fileName()<<endl;
    }
    else{
        vector<QString> temp;

        // use table header for encode rule test
        bool gbkfile = false;
        QByteArray line = file.readLine();
        QString str = codec->toUnicode(line.constData(), line.size(), &state);
        SplitString(str, temp, ",");

        if (temp[0] == "备注"){
            gbkfile = true;
        }

        line = file.readLine();
        QString str1(line);
        if(gbkfile)
            str1 = codec->toUnicode(line.constData(), line.size(), &state);

        str1 = str1.trimmed();
        temp.clear();

        SplitString(str1, temp, ",");
        pattern.Notes = temp[0];
        pattern.type = Pattern::type2index(temp[1]);

        line = file.readLine();
        while (!file.atEnd())
        {
            line = file.readLine();
            QString str(line);
            if(gbkfile)
                str = codec->toUnicode(line.constData(), line.size(), &state);

            str = str.trimmed();
            temp.clear();

            SplitString(str, temp, ",");

            pattern.AR4Files->files.push_back(ARX4(temp));

        }
        file.close();
    }
}

//delete a pattern named "pattern", include all the information saved in the database
void localAPI::deletePatternData(QString pattern){
    deleteDirectory(dir+"\\ARX4_files\\"+pattern);
}

// save datas for a pattern. might be a newly added item or item that already exists.
// create a dir if adding new item
// copy the newly added ar4 files.
// save file info.csv
void localAPI::savePatternData(QString pattern, QString  PimageAddress, QString  Notes,
                               int type, vector<QString>& fileAddresses,
                               AR4FileModel* fileModel){
    // prepare dirs
    QDir qdir(dir+"\\ARX4_files");
	if (!qdir.exists()){
		qdir.mkpath(".");
	}
    qdir.setPath(dir+"\\ARX4_files\\"+pattern);
	if (!qdir.exists()){
		qdir.mkpath(".");
	}

    // save info.csv
    QFile file(dir+"\\ARX4_files\\"+pattern+"\\info.csv");
    if (!file.open(QIODevice::WriteOnly)){
        cout<<"failed to write in a new file."<<endl;
    }
    else{
        qDebug()<<file.fileName()<<endl;
        QTextStream txtOut(&file);
        txtOut.setCodec(QTextCodec::codecForName("utf-8"));
        txtOut<<QString("备注,类型")<<endl;
        txtOut<<Notes<<","<<printTypeString[type]<<endl;
        txtOut<<QString("颜色,位置,油墨,版大小")<<endl;
        if(fileModel){
            for(auto p = fileModel->files.begin(); p < fileModel->files.end(); p++){
                txtOut<<p->color2QString()<<","
                     <<positionString[p->position]<<",";
                if(p->whiteInk)
                    txtOut<<QString("彩色+白色")<<",";
                else txtOut<<QString("仅彩色")<<",";
                txtOut<<platenSizeString[p->platenSize]<<endl;
            }
            file.close();
        }
    }


    // save Pimage
    if(PimageAddress != ""){
        if(!coverFileCopy(dir+"\\ARX4_files\\"+pattern+"\\"+PimName+"."+PimageAddress.section(".",-1), PimageAddress))
            cout<<"failed saving P image!"<<endl;
    }

    // save files
    if(fileModel){
        for(unsigned long long i = 0; i< fileModel->files.size(); i++){
            qdir.setPath(dir+"\\ARX4_files\\"+pattern+ "\\" + fileModel->files[i].color2QString());
            if (!qdir.exists()){
                qdir.mkpath(".");
            }
            if(!coverFileCopy(dir+"\\ARX4_files\\"+pattern+"\\"+fileModel->files[i].color2QString() + "\\" +
                            positionString[fileModel->files[i].position]+".arx4", fileAddresses[i]))
                qDebug()<<"failed saving arx4 files!"<<fileAddresses[i]<<endl;

            fileModel->images[i].save(dir+"\\ARX4_files\\"+pattern+"\\"+fileModel->files[i].color2QString() + "\\" +
                           positionString[fileModel->files[i].position]+".png");
        }


        // delete files
        QFileInfo Defile;
        vector<QString> colorDirs;
        if(!get_files(dir+"\\ARX4_files\\"+ pattern, colorDirs, 1)){
            std::cout<<"failed to find any file of this type."<<std::endl;
        }
        for (auto c = colorDirs.begin(); c < colorDirs.end(); c++) {
            vector<QString> positionDirs;
            if(!get_files(dir+"\\ARX4_files\\"+ pattern +"\\"+ *c, positionDirs, 0)){
                std::cout<<"failed to find any file of this type."<<std::endl;
            }
            for(auto p = positionDirs.begin(); p < positionDirs.end(); p++){
                if(p->section(".", -1) == "png") continue;
                QString posi = p->section(".",0, 0);
                qDebug()<<"found file"<<*c<<' '<<posi<<endl;
                if(fileModel->searchFile(*c, posi) == fileModel->files.end()){
                    qDebug()<<"found error files"<<endl;
                    Defile.setFile(dir+"\\ARX4_files\\"+pattern+"\\"+*c+"\\" + posi + ".png");
                    Defile.dir().remove(Defile.fileName());
                    qDebug()<<Defile.filePath()<<endl;
                    Defile.setFile(dir+"\\ARX4_files\\"+pattern+"\\"+*c+"\\" + posi + ".arx4");
                    Defile.dir().remove(Defile.fileName());
                }
            }
            positionDirs.clear();
            if(!get_files(dir+"\\ARX4_files\\"+ pattern +"\\"+ *c, positionDirs, 0)){
                std::cout<<"failed to find any file of this type."<<std::endl;
            }
            if(positionDirs.empty()){
                qdir.setPath(dir+"\\ARX4_files\\"+ pattern +"\\"+ *c);
                qdir.rmdir(".");
            }
        }
    }
}

// exam the data in the database and compute the status for the pattern.
// read the image files together.
// used in datamodel showing. after updating usually.
void localAPI::examPatternData(Pattern* pattern){
//    //AR4 file status exam & status computing
//    int status = 0;
//    if(pattern->hasBack) status += 32;
//    if(pattern->hasFront) status += 16;

//    QFileInfo fileInfo;

//    fileInfo.setFile(dir + "\\AR4\\"+pattern->name+"\\"+LFPrefix+pattern->name+".arx4");
//    if(fileInfo.isFile())  status += 8;
//    fileInfo.setFile(dir + "\\AR4\\"+pattern->name+"\\"+DFPrefix+pattern->name+".arx4");
//    if(fileInfo.isFile())  status += 4;
//    fileInfo.setFile(dir + "\\AR4\\"+pattern->name+"\\"+LBPrefix+pattern->name+".arx4");
//    if(fileInfo.isFile())  status += 2;
//    fileInfo.setFile(dir + "\\AR4\\"+pattern->name+"\\"+DBPrefix+pattern->name+".arx4");
//    if(fileInfo.isFile())  status += 1;

//    pattern->status = status;


//    //image
//    QPixmap image;
//    QImageReader Qimagereader(dir+"\\image\\"+pattern->name+"\\"+PimName+".jpg");
//    Qimagereader.setDecideFormatFromContent(true);

//    if(Qimagereader.canRead()){
//        pattern->Pimages = QPixmap::fromImageReader(&Qimagereader);
//        pattern->hasPimages = true;
//    }else pattern->hasPimages = false;

//    Qimagereader.setFileName(dir+"\\image\\"+pattern->name+"\\"+MimName+".jpg");
//    if(Qimagereader.canRead()){
//        pattern->Mimages = QPixmap::fromImageReader(&Qimagereader);
//        pattern->hasMimages = true;
//    }else pattern->hasMimages = false;
}

bool localAPI::checkFileOverlap(QString Pattern, int file, QString Dir){
   QFileInfo fileInfo;
   switch(file){
   case 0:
       if(checkSameFile(dir + "\\AR4\\"+Pattern+"\\"+DFPrefix+Pattern+".txt", Dir))
           return false;
       fileInfo.setFile(dir + "\\AR4\\"+Pattern+"\\"+DFPrefix+Pattern+".txt");
       if(fileInfo.isFile())  return true;
       else return false;
   case 1:
       if(checkSameFile(dir + "\\AR4\\"+Pattern+"\\"+LFPrefix+Pattern+".txt", Dir))
           return false;
       fileInfo.setFile(dir + "\\AR4\\"+Pattern+"\\"+LFPrefix+Pattern+".txt");
       if(fileInfo.isFile())  return true;
       else return false;
   case 2:
       if(checkSameFile(dir + "\\AR4\\"+Pattern+"\\"+LBPrefix+Pattern+".txt", Dir))
           return false;
       fileInfo.setFile(dir + "\\AR4\\"+Pattern+"\\"+LBPrefix+Pattern+".txt");
       if(fileInfo.isFile())  return true;
       else return false;
   case 3:
       if(checkSameFile(dir + "\\AR4\\"+Pattern+"\\"+DBPrefix+Pattern+".txt", Dir))
           return false;
       fileInfo.setFile(dir + "\\AR4\\"+Pattern+"\\"+DBPrefix+Pattern+".txt");
       if(fileInfo.isFile())  return true;
       else return false;
   case 4:
       if(checkSameFile(dir + "\\image\\"+Pattern+"\\"+PimName+".jpg", Dir))
           return false;
       fileInfo.setFile(dir + "\\image\\"+Pattern+"\\"+PimName+".jpg");
       if(fileInfo.isFile())  return true;
       else return false;
   case 5:
       if(checkSameFile(dir + "\\image\\"+Pattern+"\\"+MimName+".jpg", Dir))
           return false;
       fileInfo.setFile(dir + "\\image\\"+Pattern+"\\"+MimName+".jpg");
       if(fileInfo.isFile())  return true;
       else return false;
   }
}

QString localAPI::showFileDirinDatabase(QString Pattern, int file){
    switch(file){
    case 0:
        return dir + "\\AR4\\"+Pattern+"\\"+DFPrefix+Pattern+".arx4";
    case 1:
        return dir + "\\AR4\\"+Pattern+"\\"+LFPrefix+Pattern+".arx4";
    case 2:
        return dir + "\\AR4\\"+Pattern+"\\"+LBPrefix+Pattern+".arx4";
    case 3:
        return dir + "\\AR4\\"+Pattern+"\\"+DBPrefix+Pattern+".arx4";
    case 4:

        return dir+"\\ARX4_files\\"+Pattern+"\\"+PimName;
    case 5:
        return dir + "\\image\\"+Pattern+"\\"+MimName;
    }
}

void localAPI::getERPOrders(vector<Order>& orderlist, QWidget* p){
    qDebug()<<"API start reading ERP Orders"<<endl;

    QString fileNames = QFileDialog::getOpenFileName(p,"选择ERP订单文件", dir, "csv文件 (*.csv), *.csv;;全部文件(*.*)");

    QFile file(fileNames);

    vector<QString> temp;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        bool gbkfile = false;
        QByteArray line = file.readLine();
        QString str = codec->toUnicode(line.constData(), line.size(), &state);
        SplitString(str, temp, ",");
        //qDebug()<<"before test"<<endl;
        if (temp[0] == "SKU编码"){
            gbkfile = true;
        }
        while (!file.atEnd())
        {
            line = file.readLine();
            QString str(line);
            if(gbkfile)
                str = codec->toUnicode(line.constData(), line.size(), &state);

            str = str.trimmed();
            temp.clear();
            //cout<<lineStr<<endl;
            SplitString(str, temp, ",");
            if(!temp[4].contains("ZP"))
                continue;

            orderlist.push_back(Order(temp));

            orderlist.rbegin()->addOtherInfo(temp[2]);
            orderlist.rbegin()->fromERP = true;

        }
        
        file.close();
    }
    else{
        cout<<"failed to open order file."<<endl;   
    }
    qDebug()<<"API finished reading ERP Orders"<<endl;
}

void localAPI::readOrders(vector<Order>& orderlist){

    QFile file(dir+"\\orders.csv");

    vector<QString> temp;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray line = file.readLine();
        while (!file.atEnd())
        {
            line = file.readLine();
            QString str(line);
            str = str.trimmed();
            temp.clear();
            //cout<<lineStr<<endl;
            SplitString(str, temp, ",");
            Order tempOrder(temp);
            if(!tempOrder.finished){
                orderlist.push_back(Order(temp));
                orderlist.rbegin()->addOtherInfo(temp[2]);
                orderlist.rbegin()->fromERP = true;
            }
        }

        file.close();
    }
    else{
        cout<<"failed to open order file."<<endl;
    }
}

void localAPI::saveOrders(vector<Order>& orderlist){

//    QFile file(dir+"\\orders.csv");

//    vector<QString> temp;

//    if (file.open(QIODevice::WriteOnly))
//    {
//        QTextStream txtOut(&file);
//        for(int i = 0; i < orderlist.size(); i++){
//            txtOut<<orderlist[i].pattern<<","
//                    <<colorString[orderlist[i].color]<<","
//                      <<sizeString[orderlist[i].size]<<","
//                        <<orderlist[i].number<<","
//                        <<printTypeString[orderlist[i].type]<<","
//                       <<orderlist[i].finished<<endl;
//        }

//        file.close();
//    }
//    else{
//        cout<<"failed to open order file."<<endl;
//    }

}

void localAPI::readSavedTasks(OrderModel* OM, int** LeftNumbers){
    QDateTime curDateTime=QDateTime::currentDateTime();
    QString cutime = curDateTime.toString("yy-MM-dd");
    //BF information save
    QFile file(dir+"\\log\\"+cutime+"\\" +MW->printers[OM->printerIndex].name+".csv");
    if (!file.open(QIODevice::ReadWrite)){
        qDebug()<<"failed to read former log."<<file.fileName()<<endl;
    }
    else{
        vector<QString> temp;
        qDebug()<<file.fileName()<<endl;
        QByteArray line = file.readLine();
        while (!file.atEnd())
        {
            line = file.readLine();
            QString str(line);
            str = str.trimmed();
            temp.clear();

            SplitString(str, temp, ",");

            OM->tasklist.push_back(Task(temp, OM->printerIndex));
            if(OM->tasklist.rbegin()->Tstatus != FINISHED && LeftNumbers != NULL){
               // qDebug()<<"adding up to LeftNumber!"<<OM->tasklist.rbegin()->colorInfo<<endl;
                for(int i = 0; i < 5; i++){
                    LeftNumbers[OM->tasklist.rbegin()->colorInfo][i] += OM->tasklist.rbegin()->numbers[i];
                }
                LeftNumbers[OM->tasklist.rbegin()->colorInfo][5] += OM->tasklist.rbegin()->totalNum;
            }
        }
        file.close();
    }
}

void localAPI::saveTasks(OrderModel* OM){
    QDateTime curDateTime=QDateTime::currentDateTime();
    QString time = curDateTime.toString("yy-MM-dd");

    QDir qdir(dir+"\\log\\"+time);
    if (!qdir.exists()){
        qdir.mkpath(".");
    }

    QFile file(dir+"\\log\\"+time+"\\" +MW->printers[OM->printerIndex].name+".csv");
    if (!file.open(QIODevice::ReadWrite)){
        cout<<"failed to write in a new file."<<endl;
    }
    else{
        qDebug()<<file.fileName()<<endl;
        QTextStream txtOut(&file);
        txtOut<<QString("款号,打印类型,总数,颜色,ERP订单,创建时间,状态,状态更新时间,置顶,详细")<<endl;
        for(unsigned long long i = 0; i < OM->tasklist.size(); i++){
            txtOut<<OM->tasklist[i].pattern->name<<","
                    <<printTypeString[OM->tasklist[i].type]<<","
                      <<OM->tasklist[i].totalNum<<","
                        <<colorString[OM->tasklist[i].colorInfo]<<","
                          <<OM->tasklist[i].fromERP<<","
                            <<OM->tasklist[i].createTime<<","
                              <<OM->tasklist[i].Tstatus<<","
                                <<OM->tasklist[i].modifiedTime<<","
                                <<OM->tasklist[i].topped<<","
                                  <<OM->tasklist[i].sizeNumberString()<<endl;
        }
        file.close();
    }
}

void localAPI::readPrinterData(vector<Printer>& printers){
    QFile file(dir + "\\printers.csv");
    vector<QString> temp;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        int cnt = 0;
        QByteArray line = file.readLine();
        while (!file.atEnd())
        {
            line = file.readLine();
            qDebug()<<line<<endl;
            QString str(line);
            str = str.trimmed();
            Printer temp(str, cnt);
            //if(temp.availble){
                printers.push_back(temp);
                cnt++;
            //}
        }
        file.close();
    }
    else{
        cout<<"failed to open printer file."<<endl;   
    }
}

int localAPI::sendPrintingFile(QString pattern, ARX4& ar4file, QString printerName, bool front){

    QDir qdir(dir+"\\CMD\\"+printerName);
	if (!qdir.exists()){
		qdir.mkpath(".");
	}

    if(!coverFileCopy(dir+"\\CMD\\"+printerName+"\\"+ar4file.color2QString()+"-"+positionString[ar4file.position]+"-"+pattern+".arx4", dir+"\\ARX4_files\\"+pattern+"\\"+ar4file.color2QString()+"\\"+positionString[ar4file.position]+".arx4") ){
        qDebug()<<"failed to copy files while sending files."<<QString(dir+"\\CMD\\"+printerName+"\\"+ar4file.color2QString()+"-"+positionString[ar4file.position]+"-"+pattern+".arx4")
                <<dir+"\\ARX4_files\\"+pattern+"\\"+ar4file.color2QString()+"\\"+positionString[ar4file.position]+".arx4"<<endl;
        return 3;// copy failed.
    }
    else {

        QProcess p(0);

        //BF information save
        QFile file(dir+"\\CMD\\send.bat");
        file.open(QIODevice::ReadWrite| QIODevice::Text);

        QTextStream txtOut(&file);
        txtOut<<"CHCP 65001"<<endl;
        txtOut << "cd data\\CMD" << endl;
        txtOut<<"GTX4CMD.exe send -A \""+printerName+"\\"+ar4file.color2QString()+"-"+positionString[ar4file.position]+"-"+pattern+".arx4\" -P \"" + printerName + "\" -D 0 "<<endl;
        file.close();

        QString command = "data\\CMD\\send.bat";
        p.start(command);
        p.waitForFinished();
        qDebug()<<p.exitCode();

        if(p.exitCode() == 0){
            qdir.setPath(dir+"\\CMD\\"+printerName+"\\"+ar4file.color2QString()+"-"+positionString[ar4file.position]+"-"+pattern+".arx4");
            qdir.remove(ar4file.color2QString()+"-"+positionString[ar4file.position]+"-"+pattern+".arx4");
        }
        return p.exitCode();
    }
}

QPixmap localAPI::loadPics(QString _dir){
    QImageReader Qimagereader(_dir);
    Qimagereader.setDecideFormatFromContent(true);

    QPixmap pixmapM = QPixmap::fromImageReader(&Qimagereader);
    return pixmapM;
}

QPixmap localAPI::loadPics(QString name, bool isMimage){
    QImageReader Qimagereader;
    Qimagereader.setDecideFormatFromContent(true);
    if(isMimage) {
        Qimagereader.setFileName(dir+"\\ARX4_files\\"+name+"\\"+defaultColor +
                                 "\\" + positionString[0]);
        if(Qimagereader.canRead()){
            return QPixmap::fromImageReader(&Qimagereader);
        }
        else{
            Qimagereader.setFileName(dir+"\\ARX4_files\\"+name+"\\"+defaultColor +
                                     "\\" + positionString[1]);
            return QPixmap::fromImageReader(&Qimagereader);
        }
    }
    else {
        Qimagereader.setFileName(dir+"\\ARX4_files\\"+name+"\\"+PimName);
        return QPixmap::fromImageReader(&Qimagereader);
    }
}

int localAPI::extractARX4file(QString fileName, ARX4& arx4, QPixmap& image){
    qDebug()<<"inside here"<<endl;
    QFileInfo f(fileName);
    if(!f.exists()) return 4;

    if(!coverFileCopy(dir+"\\CMD\\Input\\sample.arx4", fileName) ){
        cout<<"failed to copy files while sending files."<<endl;
        return 3;// copy failed.
    }
    else {
        QProcess p(0);

        //BF information save
        QFile file(dir+"\\CMD\\extract.bat");
        file.open(QIODevice::ReadWrite| QIODevice::Text);

        QTextStream txtOut(&file);
        txtOut<<"CHCP 65001"<<endl;
        txtOut << "cd data\\CMD" << endl;
        txtOut<<"GTX4CMD.exe extract -A \"Input\\sample.arx4\" -X \"Output\\sample.xml\" -I \"Output\\sample.png\""<<endl;
        file.close();

        QString command = "data\\CMD\\extract.bat";
        p.start(command);
        p.waitForFinished();
        qDebug()<<"extract exitcode"<<p.exitCode()<<endl;
        if(p.exitCode() != 0) return p.exitCode();
        else{
            QPixmap i;
            i.load(dir+"\\CMD\\Output\\sample.png");
            image = i.scaled(QSize(200, 200),Qt::KeepAspectRatio,Qt::SmoothTransformation);
            qDebug()<<i.size()<<endl;
            arx4.platenSize = ARX4::platenSize2Index(readXml(dir+"\\CMD\\Output\\sample.xml", "szPlatenSize"));
            int Ink = readXml(dir+"\\CMD\\Output\\sample.xml", "byInk").toInt();
            if(Ink == 2) arx4.whiteInk = true;
            else arx4.whiteInk = false;
            return 0;
        }
    }
}

void localAPI::fillImagesAddresses(QString name, vector<QPixmap>& images, vector<QString>& fileAddress, bool Dialog){
    // reading info.txt
    QFile file(dir+"\\ARX4_files\\"+name+"\\info.csv");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"failed to open info file."<< file.fileName()<<endl;
    }
    else{
        vector<QString> temp;
        // use table header for encode rule test
        bool gbkfile = false;
        QByteArray line = file.readLine();
        QString str = codec->toUnicode(line.constData(), line.size(), &state);
        SplitString(str, temp, ",");

        if (temp[0] == "备注"){
            gbkfile = true;
        }

        line = file.readLine();
        // information in this line is useless
        line = file.readLine();
        while (!file.atEnd())
        {
            line = file.readLine();
            QString str(line);
            if(gbkfile)
                str = codec->toUnicode(line.constData(), line.size(), &state);

            str = str.trimmed();
            temp.clear();

            SplitString(str, temp, ",");

            QImageReader Qimagereader(dir+"\\ARX4_files\\"+name+"\\"+temp[0]+
                                      "\\" + temp[1]);
            Qimagereader.setDecideFormatFromContent(true);
            if(Qimagereader.canRead())
                images.push_back(QPixmap::fromImageReader(&Qimagereader));
            else qDebug()<<"can't read image"<<endl;

            if(Dialog)
                fileAddress.push_back(dir+"\\ARX4_files\\"+name+"\\"+temp[0]+ "\\" +
                                  temp[1]+".arx4");

        }

        file.close();
    }
}
