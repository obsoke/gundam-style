/* Object Implementation - Modelling Layer
 *
 * Object.cpp
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iCoordinator.h" // for the Coordinator Interface
#include "iGraphic.h"     // for the Graphic Interface
#include "iTexture.h"     // for the Texture Interface
#include "Object.h"       // for the Object class definition
#include "Model.h"        // for Category symbols
#include "Translation.h"  // for TEX_DEFAULT

//-------------------------------- Object -------------------------------------
//
// The Object class defines the structure of a single object in the model
//
iObject* CreateObject(iGraphic* v, unsigned char a) {

    return new Object(LIT_OBJECT, v, a);
}

iObject* CreateSprite(iGraphic* v, unsigned char a) {

    return new Object(SPRITE, v, a);
}

iObject* CreateObject(iGraphic* v, const Reflectivity* r)
{
    Category category = OPAQUE_OBJECT;
    if (r->translucent())
        category = TRANSLUCENT_OBJECT;
    return new Object(category, v, r);
}

// Clone create a clone of Object* src and returns its address
//
iObject* Clone(const iObject* src) {

    return (iObject*)src->clone();
}

// constructor initializes a lit object
//
Object::Object(Category c, iGraphic* v, unsigned char a) : 
 category(c), graphic(v), texture(nullptr), alpha(a ? a : TEX_ALPHA) {
    
    coordinator->add(this);
    reflectivity = Reflectivity();
}

// constructor initializes a reflective object
//
Object::Object(Category c, iGraphic* v, const Reflectivity* r) : 
 category(c), graphic(v), flags(TEX_DEFAULT) {
    
    coordinator->add(this);

    // store reflectivity and texture pointer
    reflectivity = *r;

    texture = nullptr;
}

// copy constructor initializes the instance pointer and calls the assignment
// operator
//
Object::Object(const Object& src) {

    coordinator->add(this);
    *this = src;
}
            
// width returns the width of the object
//
int Object::width() const { return graphic->width(); }

// height returns the width of the object
//
int Object::height() const { return graphic->height(); }

// radius returns the radius of the bounding sphere
//
float Object::radius() const { return graphic->getRadius(); }

// attach attaches texture *t to the object
//
void Object::attach(iTexture* t) {

    texture = t;
}

// render draws the object
//
void Object::render() { 
	
    if (graphic) {
        if (category == SPRITE) {
            Vector pos = position();
            graphic->beginDraw();
            texture->attach(graphic->width(), graphic->height());
            graphic->render((int)pos.x, (int)pos.y, alpha);
            texture->detach();
            graphic->endDraw();
        }
        else if (category == LIT_OBJECT) {
            graphic->setWorld(&world());
            graphic->render();
        }
        else {
            graphic->setWorld(&world());
            if (texture) {
                if (flags) texture->setFilter(flags);
                texture->attach();
            }
            graphic->set(&reflectivity);
            graphic->render();
            if (texture) texture->detach();
        }
    }
}

// destructor removes the object from the model coordinator
//
Object::~Object() {

    coordinator->remove(this);
}
