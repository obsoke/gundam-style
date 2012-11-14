#ifndef _OBJIMPORTER_H_
#define _OBJIMPORTER_H_

#include <vector>
#include <string>
#include <map>
#include "..\..\MathDecl.h"

/*********************/
/* .obj Model Importer
/* Rocket Jump and Dale
/* Gundam Style
/*********************/

#include "..\Face.h"

class Mesh;

class ObjImporter {
  static std::vector<std::string> splitElements;
  static std::vector<int> splitNumElements;
  static std::string tempElement;

  static std::string defaultPath;

  ObjImporter(const ObjImporter& s);            // prevents copying
  ObjImporter& operator=(const ObjImporter& s); // prevents assignment

  // Import helper functions
  static Vector readVector(const std::vector<std::string>& tokens, int offset = 1);
  static Face readFace(const std::vector<std::string>& tokens);
  static std::vector<int>& splitNumbers(std::string str, char separator = '/');
  static const std::vector<std::string>& split(std::string str, char separator = ' ');
  static bool tokenEmpty(std::string fileData);
  static bool hasValue(std::string str, std::string compareTo);
  static std::string fullPath(std::string fileName);
  static std::map<std::string, Mesh*> meshLibrary;
public:
  static Mesh* import(const char* fileName);
};

#endif