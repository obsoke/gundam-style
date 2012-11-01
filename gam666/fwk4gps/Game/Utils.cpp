#include "Utils.h"
#include "windows.h"

void debug(const char* output) {
  OutputDebugStringA(output);
}

void debug(std::string output) {
  OutputDebugStringA(output.c_str());
}

void debug(Vector& v) {
  debug(toString(v));
}

std::string toString(Vector& v) {
  std::stringstream ss;
  ss << "{ " << v.x << ", " << v.y << ", " << v.z << " }" << "\n";
  return ss.str();
}