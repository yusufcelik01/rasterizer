#include "Line.h"

Line::Line(Vec4 point1, Vec4 point2)
{
    this->p1 = point1;
    this->p2 = point2;

}

void Line::setD(){
    this->d = p2-p1;
}