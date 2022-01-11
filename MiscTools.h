#ifndef __MISC_TOOLS__
#define __MISC_TOOLS__


#include "Scene.h"
#include "Mesh.h"
#include "Matrix4.h"
#include "Helpers.h"


Matrix4 computeModellingTransformations(const Scene& scene, Mesh& mesh);
void backFaceCulling(const Camera& cam, Mesh& mesh, vector<int>& frontFacingTriangles, bool cullingEnabled);

void drawLine(const Line& line, vector< vector<Color> >& image, const vector< Color* >& colors);

class LineEquation
{
private:
    /* data */
public:
    int x0,x1,y0,y1;
    LineEquation();
    LineEquation(int, int, int, int);
    int getLine(int,int);
};




#endif
