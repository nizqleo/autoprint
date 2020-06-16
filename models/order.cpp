#include "order.h"



QString printTypeString[7] = {"ZPT", "FG", "K", "LS",
                            "CS001", "T", "W225"};
QString colorString[9] = {"白色", "黑色", "粉色", "蓝色",
                            "绿色", "黄色", "紫色", "红色", "混色"};

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>,wchar_t> convert;
string colorErrorString[8] = {(convert.to_bytes(L"白色")),
                               (convert.to_bytes(L"黑色")),
                               (convert.to_bytes(L"粉色")),
                               (convert.to_bytes(L"蓝色")),
                               (convert.to_bytes(L"绿色")),
                               (convert.to_bytes(L"黄色")),
                               (convert.to_bytes(L"紫色")),
                               (convert.to_bytes(L"红色"))};

Order::Order()
{

}

Order::Order(vector<string> record){

    pattern = record[0];// 0 : name
    color = color2Index(record[1]);// 1 : color
    size  = size2Index(record[2]); // 2 : size
    number = atoi(record[3].c_str()); // 3 : number
    type = type2index(record[4]); // 4 : type;

    if(color == 1 )
        isDark = true;
    else isDark = false;
}


Order::Order(string name, int c, int s, int n):pattern(name), color(c),
number(n),size(s){
    type = ZPT;

    if(color == 1 )
        isDark = true;
    else isDark = false;
}

void Order::addOtherInfo(string info){
    otherInfos = info;
}

int Order::color2Index(string c){

    if(c == colorErrorString[0]) return 0;
    if(c == colorErrorString[1]) return 1;
    if(c == colorErrorString[2]) return 2;
    if(c == colorErrorString[3]) return 3;
    if(c == colorErrorString[4]) return 4;
    if(c == colorErrorString[5]) return 5;
    if(c == colorErrorString[6]) return 6;
    if(c == colorErrorString[7]) return 7;
}

int Order::size2Index(string c){
    if(c == "S") return 0;
    if(c == "M") return 1;
    if(c == "L") return 2;
    if(c == "XL") return 3;
    if(c == "XXL") return 4;
}

printType Order::type2index(string c){
    if(c == "ZPT") return ZPT;
    if(c == "FG") return FG;
    if(c == "K") return K;
    if(c == "LS") return LS;
    if(c == "CS001") return CS001;
    if(c == "T") return T;
    if(c == "W225") return W225;
}


