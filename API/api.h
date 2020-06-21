#ifndef API_H
#define API_H

#include <vector>
#include <string>
#include <fstream>
#include <QProcess>
#include <QDebug>
#include <QImageReader>


#include <models/pattern.h>
#include <models/task.h>
#include <models/order.h>
#include <models/printer.h>
#include <models/ordermodel.h>

class MainWindow;

#include "auxiliary.h"
using namespace std;
class API
{
public:
    API();
    API(MainWindow* MW);

    //collect all of the names of the patterns in the database.
    virtual void getPatternNameList(vector<QString>& patternNames) = 0;

    //read a pattern named "pattern", fill the information based on the database
    virtual Pattern readPatternData(QString pattern) = 0;

    //delete a pattern named "pattern", include all the information saved in the database
    virtual void deletePatternData(QString pattern) = 0;

    //save datas for a pattern. might be a newly added item or item that already exists.
    virtual void savePatternData(QString pattern, QString DFAR4Address, QString DBAR4Address,
                                 QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                                 QString MimageAddress, bool hasBack, bool hasFront) = 0;

    // exam the data in the database and compute the status for the pattern.
    // read the image files together.
    // used in datamodel showing. after updating usually.
    virtual void examPatternData(Pattern* pattern) = 0;

    virtual bool checkFileOverlap(QString Pattern, int file, QString dir) = 0;

    virtual QString showFileDirinDatabase(QString Pattern, int file) = 0;

    virtual void getERPOrders(vector<Order>& orderlist) = 0;
    virtual void saveOrders(vector<Order>& orderlist) = 0;

    virtual void saveTasks(OrderModel* OM) = 0;

    virtual void readOrders(vector<Order>& orderlist) = 0;
    virtual void readSavedTasks(OrderModel* OM) = 0;

    virtual void readPrinterData(vector<Printer>& printers) = 0;

    virtual int sendPrintingFile(QString pattern, bool isDark, int num, QString printerName, bool front) = 0;

    const static QString LFPrefix;
    const static QString LBPrefix;
    const static QString DFPrefix;
    const static QString DBPrefix;

    const static QString PimName;
    const static QString MimName;

    MainWindow* MW;

private:


};

#endif // API_H
