#ifndef __VEC3_H__
#define __VEC3_H__

#include <iostream>
using namespace std;

class Vec3
{
public:
    double x, y, z;
    int colorId;

    Vec3();
    Vec3(double x, double y, double z, int colorId);
    Vec3(const Vec3 &other);

    double getElementAt(int index);
    Vec3 operator*(const int& i); 
    Vec3 operator+(const Vec3& v);
    Vec3 operator-(const Vec3& v);
    
    friend std::ostream& operator<<(std::ostream& os, const Vec3& v);
};

#endif
