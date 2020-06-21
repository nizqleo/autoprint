#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include "auxiliary.h"
#include <qdebug.h>
using namespace std;



/* Order -- class for a piece of order
 *
 * An order contains cloths with only one pattern one color and one size.
 *
 * Orders are constructed with information from ERP, cvs files or hand input.
 *
 * A Task contains multiple orders.
 *
 *
 * patterns are just a string here.
 */

enum printType {ZPT, FG, K, LS, CS001, T, W225};
extern QString printTypeString[7];
extern QString colorString[9];
extern QString colorErrorString[8];
extern QString sizeString[5];

class Order
{
public:
    Order();
    Order(vector<QString> record);
    Order(QString name, int c, int s, int n);
    static int color2Index(QString c);
    static int size2Index(QString s);
    static printType type2index(QString c);

    void addOtherInfo(QString info);

    //static const QString colorErrorString[8];

//private:
    string SKU;

    QString pattern;
    int color;
    bool isDark;
    int size;
    bool finished;
    printType type;

    int number;

    QString otherInfos;
    bool fromERP;

    int TimeStamp;
};

#endif // ORDER_H
