#ifndef _MODEL_H_
#define _MODEL_H_

/* Internal Settings local to the Modelling Layer
*
* Model.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

// File Directories
#define AUDIO_DIRECTORY   L"..\\..\\resources\\audio"
#define TEXTURE_DIRECTORY L"..\\..\\resources\\textures"
#define ASSET_DIRECTORY   L"..\\..\\resources\\assets"

// default foreground texture transparency [\x00,\xff]
#define TEX_ALPHA '\xFF'

// default color key for textures
#define COLOR_KEY 0xFF00FF00  // lime

// Timing Factors
//
#define UNITS_PER_SEC   1000
// fps maximum - should be > flicker fusion threshold
#define FPS_MAX          100
// latency - keystroke time interval 
#define KEY_LATENCY     (UNITS_PER_SEC / 2)

// camera settings
//
// camera speed control factors
#define CAM_SPEED       0.02f
#define TURNING_RADIUS  120.00f
#define ANG_CAM_SPEED   ( CAM_SPEED / TURNING_RADIUS )

// frame motion parameters
//
// factors applied to the time interval
#define FORWARD_SPEED 10.0f / UNITS_PER_SEC
#define ROT_SPEED     0.03f * FORWARD_SPEED
#define CONSTANT_ROLL 10.0f * ROT_SPEED

// factors applied to the time interval
#define SPEED 100.0f / UNITS_PER_SEC

// input device motion conversion factors - 
//
// mouse motion conversion factors
#define MOUSE_SPEED             10
#define POINTER_SCALE           10
// controller conversion factors
#define CTR_SPEED               0.05f
#define CTR_DISPLACEMENT_FACTOR 0.015f 
#define CTR_ROTATION_FACTOR     0.015f 

// audio controls
//
// initial volume settings
#define MIN_VOLUME		   0
#define DEFAULT_VOLUME	  50
#define MAX_VOLUME	     100
#define STEP_VOLUME        1
// initial frequency settings
#define MIN_FREQUENCY	   0
#define DEFAULT_FREQUENCY 50
#define MAX_FREQUENCY    100
#define STEP_FREQUENCY	   1

// foreground texture transparency [\x00,\xff]
#define TEXTURE_ALPHA '\x00'

// hud and text controls
//
// hud background image
#define HUD_IMAGE L"hudBackground.bmp"
#define HUD_HEALTH L"health.bmp"
#define HUD_THRUSTER L"thruster.bmp"
// relative limits on the size of the hud [0,1]
#define HUD_MAX    0.99f
#define HUD_MIN    0.01f
#define HUD_SPEED  (0.5f /* screen units per sec */ / UNITS_PER_SEC) 
// hud location within window [0,1]
#define HUD_W     0.99f
#define HUD_H     0.99f
// text limits within the hud [0,1]
#define TEXT_MAX   0.99f
#define TEXT_MIN   0.01f

// Text status
#define TEXT_ON  L" + " // symbol for on
#define TEXT_OFF L" o " // symbol for off

// Object categories
//
typedef enum Category {
  LIT_OBJECT,
  SPRITE,
  OPAQUE_OBJECT,
  TRANSLUCENT_OBJECT,
  ALL_HUDS,
  ALL_SOUNDS,
} Category;

#endif
