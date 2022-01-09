#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vec3.h"
#include <string>
#include "Matrix4.h"

using namespace std;

class Camera
{

public:
    int cameraId;
    int projectionType; // 1 for perspective, 0 for orthographic
    Vec3 pos;
    Vec3 gaze;
    Vec3 u;
    Vec3 v;//up vector loaded from the file
    Vec3 w;
    double left, right, bottom, top;
    double near;
    double far;
    int horRes;
    int verRes;
    string outputFileName;

    Matrix4 cameraTransformation;

    Camera();

    Camera(int cameraId,
           int projectionType,
           Vec3 pos, Vec3 gaze,
           Vec3 u, Vec3 v, Vec3 w,
           double left, double right, double bottom, double top,
           double near, double far,
           int horRes, int verRes,
           string outputFileName);

    Camera(const Camera &other);

    friend std::ostream &operator<<(std::ostream &os, const Camera &c);

    Matrix4 computeCameraTransformation();
    Matrix4 computePerspectiveTransformation();
    Matrix4 computeMperMcam();
    Matrix4 computeViewportTransformation();
};

#endif
