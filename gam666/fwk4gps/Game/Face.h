#ifndef _FACE_H_
#define _FACE_H_

#include <vector>

struct IndexList {
  int v, vt, vn;
  IndexList(int v = 0, int vt = 0, int vn = 0) :
    v(v), vt(vt), vn(vn) { };
  IndexList(const std::vector<int>& nums) : 
    v(nums[0]), vt(nums[1]), vn(nums[2]) { };
};

enum FaceType {
  Tri = 3,
  Quad = 4
};

class Face {
	  IndexList indices[4]; // support up to 4 IndexLists per Face (tri/quad)
	  int currentSize;
  public:
	  FaceType type;

    Face::Face(FaceType type) : type(type), currentSize(0) { };
    Face::Face(unsigned type = 3) : type((FaceType)type), currentSize(0) { };
	  Face* add(IndexList il) { indices[currentSize] = il; ++currentSize; return this; };
	  Face* setType(FaceType t) { type = t; return this; };
	  Face* setType(unsigned t) { type = (FaceType)t; return this; };
    const IndexList& operator[](int index) const { return indices[index]; };
};

#endif