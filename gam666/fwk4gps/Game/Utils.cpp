#include "Utils.h"

const char* toString(Vector& v) {
  std::stringstream ss;
  ss << v.x << ", " << v.y << ", " << v.z;
  return ss.str().c_str();
}