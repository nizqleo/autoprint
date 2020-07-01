#ifndef PATTERN_H
#define PATTERN_H

#include <string>
#include <QPixmap>
#include <QSize>

#include <models\color.h>
#include "models\ar4filemodel.h"

using namespace std;


/* Pattern -- a class for information about a Pattern, the basic data
 * unit in the dataset.
 *
 * Constructed by datasetModel when it's newly built and reading files.
 */

extern QString printTypeString[5];

class Pattern
{
public:

    Pattern();
    //Pattern(DataRead* DR, string name);

    Pattern(QString name);

    void save();
    void Delete();

    void computeFileReady();

    QPixmap getPimage();


    QString name;

    bool hasPimages;
    bool hasMimages;

    QPixmap Mimages;

    int type;
    QString Notes;

    AR4FileModel* AR4Files;
    vector<int> specialColor;

    static int type2index(QString c);
};

#endif // PATTERN_H
