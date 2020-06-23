#ifndef ARX4_H
#define ARX4_H

#include <QPixmap>


class ARX4
{
public:
    ARX4();

    QPixmap image;

    /* Platen sizes
     * 0:16*21
     * 1:16*18
     * 2:14*16
     * 3:10*12
     * 4:7*8
     */
    int platenSize;
    bool whiteInk;


    void readFiles(QString fileName);
};

#endif // ARX4_H
