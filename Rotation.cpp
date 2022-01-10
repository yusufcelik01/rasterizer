#include "Rotation.h"
#include <iostream>
#include <iomanip>
#include "Vec3.h"
#include "Helpers.h"
#include <cmath>

using namespace std;

Rotation::Rotation() {}

Rotation::Rotation(int rotationId, double angle, double x, double y, double z)
{
    this->rotationId = rotationId;
    this->angle = angle;
    this->ux = x;
    this->uy = y;
    this->uz = z;
}

ostream &operator<<(ostream &os, const Rotation &r)
{
    os << fixed << setprecision(3) << "Rotation " << r.rotationId << " => [angle=" << r.angle << ", " << r.ux << ", " << r.uy << ", " << r.uz << "]";

    return os;
}

Matrix4 Rotation::getMatrix()
{
    Matrix4 acc;
    double d;
    Vec3 u(ux, uy, uz, -1);//no need for color

    u = normalizeVec3(u);
    d = sqrt(u.y*u.y + u.z*u.z);

    
    double temp[4][4] = {  {1.0, 0, 0, 0},
                           {0, u.z/d, -u.y/d, 0},
                           {0, u.y/d, u.z/d, 0},
                           {0, 0, 0, 1}};
    Matrix4 rAlpha(temp);
    temp[1][2] *= -1;
    temp[2][1] *= -1;

    Matrix4 r_alpha(temp);

    double temp2[4][4] = { {d  , 0, -u.x, 0},         
                           {0  , 1,    0, 0},
                           {u.x, 0,    d, 0},
                           {0  , 0,    0, 1}};
    Matrix4 rBeta(temp2); 

    temp[0][2] *= -1;
    temp[2][0] *= -1;

    Matrix4 r_beta(temp2);

    double radian = angle * M_PI /180;
    double temp3[4][4] = {  {cos(radian), -sin(radian), 0, 0},
                            {sin(radian), cos(radian), 0, 0},
                            {0,0,1,0},
                            {0,0,0,1}};

    Matrix4 rTheta(temp3);

    /*
    acc = multiplyMatrixWithMatrix(r_alpha, r_beta);
    acc = multiplyMatrixWithMatrix(acc, rTheta);
    acc = multiplyMatrixWithMatrix(acc, rBeta);
    rTheta = multiplyMatrixWithMatrix(acc, rAlpha);
    */
    acc = multiplyMatrixWithMatrix(r_alpha, rBeta);
    acc = multiplyMatrixWithMatrix(acc, rTheta);
    acc = multiplyMatrixWithMatrix(acc, r_beta);
    rTheta = multiplyMatrixWithMatrix(acc, rAlpha);

    return rTheta;
}
