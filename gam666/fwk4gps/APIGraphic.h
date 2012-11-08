#ifndef _API_GRAPHIC_H_
#define _API_GRAPHIC_H_

/* APIGraphic Definition - Translation Layer
*
* APIGraphic.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include "APIPlatform.h" // for API headers
#include "APIBase.h"     // for the APIBase class definition
#include "iAPIGraphic.h" // for the APIGraphic Interface
#include "MathDecl.h"    // for Vector

struct AABB;

//-------------------------------- APIVertexDeclaration -----------------------
//
// APIVertexDeclaration holds the description of a vertex
//
template <class T = Vertex>
class APIVertexDeclaration {

  static D3DVERTEXELEMENT9 fmt[MAXD3DDECLLENGTH + 1];
  static unsigned vertexSize;
  static bool     isLit;

public:
  static D3DVERTEXELEMENT9* format() { return fmt; }
  static unsigned size()             { return vertexSize; }
  static bool     lit()              { return isLit; }
};

//-------------------------------- APIGraphic ---------------------------------
//
// The APIGraphic class sets the Object properties at the API level
//
class APIGraphic : public iAPIGraphic, public APIBase {

  ID3DXSprite* sprite;  // points to the interface to sprite COM object
  bool         ready;   // sprite is ready to display graphics?

protected:
  virtual ~APIGraphic();
  void    setup();

public:
  APIGraphic();
  APIGraphic(const APIGraphic&);
  APIGraphic& operator=(const APIGraphic&);
  iAPIGraphic* clone() const { return new APIGraphic(*this); }
  void setWorld(const void*);
  void setReflectivity(const void*);
  void beginDraw();
  void render(int, int, unsigned char);
  void draw() {}
  void endDraw();
  void suspend();
  void release();
  void Delete() const { delete this; }
};

//-------------------------------- APIVertexList ------------------------------
//
// The APIVertexList class implements the vertex list at the API level
//
struct IDirect3DVertexBuffer9;

template <class T = Vertex>
class APIVertexList : public APIGraphic {

  D3DPRIMITIVETYPE        type;        // primitive type
  IDirect3DVertexBuffer9* vb;          // points to the vertex buffer
  IDirect3DVertexDeclaration9* vDecl;  // vertex declaration 
  unsigned                nPrimitives; // number of primitives
  unsigned                maxNo;       // maximum number of vertices
  T*                      vertex;      // points to the array of vertices
  unsigned                nVertices;   // number of vertices

protected:
  virtual ~APIVertexList()                { release(); delete [] vertex; }
  void     setup();
  unsigned attach();

public:
  APIVertexList() : vertex(nullptr), vb(nullptr) {}
  APIVertexList(PrimitiveType, unsigned);
  APIVertexList& operator=(const APIVertexList&);
  APIVertexList(const APIVertexList& src) { vertex = nullptr; vb = nullptr;
  vDecl = nullptr; *this = src; } 
  APIVertexList* clone() const            { return new APIVertexList(*this); }
  AABB calcAABB();
  virtual unsigned add(const T& v);
  Vector  position(unsigned i) const      { return vertex[i].position(); }
  void    draw();
  void    suspend();
  void    release()                       { suspend(); }
  void    Delete() const                  { delete this; }
};

template <class T>
AABB APIVertexList<T>::calcAABB() {
  AABB aabb;
  for (unsigned i=0; i<nVertices; ++i) {
    Vector v = vertex[i].position();
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

// CreateVertexList creates an API Vertex List object
//
template <class T>
APIVertexList<T>* CreateAPIVertexList(PrimitiveType t, int np) {

  return new APIVertexList<T>(t, np);
}

// constructor allocates memory for the list
//
template <class T>
APIVertexList<T>::APIVertexList(PrimitiveType t, unsigned np) : 
nPrimitives(np), nVertices(0), vb(nullptr) {

  if (np <= 0) {
    maxNo  = 0;
    vertex = nullptr;
  }
  else {
    // Determine the number of vertices for the Primitive Type
    switch (t) {
    case POINT_LIST:     
      maxNo = np;     
      type = D3DPT_POINTLIST;     
      break;
    case LINE_LIST:      
      maxNo = 2 * np; 
      type = D3DPT_LINELIST;      
      break;
    case LINE_STRIP:     
      maxNo = np + 1; 
      type = D3DPT_LINESTRIP;     
      break;
    case TRIANGLE_LIST:  
      maxNo = 3 * np; 
      type = D3DPT_TRIANGLELIST;  
      break;
    case TRIANGLE_STRIP: 
      maxNo = np + 2; 
      type = D3DPT_TRIANGLESTRIP; 
      break;
    case TRIANGLE_FAN:   
      maxNo = np + 1; 
      type = D3DPT_TRIANGLEFAN;   
      break;
    default:             
      maxNo = np;     
      type = D3DPT_POINTLIST;
    }
    vertex = new T[maxNo];
  }
  vDecl = nullptr;
}

// assignment operator copies the vertex list
//
template <class T>
APIVertexList<T>& APIVertexList<T>::operator=(const APIVertexList<T>& src) {

  if (this != &src) {
    maxNo       = src.maxNo;
    nPrimitives = src.nPrimitives;
    nVertices   = src.nVertices;
    type        = src.type;
    (APIGraphic&)(*this) = src;
    if (vertex) {
      delete [] vertex;
      vertex = nullptr;
    }
    vertex = new T[nVertices];
    for (unsigned i = 0; i < nVertices; i++)
      vertex[i] = src.vertex[i];
    if (vb) {
      vb->Release();
      vb = nullptr;
    }
    if (vDecl) {
      vDecl->Release();
      vDecl = nullptr;
    }
  }

  return *this;
}

// add adds vertex v to the vertex list if there is room
//
template <class T>
unsigned APIVertexList<T>::add(const T& v) { 
  unsigned n = nVertices;

  if (nVertices < maxNo) 
    vertex[nVertices++] = v;

  return n;
}

// setup creates the vertex buffer and populates it
//
template <class T>
void APIVertexList<T>::setup() {

  unsigned vBufSize = APIVertexDeclaration<T>::size() * nVertices;

  if (FAILED(d3dd->CreateVertexDeclaration(APIVertexDeclaration<T>::format(),
    &vDecl)))
    error(L"APIVertexList::07 Unable to create vertex declaration");
  // create the vertex buffer
  else if (!nVertices) {
    error(L"APIVertexList::09 No vertices have been stored");
    vb = nullptr;
  }
  else if (FAILED(d3dd->CreateVertexBuffer(vBufSize, 0, 0, D3DPOOL_DEFAULT,
    &vb, nullptr))) {
      error(L"APIVertexList::11 Couldn\'t create the vertex buffer");
      vb = nullptr;
  }
  // populate the newly created vertex buffer
  else {
    void* pv;
    if (SUCCEEDED(vb->Lock(0, vBufSize, &pv, 0)))
      for (unsigned i = 0; i < nVertices; i++)
        vertex[i].populate(&pv);
    vb->Unlock();
  }
}

// attach attaches the stream of vertices to the pipeline and returns the
// number of vertices attached
//
template <class T>
unsigned APIVertexList<T>::attach() {

  if (!vb) setup();

  if (vb) {
    d3dd->SetVertexDeclaration(vDecl);
    d3dd->SetStreamSource(0, vb, 0, APIVertexDeclaration<T>::size());
  }

  return nVertices;
}

// draw attaches and draws the stream of vertices
//
template <class T>
void APIVertexList<T>::draw() {

  attach();

  if (vb)
    d3dd->DrawPrimitive(type, 0, nPrimitives);
}

// suspend releases the interface to the vertex buffer
//
template <class T>
void APIVertexList<T>::suspend() {

  if (vb) {
    vb->Release();
    vb = nullptr;
  }
  if (vDecl) {
    vDecl->Release();
    vDecl = nullptr;
  }
}

#endif
