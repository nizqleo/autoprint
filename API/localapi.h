#ifndef LOCALAPI_H
#define LOCALAPI_H
#include "api.h"

class localAPI:public API
{
public:
    localAPI();
    localAPI(string dir);

    //collect all of the names of the patterns in the database.
    void getPatternNameList(vector<string>& patternNames);

    //read a pattern named "pattern", fill the information based on the database
    Pattern readPatternData(string pattern);

    //delete a pattern named "pattern", include all the information saved in the database
    void deletePatternData(string pattern);

    //save datas for a pattern. might be a newly added item or item that already exists.
    void savePatternData(std::string pattern, QString DFAR4Address, QString DBAR4Address,
                                 QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                                 QString MimageAddress, bool hasBack, bool hasFront);

    // exam the data in the database and compute the status for the pattern.
    // read the image files together.
    // used in datamodel showing. after updating usually.
    void examPatternData(Pattern* pattern);

    bool checkFileOverlap(string Pattern, int file, QString dir);

    QString showFileDirinDatabase(string Pattern, int file);

    void getOrders(vector<Order>& orderlist);

    void setTasks(Task);
private:

    // dir for local database
    string dir;
};

#endif // LOCALAPI_H
