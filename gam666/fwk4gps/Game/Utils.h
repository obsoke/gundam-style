#ifndef _UTILS_H_
#define _UTILS_H_

#include "..\MathDecl.h"
#include <string>
#include <sstream>

void debug(const char* output);
void debug(std::string output);
void debug(Vector& v);
std::string toString(Vector& v);

#endif