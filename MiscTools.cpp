#include "MiscTools.h"
#include "Helpers.h"
#include <cmath>

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
                temp = scene.rotations[*id-1]->getMatrix();
                acc = multiplyMatrixWithMatrix(temp, acc);

                break;
            case 't':
                temp = scene.translations[*id-1]->getMatrix();
                acc = multiplyMatrixWithMatrix(temp, acc);

                break;
            case 's':
                temp = scene.scalings[*id-1]->getMatrix();
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

            c1 = makeVec3(mesh.transformedVertices[triangle.vertexIds[0]-1]);
            c2 = makeVec3(mesh.transformedVertices[triangle.vertexIds[1]-1]);
            c3 = makeVec3(mesh.transformedVertices[triangle.vertexIds[2]-1]);

            v1 = subtractVec3(c2, c1);
            v2 = subtractVec3(c3, c2);

            normal = crossProductVec3(v1, v2);

            Vec3 vStare = c1 - cam.pos;

            double dotProduct = dotProductVec3(normal, vStare);
            if(dotProduct  < 0)//if facing front
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

//draw the line on the image 
void drawLine(const Line& line, vector< vector<Color> >& image, const vector< Color* >& colors)
{
    double slope;
    int x0, x1, y0, y1;
    Color color0, color1;
    Color color(0, 0, 0);

    slope = (line.p1.y - line.p2.y)/(line.p1.x - line.p2.x);
    if(0 <= slope)
    {
        if(slope <1)// slope in between [0,1)
        {
            if(line.p1.x < line.p2.x)             
            {                                        
                x0 = round(line.p1.x);           
                y0 = round(line.p1.y);           
                color0 = *colors[line.p1.colorId-1];

                x1 = round(line.p2.x);                           
                y1 = round(line.p2.y);                           
                color1 = *colors[line.p2.colorId-1];
            }                                                   
            else                                            
            {                                               
                x0 = round(line.p2.x);
                y0 = round(line.p2.y);
                color0 = *colors[line.p2.colorId-1];

                x1 = round(line.p1.x);
                y1 = round(line.p1.y);
                color1 = *colors[line.p1.colorId-1];
            }

            int y = y0;
            int d = 2*(y0 - y1) + (x1 - x0);
            Color c = color0;
            Color dc = (color1 - color0)/(x1 - x0);
            for(int x = x0; x <= x1; x++)
            {
                image[x][y] = c.cround();

                if(d < 0){
                    y++;
                    d += 2*((y0-y1)+(x1-x0));
                }
                else{
                    d += 2*(y0-y1);
                }
                c += dc;
            }
        }
        else//slope in between (1,infinity)
        {
            if(line.p1.x < line.p2.x)//
            {
                x0 = round(line.p1.x);
                y0 = round(line.p1.y);
                color0 = *colors[line.p1.colorId-1];

                x1 = round(line.p2.x);
                y1 = round(line.p2.y);
                color1 = *colors[line.p2.colorId-1];
            }
            else
            {
                x0 = round(line.p2.x);
                y0 = round(line.p2.y);
                color0 = *colors[line.p2.colorId-1];

                x1 = round(line.p1.x);
                y1 = round(line.p1.y);
                color1 = *colors[line.p1.colorId-1];
            }//now x1 is in the north east of x0

            int x = x0;
            int d = (y0 - y1) + 2*(x1 - x0);
            Color c = color0;
            Color dc = (color1 - color0)/(y1 - y0);
            for(int y = y0; y <= y1; y++)
            {
                image[x][y] = c.cround();

                if(d < 0)
                {
                    d += 2*(x1 - x0);
                }
                else
                {
                    x++;
                    d += 2*((y0 - y1) + (x1 - x0 ));
                }
                c += dc;
            }
        }
    }
    else//slope < 0
    {
        if( -1 >= slope)//slope in between (-1, -infinity]
        {
            if(line.p1.y < line.p2.y)
            {
                x0 = round(line.p1.x);
                y0 = round(line.p1.y);           
                color0 = *colors[line.p1.colorId-1];

                x1 = round(line.p2.x);                           
                y1 = round(line.p2.y);                           
                color1 = *colors[line.p2.colorId-1];
            }
            else
            {
                x0 = round(line.p2.x);
                y0 = round(line.p2.y);
                color0 = *colors[line.p2.colorId-1];

                x1 = round(line.p1.x);
                y1 = round(line.p1.y);
                color1 = *colors[line.p1.colorId-1];
            }

            int x = x0;
            int d = -(y0 - y1) + 2*(x1 - x0);
            Color c = color0;
            Color dc = (color1 - color0)/(y1 - y0);

            for(int y = y0; y<= y1; y++)
            {
                image[x][y] = c.cround();

                if(d < 0){
                    x--;
                    d += 2*(-(y0 - y1) + (x1 - x0) );
                }
                else
                {
                    d += 2*(x1 - x0);
                }
                c += dc;
            }
        }
        else
        {

            if(line.p1.y < line.p2.y)
            {
                x0 = round(line.p1.x);
                y0 = round(line.p1.y);           
                color0 = *colors[line.p1.colorId-1];

                x1 = round(line.p2.x);                           
                y1 = round(line.p2.y);                           
                color1 = *colors[line.p2.colorId-1];
            }
            else
            {
                x0 = round(line.p2.x);
                y0 = round(line.p2.y);
                color0 = *colors[line.p2.colorId-1];

                x1 = round(line.p1.x);
                y1 = round(line.p1.y);
                color1 = *colors[line.p1.colorId-1];
            }

            
            int y = y0;
            int d = -2*(y0 - y1) + (x1 - x0);
            Color c = color0;
            Color dc = (color1 - color0)/(x0-x1);
        
            for(int x = x0; x >= x1; x--)
            {
                image[x][y] = c.cround();

                if( d < 0){
                    d += -2 *(y0 - y1);
                }
                else{
                    y++;
                    d += 2* (-(y0 - y1) + (x1 -x0));
                }
                c += dc;
            }
        }
    }
}

LineEquation::LineEquation(int x0, int x1, int y0, int y1)
{
    this->x0 = x0;
    this->y0 = y0;    
    this->x1 = x1;
    this->y1 = y1;    
}

int LineEquation::getLine(int x,int y){
    return x*(y0-y1)+y*(x1-x0)+x0*y1-y0*x1;
}
