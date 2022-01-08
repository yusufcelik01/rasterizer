#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "Triangle.h"
#include <iostream>
#include "Matrix4.h"
#include <unordered_map>

using namespace std;

class Mesh
{

public:
    int meshId;
    int type; // 0 for wireframe, 1 for solid
    int numberOfTransformations;
    vector<int> transformationIds;
    vector<char> transformationTypes;
    int numberOfTriangles;
    vector<Triangle> triangles;

    Matrix4 compositeTransformation;
    unordered_map<int, Vec3> transformedVertices;

    Mesh();
    Mesh(int meshId, int type, int numberOfTransformations,
          vector<int> transformationIds,
          vector<char> transformationTypes,
          int numberOfTriangles,
          vector<Triangle> triangles);

    friend ostream &operator<<(ostream &os, const Mesh &m);

    void computeTransformations(const Scene& scene);
};

#endif
