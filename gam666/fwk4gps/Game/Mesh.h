#ifndef _MESH_H_
#define _MESH_H_

#include "..\MathDecl.h"
#include "..\Graphic.h"
#include "..\APIVertex.h"
#include "Face.h"
#include <vector>

struct Mesh {
  std::vector<Vector> vertices;
  std::vector<Vector> uvs;
  std::vector<Vector> normals;
  std::vector<Face> faces;

  VertexList<Vertex>* build();
  void normalizeSize(float scale); // makes sure the mesh is the size you want
  void calcAABB(); // we need to know the AABB before finding out the avg size
private:
  void addFace(VertexList<Vertex>* vertexList, const Face& face); // calls addTri() or addQuad depending on the face's type
  void addTri(VertexList<Vertex>* vertexList, const Face& face);
  void addQuad(VertexList<Vertex>* vertexList, const Face& face);
  int numberOfTriangles(); //for allocation
};

#endif