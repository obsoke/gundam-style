#ifndef _API_AUDIO_BASE_H_
#define _API_AUDIO_BASE_H_

/* API Audio Base Definition - Translation Layer
 *
 * APIAudioBase.h
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- APIAudioBase -------------------------------
//
// The API Audio Base class manages the connections to the underlying Audio API
//
class  iAPIAudio;
struct IXAudio2;
struct IXAudio2MasteringVoice;
struct X3DAUDIO_LISTENER;
typedef unsigned char X3DAUDIO_HANDLE[20]; // from x3daudio.h

class APIAudioBase {

protected:

    static iAPIAudio*              audio;       // the sound card object

    static int                     width;       // width of the client area
    static int                     height;      // height of the client area
    static bool                    runinwndw;   // running in a window?

    static IXAudio2*               pXAudio2;		// XAudio2 engine
	static X3DAUDIO_HANDLE*        pX3DInstance;	// X3DAudio constants
	static X3DAUDIO_LISTENER*      pListener;		// camera's frame
	static IXAudio2MasteringVoice* pMasteringVoice; // masteringVoice
                                   
    static float                   volume;
    static float                   frequencyRatio;
    static float                   distanceScale; // scale to user units

    virtual ~APIAudioBase()        { }

public:
    void error(const wchar_t*, const wchar_t* = 0) const;
    void logError(const wchar_t*) const;
};

#endif
