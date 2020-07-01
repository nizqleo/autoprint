#include "order.h"
#include "models/pattern.h"
#include "auxiliary.h"
#include <qdebug.h>

vector<QString> colorString({"白色", "黑色", "粉色", "蓝色",
                            "绿色", "黄色", "紫色", "红色",
                          "浅粉色",

                             "天蓝色","橙色","灰色", "浅灰色","紫罗兰","未识别色"});
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
    type = Pattern::type2index(record[4]);

    if(record.size() > 5){
        finished = record[5].toInt();

    }else finished = false;

    if(color == 1 )
        isDark = true;
    else isDark = false;
}


Order::Order(QString name, int c, int s, int n):pattern(name), color(c),
number(n),size(s){

    if(color == 1 )
        isDark = true;
    else isDark = false;
    finished = true;

}

void Order::addOtherInfo(QString info){
    otherInfos = info;
}

int Order::color2Index(QString c){

    for(int i = 0; i < colorString.size(); i++){
        if(c == colorString[i])
            return i;
    }
    return colorString.size()-1;
}

int Order::size2Index(QString c){
    for(int i = 0; i < 5; i++){
        if(c == sizeString[i])
            return i;
    }
}

