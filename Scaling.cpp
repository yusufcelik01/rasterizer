#include "Scaling.h"
#include <iostream>
#include <iomanip>
#include "Matrix4.h"

using namespace std;

Scaling::Scaling() {}

Scaling::Scaling(int scalingId, double sx, double sy, double sz)
{
    this->scalingId = scalingId;
    this->sx = sx;
    this->sy = sy;
    this->sz = sz;
}

ostream &operator<<(ostream &os, const Scaling &s)
{
    os << fixed << setprecision(3) << "Scaling " << s.scalingId << " => [" << s.sx << ", " << s.sy << ", " << s.sz << "]";

    return os;
}


Matrix4 Scaling::getMatrix()
{
    double temp[4][4] = {   {sx, 0, 0, 0},
                            {0, sy, 0, 0},
                            {0, 0, sz, 0},
                            {0, 0, 0, 1 }};
    
    Matrix4 s(temp);
    return s;
}
