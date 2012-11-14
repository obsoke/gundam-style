#include "Mesh.h"
#include "..\APIVertex.h"
#include "..\Translation.h"

VertexList<Vertex>* Mesh::build(bool normalize) {
  if (normalize) normalizeSize(buildScale);
  VertexList<Vertex>* vertexList = (VertexList<Vertex>*) 
    CreateVertexList<Vertex>(TRIANGLE_LIST, numberOfTriangles());
  for (unsigned i=0; i<faces.size(); ++i)
    addFace(vertexList, faces[i]);
  vertexList->calcAABB();
  return vertexList;
}

void Mesh::addTri(VertexList<Vertex>* vertexList, const Face& face) {
  const Vector& v0 = vertices[face[0].v], 
    v1 = vertices[face[1].v], v2 = vertices[face[2].v];
  const Vector& n0 = normals[face[0].vn], 
    n1 = normals[face[1].vn], n2 = normals[face[2].vn];
  const Vector& uv0 = uvs[face[0].vt], uv1 = uvs[face[1].vt], 
    uv2 = uvs[face[2].vt];
  vertexList->add(Vertex(v0, n0, uv0.x, uv0.y));
  vertexList->add(Vertex(v1, n1, uv1.x, uv1.y));
  vertexList->add(Vertex(v2, n2, uv2.x, uv2.y));
}

void Mesh::addQuad(VertexList<Vertex>* vertexList, const Face& face) {
  const Vector& v0 = vertices[face[0].v], v1 = vertices[face[1].v], 
    v2 = vertices[face[2].v], v3 = vertices[face[3].v];
  const Vector& n0 = normals[face[0].vn], n1 = normals[face[1].vn], 
    n2 = normals[face[2].vn], n3 = normals[face[3].vn];
  const Vector& uv0 = uvs[face[0].vt], uv1 = uvs[face[1].vt], 
    uv2 = uvs[face[2].vt], uv3 = uvs[face[3].vt];
  vertexList->add(Vertex(v0, n0, uv0.x, uv0.y));
  vertexList->add(Vertex(v1, n1, uv1.x, uv1.y));
  vertexList->add(Vertex(v2, n2, uv2.x, uv2.y));
  vertexList->add(Vertex(v0, n0, uv0.x, uv0.y));
  vertexList->add(Vertex(v2, n2, uv2.x, uv2.y));
  vertexList->add(Vertex(v3, n3, uv3.x, uv3.y));
}

void Mesh::addFace(VertexList<Vertex>* vertexList, const Face& face) {
  if (face.type == Quad) 
    addQuad(vertexList, face);
  else 
    addTri(vertexList, face);
}

unsigned Mesh::numberOfTriangles() {
  int num = 0;
  for (unsigned i=0; i<faces.size(); ++i) {
    const Face& face = faces[i];
    if (face.type == Quad) 
      num += 2;
    else if (face.type == Tri) 
      ++num;
  }
  return num;
}

AABB Mesh::calcAABB() {
  AABB aabb;
  
  for (unsigned i=0; i<vertices.size(); ++i) {
    const Vector& v = vertices[i];
    if (!i) {
      aabb.minimum = v;
      aabb.maximum = v;
    }
    aabb.minimum.x = min(aabb.minimum.x, v.x);
    aabb.minimum.y = min(aabb.minimum.y, v.y);
    aabb.minimum.z = min(aabb.minimum.z, v.z);
    aabb.maximum.x = max(aabb.maximum.x, v.x);
    aabb.maximum.y = max(aabb.maximum.y, v.y);
    aabb.maximum.z = max(aabb.maximum.z, v.z);
  }

  return aabb;
}

void Mesh::normalizeSize(float scale) {
  AABB& aabb = calcAABB();
  float width = aabb.width(), height = aabb.height(), depth = aabb.depth();
  float avg = (width + height + depth) / 3.0f;
  for (unsigned i=0; i<vertices.size(); ++i) {
    Vector& v = vertices[i];
    if (width) v.x /= avg;
    if (height) v.y /= avg;
    if (depth) v.z /= avg;
    v.x *= scale;
    v.y *= scale;
    v.z *= scale;
  }
}