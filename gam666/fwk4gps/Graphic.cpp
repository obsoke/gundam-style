/* Graphic and Vertex Implementations - Modelling Layer
*
* Graphic.cpp
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include <fstream>
#include "Graphic.h"      // for Vertex and Graphic class definitions
#include "iCoordinator.h" // for the Coordinator Interface
#include "iAPIGraphic.h"  // for the APIGraphic Interface
#include "iUtilities.h"   // for error()
#include "APIVertex.h"    // for ...Vertex class definitions
#include "MathDef.h"      // for Vector
#include "MathDecl.h"     // for AABB
#include "Model.h"        // for ASSET_DIRECTORY
#include "Translation.h"

//-------------------------------- Graphic ---------------------------------
//
// The Graphic class is the base class of the Graphic hierarchy
//
iGraphic* CreateGraphic(int w, int h, Rect* source) {

  return new Graphic(w, h, source);
}

// constructor adds the Graphic to the coordinator
//
Graphic::Graphic(int w, int h, Rect* source) : width_(w), height_(h), 
    radius(sqrtf((float)(w * w + h * h))), sourceRect(source) {
  coordinator->add(this);
  apiGraphic = CreateAPIGraphic();
}

Graphic::Graphic(const Graphic& src) {

  coordinator->add(this);
  *this  = src;
}

Graphic& Graphic::operator=(const Graphic& src) {

  if (this != &src) {
    delete apiGraphic;
    if (src.apiGraphic)
      apiGraphic = src.apiGraphic->clone();
    width_  = src.width_;
    height_ = src.height_;
    radius  = src.radius;
  }

  return *this;
}

// beginDraw begins the drawing process
//
void Graphic::beginDraw() {

  if (apiGraphic) 
    apiGraphic->beginDraw();
}

// render draws the graphic
//
void Graphic::render(int x, int y, unsigned char a) {

  if (apiGraphic)
    apiGraphic->render(x, y, a, sourceRect);
}

// endDraw ends the drawing process
//
void Graphic::endDraw() {

  if (apiGraphic) 
    apiGraphic->endDraw();
}

// suspend suspends the graphic object 
//
void Graphic::suspend() {

  if (apiGraphic)
    apiGraphic->suspend();
}

// release releases the graphic object
//
void Graphic::release() {

  if (apiGraphic)
    apiGraphic->release();
}

// destructor removes the Graphic from the coordinator
//
Graphic::~Graphic() {

  delete apiGraphic;
  coordinator->remove(this);
}

//-------------------------------- Graphic Structures -------------------------
//
// prototypes for add() function used by the Create...() functions
void add(VertexList<Vertex>*, const Vector&, const Vector&, const Vector&,  
  const Vector&, const Vector&, float uScale = 1.0f, float vScale = 1.0f);
void add(VertexList<LitVertex>*, const Vector&, const Vector&, const Vector&,  
  const Vector&, const Colour&);

// CreateBox builds a triangle vertex list for a brick-like box from two
// extreme points one face at a time with all faces having the same attributes
//
iGraphic* CreateBox(float minx, float miny, float minz, float maxx, 
  float maxy, float maxz, float uScale, float vScale, bool flip) {

    VertexList<Vertex>* vertexList = 
      (VertexList<Vertex>*)CreateVertexList<Vertex>(TRIANGLE_LIST, 12);

    float x = (minx + maxx) / 2;
    float y = (miny + maxy) / 2;
    float z = (minz + maxz) / 2;
    minx -= x;
    miny -= y;
    minz -= z;
    maxx -= x;
    maxy -= y;
    maxz -= z;
    // bounding sphere
    float max;
    max = maxx > maxy ? maxx : maxy;
    max = maxz > max  ? maxz : max;
    vertexList->setRadius(1.73205f * max);
    // locate centroid at origin
    Vector p1 = Vector(minx, miny, minz),
      p2 = Vector(minx, maxy, minz),
      p3 = Vector(maxx, maxy, minz),
      p4 = Vector(maxx, miny, minz),
      p5 = Vector(minx, miny, maxz),
      p6 = Vector(minx, maxy, maxz),
      p7 = Vector(maxx, maxy, maxz),
      p8 = Vector(maxx, miny, maxz);
    if (flip) {
      add(vertexList, p4, p3, p2, p1, Vector(0, 0, 1), uScale, vScale); // front
      add(vertexList, p8, p7, p3, p4, Vector(-1, 0, 0), uScale, vScale); // right
      add(vertexList, p5, p6, p7, p8, Vector(0, 0, -1), uScale, vScale); // back
      add(vertexList, p5, p1, p2, p6, Vector(1, 0, 0), uScale, vScale); // left
      add(vertexList, p5, p8, p4, p1, Vector(0, 1, 0), uScale, vScale); // bottom
      add(vertexList, p3, p7, p6, p2, Vector(0, -1,  0), uScale, vScale); // top
    } else {
      add(vertexList, p1, p2, p3, p4, Vector(0, 0, -1), uScale, vScale); // front
      add(vertexList, p4, p3, p7, p8, Vector(1, 0,  0), uScale, vScale); // right
      add(vertexList, p8, p7, p6, p5, Vector(0, 0,  1), uScale, vScale); // back
      add(vertexList, p6, p2, p1, p5, Vector(-1, 0, 0), uScale, vScale); // left
      add(vertexList, p1, p4, p8, p5, Vector(0, -1, 0), uScale, vScale); // bottom
      add(vertexList, p2, p6, p7, p3, Vector(0, 1,  0), uScale, vScale); // top
    }
    vertexList->calcAABB();

    return vertexList;
}

iGraphic* CreateSkyboxPlane(float width, float height, float depth, int face) {
  VertexList<Vertex>* vertexList = 
    (VertexList<Vertex>*)CreateVertexList<Vertex>(TRIANGLE_LIST, 2);

  float x = width / 2, y = height / 2, z = depth / 2; 
  float minx = 0, miny = 0, minz = 0, maxx = width, maxy = height, maxz = depth;
  minx -= x;
  miny -= y;
  minz -= z;
  maxx -= x;
  maxy -= y;
  maxz -= z;
  // bounding sphere
  float max;
  max = maxx > maxy ? maxx : maxy;
  max = maxz > max  ? maxz : max;
  vertexList->setRadius(1.73205f * max);
  // locate centroid at origin
  Vector p1 = Vector(minx, miny, minz),
    p2 = Vector(minx, maxy, minz),
    p3 = Vector(maxx, maxy, minz),
    p4 = Vector(maxx, miny, minz),
    p5 = Vector(minx, miny, maxz),
    p6 = Vector(minx, maxy, maxz),
    p7 = Vector(maxx, maxy, maxz),
    p8 = Vector(maxx, miny, maxz);

  switch(face) {
  case 0: add(vertexList, p4, p3, p2, p1, Vector(0, 0, 1)); break; // front
  case 1: add(vertexList, p8, p7, p3, p4, Vector(-1, 0, 0)); break; // right
  case 2: add(vertexList, p5, p6, p7, p8, Vector(0, 0, -1)); break; // back
  case 3: add(vertexList, p5, p1, p2, p6, Vector(1, 0, 0)); break; // left
  case 4: add(vertexList, p5, p8, p4, p1, Vector(0, 1, 0)); break; // bottom
  case 5: add(vertexList, p3, p7, p6, p2, Vector(0, -1,  0)); break; // top
  }
  vertexList->calcAABB();
  return vertexList;
}

iGraphic* CreateBox(float minx, float miny, float minz, float maxx, 
  float maxy, float maxz, const Colour& colour) {

    VertexList<LitVertex>* vertexList = 
      (VertexList<LitVertex>*)CreateVertexList<LitVertex>(TRIANGLE_LIST, 12);

    float x = (minx + maxx) / 2;
    float y = (miny + maxy) / 2;
    float z = (minz + maxz) / 2;
    minx -= x;
    miny -= y;
    minz -= z;
    maxx -= x;
    maxy -= y;
    maxz -= z;
    // bounding sphere
    float max;
    max = maxx > maxy ? maxx : maxy;
    max = maxz > max  ? maxz : max;
    vertexList->setRadius(1.73205f * max);
    // locate centroid at origin
    Vector p1 = Vector(minx, miny, minz),
      p2 = Vector(minx, maxy, minz),
      p3 = Vector(maxx, maxy, minz),
      p4 = Vector(maxx, miny, minz),
      p5 = Vector(minx, miny, maxz),
      p6 = Vector(minx, maxy, maxz),
      p7 = Vector(maxx, maxy, maxz),
      p8 = Vector(maxx, miny, maxz);
    add(vertexList, p1, p2, p3, p4, colour); // front
    add(vertexList, p4, p3, p7, p8, colour); // right
    add(vertexList, p8, p7, p6, p5, colour); // back
    add(vertexList, p6, p2, p1, p5, colour); // left
    add(vertexList, p1, p4, p8, p5, colour); // bottom
    add(vertexList, p2, p6, p7, p3, colour); // top
    vertexList->calcAABB();

    return vertexList;
}

// CreateGrid builds a grid-like line list of n by n lines in the x-z plane
//
iGraphic* CreateGrid(float min, float max, int n) {

  VertexList<Vertex>* vertexList = 
    (VertexList<Vertex>*)CreateVertexList<Vertex>(LINE_LIST, 2*n+2);

  float x = (min + max) / 2;
  min -= x;
  max -= x;
  // bounding sphere
  vertexList->setRadius(1.73205f * max);
  float cur = min, inc = (max - min) / float(n - 1);
  for (int i = 0; i < n; i++, cur += inc) {
    // in the local x direction
    vertexList->add(Vertex(Vector(min, 0, cur), Vector(0, 1, 0)));
    vertexList->add(Vertex(Vector(max, 0, cur), Vector(0, 1, 0)));
    // in the local z direction
    vertexList->add(Vertex(Vector(cur, 0, min), Vector(0, 1, 0)));
    vertexList->add(Vertex(Vector(cur, 0, max), Vector(0, 1, 0)));
  }
  vertexList->calcAABB();

  return vertexList;
}

iGraphic* CreateGrid(float min, float max, int n, const Colour& colour) {

  VertexList<LitVertex>* vertexList = 
    (VertexList<LitVertex>*)CreateVertexList<LitVertex>(LINE_LIST, 2*n+2);

  float x = (min + max) / 2;
  min -= x;
  max -= x;
  vertexList->setRadius(0.707f * (max - min));
  float cur = min, inc = (max - min) / float(n - 1);
  for (int i = 0; i < n; i++, cur += inc) {
    // in the local x direction
    vertexList->add(LitVertex(Vector(min, 0, cur), colour));
    vertexList->add(LitVertex(Vector(max, 0, cur), colour));
    // in the local z direction
    vertexList->add(LitVertex(Vector(cur, 0, min), colour));
    vertexList->add(LitVertex(Vector(cur, 0, max), colour));
  }
  vertexList->calcAABB();

  return vertexList;
}

// CreateRectangleList builds a triangle list in the x-y plane from its two 
// extreme points
//
iGraphic* CreateRectangleList(float minx, float miny, float maxx, float maxy) {

  VertexList<Vertex>* vertexList = 
    (VertexList<Vertex>*)CreateVertexList<Vertex>(TRIANGLE_LIST, 2);

  float x = (minx + maxx) / 2, y = (miny + maxy) / 2;
  minx -= x;
  miny -= y;
  maxx -= x;
  maxy -= y;
  // bounding sphere
  float max;
  max = maxx > maxy ? maxx : maxy;
  vertexList->setRadius(1.73205f * max);
  // locate centroid at origin
  Vector p1 = Vector(minx, miny, 0),
    p2 = Vector(minx, maxy, 0),
    p3 = Vector(maxx, maxy, 0),
    p4 = Vector(maxx, miny, 0);
  add(vertexList, p1, p2, p3, p4, Vector(0, 0, -1)); 
  vertexList->calcAABB();

  return vertexList;
}

// TriangleList reads a vertex triangle list from file
//
iGraphic* TriangleList(const wchar_t* file) {

  iGraphic* graphic = nullptr;

  // construct filename with path
  int len = strlen(file) + strlen(ASSET_DIRECTORY) + 1;
  wchar_t* absFile = new wchar_t[len + 1];
  nameWithDir(absFile, ASSET_DIRECTORY, file, len);

  // open file for input
  std::wifstream in(absFile, std::ios::in);
  delete [] absFile;

  float x, y, z, nx, ny, nz, tu, tv, xc = 0, yc = 0, zc = 0;
  unsigned no = 0;

  // count the number of records
  while (in) { 
    in >> x >> y >> z >> nx >> ny >> nz >> tu >> tv;
    if (in.good()) {
      xc += x;
      yc += y;
      zc += z;
      no++;
    }
  }
  in.clear();
  in.seekg(0);
  if (no) {
    float max = 0;
    VertexList<Vertex>* vertexList = 
      (VertexList<Vertex>*)CreateVertexList<Vertex>(TRIANGLE_LIST, no / 3);
    xc /= no;
    yc /= no;
    zc /= no;
    for (unsigned i = 0; i < no; i++) {
      in >> x >> y >> z >> nx >> ny >> nz >> tu >> tv;
      vertexList->add(Vertex(Vector(x - xc, y - yc, (z - zc)), 
        Vector(nx, ny, nz)));
      if (x - xc > max) max = x - xc;
      if (y - yc > max) max = y - yc;
      if (z - zc > max) max = z - zc;
    }
    graphic = vertexList;
    // bounding sphere
    vertexList->setRadius(1.73205f * max);
    vertexList->calcAABB();
  }

  return graphic;
}

// TriangleList reads a litvertex triangle list from file and a specified colour
//
iGraphic* TriangleList(const wchar_t* file, const Colour& colour) {

  iGraphic* graphic = nullptr;

  // construct filename with path
  int len = strlen(file) + strlen(ASSET_DIRECTORY) + 1;
  wchar_t* absFile = new wchar_t[len + 1];
  ::nameWithDir(absFile, ASSET_DIRECTORY, file, len);

  // open file for input
  std::wifstream in(absFile, std::ios::in);
  delete [] absFile;

  float x, y, z, xc = 0, yc = 0, zc = 0;
  unsigned no = 0;

  // count the number of records
  while (in) { 
    in >> x >> y >> z;
    if (in.good()) {
      xc += x;
      yc += y;
      zc += z;
      no++;
    }
  }
  in.clear();
  in.seekg(0);
  if (no) {
    float max = 0;
    VertexList<LitVertex>* vertexList = 
      (VertexList<LitVertex>*)CreateVertexList<LitVertex>(TRIANGLE_LIST, no / 3);
    xc /= no;
    yc /= no;
    zc /= no;
    for (unsigned i = 0; i < no; i++) {
      in >> x >> y >> z;
      vertexList->add(LitVertex(Vector(x - xc, y - yc, (z - zc)), 
        colour));
      if (x - xc > max) max = x - xc;
      if (y - yc > max) max = y - yc;
      if (z - zc > max) max = z - zc;
      vertexList->setRadius(1.73205f * max);
    }
    graphic = vertexList;
    vertexList->calcAABB();
  }

  return graphic;
}

void add(VertexList<Vertex>* vertexList, const Vector& p1, const Vector& p2, 
  const Vector& p3, const Vector& p4, const Vector& n, float uScale, float vScale) {

    vertexList->add(Vertex(p1, n, 0, 1 * vScale));
    vertexList->add(Vertex(p2, n, 0, 0));
    vertexList->add(Vertex(p3, n, 1 * uScale, 0));
    vertexList->add(Vertex(p1, n, 0, 1 * vScale));
    vertexList->add(Vertex(p3, n, 1 * uScale, 0));
    vertexList->add(Vertex(p4, n, 1 * uScale, 1 * vScale));
}

void add(VertexList<LitVertex>* vertexList, const Vector& p1, const Vector& p2, 
  const Vector& p3, const Vector& p4, const Colour& colour) {

    vertexList->add(LitVertex(p1, colour));
    vertexList->add(LitVertex(p2, colour));
    vertexList->add(LitVertex(p3, colour));
    vertexList->add(LitVertex(p1, colour));
    vertexList->add(LitVertex(p3, colour));
    vertexList->add(LitVertex(p4, colour));
}

AABB Graphic::calcAABB() { return AABB(); };