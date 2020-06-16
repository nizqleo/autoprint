#include "pattern.h"

Pattern::Pattern()
{

}

//Pattern::Pattern(DataRead* DR, string name):name(name){

//}

void Pattern::sendAR4File(){

}




void Pattern::computeFileReady(){
    DarkReady = LightReady = false;

    bool b1 = (status>>3) & 1;//LF
    bool b2 = (status>>2) & 1;//DF
    bool b3 = (status>>1) & 1;//LB
    bool b4= (status) & 1;//DB

    if((!hasBack||(hasBack && b3)) && (!hasFront||(hasFront && b1 ))){
        LightReady = true;
    }
    else LightReady =false;

    if((!hasBack||(hasBack &&  b4)) && (!hasFront||(hasFront && b2))){
        DarkReady = true;
    }
    else DarkReady =false;

}


