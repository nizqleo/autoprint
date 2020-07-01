#include "pattern.h"
#include "API/api.h"


QString printTypeString[5] = {"ZPT","ZPC", "ZPLMWY","ZPWY", "未识别类型"};

Pattern::Pattern()
{
    AR4Files = new AR4FileModel;
    type = 0;
}


Pattern::Pattern(QString n):name(n){

    AR4Files = new AR4FileModel;
    type = 0;
}



void Pattern::computeFileReady(){
//    DarkReady = LightReady = false;

//    bool b1 = (status>>3) & 1;//LF
//    bool b2 = (status>>2) & 1;//DF
//    bool b3 = (status>>1) & 1;//LB
//    bool b4= (status) & 1;//DB

//    if((!hasBack||(hasBack && b3)) && (!hasFront||(hasFront && b1 ))){
//        LightReady = true;
//    }
//    else LightReady =false;

//    if((!hasBack||(hasBack &&  b4)) && (!hasFront||(hasFront && b2))){
//        DarkReady = true;
//    }
//    else DarkReady =false;
//return true;
}

int Pattern::type2index(QString c){
    //c = c.trimmed();

    for(int i = 0; i < 13; i++){
        if(c == printTypeString[i])
            return i;
    }
    return 13;
}


QPixmap Pattern::getPimage(){
    api->loadPics(name, 0);
}
