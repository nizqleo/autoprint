#ifndef PRINTER_H
#define PRINTER_H

#include <QString>
#include <vector>


using namespace std;

class Printer
{
public:
    Printer(QString info, int ID);
    Printer();

    void load(QString info);
    string save();

    int printerID;
    QString name;
    bool availble;
    bool darkprint;
    bool pro;
};

#endif // PRINTER_H
