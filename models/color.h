#ifndef COLOR_H
#define COLOR_H
#include <models/arx4.h>
#include <vector>

using namespace std;



/* The colors of a pattern
 *
 * Some of the colors are
 *
 *
 *
 */
class Color
{
public:
    Color();

    // 0:default
    int colorID;

    vector<ARX4*> files;
};

#endif // COLOR_H
