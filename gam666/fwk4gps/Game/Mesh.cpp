#include "Mesh.h"
#include "../Translation.h"

VertexList<Vertex>* Mesh::build() {
  VertexList<Vertex>* vertexList = 
	  (VertexList<Vertex>*)CreateVertexList<Vertex>(TRIANGLE_STRIP,
													numberOfTriangles());
  for(unsigned int i = 0; i < faces.size(); ++i) // add all faces to vertexList
  {
	  addFace(vertexList, faces[i]);
  }
  return vertexList;
}

void Mesh::addFace(VertexList<Vertex>* vertexList, const Face& face)
{
	if(face.type == Tri)
		addTri(vertexList, face);
	else if(face.type == Quad)
		addQuad(vertexList, face);
}

void Mesh::addTri(VertexList<Vertex>* vertexList, const Face& face)
{
	//addTri goes in the order of 0, 1, 2
	Vector v1 = vertices[face[0].v];
	Vector vn1 = normals[face[0].vn];
	Vector vt1 = uvs[face[0].vt];

	Vector v2 = vertices[face[1].v];
	Vector vn2 = normals[face[1].vn];
	Vector vt2 = uvs[face[1].vt];

	Vector v3 = vertices[face[2].v];
	Vector vn3 = normals[face[2].vn];
	Vector vt3 = uvs[face[2].vt];

	vertexList->add(Vertex(v1, vn1, vt1.x, vt1.y));
	vertexList->add(Vertex(v2, vn2, vt2.x, vt2.y));
	vertexList->add(Vertex(v3, vn3, vt3.x, vt3.y));
}

void Mesh::addQuad(VertexList<Vertex>* vertexList, const Face& face)
{
	// addQuad goes in the order of 0, 1, 2, 0, 2, 3
	Vector v1 = vertices[face[0].v];
	Vector vn1 = normals[face[0].vn];
	Vector vt1 = uvs[face[0].vt];

	Vector v2 = vertices[face[1].v];
	Vector vn2 = normals[face[1].vn];
	Vector vt2 = uvs[face[1].vt];

	Vector v3 = vertices[face[2].v];
	Vector vn3 = normals[face[2].vn];
	Vector vt3 = uvs[face[2].vt];

	Vector v4 = vertices[face[3].v];
	Vector vn4 = normals[face[3].vn];
	Vector vt4 = uvs[face[3].vt];

	vertexList->add(Vertex(v1, vn1, vt1.x, vt1.y));
	vertexList->add(Vertex(v2, vn2, vt2.x, vt2.y));
	vertexList->add(Vertex(v3, vn3, vt3.x, vt3.y));

	vertexList->add(Vertex(v1, vn1, vt1.x, vt1.y));
	vertexList->add(Vertex(v3, vn3, vt3.x, vt3.y));
	vertexList->add(Vertex(v4, vn4, vt4.x, vt4.y));
}

void Mesh::normalizeSize(float scale)
{
	// TO DO
}
  
void Mesh::calcAABB()
{
	// TO DO
}

int Mesh::numberOfTriangles()
{
	int num = 0;
	for(unsigned int i = 0; i < faces.size(); ++i)
	{
		if(faces[i].type == Tri)
			num++;
		else if(faces[i].type == Quad)
			num += 2;
	}
	return num;
}
