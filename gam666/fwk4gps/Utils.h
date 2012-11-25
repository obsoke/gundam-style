#ifndef _UTILS_H_
#define _UTILS_H_

#include "MathDecl.h"
#include <string>
#include <sstream>

void debug(const char* output);
void debug(std::string output);
void debug(const Vector& v);
void debug(float f);
std::string toString(const Vector& v);
std::string toString(float f);
<<<<<<< HEAD:gam666/fwk4gps/Game/Utils.h
std::wstring toString(int i);
=======
std::wstring toWString(const Vector& v);
std::wstring toWString(float f);
void toWCString(wchar_t* dest, const char* src, unsigned size);
>>>>>>> master:gam666/fwk4gps/Utils.h

#endif