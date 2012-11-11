#ifndef _FACE_H_
#define _FACE_H_

struct IndexList {
	int v, vt, vn;
};

enum FaceType
{
	Tri,
	Quad
};

class Face {
	IndexList faces[4]; // support up to 4 IndexLists per Face (tri/quad)
	FaceType type;
	int currentSize;
  public:
	Face() : currentSize(0) {} // constructor
	Face* add(IndexList il) { faces[currentSize] = il; ++currentSize; return this; }
	Face* setType(FaceType t) { type = t; return this; }
};

#endif