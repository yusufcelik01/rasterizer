#include <iostream>
#include <string>
#include <vector>
#include "Scene.h"
#include "Matrix4.h"
#include "Helpers.h"
#include "MiscTools.h"
#include "Triangle.h"

using namespace std;

Scene *scene;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Please run the rasterizer as:" << endl
             << "\t./rasterizer <input_file_name>" << endl;
        return 1;
    }
    else
    {
        const char *xmlPath = argv[1];

        scene = new Scene(xmlPath);



        for(Mesh* mesh: scene->meshes) 
        {
            Matrix4 mModel = computeModellingTransformations(*scene, *mesh);
            mesh->transformedVertices = {};
            for(Triangle triangle: mesh->triangles)
            {
                Vec4 v1,v2,v3;

                v1 = makeVec4(*(scene->vertices[triangle.vertexIds[0]-1]));
                v2 = makeVec4(*(scene->vertices[triangle.vertexIds[1]-1]));
                v3 = makeVec4(*(scene->vertices[triangle.vertexIds[2]-1]));

                mesh->transformedVertices[triangle.vertexIds[0]-1] = multiplyMatrixWithVec4(mModel, v1);
                mesh->transformedVertices[triangle.vertexIds[1]-1] = multiplyMatrixWithVec4(mModel, v2);
                mesh->transformedVertices[triangle.vertexIds[2]-1] = multiplyMatrixWithVec4(mModel, v3);


            }
        }



        for (size_t i = 0; i < scene->cameras.size(); i++)
        {
            // initialize image with basic values
            scene->initializeImage(scene->cameras[i]);

            //do them before the pipeline because multiple cameras
            //use the same vertices with same modelling transformations

            // do forward rendering pipeline operations
            scene->forwardRenderingPipeline(scene->cameras[i]);

            // generate PPM file
            scene->writeImageToPPMFile(scene->cameras[i]);

            // Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
            // Notice that os_type is not given as 1 (Ubuntu) or 2 (Windows), below call doesn't do conversion.
            // Change os_type to 1 or 2, after being sure that you have ImageMagick installed.
            scene->convertPPMToPNG(scene->cameras[i]->outputFileName, 99);
        }

        return 0;
    }
}
