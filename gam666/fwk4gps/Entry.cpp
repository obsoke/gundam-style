/* An Open Instructional Framework for Game Programming Students
*
* Entry.cpp
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h> // for WinMain and Windows Types
#include "Game\Utilities\ObjImporter.h"

#include "Game\Game.h"  // for the Design class definition

// Entry point for the Application
//
int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hprev, LPSTR cp, int show) {
	ObjImporter::ImportObjModel("..\\..\\resources\\assets\\gundam.obj");
  //Game game(hinst, show);
  return 0;
}
