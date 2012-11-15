/* Texture Implementation - Modelling Layer
*
* Texture.cpp
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include "Texture.h"      // for Texture class definition
#include "iCoordinator.h" // for the Coordinator Interface
#include "APITexture.h"   // for the APITexture Interface
#include "iUtilities.h"   // for error(), nameWithDir()
#include "Model.h"        // for TEXTURE_DIRECTORY and COLOR_KEY

//-------------------------------- Texture ------------------------------------
//
// The Texture class defines the structure for a texture in the Modelling Layer
//
// CreateTexture creates a Texture object
//
iTexture* CreateTexture(const wchar_t* file, unsigned key) {
  iTexture* tex;
  if (Texture::textureLibrary.find(file) != Texture::textureLibrary.end()) {
    tex = Texture::textureLibrary[file];
  } else {
    tex = new Texture(file, key);
    Texture::textureLibrary[file] = tex;
  }
  return tex;
}

iTexture* Clone(const iTexture* src) {

  return (iTexture*)src->clone();
}

// constructor adds a pointer to the texture to the coordinator, creates the full
// name of the texture file, stores the name and creates the texture's api
// representation
//
Texture::Texture(const wchar_t* file, unsigned key) {

  coordinator->add(this);

  wchar_t* fileWithPath = nullptr;
  if (file) {
    // add the directory to create the relative filename
    int len = strlen(file) + strlen(TEXTURE_DIRECTORY) + 1;
    fileWithPath = new wchar_t[len + 1];
    ::nameWithDir(fileWithPath, TEXTURE_DIRECTORY, file, len);
  }

  apiTexture = CreateAPITexture(fileWithPath, key ? key : COLOR_KEY);

  if (fileWithPath) delete [] fileWithPath;
}

// copy constructor initializes the instance pointers and calls the
// assignment operator
//
Texture::Texture(const Texture& src) {

  coordinator->add(this);

  apiTexture = nullptr;
  *this      = src;
}

// assignment operator discards the old data and copies new data
// from src - does not copy the APITexture from src but creates
// a new APITexture instead
//
Texture& Texture::operator=(const Texture& src) {

  if (this != &src) {
    if (apiTexture)
      apiTexture->Delete();
    if (src.apiTexture)
      apiTexture = src.apiTexture->clone();
    else
      apiTexture = nullptr;
  }

  return *this;
}

// setAnisotropy sets the degree of anisotropic filtering
//
void Texture::setAnisotropy(int d) {
  APITexture::setAnisotropy(d);
}

// setFilter sets the sampling filter on the pipeline
//
void Texture::setFilter(unsigned flags) const {
  if (apiTexture)
    apiTexture->setFilter(flags);
}

void Texture::setAddressing(unsigned flags) const {
  if (apiTexture)
    apiTexture->setAddressing(flags);
}

// attach attach the texture to the pipeline
//
void Texture::attach(int w, int h) {

  if (apiTexture)
    apiTexture->attach(w, h);
}

// detach detaches the texture from the pipeline
//
void Texture::detach() {

  if (apiTexture)
    apiTexture->detach();
}

// suspend suspends the apiTexture 
//
void Texture::suspend() {

  if (apiTexture)
    apiTexture->suspend();
}

// release releases the apiTexture 
//
void Texture::release() {

  if (apiTexture)
    apiTexture->release();
}

// destructor deletes the graphics device representation and removes the 
// pointer to the texture from the coordinator
//
Texture::~Texture() {

  apiTexture->Delete();
  coordinator->remove(this);
}

std::map<const wchar_t*, iTexture*> Texture::textureLibrary = 
  std::map<const wchar_t*, iTexture*>();

void Texture::disposeLibrary() {
  std::map<const wchar_t*, iTexture*>::iterator it;
  for (it = Texture::textureLibrary.begin(); it != Texture::textureLibrary.end(); ++it) {
    if (it->second) delete it->second;
    it->second = nullptr;
  }
}