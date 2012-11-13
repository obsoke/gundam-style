#ifndef _OBJIMPORTER_H_
#define _OBJIMPORTER_H_

#include <vector>
#include <string>
#include "..\..\MathDecl.h"

/*********************/
/* .obj Model Importer
/* Rocket Jump and Dale
/* Gundam Style
/*********************/

#include "..\Face.h"

struct Mesh;

class ObjImporter {
  static std::string defaultPath;

  ObjImporter(const ObjImporter& s);            // prevents copying
  ObjImporter& operator=(const ObjImporter& s); // prevents assignment

  // Import helper functions
  static Vector readVector(const std::vector<std::string>& tokens, int offset = 1);
  static Face readFace(const std::vector<std::string>& tokens);
  template <class T> 
    static std::vector<T> splitNumbers(std::string str, char separator = '/');
  static bool tokenEmpty(std::string fileData);
  static bool hasValue(std::string str, std::string compareTo);
  static std::string fullPath(std::string fileName);
public:
  static Mesh* import(const char* fileName);
};

#endif