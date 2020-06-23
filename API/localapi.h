#ifndef LOCALAPI_H
#define LOCALAPI_H
#include "api.h"
#include <QDir>
#include <QFileDialog>

class localAPI:public API
{
public:
    localAPI();
    localAPI(MainWindow* MW, QString dir);

    //collect all of the names of the patterns in the database.
    void getPatternNameList(vector<QString>& patternNames);

    //read a pattern named "pattern", fill the information based on the database
    Pattern readPatternData(QString pattern);

    //delete a pattern named "pattern", include all the information saved in the database
    void deletePatternData(QString pattern);

    //save datas for a pattern. might be a newly added item or item that already exists.
    void savePatternData(QString pattern, QString DFAR4Address, QString DBAR4Address,
                                 QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                                 QString MimageAddress, bool hasBack, bool hasFront);

    // exam the data in the database and compute the status for the pattern.
    // read the image files together.
    // used in datamodel showing. after updating usually.
    void examPatternData(Pattern* pattern);

    bool checkFileOverlap(QString Pattern, int file, QString dir);

    QString showFileDirinDatabase(QString Pattern, int file);

    void getERPOrders(vector<Order>& orderlist, QWidget* p);
    void saveOrders(vector<Order>& orderlist);
    void readOrders(vector<Order>& orderlist);

    void saveTasks(OrderModel* OM);
    void readSavedTasks(OrderModel* OM);

    void readPrinterData(vector<Printer>& printers);

    int sendPrintingFile(QString pattern, bool isDark, QString printerName, bool front);

    void sendSampleToPrinter(string printerName);

    QPixmap loadPics(QString dir);
    QPixmap loadPics(QString name, bool isMimage);

private:
    QTextCodec* codec;
    QTextCodec::ConverterState state;
    // dir for local database
    QString dir;
};

#endif // LOCALAPI_H
