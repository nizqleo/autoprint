#ifndef PATTERN_H
#define PATTERN_H

#include <string>
#include <QPixmap>
#include <QSize>
#include <models\arx4.h>
#include <models\color.h>

using namespace std;


/* Pattern -- a class for information about a Pattern, the basic data
 * unit in the dataset.
 *
 * Constructed by datasetModel when it's newly built and reading files.
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

    // parameters
    QString name;
    bool hasBack;
    bool hasFront;
    bool hasPimages;
    bool hasMimages;
    bool DarkReady;
    bool LightReady;

    /* 111111 == 63
     * 1     : has back 32
     *  1    : has front 16
     *   1   : has LF 8
     *    1  : has DF 4
     *     1 : has LB 2
     *      1: has DB 1
     */
    int status;

    QPixmap Pimages;
    QPixmap Mimages;

    void computeFileReady();

    vector<ARX4> files;
    vector<Color> colors;
};

#endif // PATTERN_H
