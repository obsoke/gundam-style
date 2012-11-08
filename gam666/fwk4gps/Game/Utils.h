#ifndef _UTILS_H_
#define _UTILS_H_

#include "..\MathDecl.h"
#include <string>
#include <sstream>

void debug(const char* output);
void debug(std::string output);
void debug(const Vector& v);
void debug(float f);
std::string toString(const Vector& v);
std::string toString(float f);

#endif