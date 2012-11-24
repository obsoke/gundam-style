#include "Utils.h"
#include "windows.h"

void debug(const char* output) {
  OutputDebugStringA(output);
}

void debug(std::string output) {
  OutputDebugStringA(output.c_str());
}

void debug(const Vector& v) {
  debug(toString(v));
}

void debug(float f) {
  debug(toString(f));
}

std::string toString(const Vector& v) {
  std::stringstream ss;
  ss << "{ " << v.x << ", " << v.y << ", " << v.z << " }" << "\n";
  return ss.str();
}

std::string toString(float f) {
  std::stringstream ss;
  ss << f << "\n";
  return ss.str();
}

std::wstring toString(int i) {
  std::wstringstream ss;
  ss << i ;
  return ss.str();
}