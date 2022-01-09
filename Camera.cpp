#include "Camera.h"
#include <string>
#include <iostream>
#include <iomanip>
#include "Matrix4.h"
#include "Helpers.h"

using namespace std;

Camera::Camera() {}

Camera::Camera(int cameraId,
               int projectionType,
               Vec3 pos, Vec3 gaze,
               Vec3 u, Vec3 v, Vec3 w,
               double left, double right, double bottom, double top,
               double near, double far,
               int horRes, int verRes,
               string outputFileName)
{

    this->cameraId = cameraId;
    this->projectionType = projectionType;
    this->pos = pos;
    this->gaze = gaze;
    this->u = u;
    this->v = v;
    this->w = w;
    this->left = left;
    this->right = right;
    this->bottom = bottom;
    this->top = top;
    this->near = near;
    this->far = far;
    this->horRes = horRes;
    this->verRes = verRes;
    this->outputFileName = outputFileName;
}

Camera::Camera(const Camera &other)
{
    this->cameraId = other.cameraId;
    this->projectionType = other.projectionType;
    this->pos = other.pos;
    this->gaze = other.gaze;
    this->u = other.u;
    this->v = other.v;
    this->w = other.w;
    this->left = other.left;
    this->right = other.right;
    this->bottom = other.bottom;
    this->top = other.top;
    this->near = other.near;
    this->far = other.far;
    this->horRes = other.horRes;
    this->verRes = other.verRes;
    this->outputFileName = other.outputFileName;
}

ostream &operator<<(ostream &os, const Camera &c)
{
    const char *camType = c.projectionType ? "perspective" : "orthographic";

    os << fixed << setprecision(6) << "Camera " << c.cameraId << " (" << camType << ") => pos: " << c.pos << " gaze: " << c.gaze << endl
       << "\tu: " << c.u << " v: " << c.v << " w: " << c.w << endl
       << fixed << setprecision(3) << "\tleft: " << c.left << " right: " << c.right << " bottom: " << c.bottom << " top: " << c.top << endl
       << "\tnear: " << c.near << " far: " << c.far << " resolutions: " << c.horRes << "x" << c.verRes << " fileName: " << c.outputFileName;

    return os;
}


Matrix4 Camera::computeCameraTransformation()
{
    double temp[4][4] = {   {1, 0, 0, -pos.x},
                            {0, 1, 0, -pos.y},
                            {0, 0, 1, -pos.z},
                            {0, 0, 0,   1   }};
    
    Matrix4 t(temp);//translation matrix

    double temp2[4][4] = {  {u.x, u.y, u.z, 0},
                            {v.x, v.y, v.z ,0},
                            {w.x, w.y, w.z ,0},
                            {0, 0, 0 ,1}};
    
    Matrix4 r(temp2);

    this->cameraTransformation = multiplyMatrixWithMatrix(r, t);
    return cameraTransformation;
}


Matrix4 Camera::computePerspectiveTransformation()
{
    //if projection type is orthographic then return M_orth
    //if it is perspective return M_perspective = M_orth . M_p2o

    double temp[4][4] = {   {2/(right-left), 0, 0 ,-(right+left)/(right-left)},
                            {0, 2/(top-bottom), 0, -(top+bottom)/(top-bottom)},
                            {0, 0, -2/(far-near), -(far+near)/(far-near)},
                            {0, 0, 0, 1}};

    Matrix4 mPer(temp);//mPer = M_orth

    if(projectionType)//if we are dealing with perspective projection
    {
        double temp2[4][4] = {  {near, 0, 0, 0},       
                                {0, near, 0, 0},
                                {0, 0, far+near, far*near},
                                {0, 0, -1, 0}};

        Matrix4 m_p2o(temp2);
        mPer = multiplyMatrixWithMatrix(mPer, m_p2o);
    }


    return mPer;
}

Matrix4 Camera::computeMperMcam()
{
    return multiplyMatrixWithMatrix(computePerspectiveTransformation(),
                                        computeCameraTransformation());
}

Matrix4 Camera::computeViewportTransformation()
{
    int x = horRes;
    int y = verRes;
    double temp[4][4] = {   {x/2.0, 0, 0 ,(x-1)/2.0},
                            {0, y/2.0, 0, (y-1)/2.0},
                            {0, 0, 0.5, 0.5},
                            {0, 0, 0, 1}};
    //notice that viewport transformation is normally a matrix of size 3x4
    //but for simplicity we will use a matrix of 4x4 (no class defined for it)
    //then we will get rid of the last element of the resulting vector

    Matrix4 mVp(temp);

    return mVp;
}
