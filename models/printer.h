#ifndef PRINTER_H
#define PRINTER_H

#include <string>
#include <vector>
#include <auxiliary.h>

using namespace std;

class printer
{
public:
    printer(string info);
    printer();

    void load(string info);
    string save();

    int printerID;
    bool availble;
    bool darkprint;
};

#endif // PRINTER_H
