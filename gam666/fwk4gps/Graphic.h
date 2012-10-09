#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

/* Graphic and Vertex Definitions - Modelling Layer
 *
 * Graphic.h
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iGraphic.h"   // for the Graphic Interface
#include "MathDecl.h"   // for Vector, Colour
#include "APIGraphic.h" // for APIGraphic class definition

//-------------------------------- Graphic ------------------------------------
//
// The Graphic class associates a graphic with the coordinator
//
class iAPIGraphic;

class Graphic : public iGraphic {

    int          width_;       // width of the enclosing rectangle
    int          height_;      // height of the enclosing rectangle
    float        radius;       // radius of bounding sphere
    iAPIGraphic* apiGraphic;   // points to the apiGraphic

  protected:   
    Graphic(const Graphic&);
    Graphic& operator=(const Graphic&);
    virtual ~Graphic();
  public:
    Graphic(int = 0, int = 0);
    void* clone() const { return new Graphic(*this); }
    void setWorld(const void*) {}
    void set(void*) {}
    void setRadius(float r) { radius = r; }
    float getRadius() const { return radius; }
    int  width() const  { return width_; }
    int  height() const { return height_; }
    void beginDraw();
    void render(int, int, unsigned char);
    void endDraw();
    void suspend();
    void restore() { }
    void release();
    void Delete() const { delete this; }
};

//-------------------------------- VertexList ---------------------------------
//
// The VertexList class defines the structure of a set of <T> vertices
//
template <class T = Vertex>
class VertexList : public Graphic {

    APIVertexList<T>* apiVertexList; // points to the API Primitive Set
    
  protected:
    virtual ~VertexList()                 { if (apiVertexList)
                                             apiVertexList->Delete(); }

  public:
    VertexList() : apiVertexList(nullptr) { }
    VertexList(PrimitiveType, int);
    VertexList(const VertexList& src)     { apiVertexList = nullptr; 
                                            *this = src; }
    VertexList& operator=(const VertexList&);
    void*   clone() const                 { return new VertexList(*this); }
    virtual unsigned add(const T& v);
    Vector  position(unsigned) const;
    void    setWorld(const void* w)       { apiVertexList->setWorld(w); }
    void    set(void* r)                  { apiVertexList->setReflectivity(r); }
    void render(int x, int y, unsigned char a)  { apiVertexList->draw(); }
    void    suspend()                     { apiVertexList->suspend(); }
    void    release()                     { apiVertexList->release(); }
    void    Delete() const                { delete this; }
};

// CreateVertexList creates a Vertex List object
//
template <class T>
iGraphic* CreateVertexList(PrimitiveType t, int np) {

    return new VertexList<T>(t, np);
}

// constructor allocates memory for the list and creates the Translation
//
template <class T>
VertexList<T>::VertexList(PrimitiveType t, int np) {

    apiVertexList = CreateAPIVertexList<T>(t, np);
}

// assignment operator copies the vertex list and clone the Translation
//
template <class T>
VertexList<T>& VertexList<T>::operator=(const VertexList<T>& src) {

    if (this != &src) {
        if (apiVertexList)
            apiVertexList->Delete();
        apiVertexList = src.apiVertexList->clone();
    }

    return *this;
}

// add adds vertex v at the API level and returns its index
//
template <class T>
unsigned VertexList<T>::add(const T& v) {

    return apiVertexList ? apiVertexList->add(v) : 0; 
}

// position returns the position of vertex i
//
template <class T>
Vector VertexList<T>::position(unsigned i) const { 
    
    return apiVertexList ? apiVertexList->position(i) : Vector(); 
}

#endif
