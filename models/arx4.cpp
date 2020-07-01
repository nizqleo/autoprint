#include "arx4.h"
#include "models\pattern.h"
#include "API/api.h"

QString positionString[4] = {"正面", "背面", "袖子", "未识别位置"};
QString platenSizeString[6] = {"16x21","16x18","14x16","10x12","7x8","未识别尺寸"};
QString InkString[2] = {"仅彩色", "彩色+白色"};

/* class for ARX4 files
 */
ARX4::ARX4()
{

}

ARX4::ARX4(vector<QString>& temp){
    colorQStr2Index(temp[0]);
    position = position2Index(temp[1]);
    whiteInk = (temp[2] == "彩色+白色");
    platenSize = platenSize2Index(temp[3]);
}

int ARX4::position2Index(QString c){
    for(int i = 0; i < 4; i++){
        if(c == positionString[i])
            return i;
    }
}

int ARX4::platenSize2Index(QString c){
    for(int i = 0; i < 6; i++){
        if(c == platenSizeString[i])
            return i;
    }
}

void ARX4::colorQStr2Index(QString c){
    if(c == "默认")
        applyColor = -1;
    else{
        vector<QString> temp;
        SplitString(c, temp, ";");
        for (auto p = temp.begin(); p < temp.end(); p ++) {
            applyColor = Order::color2Index(*p);
        }
    }
}

QString ARX4::color2QString() const{
    if(applyColor == -1)
        return API::defaultColor;
    else return colorString[applyColor];
}
