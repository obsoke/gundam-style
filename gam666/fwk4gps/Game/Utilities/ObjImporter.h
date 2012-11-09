#ifndef _OBJIMPORTER_H_
#define _OBJIMPORTER_H_

/**
.obj Model Importer
Rocket Jump and Dale
Gundam Style
**/
class iGraphic;
class ObjImporter {
    ObjImporter(const ObjImporter& s);            // prevents copying
    ObjImporter& operator=(const ObjImporter& s); // prevents assignment
  public:
	static iGraphic* ImportObjModel(const char* fileName);
};

#endif