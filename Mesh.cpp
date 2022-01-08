#include <vector>
#include "Triangle.h"
#include "Mesh.h"
#include <iostream>
#include <iomanip>

using namespace std;

Mesh::Mesh() {}

Mesh::Mesh(int meshId, int type, int numberOfTransformations,
             vector<int> transformationIds,
             vector<char> transformationTypes,
             int numberOfTriangles,
             vector<Triangle> triangles)
{
    this->meshId = meshId;
    this->type = type;
    this->numberOfTransformations = numberOfTransformations;
    this->numberOfTriangles = numberOfTriangles;

    this->transformationIds = transformationIds;
    this->transformationTypes = transformationTypes;
    this->triangles = triangles;

}

ostream &operator<<(ostream &os, const Mesh &m)
{
    os << "Mesh " << m.meshId;

    if (m.type == 0)
    {
        os << " wireframe(0) with ";
    }
    else
    {
        os << " solid(1) with ";
    }

    os << fixed << setprecision(3) << m.numberOfTransformations << " transformations and " << m.numberOfTriangles << " triangles"
       << endl << "\tTriangles are:" << endl << fixed << setprecision(0);

    for (size_t i = 0; i < m.triangles.size(); i++) {
        os << "\t\t" << m.triangles[i].vertexIds[0] << " " << m.triangles[i].vertexIds[1] << " " << m.triangles[i].vertexIds[2] << endl;
    }

    return os;
}


void Mesh::computeTransformations(const Scene& scene)
{
    Matrix4 acc = getIdentityMatrix();
    Matrix4 temp;

    vector<int>::iterator id = transformationIds.begin();
    vector<char>::iterotor type = transformationTypes.begin();
    for( ; id != transformationIds.end(); id++, type++){
        switch(*type)
        {
            case r:
                temp = scene.rotations[id]->getMatrix();
                acc = multiplyMatrixWithMatrix(temp, acc);

                break;
            case t:
                temp = scene.translations[id]->getMatrix();
                acc = multiplyMatrixWithMatrix(temp, acc);

                break;
            case s:
                temp = scene.scalings[id]->getMatrix();
                acc = multiplyMatrixWithMatrix(temp, acc);

                break;
        } 
    
    }

    return acc;
}
