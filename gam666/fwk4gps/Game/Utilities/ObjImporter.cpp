#include <iostream>
#include <fstream>
#include <sstream>  //for std::istringstream
#include <iterator> //for std::istream_iterator
#include <vector>   //for std::vector

#include "..\..\Utils.h"
#include "..\Face.h"
#include "..\Mesh.h"
#include "ObjImporter.h"

using namespace std;

Mesh* ObjImporter::import(const char* fileName) {
  Mesh* mesh;
  if (meshLibrary.find(fileName) != meshLibrary.end()) {
    mesh = meshLibrary[fileName];
  } else {
    mesh = new Mesh;
    string lines, type;
    ifstream file(fullPath(fileName));
    if(file.is_open()) { // if we opened the file...
      while(file.good()) { // while not EOF or an error
        getline(file, lines, '\n'); // grab line until newline
        if (!tokenEmpty(lines)) {
          const vector<string>& tokens = split(lines);
          type = tokens[0];
          if (hasValue(type, "v")) // vertex
            mesh->vertices.push_back(readVector(tokens));
          else if (hasValue(type, "vn")) // vertex normal
            mesh->normals.push_back(readVector(tokens));
          else if (hasValue(type, "vt")) // texture vertex
            mesh->uvs.push_back(readVector(tokens));
          else if (hasValue(type, "f")) // face
            mesh->faces.push_back(readFace(tokens));
          else if (hasValue(type, "g")) { // group
            // TO DO
          }
        }
      }
    }
    meshLibrary[fileName] = mesh;
  }
  return mesh;
}

Vector ObjImporter::readVector(const std::vector<std::string>& tokens, int offset) {
  return Vector(
    (float)atof(tokens[offset].c_str()), 
    (float)atof(tokens[offset+1].c_str()),
    (float)atof(tokens[offset+2].c_str())
  );
}

Face ObjImporter::readFace(const std::vector<std::string>& tokens) {
  int size = tokens.size(), offset = 1;
  Face face(size - offset);
  for (int i = offset; i < size; ++i) {
    std::vector<int>& numbers = splitNumbers(tokens[i]);
    if (numbers.size() < 3)
      int x = 0;
    for (unsigned j = 0; j < numbers.size(); ++j)
      if (numbers[j] > 0)
        --numbers[j];
    face.add(IndexList(numbers));
  }
  return face;
}

const std::vector<std::string>& ObjImporter::split(std::string str, char separator) {
  splitElements.clear();
  int current = 0, next = 0;
  while (next != -1) {
    next = str.find(separator, current + 1);
    const std::string& element = str.substr(current, next - current);
    if (element.length())
      splitElements.push_back(element);
    current = next + 1;
  }
  return splitElements;
}

std::vector<int>& ObjImporter::splitNumbers(std::string str, char separator) {
  splitNumElements.clear();
  int current = 0, next = 0;
  while (next != -1) {
    next = str.find(separator, current);
    const std::string& element = str.substr(current, next - current);
    int number = element.length() ? atoi(element.c_str()) : 0;
    splitNumElements.push_back(number);
    current = next + 1;
  }
  return splitNumElements;
}

bool ObjImporter::hasValue(std::string str, std::string compareTo) {
  return (str.compare(compareTo) == 0);
}

bool ObjImporter::tokenEmpty(std::string fileData) {
  return (!fileData.length() || fileData[0] == '#');
}

std::string ObjImporter::fullPath(std::string fileName) {
  std::string location;
  location += defaultPath;
  location += fileName;
  return location;
};

std::string ObjImporter::defaultPath = "..\\..\\resources\\assets\\";
std::string ObjImporter::tempElement = "";
std::vector<std::string> ObjImporter::splitElements = std::vector<std::string>();
std::vector<int> ObjImporter::splitNumElements = std::vector<int>();
std::map<std::string, Mesh*> ObjImporter::meshLibrary = std::map<std::string, Mesh*>();

void ObjImporter::disposeLibrary() {
  std::map<std::string, Mesh*>::iterator it;
  for (it = meshLibrary.begin(); it != meshLibrary.end(); ++it) {
    if (it->second) delete it->second;
    it->second = nullptr;
  }
}