#ifndef PATTERN_H
#define PATTERN_H

#include <string>
#include <QPixmap>
#include <QSize>

using namespace std;


/* Pattern -- a class for information about a Pattern, the basic data
 * unit in the dataset.
 *
 *
 *
 *
 */
class Pattern
{
public:
    Pattern();
    //Pattern(DataRead* DR, string name);

    void sendAR4File();
    void save();
    void Delete();

    const static string LFFileNamePrefix;
    const static string LBFileNamePrefix;
    const static string DFFileNamePrefix;
    const static string DBFileNamePrefix;

private:
    string name;
    bool hasBack;
    bool hasFront;



    QPixmap Pimages;
    QPixmap Mimages;
};

#endif // PATTERN_H
