#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"
#include "MiscTools.h"
#include "Line.h"
#include <map>




using namespace tinyxml2;
using namespace std;

/*
	Transformations, clipping, culling, rasterization are done here.
	You may define helper functions.
*/
void Scene::forwardRenderingPipeline(Camera *camera)
{
    Matrix4  mCam, mPer, Mvp;
    
    //if(camera->cameraId != 5 ){return;}//TODO debug
    for(Mesh* mesh: meshes)
    {
        //************** BACK FACE CULLING************
        vector<int> frontFacingTriangles = {};
        unordered_map<int, Vec4> processedVertices = {};

        backFaceCulling(*camera, *mesh, frontFacingTriangles, cullingEnabled);

        //*********** CAMERA TRANSFORMATIONS *********

        Matrix4 MperMcam = camera->computeMperMcam();

        //fill transformed vertices to unoredered_map
        for(int triangleId: frontFacingTriangles)
        {
            //
            Vec4 v1,v2,v3; 
            Triangle triangle = mesh->triangles[triangleId];
            v1 = mesh->transformedVertices[triangle.vertexIds[0]-1];
            v2 = mesh->transformedVertices[triangle.vertexIds[1]-1];
            v3 = mesh->transformedVertices[triangle.vertexIds[2]-1];

            v1 = multiplyMatrixWithVec4(MperMcam, v1);
            v2 = multiplyMatrixWithVec4(MperMcam, v2);
            v3 = multiplyMatrixWithVec4(MperMcam, v3);


            processedVertices.try_emplace(triangle.vertexIds[0], v1);
            processedVertices.try_emplace(triangle.vertexIds[1], v2);
            processedVertices.try_emplace(triangle.vertexIds[2], v3);
            //cout << triangle.vertexIds[0] << " " << triangle.vertexIds[1] << " " << triangle.vertexIds[2] << " " <<endl;//TODO debug
        }//CVV 
        //Vec4 v =processedVertices[-1]; //TODO debug
        //cout << v.x << " " << v.y << ' ' << v.z << " " << v.t << endl; 
        //cout << "-1 \n";

        //**************** CLIPPING ******************
        //

        if(!mesh->type)//if mesh is a wireframe mesh
        {
            vector<Line> lines = {};
            map< pair<int, int>, bool> isLineAdded = {};

            for(int triangleId: frontFacingTriangles)
            {
                Triangle triangle = mesh->triangles[triangleId];
                int vId1,vId2,vId3;

                vId1 = triangle.vertexIds[0];
                vId2 = triangle.vertexIds[1];
                vId3 = triangle.vertexIds[2];

                if(vId2 < vId1){
                    swap(vId1, vId2);//vId1 is smaller now
                }
                if(vId3 < vId1){
                    swap(vId1, vId3);//vId1 is the smallest
                }
                if(vId3 < vId2){
                    swap(vId2, vId3);
                }
                // now ids are sorted


                if(!isLineAdded[make_pair(vId1, vId2)] )
                {//if that line is not present add it
                    Vec4 p1,p2;

                    p1 = processedVertices[vId1];
                    p2 = processedVertices[vId2];

                    lines.push_back(Line(p1, p2));
                    isLineAdded.try_emplace({vId1, vId2});
                    
                }
                if(!isLineAdded[make_pair(vId2, vId3)] )
                {
                    Vec4 p2, p3;

                    p2 = processedVertices[vId2];
                    p3 = processedVertices[vId3];

                    lines.push_back(Line(p2, p3));
                    isLineAdded.try_emplace({vId2, vId3});
                
                }
                if(!isLineAdded[make_pair(vId1, vId3)] )
                {
                    Vec4 p1, p3;
                
                    p1 = processedVertices[vId1];
                    p3 = processedVertices[vId3];

                    lines.push_back(Line(p1, p3));
                    isLineAdded.try_emplace({vId1, vId3});
                }
            }
			// TODO start culling
			// const int l,r,b,t;
			// for(Line& line : lines){
			// }

            //******* prespective division & viewport transformation ****
            Matrix4 mViewport = camera->computeViewportTransformation();

            for(Line& line: lines)
            {
                Vec4 point1, point2;

                if(camera->projectionType)
                {
                    point1 = perspectiveDivide(line.p1);
                    point2 = perspectiveDivide(line.p2);
                }
                line.p1 = multiplyMatrixWithVec4(mViewport, point1);
                line.p2 = multiplyMatrixWithVec4(mViewport, point2);
            }

            for(Line line: lines)
            {
                /*
                Color color(0, 0, 0);
                this->image[round(line.p1.x)][round(line.p1.y)] = color;
                this->image[round(line.p2.x)][round(line.p2.y)] = color;
                */
                drawLine(line, this->image, this->colorsOfVertices);
            }
        }
        else//if mesh is a solid mesh
        {
            Matrix4 mViewport = camera->computeViewportTransformation();

            for(pair<const int, Vec4>& vec : processedVertices)
            {
                if(camera->projectionType)
                {
                    vec.second = perspectiveDivide(vec.second);
                }
                vec.second = multiplyMatrixWithVec4(mViewport, vec.second);
            }

			for (int id : frontFacingTriangles)
			{
                
				Triangle triangle = mesh->triangles[id];

				Vec4 vec0 = processedVertices[triangle.getFirstVertexId()];
				Vec4 vec1 = processedVertices[triangle.getSecondVertexId()];
                Vec4 vec2 = processedVertices[triangle.getThirdVertexId()];

				int x0,y0,x1,y1,x2,y2;
				Color c0, c1, c2;
				x0 = vec0.x;
				y0 = vec0.y;
				c0 = *(this->colorsOfVertices[vec0.colorId-1]);

				x1 = vec1.x;
				y1 = vec1.y;
				c1 = *(this->colorsOfVertices[vec1.colorId-1]);
				
				x2 = vec2.x;
				y2 = vec2.y;
				c2 = *(this->colorsOfVertices[vec2.colorId-1]);

				LineEquation f01(x0,x1,y0,y1);
				LineEquation f12(x1,x2,y1,y2);
				LineEquation f20(x2,x0,y2,y0);

				int xmin = min(x0,min(x1,x2));
				int ymin = min(y0,min(y1,y2));
				int xmax = max(x0,max(x1,x2));
				int ymax = max(y0,max(y1,y2));
				double denom12 = 1.0/f12.getLine(x0,y0);
				double denom20 = 1.0/f20.getLine(x1,y1);
				double denom01 = 1.0/f01.getLine(x2,y2);
				for (int y = ymin; y <= ymax; y++){
					for (int x = xmin; x <= xmax; x++)
					{
                        if(x < 0 || y <0){
                            continue;
                        }
						double alpha = f12.getLine(x,y)*denom12;
						double beta = f20.getLine(x,y)*denom20;
						double gamma = f01.getLine(x,y)*denom01;
						if(alpha >= 0 && beta >= 0 && gamma >= 0){
							Color c = c0*alpha + c1*beta + c2*gamma;

							image[x][y] = c.cround();
						}
					}
				}
			}
        }
    }

    size_t numberOfMeshes = this->meshes.size();
    vector<unordered_map<int, Vec3>> meshVertices;//move to mesh class
    
    for(size_t i=0; i<numberOfMeshes; i++)//calculate new meshes
    {
        //mesh = this->meshes[i];//processing i'th mesh
        meshVertices.push_back({});//allocate new map
    
    }
}

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	// read background color
	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	pElement = pRoot->FirstChildElement("Culling");
	if (pElement != NULL) {
		str = pElement->GetText();
		
		if (strcmp(str, "enabled") == 0) {
			cullingEnabled = true;
		}
		else {
			cullingEnabled = false;
		}
	}

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != NULL)
	{
		Camera *cam = new Camera();

		pCamera->QueryIntAttribute("id", &cam->cameraId);

		// read projection type
		str = pCamera->Attribute("type");

		if (strcmp(str, "orthographic") == 0) {
			cam->projectionType = 0;
		}
		else {
			cam->projectionType = 1;
		}

		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

		cam->gaze = normalizeVec3(cam->gaze);
		cam->u = crossProductVec3(cam->gaze, cam->v);
		cam->u = normalizeVec3(cam->u);

		cam->w = inverseVec3(cam->gaze);
		cam->v = crossProductVec3(cam->u, cam->gaze);
		cam->v = normalizeVec3(cam->v);

		camElement = pCamera->FirstChildElement("ImagePlane");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &cam->left, &cam->right, &cam->bottom, &cam->top,
			   &cam->near, &cam->far, &cam->horRes, &cam->verRes);

		camElement = pCamera->FirstChildElement("OutputName");
		str = camElement->GetText();
		cam->outputFileName = string(str);

		cameras.push_back(cam);

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// read vertices
	pElement = pRoot->FirstChildElement("Vertices");
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = pVertex->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = pVertex->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		vertices.push_back(vertex);
		colorsOfVertices.push_back(color);

		pVertex = pVertex->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	pElement = pRoot->FirstChildElement("Translations");
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != NULL)
	{
		Translation *translation = new Translation();

		pTranslation->QueryIntAttribute("id", &translation->translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}

	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != NULL)
	{
		Scaling *scaling = new Scaling();

		pScaling->QueryIntAttribute("id", &scaling->scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}

	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != NULL)
	{
		Rotation *rotation = new Rotation();

		pRotation->QueryIntAttribute("id", &rotation->rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read meshes
	pElement = pRoot->FirstChildElement("Meshes");

	XMLElement *pMesh = pElement->FirstChildElement("Mesh");
	//XMLElement *meshElement;
	while (pMesh != NULL)
	{
		Mesh *mesh = new Mesh();

		pMesh->QueryIntAttribute("id", &mesh->meshId);

		// read projection type
		str = pMesh->Attribute("type");

		if (strcmp(str, "wireframe") == 0) {
			mesh->type = 0;
		}
		else {
			mesh->type = 1;
		}

		// read mesh transformations
		XMLElement *pTransformations = pMesh->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		while (pTransformation != NULL)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			mesh->transformationTypes.push_back(transformationType);
			mesh->transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}

		mesh->numberOfTransformations = mesh->transformationIds.size();

		// read mesh faces
		char *row;
		char *clone_str;
		int v1, v2, v3;
		XMLElement *pFaces = pMesh->FirstChildElement("Faces");
        str = pFaces->GetText();
		clone_str = strdup(str);

		row = strtok(clone_str, "\n");
		while (row != NULL)
		{
			int result = sscanf(row, "%d %d %d", &v1, &v2, &v3);
			
			if (result != EOF) {
				mesh->triangles.push_back(Triangle(v1, v2, v3));
			}
			row = strtok(NULL, "\n");
		}
		mesh->numberOfTriangles = mesh->triangles.size();
		meshes.push_back(mesh);

		pMesh = pMesh->NextSiblingElement("Mesh");
	}
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFileName.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFileName << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		if(system(command.c_str()) == -1) return;
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
		if(system(command.c_str()) == -1) return ;
	}

	// default action - don't do conversion
	else
	{
	}
}
