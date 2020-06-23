#include "order.h"



QString printTypeString[14] = {"ZPT", "FG", "K", "LS",
                            "WT","WY","XCWT","WT",
                              "ZPC", "ZPLMWY","WT","ZPWY",
                              "W225","未识别类型"};
QString colorString[14] = {"白色", "黑色", "粉色", "蓝色",
                            "绿色", "黄色", "紫色", "红色",
                          "浅粉色","天蓝色","橙色","灰色",
                          "紫罗兰", "未识别色"};
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


Order::Order(QString name, int c, int s, int n, int t):pattern(name), color(c),
number(n),size(s), type(t){

    if(color == 1 )
        isDark = true;
    else isDark = false;
    finished = true;
}

void Order::addOtherInfo(QString info){
    otherInfos = info;
}

int Order::color2Index(QString c){

    //qDebug()<<"reading tasks"<<c<<endl;
    for(int i = 0; i < 13; i++){
        if(c == colorString[i])
            return i;
    }
    return 13;
}

int Order::size2Index(QString c){
    for(int i = 0; i < 5; i++){
        if(c == sizeString[i])
            return i;
    }
}

int Order::type2index(QString c){
    //c = c.trimmed();

    for(int i = 0; i < 13; i++){
        if(c == printTypeString[i])
            return i;
    }
    return 13;
}


