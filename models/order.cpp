#include "order.h"



QString printTypeString[7] = {"ZPT", "FG", "K", "LS",
                            "CS001", "T", "W225"};
QString colorString[9] = {"白色", "黑色", "粉色", "蓝色",
                            "绿色", "黄色", "紫色", "红色", "混色"};
QString sizeString[5] = {"S", "M", "L", "XL", "XXL"};


QString colorErrorString[8] = {"白色",
                               "黑色",
                               "粉色",
                               "蓝色",
                               "绿色",
                               "黄色",
                               "紫色",
                               "红色"};

Order::Order()
{

}

Order::Order(vector<QString> record){

    pattern = record[0];// 0 : name
    color = color2Index(record[1]);// 1 : color
    size  = size2Index(record[2]); // 2 : size
    number = record[3].toInt(); // 3 : number
    type = type2index(record[4]); // 4 : type;

    if(record.size() > 5){
        finished = record[5].toInt();

    }else finished = false;

    if(color == 1 )
        isDark = true;
    else isDark = false;


}


Order::Order(QString name, int c, int s, int n):pattern(name), color(c),
number(n),size(s){
    type = ZPT;

    if(color == 1 )
        isDark = true;
    else isDark = false;
    finished = true;
}

void Order::addOtherInfo(QString info){
    otherInfos = info;
}

int Order::color2Index(QString c){

    if(c == colorErrorString[0]) return 0;
    if(c == colorErrorString[1]) return 1;
    if(c == colorErrorString[2]) return 2;
    if(c == colorErrorString[3]) return 3;
    if(c == colorErrorString[4]) return 4;
    if(c == colorErrorString[5]) return 5;
    if(c == colorErrorString[6]) return 6;
    if(c == colorErrorString[7]) return 7;
}

int Order::size2Index(QString c){
    if(c == QString("S")) return 0;
    if(c == QString("M")) return 1;
    if(c == QString("L")) return 2;
    if(c == QString("XL")) return 3;
    if(c == QString( "XXL")) return 4;
}

printType Order::type2index(QString c){
    //c = c.trimmed();
    if(c == QString("ZPT")) return ZPT;
    if(c == QString("FG")) return FG;
    if(c == QString("K")) return K;
    if(c == QString("LS")) return LS;
    if(c == QString("CS001")) return CS001;
    if(c == QString( "T")) return T;
    if(c == QString("W225")) return W225;
}


