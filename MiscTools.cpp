#include "MiscTools.h"

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
