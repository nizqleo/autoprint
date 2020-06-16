#include "localapi.h"

localAPI::localAPI()
{

}

localAPI::localAPI(string dir):dir(dir){

    }


//collect all of the names of the patterns in the database.
void localAPI::getPatternNameList(vector<string>& patternNames){
    if(!get_files(dir+"\\AR4", patternNames)){
        std::cout<<"failed to find any file of this type."<<std::endl;
    }
}

//read a pattern named "pattern", fill the information based on the database
Pattern localAPI::readPatternData(string pattern){

    Pattern tempPattern;
    tempPattern.name = pattern;

    QFileInfo fileInfo;
    QPixmap image;
    QSize picSize(250, 250);
    string fileName = dir+"\\image\\"+pattern+"\\"+PimName+".jpg";
    if(image.load(QString::fromStdString(fileName))){
        tempPattern.Pimages = image.scaled(picSize);
        tempPattern.hasPimages = true;
    }else {
        tempPattern.hasPimages = false;
        cout<<fileName<<" no P image loaded."<<endl;
    }

    fileName = dir+"\\image\\"+pattern+"\\"+MimName+".jpg";
    if(image.load(QString::fromStdString(fileName))){
        tempPattern.Mimages = image.scaled(picSize);
        tempPattern.hasMimages = true;
    }else {
        tempPattern.hasMimages = false;
        cout<<fileName<<" no M image loaded."<<endl;
    }


    // reading info.txt
    int status = 0;
    ifstream inFile(dir+"\\AR4\\"+pattern+"\\info.txt", ios::out);
    if(!inFile){
        cout<<"failed to open info file."<<endl;
    }
    else{
        string lineStr;
        getline(inFile, lineStr);
        if(lineStr[0]== '3') {status = 48; tempPattern.hasBack = tempPattern.hasFront = true;}
        if(lineStr[0]== '2') {status = 32; tempPattern.hasBack = true; tempPattern.hasFront = false;}
        if(lineStr[0]== '1') {status = 16; tempPattern.hasBack = false; tempPattern.hasFront = true;}
        inFile.close();
    }


    //AR4
    fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+pattern+"\\"+LFPrefix+pattern+".arx4"));
    if(fileInfo.isFile())  status += 8;
    fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+pattern+"\\"+DFPrefix+pattern+".arx4"));
    if(fileInfo.isFile())  status += 4;
    fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+pattern+"\\"+LBPrefix+pattern+".arx4"));
    if(fileInfo.isFile())  status += 2;
    fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+pattern+"\\"+DBPrefix+pattern+".arx4"));
    if(fileInfo.isFile())  status += 1;

    tempPattern.status = status;
    tempPattern.computeFileReady();

    return tempPattern;
}

//delete a pattern named "pattern", include all the information saved in the database
void localAPI::deletePatternData(string pattern){
    std::remove((dir+"\\image\\"+pattern+"\\"+PimName+".jpg").data());
    std::remove((dir+"\\image\\"+pattern+"\\"+MimName+".jpg").data());
    std::remove((dir+"\\AR4\\"+pattern+"\\"+LBPrefix+pattern+".arx4").data());
    std::remove((dir+"\\AR4\\"+pattern+"\\"+LFPrefix+pattern+".arx4").data());
    std::remove((dir+"\\AR4\\"+pattern+"\\"+DBPrefix+pattern+".arx4").data());
    std::remove((dir+"\\AR4\\"+pattern+"\\"+DFPrefix+pattern+".arx4").data());
    std::remove((dir+"\\AR4\\"+pattern+"\\info.txt").data());

//    std::remove((dir+"\\AR4\\"+pattern+"\\浅.txt").data());
//    std::remove((dir+"\\AR4\\"+pattern+"\\qian.txt").data());
//    //DeleteFile((dir+"\\AR4\\"+pattern+"\\"+LFPrefix+pattern+".txt").c_str());

    std::cout<<rmdir((dir+"\\AR4\\"+pattern+"\\").c_str())<<std::endl;
    std::cout<<rmdir((dir+"\\image\\"+pattern+"\\").c_str())<<std::endl;
}

//save datas for a pattern. might be a newly added item or item that already exists.
void localAPI::savePatternData(std::string pattern, QString DFAR4Address, QString DBAR4Address,
                             QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                               QString MimageAddress, bool hasBack, bool hasFront){

    string folderPath = dir+"\\image";
    if (0 != access(folderPath.c_str(), 0))
        mkdir(folderPath.c_str());
    folderPath = dir+"\\AR4";
    if (0 != access(folderPath.c_str(), 0))
        mkdir(folderPath.c_str());


    //AR4 file copy
    folderPath = dir+"\\AR4\\"+pattern;
    if (0 != access(folderPath.c_str(), 0))
        mkdir(folderPath.c_str());   // 返回 0 表示创建成功，-1 表示失败

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
    folderPath = dir+"\\image\\"+pattern;
    if (0 != access(folderPath.c_str(), 0))
        mkdir(folderPath.c_str());

    if(PimageAddress != ""){
        if(!coverFileCopy(dir+"\\image\\"+pattern+"\\"+PimName+".jpg", PimageAddress))
            cout<<"failed saving P image!"<<endl;
    }
    if(MimageAddress != ""){
        if(!coverFileCopy(dir+"\\image\\"+pattern+"\\"+MimName+".jpg", MimageAddress))
            cout<<"failed saving M image!"<<endl;
    }

    //BF information save
    std::ofstream outFile(dir+"\\AR4\\"+pattern+"\\info.txt", ios::out);
    if(!outFile){
        cout<<"failed to write in a new file."<<endl;
    }
    outFile<<hasBack*2+hasFront<<endl;
    outFile.close();
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

    fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+pattern->name+"\\"+LFPrefix+pattern->name+".arx4"));
    if(fileInfo.isFile())  status += 8;
    fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+pattern->name+"\\"+DFPrefix+pattern->name+".arx4"));
    if(fileInfo.isFile())  status += 4;
    fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+pattern->name+"\\"+LBPrefix+pattern->name+".arx4"));
    if(fileInfo.isFile())  status += 2;
    fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+pattern->name+"\\"+DBPrefix+pattern->name+".arx4"));
    if(fileInfo.isFile())  status += 1;

    pattern->status = status;


    //image
    QPixmap image;
    QSize picSize(250, 250);
    string fileName = dir+"\\image\\"+pattern->name+"\\"+PimName+".jpg";
    if(image.load(QString::fromStdString(fileName))){
        pattern->Pimages = image.scaled(picSize);;
        pattern->hasPimages = true;
    }else pattern->hasPimages = false;

    fileName = dir+"\\image\\"+pattern->name+"\\"+MimName+".jpg";
    if(image.load(QString::fromStdString(fileName))){
        pattern->Mimages = image.scaled(picSize);;
        pattern->hasMimages = true;
    }else pattern->hasMimages = false;
}


bool localAPI::checkFileOverlap(string Pattern, int file, QString Dir){
   QFileInfo fileInfo;
   switch(file){
   case 0:
       if(checkSameFile(QString::fromStdString(dir + "\\AR4\\"+Pattern+"\\"+DFPrefix+Pattern+".txt"), Dir))
           return false;
       fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+Pattern+"\\"+DFPrefix+Pattern+".txt"));
       if(fileInfo.isFile())  return true;
       else return false;
   case 1:
       if(checkSameFile(QString::fromStdString(dir + "\\AR4\\"+Pattern+"\\"+LFPrefix+Pattern+".txt"), Dir))
           return false;
       fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+Pattern+"\\"+LFPrefix+Pattern+".txt"));
       if(fileInfo.isFile())  return true;
       else return false;
   case 2:
       if(checkSameFile(QString::fromStdString(dir + "\\AR4\\"+Pattern+"\\"+LBPrefix+Pattern+".txt"), Dir))
           return false;
       fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+Pattern+"\\"+LBPrefix+Pattern+".txt"));
       if(fileInfo.isFile())  return true;
       else return false;
   case 3:
       if(checkSameFile(QString::fromStdString(dir + "\\AR4\\"+Pattern+"\\"+DBPrefix+Pattern+".txt"), Dir))
           return false;
       fileInfo.setFile(QString::fromStdString(dir + "\\AR4\\"+Pattern+"\\"+DBPrefix+Pattern+".txt"));
       if(fileInfo.isFile())  return true;
       else return false;
   case 4:
       if(checkSameFile(QString::fromStdString(dir + "\\image\\"+Pattern+"\\"+PimName+".jpg"), Dir))
           return false;
       fileInfo.setFile(QString::fromStdString(dir + "\\image\\"+Pattern+"\\"+PimName+".jpg"));
       if(fileInfo.isFile())  return true;
       else return false;
   case 5:
       if(checkSameFile(QString::fromStdString(dir + "\\image\\"+Pattern+"\\"+MimName+".jpg"), Dir))
           return false;
       fileInfo.setFile(QString::fromStdString(dir + "\\image\\"+Pattern+"\\"+MimName+".jpg"));
       if(fileInfo.isFile())  return true;
       else return false;
   }
}


QString localAPI::showFileDirinDatabase(string Pattern, int file){
    switch(file){
    case 0:
        return QString::fromStdString(dir + "\\AR4\\"+Pattern+"\\"+DFPrefix+Pattern+".arx4");
    case 1:
        return QString::fromStdString(dir + "\\AR4\\"+Pattern+"\\"+LFPrefix+Pattern+".arx4");
    case 2:
        return QString::fromStdString(dir + "\\AR4\\"+Pattern+"\\"+LBPrefix+Pattern+".arx4");
    case 3:
        return QString::fromStdString(dir + "\\AR4\\"+Pattern+"\\"+DBPrefix+Pattern+".arx4");
    case 4:
        return QString::fromStdString(dir + "\\image\\"+Pattern+"\\"+PimName+".jpg");
    case 5:
        return QString::fromStdString(dir + "\\image\\"+Pattern+"\\"+MimName+".jpg");
    }
}


void localAPI::getOrders(vector<Order>& orderlist){

    ifstream inFile(dir+"\\orderC.csv", ios::in);
    //ifstream inFile(dir+"\\small.csv", ios::in);
    if(!inFile){
        cout<<"failed to open order file."<<endl;
    }

    string lineStr;
    vector<string> temp;

    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    getline(inFile, lineStr);
    //cout<<lineStr<<endl;
    while (getline(inFile, lineStr))
    {
        temp.clear();
        //cout<<lineStr<<endl;
        SplitString(lineStr, temp, ",");
        //cout<<temp[1]<<' '<<UnicodeToUTF8(conv.from_bytes(temp[1]))<<endl;
        wstring wc = conv.from_bytes(temp[1]);

        wstring white = L"白色";
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>,wchar_t> convert;
        std::string swhite = convert.to_bytes(white);
        //cout<<(temp[1] == swhite)<<endl;

        orderlist.push_back(Order(temp));

        orderlist.rbegin()->addOtherInfo(temp[2]);
        orderlist.rbegin()->fromERP = true;
    }
    cout<<"order reading ready."<<endl;
}

void localAPI::setTasks(Task){

}
