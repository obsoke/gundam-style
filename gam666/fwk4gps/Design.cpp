/* Design Implementation - Application Layer
 *
 * Design.cpp
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Design.h"      // for the Design class definition
#include "iSound.h"      // for the Sound Interface
#include "iText.h"       // for the Text Interface
#include "iHUD.h"        // for the HUD Interface
#include "iLight.h"      // for the Light Interface
#include "iObject.h"     // for the Object Interface
#include "iTexture.h"    // for the Texture Interface
#include "Camera.h"      // for the Camera class
#include "iGraphic.h"    // for the Graphic Interface
#include "MathDef.h"     // for Matrix operations
#include "Model.h"       // for ROLL_SPEED
#include "Translation.h" // for Action enumerations

//-------------------------------- Design -------------------------------------
//
// The Design class implements the game design within the Modelling Layer
//
// constructor initializes the engine and the instance pointers
//
Design::Design(void* h, int s) : Coordinator(h, s) {

	// pointers to the objects
    parent    = nullptr;
    child     = nullptr;
    square    = nullptr;
    sprite    = nullptr;
    rollLeft  = nullptr;
    spinRight = nullptr;

	// pointers to the lights
    pointLight   = nullptr;
	spotLight    = nullptr;
	distantLight = nullptr;

    // pointers to the sounds
    background   = nullptr;
    discrete     = nullptr;
    locall       = nullptr;
    localr       = nullptr;
    objectSnd    = nullptr;
}

// initialize initializes the general display design coordinator, creates the 
// primitive sets, textures, objects, lights, sounds, cameras, and text items
//
void Design::initialize() {

    // general display design
    //
    setAmbientLight(0.2f, 0.2f, 0.2f);
    iHUD* hud = CreateHUD(CreateGraphic(), 0.1f, 0.1f, 0.43f, 0.43f, CreateTexture(HUD_IMAGE));
	setTimerText(CreateText(Rectf(0.0f, 0.05f, 0.2f, 0.15f), hud, L"", 
     TEXT_HEIGHT, TEXT_TYPEFACE, TEXT_LEFT));

    // sprites
    //
    CreateSprite(CreateGraphic(), '\xFF')->attach(CreateTexture(L"stonehenge.bmp"));
    sprite = CreateSprite(CreateGraphic(120, 120));
    sprite->attach(CreateTexture(L"sprite.bmp"));
    sprite->translate(0, 20, 0);

    // cameras ----------------------------------------------------------------

    // camera at a distance - in lhs coordinates
    iCamera* camera = CreateCamera();
    camera->translate(-5, 0, -80);
    // second camera attached to the box
    iCamera* objectCamera = CreateCamera();
	CreateText(Rectf(0, 0.05f, 0.65f, 0.15f), hud, L" Camera: at ", position, 
     Camera::getCurrent(), ' ', 1, 16, L"ARIAL", TEXT_CENTER);
    CreateText(Rectf(0.50f, 0.05f, 0.90f, 0.15f), hud, L" heading ", orient, 
     Camera::getCurrent(), 'z', 100,  16, L"ARIAL", TEXT_CENTER);

    // model ------------------------------------------------------------------

    Colour blue(0.1f, 0.1f, 0.9f);
    Colour green(0.1f, 0.8f, 0.1f);
    Colour red(.9f, .1f, .1f, 0.5f);
    Colour grey(.2f, .2f, .2f);

    // vertex lists
    iGraphic* boxg  = CreateBox(-10, -10, -10, 10, 10, 10, green);
    iGraphic* boxb  = CreateBox(-5, -5, -5, 5, 5, 5, blue);
    iGraphic* gridw = CreateGrid(-25, 25, 10, grey);
    iGraphic* plate = TriangleList(L"colouredsquare.txt", red);

    // create objects
    square = CreateObject(plate);
    parent = CreateObject(boxg);
    child  = CreateObject(boxb);
    iObject* xz = CreateObject(gridw);
    iObject* xy = Clone(xz);
    iObject* yz = Clone(xz);

    // position and attach
    child->translate(0, 0, 10);
    child->attachTo(parent);
    parent->translate(-8, -20, 40);
    objectCamera->attachTo(parent);
    xz->translate(25, 0, 25);
    xy->rotatex(1.5708f);
    xy->translate(25, 25, 0);
    yz->rotatez(1.5708f);
    yz->translate(0, 25, 25);
	iTexture* daisy = CreateTexture(L"daisy.bmp");
    parent->attach(daisy);

    iGraphic* box = CreateBox(-10, -10, -10, 10, 10, 10);
    Reflectivity greenish(green);
    rollLeft = CreateObject(box, &greenish);
	rollLeft->attach(daisy);
    rollLeft->translate(-40, -20, 30);
    objectCamera->attachTo(rollLeft);

    Reflectivity whiteish(Colour(1, 1, 1, 0.5f));
    spinRight = CreateObject(box, &whiteish);
    spinRight->translate(30, -15, 30);
    iTexture* checktga = CreateTexture(L"check.tga");
	spinRight->attach(checktga);

    iGraphic* longPlate = CreateBox(-30, -10, 0, 30, 10, 600);
    Reflectivity bluish(blue);
    iObject* floor = CreateObject(longPlate, &whiteish);
    floor->translate(20, -80, 440);
    iTexture* check = CreateTexture(L"check.bmp");
    floor->attach(check);

    // lighting ---------------------------------------------------------------

    Colour white(1, 1, 1);
    Colour yellow(1, 1, 0);
    Colour turquoise(0, 0.8f, 0.6f);
    Colour black(0, 0, 0);

    // Create far away red point-source light
	pointLight = CreatePointLight(red, red, white, 1000.0f, true);
    pointLight->translate(-100.f, 100.f, -100.f);

    // create yellow/yellow/white spotlight (pointing in the world z direction)
	spotLight = CreateSpotLight(yellow, yellow, white, 300.0f, true, 1, 0.00005f, 
	 0.00001f, .40f, .25f, 0.999f);
    spotLight->translate(-20.f, -20.f, -100.f);

    // create turquoise directional light (pointing in the world -y direction)
 	distantLight = CreateDistantLight(turquoise, turquoise, black, false);
    distantLight->translate(10, 1000, 0);
    distantLight->rotatex(1.57f); // rotate into the world -y direction
    distantLight->rotatez(0.20f); // rotate 11 degrees towards  world +x 

    // Heads Up Display Text --------------------------------------------------

    // object data
    CreateText(Rectf(0, 0.30f, 0.7f, 0.42f), hud, L" Roller x orientation ",
     orient, rollLeft, 'x', 100);
    CreateText(Rectf(0, 0.42f, 0.7f, 0.54f), hud, L" Roller y orientation ", 
     orient, rollLeft, 'y', 100);
    CreateText(Rectf(0, 0.54f, 0.7f, 0.66f), hud, L" Spinner z orientation ", 
     orient, spinRight, 'z', 100);

    // lighting data
	CreateText(Rectf(0, 0.66f, 0.5f, 0.74f), hud, L"Point light ", onOff, 
     pointLight);
	CreateText(Rectf(0, 0.74f, 0.5f, 0.82f), hud, L"Spot light ", onOff,
     spotLight);
 	CreateText(Rectf(0, 0.82f, 0.5f, 0.90f), hud, L"Directional light ",
     onOff, distantLight);

	// audio ------------------------------------------------------------------
    
    // create background sound
	if (file(SND_BKGRD)) {
		background = CreateSound(file(SND_BKGRD));
	    CreateText(Rectf(0.5f, 0.66f, 1, 0.74f), hud, L"Background ", onOff,
         background);
    }

	// create discrete sound
	if (file(SND_DISCR)) {
		discrete = CreateSound(file(SND_DISCR), false);
	    CreateText(Rectf(0.5f, 0.74f, 1, 0.82f), hud, L"Discrete ", onOff, 
         discrete);
    }

	// create local sound on the left
	if (file(SND_LOCAL_L)) {
		locall = CreateLocalSound(file(SND_LOCAL_L), true, 90);
		locall->translate(-30, 5, 40);
		locall->rotatey(3.14f);
	    CreateText(Rectf(0.5f, 0.82f, 1, 0.90f), hud, L"Local left ", onOff,
         locall);
	}

	// create local sound on the right
	if (file(SND_LOCAL_R)) {
		localr = CreateLocalSound(file(SND_LOCAL_R), true, 90);
		localr->translate(30, 5, 40);
		localr->rotatey(3.14f);
	    CreateText(Rectf(0.5f, 0.90f, 1, 0.98f), hud, L"Local right ", onOff,
         localr);
	}

	// create a local sound attached to right object
	if (file(SND_OBJECT)) {
		objectSnd = CreateLocalSound(file(SND_OBJECT), true, 90);
        objectSnd->attachTo(spinRight);
    }
}

// update updates the position and orientation of each object according to the 
// actions initiated by the user
//
void Design::update() {

    static bool left = false, down = false;
    int delta = now - lastUpdate;
    float del = delta * SPEED;
    float dsx = left ? - del : del, dsy = 0;
    int dz = 0;  // roll  the square around || to world z axis
    int dx = 0;  // pitch the box    around || to world x axis
    int dr = 0;  // roll the left box about an axis || to world z axis
    int ds = 0;  // spin the right box about an axis || to world y axis

    // add changes introduced through keyboard input
    if (pressed(SPRITE_MINUS_X))
        dsx -= del;
    if (pressed(SPRITE_PLUS_X))
        dsx += del;
    if (pressed(SPRITE_MINUS_Y))
        dsy -= del;
    if (pressed(SPRITE_PLUS_Y))
        dsy += del;
    if (pressed(ROLL_SQUARE))
        dz += delta;
    if (pressed(PITCH_BOX))
        dx += delta;

    // keep sprite within limits and reverse directions at limits
    Vector p = sprite->position();
    if (p.x + dsx <= 0) {
        dsx = 2 * p.x + dsx;
        left = false;
    }
    else if (p.x + dsx + sprite->width() >= width) {
        dsx = 2 * (width - p.x - sprite->width()) - dsx;
        left = true;
    }
    if (p.y + dsy <= 0) {
        dsy = 2 * p.y + dsy;
        down = false;
    }
    else if (p.y + dsy + sprite->height() >= height) {
        dsy = 2 * (height - p.y - sprite->height()) - dsy;
        down = true;
    }
    sprite->translate(dsx, dsy, 0);

	// adjust the orientations for user input
    if (parent) 
	     parent->rotatex(dx * ROT_SPEED + CONSTANT_ROLL);
    if (square) 
	     square->rotatez(dz * ROT_SPEED);
    
    // attach/detach child object
    if (pressed(MDL_ATT_CHILD) && child && parent)
        child->attachTo(parent);
    if (pressed(MDL_DET_CHILD) && child && parent)
        child->attachTo(nullptr);

    // boxes ------------------------------------------------------------------

    // add changes introduced by user input
    if (pressed(ROLL_LEFT_BOX))
        dr += delta;
    if (pressed(SPIN_RIGHT_BOX))
        ds += delta;
    if (spinRight) 
	     spinRight->rotatey(ds * ROT_SPEED + CONSTANT_ROLL);
    if (rollLeft) 
	    rollLeft->rotatez(dr * ROT_SPEED + CONSTANT_ROLL);

    // lighting ---------------------------------------------------------------

    if (pressed(LIGHT_POINT) && pointLight)
		pointLight->toggle();
	if (pressed(LIGHT_SPOT) && spotLight)
		spotLight->toggle();
	if (pressed(LIGHT_DISTANT) && distantLight)
		distantLight->toggle();

    // texture filtering ---------------------------------------------------------------
    if (pressed(TEXTURE_POINT) && rollLeft && spinRight) {
        rollLeft->setTextureFilter(TEX_MIN_POINT | TEX_MAG_POINT);
        spinRight->setTextureFilter(TEX_MIN_POINT | TEX_MAG_POINT);
        release(TEXTURE_POINT);
    }
    if (pressed(TEXTURE_LINEAR) && rollLeft && spinRight) {
        rollLeft->setTextureFilter(TEX_MIN_LINEAR | TEX_MAG_LINEAR);
        spinRight->setTextureFilter(TEX_MIN_LINEAR | TEX_MAG_LINEAR);
        release(TEXTURE_LINEAR);
    }

	// audio ------------------------------------------------------------------
    
    if (pressed(AUD_BKGRD) && background)
        background->toggle();
    if (pressed(AUD_IMPLS) && discrete)
        discrete->toggle();
    if (pressed(AUD_LOCALL) && locall)
        locall->toggle();
    if (pressed(AUD_LOCALR) && localr)
        localr->toggle();
    if (pressed(AUD_OBJECT) && objectSnd)
        objectSnd->toggle();
}
