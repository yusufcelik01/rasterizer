#ifndef __TRIANGLE_LINES__
#define __TRIANGLE_LINES__

#include "Vec4.h"

class Line{
    public:
        Vec4 p1,p2;
        Vec4 d;

    Line();
    Line(Vec4 p1, Vec4 p2);
    void setD();
};



#endif
