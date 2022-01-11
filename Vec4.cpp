
#include "Vec4.h"
#include <iomanip>

using namespace std;

Vec4::Vec4()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->t = 0.0;
    this->colorId = -1;
}

Vec4::Vec4(double x, double y, double z, double t, int colorId)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->t = t;
    this->colorId = colorId;
}
Vec4::Vec4(const Vec4 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->t = other.t;
    this->colorId = other.colorId;
}

Vec4 Vec4::operator=(const Vec4& other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->t = other.t;
    this->colorId = other.colorId;

    return *this;
}

double Vec4::getElementAt(int index)
{
    switch (index)
    {
    case 0:
        return this->x;

    case 1:
        return this->y;

    case 2:
        return this->z;

    case 3:
        return this->t;

    default:
        return this->t;
    }
}

ostream& operator<<(ostream& os, const Vec4& v) {
    
    os << fixed << setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.t << "]";

    return os;
}

Vec4 Vec4::operator-(const Vec4 p1){
    Vec4 temp;
    temp.x = this->x-p1.x;
    temp.y = this->y-p1.y;
    temp.z = this->z-p1.z;
    temp.t = this->t;
    temp.colorId = this->colorId;
    return temp;
}

Vec4 Vec4::operator+(const Vec4 p1){
    Vec4 temp;
    temp.x = this->x+p1.x;
    temp.y = this->y+p1.y;
    temp.z = this->z+p1.z;
    temp.t = this->t;
    temp.colorId = this->colorId;
    return temp;
}