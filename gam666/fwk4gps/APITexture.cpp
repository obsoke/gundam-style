/* APITexture Implementation - Translation Layer
 *
 * APITexture.cpp
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIPlatform.h" // for API headers
#include "APITexture.h"  // for APITexture class definition
#include "Translation.h" // for TEX_MIN_POINT ...
#include "iUtilities.h"  // for strlen()

//-------------------------------- APITexture ---------------------------------
//
// The APITexture class implements a texture at the API level
//
iAPITexture* CreateAPITexture(const wchar_t* file, unsigned key) {

	return new APITexture(file, key);
}

// constructor initializes the texture identifier
//
APITexture::APITexture(const wchar_t* file, unsigned k) : key(k), addressing(1) {

	if (file) {
        int len = strlen(file);
	    this->file = new wchar_t[len + 1];
	    strcpy(this->file, file, len);
    }
    else
        this->file = nullptr;

    tex    = nullptr;
}

APITexture::APITexture(const APITexture& src) {

    file  = nullptr;
    tex   = nullptr;
    *this = src;    
}

iAPITexture& APITexture::operator=(const APITexture& src) {

    if (this != &src) {
        if (file)
            delete [] file;
        if (src.file) {
            int len = strlen(src.file);
            file    = new wchar_t[len + 1];
            strcpy(file, src.file, len);
        }
        else
            file = nullptr;
        key = src.key;
        suspend();
        tex = nullptr;
    }

    return *this;
}

// setAnisotropy sets the degree of anisotropic filtering
//
void APITexture::setAnisotropy(int d) {

    if (d3dd) {
        D3DCAPS9 caps;
        // maximum number of lights supported by the APIDisplay device
        d3dd->GetDeviceCaps(&caps);
        // set anisotropic filtering to the maximum available on the device
        if (FAILED(d3dd->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,
         caps.MaxAnisotropy * d / 10 - 1)))
            error(L"APITexture::17 Failed to set up anisotropic filtering");
    }
}

// setup creates the api texture from the texture file
//
void APITexture::setup(int w, int h) {

    // create a texture COM object from the texture file
    //
	if (file && FAILED(D3DXCreateTextureFromFileEx(d3dd, file, 
     w ? w : width, h ? h : height, D3DX_DEFAULT, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
     D3DX_DEFAULT, D3DX_DEFAULT, key, nullptr, nullptr, &tex))) {
		error(L"APITexture::11 Failed to create texture COM object from file");
		tex = nullptr;
	}
}

// attach attaches the texture to the drawing pipeline
//
void APITexture::attach(int w, int h) {

    if (!tex) setup(w, h);

    if (tex) {
        texture = tex;
        d3dd->SetTexture(0, tex);
        setSamplerState(filter);
        setAddressingState(addressing);
    }
}

// detach detaches the texture from the pipeline
//
void APITexture::detach() { 
    
    texture = nullptr; 
    d3dd->SetTexture(0, nullptr);
}

// suspend releases the api texture
//
void APITexture::suspend() {

    // release the Interface to the texture COM object
    if (tex) {
        tex->Release();
        tex = nullptr;
    }
}

// releases suspends the api texture
//
void APITexture::release() {

    suspend();
}

// destructor releases the api texture 
//
APITexture::~APITexture() {

   release();
}

// setSamplerState sets the sampling state for the texture according 
// to the filtering flags
//
void APITexture::setSamplerState(unsigned flags) const {

    if (flags & TEX_MIN_POINT)
        d3dd->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    else if (flags & TEX_MIN_LINEAR)
        d3dd->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    else if (flags & TEX_MIN_ANISOTROPIC)
        d3dd->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC); 
    if (flags & TEX_MAG_POINT)
        d3dd->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    else if (flags & TEX_MAG_LINEAR)
        d3dd->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    else if (flags & TEX_MAG_ANISOTROPIC)
        d3dd->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
}

void APITexture::setAddressingState(unsigned flags) {
  if (flags & D3DTADDRESS_CLAMP) {
    d3dd->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);   
    d3dd->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP); 
  } else if (flags & D3DTADDRESS_WRAP) {
    d3dd->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);   
    d3dd->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
  }
}
