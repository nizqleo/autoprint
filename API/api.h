#ifndef API_H
#define API_H

#include <vector>
#include <string>
#include <fstream>
#include <QProcess>
#include <QDebug>
#include <QImageReader>
#include <QTextCodec>

#include <models/pattern.h>
#include <models/task.h>
#include <models/order.h>
#include <models/printer.h>
#include <models/ordermodel.h>
#include <models/arx4.h>
#include <mainwindow.h>


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
    virtual void readPatternData(Pattern& pattern) = 0;

    //delete a pattern named "pattern", include all the information saved in the database
    virtual void deletePatternData(QString pattern) = 0;

    //save datas for a pattern. might be a newly added item or item that already exists.
    // create a dir if adding new item
    // copy the newly added ar4 files.
    // save file info.csv
    virtual void savePatternData(QString pattern, QString PimageAddress, QString  Notes, int type, vector<QString>& fileAddresses,
                                 AR4FileModel* fileModel) = 0;

    // exam the data in the database and compute the status for the pattern.
    // read the image files together.
    // used in datamodel showing. after updating usually.
    virtual void examPatternData(Pattern* pattern) = 0;

    virtual bool checkFileOverlap(QString Pattern, int file, QString dir) = 0;

    virtual QString showFileDirinDatabase(QString Pattern, int file) = 0;

    virtual void getERPOrders(vector<Order>& orderlist, QWidget* p) = 0;
    virtual void saveOrders(vector<Order>& orderlist) = 0;

    virtual void saveTasks(OrderModel* OM) = 0;

    virtual void readOrders(vector<Order>& orderlist) = 0;
    virtual void readSavedTasks(OrderModel* OM, int** LeftNumbers) = 0;

    virtual void readPrinterData(vector<Printer>& printers) = 0;

    virtual int sendPrintingFile(QString pattern, ARX4& file, QString printerName, bool front) = 0;

    virtual QPixmap loadPics(QString dir) = 0;

    virtual QPixmap loadPics(QString name, bool isMimage) = 0;

    /* extract informations from the arx4 file
     * fill the informations into arx4
     * save image into images
     */
    virtual int extractARX4file(QString fileName, ARX4& arx4, QPixmap& images) = 0;

    /* read images extracted from the AR4 files, read according to the info.csv
     *
     */
    virtual void fillImagesAddresses(QString name, vector<QPixmap>& images, vector<QString>& fileAddress, bool Dialog = true) = 0;

    const static QString LFPrefix;
    const static QString LBPrefix;
    const static QString DFPrefix;
    const static QString DBPrefix;

    const static QString PimName;
    const static QString MimName;

    const static QString defaultColor;

    MainWindow* MW;

private:


};

extern API* api;

#endif // API_H
