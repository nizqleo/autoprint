#include "localapi.h"
#include "mainwindow.h"

localAPI::localAPI()
{

}

localAPI::localAPI(MainWindow* MW, QString dir): API(MW), dir(dir){

    }


//collect all of the names of the patterns in the database.
void localAPI::getPatternNameList(vector<QString>& patternNames){
    if(!get_files(dir+"\\AR4", patternNames)){
        std::cout<<"failed to find any file of this type."<<std::endl;
    }
}

//read a pattern named "pattern", fill the information based on the database
Pattern localAPI::readPatternData(QString pattern){

    Pattern tempPattern;
    tempPattern.name = pattern;

    QFileInfo fileInfo;

    //image
    QImageReader Qimagereader(dir+"\\image\\"+tempPattern.name+"\\"+PimName);
    Qimagereader.setDecideFormatFromContent(true);

    if(Qimagereader.canRead()){
        tempPattern.Pimages = QPixmap::fromImageReader(&Qimagereader);
        tempPattern.hasPimages = true;
    }else tempPattern.hasPimages = false;

    Qimagereader.setFileName(dir+"\\image\\"+tempPattern.name+"\\"+MimName);
    if(Qimagereader.canRead()){
        tempPattern.Mimages = QPixmap::fromImageReader(&Qimagereader);
        tempPattern.hasMimages = true;
    }else tempPattern.hasMimages = false;


    // reading info.txt
    int status = 0;
    QFile file(dir+"\\AR4\\"+pattern+"\\info.txt");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        cout<<"failed to open info file."<<endl;
    }
    else{
        QByteArray line = file.readLine();
        QString str(line);
        int i = str.toInt();
        if(i==3) {status = 48; tempPattern.hasBack = tempPattern.hasFront = true;}
        if(i==2) {status = 32; tempPattern.hasBack = true; tempPattern.hasFront = false;}
        if(i==1) {status = 16; tempPattern.hasBack = false; tempPattern.hasFront = true;}
        file.close();
    }


    //AR4
    fileInfo.setFile(dir + "\\AR4\\"+pattern+"\\"+LFPrefix+pattern+".arx4");
    if(fileInfo.isFile())  status += 8;
    fileInfo.setFile(dir + "\\AR4\\"+pattern+"\\"+DFPrefix+pattern+".arx4");
    if(fileInfo.isFile())  status += 4;
    fileInfo.setFile(dir + "\\AR4\\"+pattern+"\\"+LBPrefix+pattern+".arx4");
    if(fileInfo.isFile())  status += 2;
    fileInfo.setFile(dir + "\\AR4\\"+pattern+"\\"+DBPrefix+pattern+".arx4");
    if(fileInfo.isFile())  status += 1;

    tempPattern.status = status;
    tempPattern.computeFileReady();

    return tempPattern;
}

//delete a pattern named "pattern", include all the information saved in the database
void localAPI::deletePatternData(QString pattern){

    QFileInfo file;

    file.setFile(dir+"\\image\\"+pattern+"\\"+PimName+".jpg"); file.dir().remove(file.fileName());

    file.setFile(dir+"\\image\\"+pattern+"\\"+MimName+".jpg"); file.dir().remove(file.fileName());

    file.setFile(dir+"\\AR4\\"+pattern+"\\"+LBPrefix+pattern+".arx4"); file.dir().remove(file.fileName());

    file.setFile(dir+"\\AR4\\"+pattern+"\\"+LFPrefix+pattern+".arx4"); file.dir().remove(file.fileName());

    file.setFile(dir+"\\AR4\\"+pattern+"\\"+DBPrefix+pattern+".arx4"); file.dir().remove(file.fileName());

    file.setFile(dir+"\\AR4\\"+pattern+"\\"+DFPrefix+pattern+".arx4"); file.dir().remove(file.fileName());

    file.setFile(dir+"\\AR4\\"+pattern+"\\info.txt"); file.dir().remove(file.fileName());

    QDir qdir(dir+"\\AR4\\"+pattern+"\\");
    qdir.rmdir(".");
    qdir.setPath(dir+"\\image\\"+pattern+"\\");
    qdir.rmdir(".");

}

//save datas for a pattern. might be a newly added item or item that already exists.
void localAPI::savePatternData(QString pattern, QString DFAR4Address, QString DBAR4Address,
                             QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                               QString MimageAddress, bool hasBack, bool hasFront){
    QDir qdir(dir+"\\image");
	if (!qdir.exists()){
		qdir.mkpath(".");
	}
    qdir.setPath(dir+"\\AR4");
	if (!qdir.exists()){
		qdir.mkpath(".");
	}

    // patterns
    qdir.setPath(dir+"\\AR4\\"+pattern);
	if (!qdir.exists()){
		qdir.mkpath(".");
	}

    if(LFAR4Address != ""){
        if(!coverFileCopy(dir+"\\AR4\\"+pattern+"\\"+LFPrefix+pattern+".arx4", LFAR4Address))
            cout<<"failed saving LF AR4 file!"<<endl;
    }
    if(DFAR4Address != ""){
        if(!coverFileCopy(dir+"\\AR4\\"+pattern+"\\"+DFPrefix+pattern+".arx4", DFAR4Address))
            cout<<"failed saving DF AR4 file!"<<endl;
    }
    if(LBAR4Address != ""){
        if(!coverFileCopy(dir+"\\AR4\\"+pattern+"\\"+LBPrefix+pattern+".arx4", LBAR4Address))
            cout<<"failed saving LB AR4 file!"<<endl;
    }
    if(DBAR4Address != ""){
        if(!coverFileCopy(dir+"\\AR4\\"+pattern+"\\"+DBPrefix+pattern+".arx4", DBAR4Address))
            cout<<"failed saving DB AR4 file!"<<endl;
    }


    //image file copy
    qdir.setPath(dir+"\\image\\"+pattern);
	if (!qdir.exists()){
		qdir.mkpath(".");
	}

    if(PimageAddress != ""){
        if(!coverFileCopy(dir+"\\image\\"+pattern+"\\"+PimName+"."+PimageAddress.section(".",-1), PimageAddress))
            cout<<"failed saving P image!"<<endl;
    }
    if(MimageAddress != ""){
        if(!coverFileCopy(dir+"\\image\\"+pattern+"\\"+MimName+"."+MimageAddress.section(".",-1), MimageAddress))
            cout<<"failed saving M image!"<<endl;
    }

    //BF information save
    QFile file(dir+"\\AR4\\"+pattern+"\\info.txt");
    if (!file.open(QIODevice::ReadWrite| QIODevice::Text)){
        cout<<"failed to write in a new file."<<endl;
    }
    else{
        QTextStream txtOut(&file);
        txtOut << hasBack*2+hasFront << endl;
        file.close();
    }
    
}

// exam the data in the database and compute the status for the pattern.
// read the image files together.
// used in datamodel showing. after updating usually.
void localAPI::examPatternData(Pattern* pattern){
    //AR4 file status exam & status computing
    int status = 0;
    if(pattern->hasBack) status += 32;
    if(pattern->hasFront) status += 16;

    QFileInfo fileInfo;

    fileInfo.setFile(dir + "\\AR4\\"+pattern->name+"\\"+LFPrefix+pattern->name+".arx4");
    if(fileInfo.isFile())  status += 8;
    fileInfo.setFile(dir + "\\AR4\\"+pattern->name+"\\"+DFPrefix+pattern->name+".arx4");
    if(fileInfo.isFile())  status += 4;
    fileInfo.setFile(dir + "\\AR4\\"+pattern->name+"\\"+LBPrefix+pattern->name+".arx4");
    if(fileInfo.isFile())  status += 2;
    fileInfo.setFile(dir + "\\AR4\\"+pattern->name+"\\"+DBPrefix+pattern->name+".arx4");
    if(fileInfo.isFile())  status += 1;

    pattern->status = status;


    //image
    QPixmap image;
    QImageReader Qimagereader(dir+"\\image\\"+pattern->name+"\\"+PimName+".jpg");
    Qimagereader.setDecideFormatFromContent(true);

    if(Qimagereader.canRead()){
        pattern->Pimages = QPixmap::fromImageReader(&Qimagereader);
        pattern->hasPimages = true;
    }else pattern->hasPimages = false;

    Qimagereader.setFileName(dir+"\\image\\"+pattern->name+"\\"+MimName+".jpg");
    if(Qimagereader.canRead()){
        pattern->Mimages = QPixmap::fromImageReader(&Qimagereader);
        pattern->hasMimages = true;
    }else pattern->hasMimages = false;
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

        return dir + "\\image\\"+Pattern+"\\"+PimName;
    case 5:
        return dir + "\\image\\"+Pattern+"\\"+MimName;
    }
}

void localAPI::getERPOrders(vector<Order>& orderlist){
    cout<<"start reading ERP Orders"<<endl;

    QFile file(dir+"\\ERP.csv");

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

            orderlist.push_back(Order(temp));

            orderlist.rbegin()->addOtherInfo(temp[2]);
            orderlist.rbegin()->fromERP = true;
        }
        
        file.close();
    }
    else{
        cout<<"failed to open order file."<<endl;   
    }
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

    QFile file(dir+"\\orders.csv");

    vector<QString> temp;

    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream txtOut(&file);
        for(int i = 0; i < orderlist.size(); i++){
            txtOut<<orderlist[i].pattern<<","
                    <<colorString[orderlist[i].color]<<","
                      <<sizeString[orderlist[i].size]<<","
                        <<orderlist[i].number<<","
                        <<printTypeString[orderlist[i].type]<<","
                       <<orderlist[i].finished<<endl;
        }

        file.close();
    }
    else{
        cout<<"failed to open order file."<<endl;
    }

}

void localAPI::readSavedTasks(OrderModel* OM){
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

            OM->tasklist.push_back(Task(temp));

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
        txtOut<<"款号,打印类型,总数,颜色,ERP订单,创建时间,状态,状态更新时间,置顶,详细"<<endl;
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
            printers.push_back(temp);
            cnt++;
        }
        file.close();
    }
    else{
        cout<<"failed to open printer file."<<endl;   
    }
}


int localAPI::sendPrintingFile(QString pattern, bool isDark, int num, QString printerName, bool front){

    QDir qdir(dir+"\\CMD\\"+printerName);
	if (!qdir.exists()){
		qdir.mkpath(".");
	}

    // reading info.txt
    int status = 0;
    QFile file(dir+"\\AR4\\"+pattern+"\\info.txt");
    bool hasBack, hasFront;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        cout<<"failed to open info file."<<endl;
    else{
        QTextStream txtOut(&file);
        QByteArray line = file.readLine();
        QString str(line);
        int i = str.toInt();
        cout<<i<<endl;
        if(i==3) {status = 48; hasBack = hasFront = true;}
        if(i==2) {status = 32; hasBack = true; hasFront = false;}
        if(i==1) {status = 16; hasBack = false; hasFront = true;}
        file.close();
    }


    //cout<<"dark"<<' '<<isDark<<endl;
    if(isDark){
        if(front){
            if(hasFront){
                // DF
                // copy file
                if(!coverFileCopy(dir+"\\CMD\\"+printerName+"\\"+DFPrefix+pattern+".arx4", dir+"\\AR4\\"+pattern+"\\"+DFPrefix+pattern+".arx4") ){
                    cout<<"failed to copy files while sending files."<<endl;
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
                    txtOut<<"GTX4CMD.exe send -A \""+printerName+"\\"+DFPrefix+pattern+".arx4\" -P \"" + printerName + "\" -D 0 "<<endl;
                    file.close();
    
                    QString command = "data\\CMD\\send.bat";
                    p.start(command);
                    p.waitForFinished();
                    qDebug()<<p.exitCode();
                    return p.exitCode();
                }
            }else return -1;// the pattern doesn't have this file
        }
        if(!front){
            if(hasBack){
                // DB
                // copy file
                if(!coverFileCopy(dir+"\\CMD\\"+printerName+"\\"+DBPrefix+pattern+".arx4", dir+"\\AR4\\"+pattern+"\\"+DBPrefix+pattern+".arx4") ){
                    cout<<"failed to copy files while sending files."<<endl;
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
                    txtOut<<"GTX4CMD.exe send -A \""+printerName+"\\"+DBPrefix+pattern+".arx4\" -P \"" + printerName + "\" -D 0 "<<endl;
                    file.close();

                    QString command = "data\\CMD\\send.bat";
                    p.start(command);
                    p.waitForFinished();
                    qDebug()<<p.exitCode();
                    return p.exitCode();
                }
            }else return -1;
        }
    }

    if(!isDark){
        if(front){
            if(hasFront){
                // LF
                // copy file
                if(!coverFileCopy(dir+"\\CMD\\"+printerName+"\\"+LFPrefix+pattern+".arx4", dir+"\\AR4\\"+pattern+"\\"+LFPrefix+pattern+".arx4") ){
                    cout<<"failed to copy files while sending files."<<endl;
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
                    txtOut<<"GTX4CMD.exe send -A \""+printerName+"\\"+LFPrefix+pattern+".arx4\" -P \"" + printerName + "\" -D 0 "<<endl;
                    file.close();

                    QString command = "data\\CMD\\send.bat";
                    p.start(command);
                    p.waitForFinished();
                    qDebug()<<p.exitCode();
                    return p.exitCode();
                }
            }else return -1;
        }

        if(!front){
            if(hasBack){
                // LB
                // copy file
                if(!coverFileCopy(dir+"\\CMD\\"+printerName+"\\"+LBPrefix+pattern+".arx4", dir+"\\AR4\\"+pattern+"\\"+LBPrefix+pattern+".arx4") ){
                    cout<<"failed to copy files while sending files."<<endl;
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
                    txtOut<<"GTX4CMD.exe send -A \""+printerName+"\\"+LBPrefix+pattern+".arx4\" -P \"" + printerName + "\" -D 0 "<<endl;
                    file.close();

                    QString command = "data\\CMD\\send.bat";
                    p.start(command);
                    p.waitForFinished();
                    qDebug()<<p.exitCode();
                    return p.exitCode();
                }
            }else return -1;
        }
    }
}
