#ifndef __MISC_TOOLS__
#define __MISC_TOOLS__


#include "Scene.h"
#include "Mesh.h"
#include "Matrix4.h"
#include "Helpers.h"


Matrix4 computeModellingTransformations(const Scene& scene, Mesh& mesh);
void backFaceCulling(const Camera& cam, Mesh& mesh, vector<int>& frontFacingTriangles, bool cullingEnabled);


#endif
