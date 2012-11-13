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
	// TO DO
}

void Mesh::addQuad(VertexList<Vertex>* vertexList, const Face& face)
{
	// addQuad goes in the order of 0, 1, 2, 0, 2, 3
	// TO DO
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
