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
};

#endif