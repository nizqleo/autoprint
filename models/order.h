#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include "auxiliary.h"
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
extern string colorErrorString[8];

class Order
{
public:
    Order();
    Order(vector<string> record);
    Order(string name, int c, int s, int n);
    static int color2Index(string c);
    static int size2Index(string s);
    static printType type2index(string c);

    void addOtherInfo(string info);

    //static const QString colorErrorString[8];

//private:
    string SKU;

    string pattern;
    int color;
    bool isDark;
    int size;
    printType type;

    int number;

    string otherInfos;
    bool fromERP;

    int TimeStamp;
};

#endif // ORDER_H
