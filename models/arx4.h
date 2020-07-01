#ifndef ARX4_H
#define ARX4_H

#include <QPixmap>
#include <vector>
#include <auxiliary.h>
#include <models\order.h>

using namespace std;

extern QString positionString[4];
extern QString platenSizeString[6];
extern QString InkString[2];

class Pattern;

class ARX4
{
public:
    ARX4();
    ARX4(vector<QString>& temp);


    //bool imageSaved;

    /* Platen sizes
     * 0:16*21
     * 1:16*18
     * 2:14*16
     * 3:10*12
     * 4:7*8
     */
    int platenSize;
    bool whiteInk;

    int applyColor;
    int position;

    QString color2QString() const;
    void colorQStr2Index(QString c);

    static int position2Index(QString c);
    static int platenSize2Index(QString c);

};

#endif // ARX4_H
