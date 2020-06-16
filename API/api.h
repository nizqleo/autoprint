#ifndef API_H
#define API_H

#include <vector>
#include <string>
#include <fstream>

#include <models/pattern.h>
#include <models/task.h>
#include <models/order.h>

#include "auxiliary.h"
using namespace std;
class API
{
public:
    API();

    //collect all of the names of the patterns in the database.
    virtual void getPatternNameList(vector<string>& patternNames) = 0;

    //read a pattern named "pattern", fill the information based on the database
    virtual Pattern readPatternData(string pattern) = 0;

    //delete a pattern named "pattern", include all the information saved in the database
    virtual void deletePatternData(string pattern) = 0;

    //save datas for a pattern. might be a newly added item or item that already exists.
    virtual void savePatternData(std::string pattern, QString DFAR4Address, QString DBAR4Address,
                                 QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                                 QString MimageAddress, bool hasBack, bool hasFront) = 0;

    // exam the data in the database and compute the status for the pattern.
    // read the image files together.
    // used in datamodel showing. after updating usually.
    virtual void examPatternData(Pattern* pattern) = 0;

    //
    virtual bool checkFileOverlap(string Pattern, int file, QString dir) = 0;

    virtual QString showFileDirinDatabase(string Pattern, int file) = 0;

    virtual void getOrders(vector<Order>& orderlist) = 0;

    virtual void setTasks(Task) = 0;


    const static string LFPrefix;
    const static string LBPrefix;
    const static string DFPrefix;
    const static string DBPrefix;

//    const static wstring wLFPrefix;
//    const static wstring wLBPrefix;
//    const static wstring wDFPrefix;
//    const static wstring wDBPrefix;

    const static string PimName;
    const static string MimName;

};

#endif // API_H
