/* APIGraphic Implementation - Translation Layer
*
* APIGraphic.cpp
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include "APIGraphic.h" // for the APIGraphic class definition
#include "MathDecl.h"

// note - all other code is currently template code - see APIGraphic.h

//-------------------------------- APIGraphic ---------------------------------
//
// CreateAPIGraphic creates a graphic representation at the Translation Layer
//
iAPIGraphic* CreateAPIGraphic() {

  return new APIGraphic();
}

// the constructors initialize the instance variables
//
APIGraphic::APIGraphic() {

  sprite = nullptr;
  ready  = false;
}

APIGraphic::APIGraphic(const APIGraphic& src) {

  sprite = nullptr;
  *this  = src;
}

// assignment operator reinitializes the instance variables

APIGraphic& APIGraphic::operator=(const APIGraphic& src) {

  if (this != &src) {
    if (sprite)
      sprite->Release();
    sprite = nullptr;
    ready  = false;
  }

  return *this;
}

// setWorld sets the world transformation
//
void APIGraphic::setWorld(const void* world) {

  if (d3dd) 
    d3dd->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)world);
}

// setReflectivity sets the material reflectivity
//
void APIGraphic::setReflectivity(const void* reflectivity) {

  if (d3dd) {
    Reflectivity r;
    if (reflectivity)
      r = *((const Reflectivity*)reflectivity);
    else
      r = Reflectivity(Colour(1, 1, 1));
    D3DMATERIAL9 m;
    ZeroMemory(&m, sizeof(m));
    m.Ambient  = 
      D3DXCOLOR(r.ambient.r, r.ambient.g, r.ambient.b, r.ambient.a);
    m.Diffuse  = 
      D3DXCOLOR(r.diffuse.r, r.diffuse.g, r.diffuse.b, r.diffuse.a);
    m.Specular = 
      D3DXCOLOR(r.specular.r, r.specular.g, r.specular.b, r.specular.a);
    m.Power    = r.power; 
    d3dd->SetMaterial(&m);
  }
}

// setup sets up the api graphic object
//
void APIGraphic::setup() {

  if (!sprite) {
    // retrieves an interface to the sprite COM object
    if (FAILED(D3DXCreateSprite(d3dd, &sprite)))
      error(L"APIGraphic::10 Failed to create sprite COM object");
    else
      ready = true;
  }
  else {
    // notifies the object that the display device has been reset
    sprite->OnResetDevice();
    ready = true;
  }
}

// beginDraw begins the drawing by the api graphic object
//
void APIGraphic::beginDraw() {

  if (!ready) setup();

  if (d3dd && sprite)
    sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

// render draws the graphic
//
void APIGraphic::render(int x, int y, unsigned char a, Rect* source) {
  if (!ready) setup();

  if (d3dd && sprite) {
    D3DXVECTOR3 topLeft((float)x, (float)y, 1.f);
    sprite->Draw(texture, source ? &createRect(source) : nullptr, nullptr,
      &topLeft, D3DCOLOR_RGBA(255, 255, 255, a ? a : '\xFF'));
  }
}

RECT APIGraphic::createRect(Rect* source) {
  RECT r;
  r.left = source->x;
  r.top = source->y;
  r.right = source->x + source->width;
  r.bottom = source->y + source->height;
  return r;
}

// endDraw ends the drawing by the api graphic object
//
void APIGraphic::endDraw() {

  if (sprite) {
    sprite->End();
  }
}

// suspend suspends the api graphic object 
//
void APIGraphic::suspend() {

  // notify the sprite COM object that it has lost the display device
  if (sprite)
    sprite->OnLostDevice();
  ready = false;
}

// release releases the api graphic object
//
void APIGraphic::release() {

  suspend();

  // release the interface to the sprite COM object
  if (sprite) {
    sprite->Release();
    sprite = nullptr;
  }
}

// destructor destroys the api graphic object
//
APIGraphic::~APIGraphic() {

  release();
}
