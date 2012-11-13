#include <iostream>
#include <fstream>
#include <sstream>  //for std::istringstream
#include <iterator> //for std::istream_iterator
#include <vector>   //for std::vector

#include "..\Utils.h"
#include "..\Face.h"
#include "..\Mesh.h"
#include "ObjImporter.h"

using namespace std;

Mesh* ObjImporter::import(const char* fileName) {
  Mesh* mesh = new Mesh;
  string fileData;
  ifstream file(fullPath(fileName));
  if(file.is_open()) { // if we opened the file...
    while(file.good()) { // while not EOF or an error
      getline(file, fileData, '\n'); // grab line until newline
      if (!tokenEmpty(fileData)) {
        // tokenize string
        std::istringstream ss(fileData);
        std::istream_iterator<std::string> begin(ss), end;
        std::vector<std::string> tokens(begin, end);
        std::string type = tokens[0];
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
    std::vector<int>& numbers = splitNumbers<int>(tokens[i]);
    for (unsigned j = 0; j < numbers.size(); ++j)
      --numbers[j];
    face.add(IndexList(numbers));
  }
  return face;
}

template<class T>
std::vector<T> ObjImporter::splitNumbers(std::string str, char separator) {
  std::vector<T> numbers;
  std::istringstream stream(str.c_str()); // stream to iterate over
  std::string num; // output of iterator
  while (getline(stream, num, separator))
    numbers.push_back((T)atof(num.c_str()));
  return numbers;
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