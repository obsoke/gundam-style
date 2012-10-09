/* APIVertex Implementation - Translation Layer
 *
 * APIVertex.cpp
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIVertex.h" // for Vertex static variables

//-------------------------------- Vertex -------------------------------------
//
// Vertex holds the data for a single normal vertex
//
// constructors initialize the vertex to the values received
//
Vertex::Vertex() : x(0), y(0), z(0), nx(0), ny(0), nz(0), tu(0), tv(0) {}

Vertex::Vertex(const Vector& p, const Vector& n, float ttu, float ttv) :
 x(p.x), y(p.y), z(p.z), nx(n.x), ny(n.y), nz(n.z), tu(ttu), 
 tv(ttv) {}

// populate populates *pv with the vertex data
//
void Vertex::populate(void** pv) const {
    float* p = *(float**)pv;

    *p++ = x;
    *p++ = y;
    *p++ = z;
    *p++ = nx;
    *p++ = ny;
    *p++ = nz;
    *p++ = tu;
    *p++ = tv;
    *pv  = p;
}

// position returns the position of the vertex in local coordinates
//
Vector Vertex::position() const {
    return Vector(x, y, z);
}

//-------------------------------- LitVertex ----------------------------------
//
// LitVertex holds the data for a single lit vertex
//
LitVertex::LitVertex() : x(0), y(0), z(0), c(0) {}

LitVertex::LitVertex(const Vector& p, const Colour& x, float ttu, 
 float ttv) : x(p.x), y(p.y), z(p.z), c(x) {}

// populate populates *pv with the vertex data
//
void  LitVertex::populate(void** pv) const {
    float* p = *(float**)pv;

    *p++ = x;
    *p++ = y;
    *p++ = z;
    *((unsigned*)p++) = COLOUR_TO_ARGB(c);
    *pv  = p;
}

// position returns the position of the vertex in local coordinates
//
Vector LitVertex::position() const {
    return Vector(x, y, z);
}

//-------------------------------- PlainVertex --------------------------------
//
// PlainVertex holds the data for a single position-only vertex
//
PlainVertex::PlainVertex() : x(0), y(0), z(0) {}

PlainVertex::PlainVertex(const Vector& p) : x(p.x), y(p.y), 
 z(p.z) {}

// populate populates *pv with the vertex data
//
void  PlainVertex::populate(void** pv) const {
    float* p = *(float**)pv;

    *p++ = x;
    *p++ = y;
    *p++ = z;
    *pv  = p;
}

// position returns the position of the vertex in local coordinates
//
Vector PlainVertex::position() const {
    return Vector(x, y, z);
}

