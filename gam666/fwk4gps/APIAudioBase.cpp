/* API Audio Base Implementation - Translation Layer
 *
 * APIAudioBase.cpp
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fstream>        // for ostream << close()
#include "APIPlatform.h"  // for API headers
#include "APIAudioBase.h" // for APIAudioBase class definition
#include "iUtilities.h"   // for strcpy, strcat

//-------------------------------- APIAudioBase ------------------------------------
//
// APIAudioBase is the base class for the Translation Layer
//
// Addresses of objects that hold the system configuration
//
iAPIAudio*          APIAudioBase::audio           = nullptr;

// The API graphics connectivity is defined by the APIDisplay object
//
int                     APIAudioBase::width       = 0;
int                     APIAudioBase::height      = 0;
bool                    APIAudioBase::runinwndw   = true;

// The API audio connectivity is defined by the APIAudio object
//
IXAudio2* 				APIAudioBase::pXAudio2        = nullptr; // XAudio2 engine
X3DAUDIO_HANDLE* 		APIAudioBase::pX3DInstance    = nullptr; // X3DAudio engine
X3DAUDIO_LISTENER* 		APIAudioBase::pListener       = nullptr; // cameras's frame
IXAudio2MasteringVoice* APIAudioBase::pMasteringVoice = nullptr; // mastering voice
                                          // is default audio sink for all voices
float                   APIAudioBase::volume         = 0;
float                   APIAudioBase::frequencyRatio = 1.0f;
float                   APIAudioBase::distanceScale  = 1.0f;

// logError adds msg to the error.log file
//
void APIAudioBase::logError(const wchar_t* msg) const {

    std::wofstream fp("error.log", std::ios::app);
    if (fp) {
         fp << msg << std::endl;
         fp.close();
    }
}

// error adds the message to log file
//
void APIAudioBase::error(const wchar_t* msg, const wchar_t* more) const {

    int len = strlen(msg);
    if (more) len += strlen(more);
    wchar_t* str = new wchar_t[len + 1];
	strcpy(str, msg, len);
	if (more) strcat(str, more, len);

    logError(str);

    delete [] str;
}

