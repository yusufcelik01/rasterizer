#include "MiscTools.h"
#include "Helpers.h"

using namespace std;

Matrix4 computeModellingTransformations(const Scene& scene,  Mesh& mesh)
{
    Matrix4 acc = getIdentityMatrix();
    Matrix4 temp;
    vector<int>::iterator id = mesh.transformationIds.begin();
    vector<char>::iterator type = mesh.transformationTypes.begin();
    for( ; id != mesh.transformationIds.end(); id++, type++){
        switch(*type)
        {
            case 'r':
                temp = scene.rotations[*id]->getMatrix();
                acc = multiplyMatrixWithMatrix(temp, acc);

                break;
            case 't':
                temp = scene.translations[*id]->getMatrix();
                acc = multiplyMatrixWithMatrix(temp, acc);

                break;
            case 's':
                temp = scene.scalings[*id]->getMatrix();
                acc = multiplyMatrixWithMatrix(temp, acc);

                break;
        } 
    
    }

    return acc;
}


void backFaceCulling(const Camera& cam, Mesh& mesh, vector<int>& frontFacingTriangles, bool cullingEnabled)
{

    if(cullingEnabled)
    {
        size_t i=0;//triangle index i.e id
        for(Triangle triangle: mesh.triangles)
        {
            Vec3 c1,c2,c3;//corners of the triangle
            Vec3 v1, v2;// edge vectors of a triangle
            Vec3 normal;// normal of the triangle

            c1 = makeVec3(mesh.transformedVertices[triangle.vertexIds[0]]);
            c2 = makeVec3(mesh.transformedVertices[triangle.vertexIds[1]]);
            c3 = makeVec3(mesh.transformedVertices[triangle.vertexIds[2]]);

            v1 = subtractVec3(c2, c1);
            v2 = subtractVec3(c1, c3);

            normal = crossProductVec3(v2, v1);

            if( dotProductVec3(normal, cam.gaze) < 0)//if facing front
            {
                frontFacingTriangles.push_back(i);
            }

            i++;//increment index
        }

    }
    else//culling disabled
    {
        size_t size = mesh.triangles.size();
        for(size_t i=0; i<size; i++)
        {
            frontFacingTriangles.push_back(i);
        }
    }



}

