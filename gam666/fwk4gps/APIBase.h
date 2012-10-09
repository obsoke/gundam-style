#ifndef _API_BASE_H_
#define _API_BASE_H_

/* API Base Definition - Translation Layer
 *
 * APIBase.h
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "APIPlatform.h" // for the Pipeline setting

//-------------------------------- APIBase ------------------------------------
//
// The APIBase class manages the connections to the underlying APIs
//
class  iAPIDisplaySet;
class  iAPIInputDeviceSet;
class  iAPIDisplay;
class  iAPIWindow;
struct IDirect3D9;
struct IDirect3DDevice9;
struct IDirect3DTexture9;
struct ID3DXSprite;

class APIBase {

protected:

    static iAPIDisplaySet*         displaySet;     // the attached displays
    static iAPIInputDeviceSet*     keyboardSet;    // the attached keyboards
    static iAPIInputDeviceSet*     pointerSet;     // the attached pointers
    static iAPIInputDeviceSet*     controllerSet;  // the attached controllers

    static iAPIDisplay*            display;      // the graphics card object
    static iAPIWindow*             window;       // the application window

    static void*                   application;  // points to the application
    static void*                   hwnd;         // handle to application window

    static IDirect3D9*             d3d;          // points to Direct3D object
    static IDirect3DDevice9*       d3dd;         // points to Direct3D display
    static IDirect3DTexture9*      texture;      // points to the current texture
    static ID3DXSprite*            manager;      // sprite manager for text objects

    static int                     width;        // width of the client area
    static int                     height;       // height of the client area
    static bool                    runinwndw;    // running in a window?

    virtual ~APIBase()             { }

public:
    static void error(const wchar_t*, const wchar_t* = 0);
    static void logError(const wchar_t*);
};

#endif
