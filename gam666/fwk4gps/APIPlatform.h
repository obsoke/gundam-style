#ifndef _API_PLATFORM_SETTINGS_H_
#define _API_PLATFORM_SETTINGS_H_

/* Header for Configuring the Translation Layer
*
* APIPlatformSettings.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* contributions by: Jon Buckley '11
* distributed under TPL - see ../Licenses.txt
*/

#include <d3d9.h>      // for basic D3D
#include <d3dx9.h>     // for D3D utilities
#include <d3dx9core.h> // for LPD3DXFONT
#include <d3dx9mesh.h> // for D3DXLoadMeshFromX
#include <xaudio2.h>   // for XAudio2
#include <X3DAudio.h>  // for X3D

#define D3D_NO_DOC_FORMATS 6
#define D3D_DOC_FORMATS { D3DFMT_X8R8G8B8, D3DFMT_A8R8G8B8,\
  D3DFMT_A2R10G10B10, D3DFMT_X1R5G5B5, D3DFMT_A1R5G5B5,\
  D3DFMT_R5G6B5 }
// short descriptions for each format. In practice, you
// just might want to call them either "16" or "32" (so
// the user is deciding between 16 and 32 bit colour)
#define D3D_FORMAT_DESC { L"X32", L"A32", L"A30+2", L"X15", L"A16", L"16" }

#endif
