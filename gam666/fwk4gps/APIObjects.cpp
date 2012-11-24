#include "APIObjects.h"
#include "APIWindow.h"
#include "APIAudio.h"
#include "APIDisplay.h"
#include "APIUserInput.h"
#include "Model.h"

APIObjects::APIObjects(void* handle, int show) {
  window      = CreateAPIWindow(handle, show);
  userInput   = CreateAPIUserInput(AUDIO_DIRECTORY);
  display     = CreateAPIDisplay();
  audio       = CreateAPIAudio(1.0f, MIN_VOLUME, MAX_VOLUME, MIN_FREQUENCY, 
    MAX_FREQUENCY, DEFAULT_VOLUME, DEFAULT_FREQUENCY);
}

APIObjects::~APIObjects() {
  audio->Delete();
  display->Delete();
  userInput->Delete();
  window->Delete();
}