#ifndef PRINTER_H
#define PRINTER_H

#include <string>
#include <vector>
#include <auxiliary.h>

using namespace std;

class Printer
{
public:
    Printer(string info);
    Printer();

    void load(string info);
    string save();

    int printerID;
    bool availble;
    bool darkprint;
};

#endif // PRINTER_H
