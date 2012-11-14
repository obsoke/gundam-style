#ifndef _MESH_H_
#define _MESH_H_

#include "..\MathDecl.h"
#include "..\Graphic.h"
#include "..\APIVertex.h"
#include "Face.h"
#include <vector>

class Mesh {
    void addTri(VertexList<Vertex>* vertexList, const Face& face);
    void addQuad(VertexList<Vertex>* vertexList, const Face& face);
    void addFace(VertexList<Vertex>* vertexList, const Face& face);
    unsigned numberOfTriangles();
    void normalizeSize(float scale = 1);
  public:
    float buildScale;
    std::vector<Vector> vertices;
    std::vector<Vector> uvs;
    std::vector<Vector> normals;
    std::vector<Face> faces;

    Mesh (float buildScale = 45) : buildScale(buildScale) { };

    VertexList<Vertex>* build(bool normalize = true);
    AABB calcAABB();
};

#endif