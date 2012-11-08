#ifndef _API_VERTEX_H_
#define _API_VERTEX_H_

/* APIVertex Constants - Translation Layer
*
* APIVertex.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include "APIPlatform.h" // for the D3D constants
#include "APIGraphic.h"  // for Vertex, LitVertex definitions

//-------------------------------- PlainVertex --------------------------------
//
// The PlainVertex class defines the structure for a position-only vertex
//
class PlainVertex {

  float  x; // x coordinate in the local frame
  float  y; // y coordinate in the local frame
  float  z; // z coordinate in the local frame

public:
  PlainVertex();
  PlainVertex(const Vector&);
  void   populate(void**) const;
  Vector position() const;
};

//-------------------------------- LitVertex ----------------------------------
//
// The LitVertex class defines the structure for a single coloured vertex
//
class LitVertex {

  float  x; // x coordinate in the local frame
  float  y; // y coordinate in the local frame
  float  z; // z coordinate in the local frame
  Colour c; // colour

public:
  LitVertex();
  LitVertex(const Vector&, const Colour&, float = 0, float = 0);
  void   populate(void**) const;
  Vector position() const;
};

//-------------------------------- Vertex -------------------------------------
//
// The Vertex class defines the structure for a single vertex
//
class Vertex {

  float x;   // x coordinate in the local frame
  float y;   // y coordinate in the local frame
  float z;   // z coordinate in the local frame
  float nx;  // x coordinate of normal in the local frame
  float ny;  // y coordinate of normal in the local frame
  float nz;  // z coordinate of normal in the local frame
  float tu;  // u coordinate of texture
  float tv;  // v coordinate of texture

public:
  Vertex();
  Vertex(const Vector&, const Vector&, float = 0, float = 0);
  void   populate(void**) const;
  Vector position() const;
};

template <>
D3DVERTEXELEMENT9 APIVertexDeclaration<Vertex>::fmt[MAXD3DDECLLENGTH + 1] 
= {
  { 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
  { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
  { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},       
  D3DDECL_END()};

  template<>
  unsigned APIVertexDeclaration<Vertex>::vertexSize = 32;

  template <>
  D3DVERTEXELEMENT9 APIVertexDeclaration<LitVertex>::fmt[MAXD3DDECLLENGTH + 1] 
  = {
    { 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
    D3DDECL_END()};

    template <>
    unsigned APIVertexDeclaration<LitVertex>::vertexSize = 16;

    template <>
    D3DVERTEXELEMENT9 APIVertexDeclaration<PlainVertex>::fmt[MAXD3DDECLLENGTH + 1] 
    = {
      { 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
      D3DDECL_END()};

      template <>
      unsigned APIVertexDeclaration<PlainVertex>::vertexSize = 12;

#endif
