#ifndef _TRANSLATION_H_
#define _TRANSLATION_H_

/* Internal Settings local to the Translation Layer
 *
 * Translation.h
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#define WND_CAPTION      L"Gundam Style"

#define MAX_DESC 255     // max string length throughout

// Default Window Dimensions
#define WND_WIDTH  800   // minimum window width
#define WND_HEIGHT 600   // minimum window height

// shared with dialog resource (follows windows conventions)
//
#define IDD_DLG 101
#define IDC_DIS 102
#define IDC_RES 103
#define IDC_GO  104
#define IDC_KEY 105
#define IDC_ACT 106
#define IDC_CNT 107
#define IDC_TRG 108
#define IDC_YRV 109
#define IDC_ZON 110
#define IDC_AUD 111
#define IDC_SFL 112
#define IDC_PTO 113
#define IDC_CTO 114

// DO NOT REMOVE WITHOUT WRITTEN PERMISSION
#define COPYRIGHT_LINE_1 "Gundam Style is Copyright (c) 2012, Rocket Jump and Dale. Source code is distributed under the"
#define COPYRIGHT_LINE_2 "PostgreSQL Open Source License (TPL).  For the full text, see ../Licenses.txt"
#define COPYRIGHT_LINE_3 "Audio clips are redistributed under Creative Commons Sampling Plus 1.0 licenses.  "
#define COPYRIGHT_LINE_4 "For full text, see ../Licenses.txt"

// backbuffer background colour
//
#define BGROUND_R 200
#define BGROUND_G 200
#define BGROUND_B 200

// maximum number of attached input devices
#define MAX_INPUT_DEVICES 32

// Input Device Types
//
typedef enum UserDeviceType {
    KEYBOARD,
    POINTER,
    CONTROLLER, 
} UserDeviceType;

// key symbols used for action-key mapping (do not initialize these constants)
//
typedef enum Key { 
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_SPACE,
    KEY_ENTER,
    KEY_UP   ,
    KEY_DOWN, 
    KEY_PGUP, 
    KEY_PGDN, 
    KEY_LEFT, 
    KEY_RIGHT,
    KEY_NUM1,
    KEY_NUM2,
    KEY_NUM3,
    KEY_NUM4,
    KEY_NUM5,
    KEY_NUM6,
    KEY_NUM7,
    KEY_NUM8,
    KEY_NUM9,
    KEY_ESCAPE,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_O_BRACKET,
	KEY_C_BRACKET,
	KEY_BACKSLASH,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_TIMES,
	KEY_GRAVE,
	KEY_MINUS,
	KEY_UNDERSCORE,
	KEY_EQUALS,
	KEY_PLUS
} Key;

// keys listed in the user dialog
//
#define KEY_DESCRIPTIONS { \
	L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", \
	L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R", L"S", L"T", \
	L"U", L"V", L"W", L"X", L"Y", L"Z", \
	L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0", \
	L"F1", L"F2", L"F3", L"F4", L"F5", L"F6", L"F7", \
	L"F8", L"F9", L"F10", L"F11", L"F12", \
	L"Space", L"Enter", L"Up", L"Down", L"PageUp", L"PageDown", \
	L"Left", L"Right", \
	L"NumPad 1", L"NumPad 2", L"NumPad 3", L"NumPad 4", L"NumPad 5", \
	L"NumPad 6", L"NumPad 7", L"NumPad 8", L"NumPad 9", \
	L"Escape", L";", L"'", L"[", L"]", L"\\", L",", \
	L".", L"/", L"*", L"`", L"-", L"_", L"=", L"+" \
}

// pointer buttons listed in the user dialog
//
#define POINTER_OBJECT_DESCRIPTIONS { \
    L"Left Mouse", L"Center Mouse", L"Right Mouse", L"E-W", L"N-S", L"Pitch", \
}

// controller buttons listed in the user dialog
//
#define CONTROLLER_OBJECT_DESCRIPTIONS { \
    L"Trigger", L"Button 1", L"Button 2", L"Button 3", L"Button 4", \
    L"Button 5", L"Button 6", L"Button 7", L"Button 8", L"Button 9", \
    L"Button 10", L"North", L"East", L"West",  L"South" \
}

//-------------------------------- Action Enumerations -------------------------
//
// to add an action
// - add the enumeration constant for the new action
// - add the friendly description of the new action
// - add the default mappings for the new action in Mappings.h
//
// enumeration constants
typedef enum Action {
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_BACKWARD,
    MOVE_FORWARD,
    ROLL_SQUARE,
    PITCH_BOX,
    ROLL_LEFT_BOX,
    SPIN_RIGHT_BOX,
    MDL_DET_CHILD,
    MDL_ATT_CHILD,
	CAM_PITCH_UP,
	CAM_PITCH_DOWN,
	CAM_YAW_LEFT,
	CAM_YAW_RIGHT,
	CAM_ADVANCE,
	CAM_RETREAT,
    CAM_ROLL_LEFT,
    CAM_ROLL_RIGHT,
    CAM_PAN_LEFT,
    CAM_PAN_RIGHT,
    CAM_PAN_LEFT_ALT,
    CAM_PAN_RIGHT_ALT,
    CAM_FLY_UP,
    CAM_FLY_DOWN,
    CAMERA_SELECT,
    WIRE_FRAME_SELECT,
    Z_BUFFERING_SELECT,
    W_BUFFERING_SELECT,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_DISTANT,
    TEXTURE_POINT,
    TEXTURE_LINEAR,
	HUD_SELECT,
	HUD_LEFT,
	HUD_RIGHT,
	HUD_UP,
	HUD_DOWN,
    MDL_NEW_OBJECT,
    MDL_NEW_SOUND,
    MDL_NEW_LIGHT,
    LIGHT_CLONE,
	AUD_BKGRD,
	AUD_IMPLS,
	AUD_VOLUME_INC,
	AUD_VOLUME_DEC,
	AUD_FREQ_INC,
	AUD_FREQ_DEC,
    AUD_LOCALL,
    AUD_LOCALR,
    AUD_OBJECT,
    CAM_SMOOTH_YAW,
    CAM_SMOOTH_PITCH,
    CAM_SMOOTH_F_B,
    GF_CT_POSX,
    GF_CT_POSY,
    GF_CT_DSPZ,
    GF_CT_ROTZ,
    THRUSTER,
	FIRE_WEAPON_1,
	FIRE_WEAPON_2,
	FIRE_WEAPON_3
} Action;

// user friendly descriptions of actions - used in the dialog box
// (descriptions should not exceed MAX_DESC characters)
//
#define ACTION_DESCRIPTIONS {\
    L"Sprite X +", \
    L"Sprite X -", \
    L"Sprite Y +", \
    L"Sprite Y -", \
	L"Roll Square",   \
	L"Pitch Box",\
    L"Roll Left Box", \
    L"Spin Right Box", \
    L"Detach Child Box", \
    L"Attach Child Box", \
	L"Pitch Camera Up",     \
	L"Pitch Camera Down",   \
	L"Yaw Camera Left",     \
	L"Yaw Camera Right",    \
	L"Move Camera Forward",     \
	L"Move Camera Backward",    \
    L"Roll Camera Left", \
    L"Roll Camera Right", \
    L"Pan Camera Left", \
    L"Pan Camera Right", \
    L"Pan Camera Left Alt", \
    L"Pan Camera Right Alt", \
    L"Fly Camera Up", \
    L"Fly Camera Down", \
    L"Select Camera", \
    L"Toggle Wire Frame Mode", \
    L"Toggle Z-Buffering Mode", \
    L"Toggle W-Buffering Mode", \
	L"Toggle Point Light",   \
	L"Toggle Spot Light",    \
	L"Toggle Distant Light", \
    L"Nearest Neighbour Filtering", \
    L"Bilinear Filtering", \
	L"Toggle Heads Up Display",      \
	L"Heads Up Display - Move Left", \
	L"Heads Up Display - Move Right",\
	L"Heads Up Display - Move Up",   \
	L"Heads Up Display - Move Down",  \
    L"Clone Right Box", \
    L"Clone Left Sound", \
    L"Clone SpotLight", \
	L"Toggle Background Sound", \
	L"Explosion Sound", \
	L"Increase Volume", \
	L"Decrease Volume", \
	L"Speed Up Sound",  \
	L"Slow Down Sound", \
	L"Toggle Local Left Sound", \
	L"Toggle Local Right Sound", \
	L"Toggle Object Sound", \
    L"Toggle Cloned Light", \
    L"Mouse Displacement X", \
    L"Mouse Displacement Y", \
    L"Mouse Rotation Z", \
    L"Controller Displacement X", \
    L"Controller Displacement Y", \
    L"Controller Displacement Z", \
    L"Controller Displacement X", \
    L"Thruster", \
	L"Weapon 1", \
	L"Weapon 2", \
	L"Weapon 3" \
}

// initial mappings of actions to keys
//
#define ACTION_KEY_MAP {\
    KEY_D, KEY_A, KEY_S, KEY_W, \
    KEY_R, KEY_U, KEY_I, KEY_T, \
    KEY_O, KEY_P, \
    KEY_PGUP, KEY_PGDN, KEY_Z, KEY_C, KEY_W, KEY_S, KEY_Q, KEY_E, \
    KEY_A, KEY_D, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, \
    KEY_X, KEY_COMMA, KEY_8, KEY_9, \
    KEY_J, KEY_K, KEY_L, KEY_G, KEY_H, \
    KEY_F, KEY_V, \
	KEY_B, KEY_N, KEY_M, \
    KEY_0, KEY_1, KEY_2, \
	KEY_1, KEY_F3, KEY_F4, KEY_F6, KEY_F7, KEY_W, KEY_S, KEY_F8, KEY_F9, KEY_0, \
    KEY_1, KEY_1, KEY_1, KEY_1, KEY_1, KEY_1, KEY_1, \
    KEY_SPACE, \
	KEY_Q, KEY_E, KEY_R \
}

// initial mappings of pointer objects to actions
//
#define PTR_OBJ_ACTION_MAP { \
    CAM_PITCH_UP, CAM_PITCH_DOWN, ROLL_SQUARE, CAM_SMOOTH_YAW, CAM_SMOOTH_PITCH, CAM_SMOOTH_F_B, \
}

// initial mappings of controller objects to actions
//
#define CTR_OBJ_ACTION_MAP { \
    THRUSTER, CAM_PITCH_DOWN, CAM_YAW_LEFT, CAM_YAW_RIGHT, \
    CAM_ADVANCE, CAM_RETREAT, CAM_ROLL_LEFT, CAM_ROLL_RIGHT, \
    CAM_PAN_LEFT, CAM_PAN_RIGHT, CAM_PAN_LEFT_ALT, \
    CAM_PAN_LEFT, GF_CT_POSX, GF_CT_POSY, GF_CT_DSPZ, \
}

/* { A, B, X, Y,
     L1, R1, Back, Start,
     L3, R3, ?,
     ?, LeftStickX, LeftStickY ? } */

// Model Sounds
//
// to add a configurable sound
// - add its enumeration constant
// - add its description
// - add the default filename for the sound in Mappings.h
//
typedef enum ModelSound {
	SND_BKGRD,
	SND_DISCR,
	SND_OBJECT,
	SND_LOCAL_L,
	SND_LOCAL_R
} ModelSound;

// friendly descriptions of configurable sounds used in the dialog box
// (descriptions should not exceed MAX_DESC characters)
//
#define SOUND_DESCRIPTIONS {\
	L"Background",\
	L"Discrete",\
	L"Object",\
	L"Local Left",\
	L"Local Right"\
}

// initial selection of configurable sounds 
//
// include the authors name for CCS+ accreditation
//
#define SOUND_MAPPINGS {\
	L"Crickets (by reinsamba) .xwma",\
	L"Gong (by reinsamba) .xwma",\
	L"Street_accordeonist (by reinsamba) .xwma",\
	L"Goat (by reinsamba) .xwma",\
	L"Fortaleza election campaign (by reinsamba) .xwma",\
}

// Primitive Types
//
typedef enum PrimitiveType {
    POINT_LIST     = 1,
    LINE_LIST      = 2,
    LINE_STRIP     = 3,
    TRIANGLE_LIST  = 4,
    TRIANGLE_STRIP = 5,
    TRIANGLE_FAN   = 6,
} PrimitiveType;

// Rendering State Flags
//
typedef enum RenderState {
    ALPHA_BLEND    = 1,
    LIGHTING       = 2,
    WIRE_FRAME     = 3,
    Z_BUFFERING    = 4,
    W_BUFFERING    = 5,
    DITHERING      = 6,
    SPECULARITY    = 7,
} RenderState;

// Light Types
//
typedef enum LightType {
    POINT_LIGHT,
    SPOT_LIGHT,
    DIRECTIONAL_LIGHT
} LightType;

#define MAX_ACTIVE_LIGHTS 8

// Font description flags
//
#define TEXT_LEFT          1
#define TEXT_RIGHT         2
#define TEXT_CENTER        4
#define TEXT_TOP           8
#define TEXT_BOTTOM       16
#define TEXT_MIDDLE       32
#define TEXT_NORMAL       64
#define TEXT_MEDIUM      128
#define TEXT_SEMIBOLD    256
#define TEXT_BOLD        512
#define TEXT_TRUETYPE   1024
#define TEXT_RASTER     2048
#define TEXT_ANTIALIAS  4096
#define TEXT_PROOF      8192
#define TEXT_CLEARTYPE 16384
#define TEXT_EXTRA     32768
#define TEXT_DEFAULT TEXT_LEFT | TEXT_TOP | TEXT_NORMAL 
#define TEXT_HEIGHT       15
#define TEXT_TYPEFACE L"ARIAL"
// font colour [0,255]
#define TEXT_R 0
#define TEXT_G 0
#define TEXT_B 0
#define TEXT_A 255
#define TEXT_COLOUR  (TEXT_A << 24 | TEXT_R << 16 | TEXT_G << 8 | TEXT_B)

// Texture filtering flags
//
#define MAX_STAGES           2
#define TEX_MIN_POINT        1
#define TEX_MIN_LINEAR       2
#define TEX_MIN_ANISOTROPIC  4
#define TEX_MAG_POINT        8
#define TEX_MAG_LINEAR      16
#define TEX_MAG_ANISOTROPIC 32
#define TEX_MIPMAP          64
#define TEX_DEFAULT TEX_MIN_LINEAR | TEX_MAG_LINEAR | TEX_MIPMAP

//-------------------------------- APIDisplay ---------------------------------
//
// HUD Transparency
//
#define HUD_ALPHA 1

//-------------------------------- APIAudio -----------------------------------
//
// sound parameters
//
#define SOUND_DISTANCE_FACTOR       1.0f  // metres per coordinator unit
#define ATTENUATION_DISTANCE_FACTOR 10    // where attenuation begins

//-------------------------------- Common Structs -----------------------------
//
typedef struct Recti {
    int topLeftX;
    int topLeftY;
    int bottomRightX;
    int bottomRightY;
    Recti(int tlx = 0, int tly = 0, int brx = 1, int bry = 1) : 
     topLeftX(tlx), topLeftY(tly), bottomRightX(brx), bottomRightY(bry) {}
} Recti;

typedef struct Rectf {
    float topLeftX;
    float topLeftY;
    float bottomRightX;
    float bottomRightY;
    Rectf(float tlx = 0.0f, float tly = 0.0f, float brx = 1.0f, 
     float bry = 1.0f) : 
     topLeftX(tlx), topLeftY(tly), bottomRightX(brx), bottomRightY(bry) {}
    Recti absolute(int w, int h) { 
        return Recti((int)(topLeftX * w), (int)(topLeftY * h), 
         (int)(bottomRightX * w), (int)(bottomRightY * h)); 
    }
} Rectf;

#endif
