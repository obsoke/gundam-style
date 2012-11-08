/* APIDisplay Implementation - Translation Layer
*
* APIDisplay.cpp
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include "APIPlatform.h" // for API headers
#include "APIDisplay.h"  // for the APIDisplay class definition
#include "APILight.h"    // for APILight::alloc(), APILight::dealloc()
#include "Model.h"       // for Texture_WIDTH, TEXTURE_HEIGHT
#include "MathDef.h"     // for Matrix * Matrix
#include "iUtilities.h"  // for sprintf() ...
#include "Translation.h" // 

// Maximum number of attached ...
//
#define MAX_ADAPTERS     10
#define MAX_MODES       100
#define MAX_P_FORMATS    20

//-------------------------------- APIDisplaySet ------------------------------
//
// The APIDisplaySet object holds the display information for all Displays
//
// CreateAPIDisplay creates the APIDisplaySet object
//
iAPIDisplaySet* CreateAPIDisplaySet() {

  return new APIDisplaySet();
}

// constructor initializes the instance pointers and retrieves an interface
// to the Direct3D device
//
APIDisplaySet::APIDisplaySet() {

  // obtain Interface to Direct3D COM object
  d3d = Direct3DCreate9(D3D_SDK_VERSION);
  if (!d3d)
    error(L"APIDisplaySet::01 Failed to make Direct3D object");
  modeDim = nullptr;
  modeDes = nullptr;
  adptDes = nullptr;
}

// interrogate interrogates the host system for the available configurations
//
bool APIDisplaySet::interrogate() {

  bool     rc = false;
  wchar_t  str[MAX_DESC + 1];

  D3DADAPTER_IDENTIFIER9 d3di;
  D3DDISPLAYMODE         mode;
  D3DFORMAT              Format[]  = D3D_DOC_FORMATS;
  // friendly descriptions for each format
  wchar_t*               fmtdesc[] = D3D_FORMAT_DESC;

  // adapter count
  nAdapters = d3d->GetAdapterCount();
  // maximum mode count
  nModes = 0;
  // pixel format count
  // number of pixel formats as described in the Direct3D documentation
  nPixelFmts = D3D_NO_DOC_FORMATS;

  // determine maximum number of modes amongst all adapters
  for (int id = 0; id < nAdapters; id++) {
    if (SUCCEEDED(d3d->GetAdapterIdentifier(id, 0, &d3di))) {
      for (int ip = 0; ip < nPixelFmts; ip++) {
        int i = d3d->GetAdapterModeCount(id, Format[ip]);
        if (i > nModes)
          nModes = i;
      }
    }
  }
  if (nAdapters > MAX_ADAPTERS) {
    sprintf(str, nAdapters, L" Adapters found - increase MAX_ADAPTERS");
    error(str);
    nAdapters = MAX_ADAPTERS;
  }
  if (nModes > MAX_MODES) {
    sprintf(str, nModes, L" Modes found - increase MAX_MODES");
    error(str);
    nModes = MAX_MODES;
  }
  if (nPixelFmts > MAX_P_FORMATS) {
    sprintf(str, nPixelFmts, 
      L" Pixel Formats found - increase MAX_P_FORMATS");
    error(str);
    nPixelFmts = MAX_P_FORMATS;
  }

  // set dimensions, allocate memory for descriptions,
  // and allocate private memory for mode dimensions
  if (modeDim) delete [] modeDim;
  if (modeDes) delete [] modeDes;
  if (adptDes) delete [] adptDes;
  modeDim = new int[nAdapters * nModes * nPixelFmts][2];
  modeDes = new wchar_t[nAdapters * nModes * nPixelFmts][MAX_DESC + 1];
  adptDes = new wchar_t[nAdapters][MAX_DESC + 1];

  // enumerate and set all descriptions
  for (int id = 0; id < nAdapters; id++) {
    if (SUCCEEDED(d3d->GetAdapterIdentifier(id, 0, &d3di))) {
      rc = false;
      for (int ip = 0; ip < nPixelFmts; ip++) {
        // mode count
        int nr = d3d->GetAdapterModeCount(id, Format[ip]);
        nr = (nr > nModes) ? nModes : nr;
        for (int ir = 0; ir < nr; ir++) {
          int i = (id * nModes + ir) * nPixelFmts + ip;
          if (SUCCEEDED(d3d->EnumAdapterModes(id, Format[ip], ir, 
            &mode))
            && mode.Width >= WND_WIDTH && mode.Height >= WND_HEIGHT &&
            (D3D_OK == d3d->CheckDeviceFormat(id, D3DDEVTYPE_HAL, 
            mode.Format,
            D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE,  D3DFMT_D32) ||   
            D3D_OK == d3d->CheckDeviceFormat(id, D3DDEVTYPE_HAL, 
            mode.Format,
            D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16) ||
            D3D_OK == d3d->CheckDeviceFormat(id, D3DDEVTYPE_REF, 
            mode.Format,
            D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32) ||
            D3D_OK == d3d->CheckDeviceFormat(id, D3DDEVTYPE_REF, 
            mode.Format,
            D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16))) {
              wchar_t hz[20] = L"";
              if (mode.RefreshRate)
                wsprintf(hz, L"(%d Hz)", mode.RefreshRate);
              wsprintf(str, L"%dx%d %ls %ls bits", mode.Width, 
                mode.Height, hz, fmtdesc[ip]);
              rc = true;
              // store the description and dimensions
              modeDim[i][0] = mode.Width;
              modeDim[i][1] = mode.Height;
              strcpy(modeDes[i], str, MAX_DESC);
          }
          else {
            // store empty description and dimensions
            modeDim[i][0] = 0;
            modeDim[i][1] = 0;
            modeDes[i][0] = L'\0';
          }
        }
      }
      // adapter description
      if (rc)
        strcpyFromMB(adptDes[id], d3di.Description, MAX_DESC); 
      else
        adptDes[id][0] = L'\0';
    }
  }

  return rc;
}

// getModeDesc returns the description for display id, mode ir, format ip
//
const wchar_t* APIDisplaySet::modeDesc(int id, int ir, int ip) const {

  return modeDes[ (id * nModes + ir) * nPixelFmts + ip ];
}

// getWidth returns the width of display id, mode ir, format ip
//
int APIDisplaySet::getWidth(int id, int ir, int ip) const {

  return modeDim[ (id * nModes + ir) * nPixelFmts + ip ][0];
}

// getWidth returns the width of display id, mode ir, format ip
//
int APIDisplaySet::getHeight(int id, int ir, int ip) const {

  return modeDim[ (id * nModes + ir) * nPixelFmts + ip ][1];
}

// destructor releases the Interface to the Direct3D object and
// deallocates memory
//
APIDisplaySet::~APIDisplaySet() {

  if (d3d) {
    d3d->Release();
    d3d = nullptr;
  }

  // mode dimensions memory
  if (modeDim) {
    delete [] modeDim;
    modeDim = nullptr;
  }
  if (modeDes) {
    delete [] modeDes;
    modeDes = nullptr;
  }
  if (adptDes) {
    delete [] adptDes;
    adptDes = nullptr;
  }
}

//-------------------------------- APIDisplay ---------------------------------
//
// The APIDisplay object manages the API Graphics connectivity
//
// CreateAPIDisplay creates the APIDisplay object
//
iAPIDisplay* CreateAPIDisplay() {

  return new APIDisplay();
}

// constructor initializes the instance variables
//
APIDisplay::APIDisplay()  { 

  display = this;

  d3dd    = nullptr;
  manager = nullptr;
  width   = WND_WIDTH;
  height  = WND_HEIGHT;
}

// configure sets the display, mode, and pixel format parameters
//
void APIDisplay::configure(int a, int m, int p) {

  displayId = a;
  mode      = m;
  pixel     = p;
}

// setup creates the display device according to the user's selection
// and associates the device with the application window (HWND)hwnd
//
bool APIDisplay::setup() {

  bool rc = false;

  // set the D3D presentation parameters
  UINT adapter;
  D3DFORMAT d3dFormat;
  ZeroMemory(&d3dpp, sizeof d3dpp);
  d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.BackBufferCount = 1;
  d3dpp.EnableAutoDepthStencil = TRUE;
  D3DDISPLAYMODE d3ddm;
  if (!runinwndw) {
    D3DFORMAT Format[] = D3D_DOC_FORMATS;
    d3dFormat = Format[pixel];
    if (FAILED(d3d->EnumAdapterModes(displayId, d3dFormat, mode, &d3ddm))) 
    {
      error(L"APIDisplay::10 Failed to get selected display mode");
      error(L"APIDisplay::11 Defaulting to windowed mode");
      runinwndw = true;
    }
    else {
      adapter   = displayId;
      width     = d3ddm.Width;
      height    = d3ddm.Height;
      d3dFormat = d3ddm.Format;
      d3dpp.FullScreen_RefreshRateInHz = d3ddm.RefreshRate;
    }
  }
  if (runinwndw) {
    adapter = D3DADAPTER_DEFAULT;
    d3d->GetAdapterDisplayMode(adapter, &d3ddm);
    d3dpp.Windowed = TRUE;
    d3dFormat = d3ddm.Format;
  }
  d3dpp.BackBufferWidth  = width;
  d3dpp.BackBufferHeight = height;
  d3dpp.BackBufferFormat = d3dFormat;

  // find the best format for depth buffering and stenciling
  //
  D3DDEVTYPE devtype;
  if (D3D_OK == d3d->CheckDeviceFormat(adapter, D3DDEVTYPE_HAL,
    d3dFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32)) {
      d3dpp.AutoDepthStencilFormat = D3DFMT_D32; // depth buffer
      devtype = D3DDEVTYPE_HAL;                  // HAL device   
  }
  else if (D3D_OK == d3d->CheckDeviceFormat(adapter, D3DDEVTYPE_HAL, 
    d3dFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16)) {
      d3dpp.AutoDepthStencilFormat = D3DFMT_D16;  // depth buffer
      devtype = D3DDEVTYPE_HAL;                   // HAL Device
  }
  // if the above attempts fail, use the REF (software emulation) device
  // with a 32-bit depth buffer rather than the HAL (hardware accelerated) 
  // device
  else if (D3D_OK == d3d->CheckDeviceFormat(adapter, D3DDEVTYPE_REF,
    d3dFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32)) {
      d3dpp.AutoDepthStencilFormat = D3DFMT_D32;   // depth buffer
      devtype = D3DDEVTYPE_REF;                    // REF Device
  }
  // if all else fails, use the REF (software emulation) with a 16-bit
  // depth buffer, hoping that it will work. (If it doesn't, we are out
  // of luck anyway.)
  else {
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;   // depth buffer
    devtype = D3DDEVTYPE_REF;                    // REF Device
  }
  DWORD total;
  if(SUCCEEDED(d3d->CheckDeviceMultiSampleType(
    D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format, true, 
    D3DMULTISAMPLE_NONMASKABLE, &total))) {
      d3dpp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
      d3dpp.MultiSampleQuality = total - 1;
  }

  // extract the device capabilities and configure the limits
  D3DCAPS9 caps;
  d3d->GetDeviceCaps(adapter, devtype, &caps);
  hasWBuffering = (caps.RasterCaps & D3DPRASTERCAPS_WBUFFER) != 0L;

  // hardware or software vertex processing?
  DWORD behaviorFlags;
  if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0)
    behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
  else
    behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

  // retrieve the Interface to the D3D APIDisplay device
  if (d3dd)
    error(L"APIDisplay::11 Pointer to Direct3D interface is not nullptr");
  else if (FAILED(d3d->CreateDevice(adapter, devtype, (HWND)hwnd, 
    behaviorFlags, &d3dpp, &d3dd)))
    error(L"APIDisplay::12 Failed to create Direct3D device");
  else {
    // maximum number of lights supported by the APIDisplay device
    d3dd->GetDeviceCaps(&caps);
    // set anisotropic filtering to the maximum available on the device
    if (FAILED(d3dd->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,
      caps.MaxAnisotropy - 1)))
      error(L"APIDisplay::17 Failed to set up anisotropic filtering");

    // create a sprite COM object to manage the drawing of the hud texture
    // and the drawing of the text item fonts
    if (!manager && FAILED(D3DXCreateSprite(d3dd, &manager)))
      error(L"APIDisplay::18 Failed to create the sprite manager");

    // setup successful
    rc = true;
  }

  // complete the setup
  if (rc) {
    if (total) d3dd->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
    setupBlending();
  }
  return rc;
}

// setProjection sets the projection parameters
//
void APIDisplay::setProjection(void* projection) {

  if (d3dd)
    d3dd->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)projection);
}

// setupBlending sets up colour dithering and sets the formula for
// alpha blending
//
void APIDisplay::setupBlending() {

  if (d3dd) {
    // how alpha-blending is done (when drawing transparent things)
    d3dd->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    d3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  }
}

// beginDrawFrame applies the view transformation, sets the ambient lighting,
// and clears the backbuffer
//
void APIDisplay::beginDrawFrame(const void* view) {

  // set the view transformation
  //
  if (d3dd && view) d3dd->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)view);

  // clear the backbuffer
  //
  if (d3dd) {
    d3dd->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
      D3DCOLOR_XRGB(BGROUND_R, BGROUND_G, BGROUND_B), 1.0, 0);
    d3dd->BeginScene();
  }
}

// set turns on off the specified render state
//
void APIDisplay::set(RenderState state, bool b) {

  if (d3dd) {
    switch (state) {
    case ALPHA_BLEND:
      d3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, b);
      break;
    case LIGHTING:
      d3dd->SetRenderState(D3DRS_LIGHTING, b);
      break;
    case WIRE_FRAME:
      d3dd->SetRenderState(D3DRS_FILLMODE, b ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
    case Z_BUFFERING:
      d3dd->SetRenderState(D3DRS_ZENABLE, b ? D3DZB_TRUE : D3DZB_FALSE);
      break;
    case W_BUFFERING:
      if (hasWBuffering) d3dd->SetRenderState(D3DRS_ZENABLE, b ? D3DZB_USEW : D3DZB_FALSE);
      break;
    case DITHERING:
      d3dd->SetRenderState(D3DRS_DITHERENABLE, b);
      break;
    case SPECULARITY:
      d3dd->SetRenderState(D3DRS_SPECULARENABLE, b);
      break;
    }
  }
}

// beginDraw begins the drawing of all text items
//
void APIDisplay::beginDraw(unsigned flags) {

  DWORD APIDisplayFlags = 0;

  if (flags & HUD_ALPHA) 
    APIDisplayFlags |= D3DXSPRITE_ALPHABLEND;

  if (d3dd && manager)
    manager->Begin(APIDisplayFlags);
}

// endDraw ends the drawing of all text items
//
void APIDisplay::endDraw() {

  if (d3dd && manager)
    manager->End();
}

// endDrawFrame presents the backbuffer to the primary buffer
//
void APIDisplay::endDrawFrame() {

  // present the backbuffer to the primary buffer
  //
  if (d3dd) {
    d3dd->EndScene();
    if (FAILED(d3dd->Present(nullptr, nullptr, nullptr, nullptr)))
      error(L"APIDisplay::40 Failed to flip backbuffer");
  }
}

// suspend prepares the APIDisplay device for de-activation
//
void APIDisplay::suspend() {

  // inform the sprite manager that its device is lost
  if (d3dd && manager)
    manager->OnLostDevice();
}

// restore re-activates the APIDisplay device
//
bool APIDisplay::restore() {

  bool rc = false;

  if (d3dd) {
    HRESULT hr = d3dd->TestCooperativeLevel();
    if (hr == S_OK)
      rc = true;
    else {
      while (hr != D3DERR_DEVICENOTRESET) {
        hr = d3dd->TestCooperativeLevel();
      }
      // reset the APIDisplay device
      hr = d3dd->Reset(&d3dpp);
      rc = SUCCEEDED(hr);
    }
  }
  if (rc) {
    // reacquire sprite manager references to video memory
    if (manager) 
      manager->OnResetDevice();
  }

  // complete the restoration

  if (rc)
    setupBlending();

  return rc;
}

// release releases the interfaces to the APIDisplay device
//
void APIDisplay::release() {

  suspend();

  // release the font manager
  if (manager) {
    manager->Release();
    manager = nullptr;
  }

  // release the APIDisplay device
  if (d3dd) {
    d3dd->Release();
    d3dd = nullptr;
  }
}

void APIDisplay::setViewport(const Viewport& viewport) {
  if (d3dd) {
    d3dd->SetViewport(&viewport.toDX());
  }
}

// destructor releases the APIDisplay object along with the Interface to the
// reference object
//
APIDisplay::~APIDisplay() {

  release();
}


