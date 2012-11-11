#ifndef _OBJIMPORTER_H_
#define _OBJIMPORTER_H_
#include <vector>
/**
.obj Model Importer
Rocket Jump and Dale
Gundam Style
**/
class iGraphic;
class Face;
struct Vector;
class ObjImporter {
    ObjImporter(const ObjImporter& s);            // prevents copying
    ObjImporter& operator=(const ObjImporter& s); // prevents assignment
  public:
	static iGraphic* ImportObjModel(const char* fileName, std::vector<Vector>* vl, 
	std::vector<Vector>* vnl, std::vector<Vector>* vtl, std::vector<Face>* fl);
};

#endif