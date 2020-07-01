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
    //localAPI(MainWindow* MW, QString dir);

    //collect all of the names of the patterns in the database.
    void getPatternNameList(vector<QString>& patternNames);

    //read a pattern named "pattern", fill the information based on the database
    void readPatternData(Pattern& pattern);

    //delete a pattern named "pattern", include all the information saved in the database
    void deletePatternData(QString pattern);

    //save datas for a pattern. might be a newly added item or item that already exists.
    // create a dir if adding new item
    // copy the newly added ar4 files.
    // save file info.csv
    void savePatternData(QString pattern, QString PimageAddress, QString  Notes, int type,vector<QString>& fileAddresses,
                         AR4FileModel* fileModel);

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
    void readSavedTasks(OrderModel* OM, int** LeftNumbers);

    void readPrinterData(vector<Printer>& printers);

    int sendPrintingFile(QString pattern, ARX4& file, QString printerName, bool front);

    void sendSampleToPrinter(string printerName);

    int extractARX4file(QString fileName, ARX4& arx4, QPixmap& images);

    void fillImagesAddresses(QString name, vector<QPixmap>& images, vector<QString>& fileAddress, bool Dialog = true);

    QPixmap loadPics(QString dir);
    QPixmap loadPics(QString name, bool isMimage);

private:
    QTextCodec* codec;
    QTextCodec::ConverterState state;
    // dir for local database
    QString dir;
};

#endif // LOCALAPI_H
