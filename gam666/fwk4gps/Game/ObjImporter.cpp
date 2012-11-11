#include <iostream>
#include <fstream>
#include <sstream>  //for std::istringstream
#include <iterator> //for std::istream_iterator
#include <vector>   //for std::vector
#include "..\..\APIVertex.h"    // for ...Vertex class definitions
#include "..\Utils.h"
#include "..\Face.h"
#include "ObjImporter.h"
using namespace std;
iGraphic* ObjImporter::ImportObjModel(const char* fileName, std::vector<Vector>* vl, 
	std::vector<Vector>* vnl, std::vector<Vector>* vtl, std::vector<Face>* fl)
{
	string str; // string object to put file data into
	// open model file
	ifstream mf;
	mf.open(fileName);
	if(mf.is_open()) // if we opened the file...
	{
		while(mf.good()) // while not EOF or an error
		{
			getline(mf, str, '\n'); // grab line until newline
			if (str.length() > 0 && !(str[0] == '#')) // dont care for empty  lines or comments
			{
				// tokenize string setup
				std::istringstream ss(str);
				std::istream_iterator<std::string> begin(ss), end;
				//putting all the tokens in the vector
				std::vector<std::string> arrayTokens(begin, end); 
				//arrayTokens contains all the tokens - use it!
				if(arrayTokens[0].compare("v") == 0) // vertex
				{
					Vector v;
					v.x = atof(arrayTokens[1].c_str());
					v.y = atof(arrayTokens[2].c_str());
					v.z = atof(arrayTokens[3].c_str());
					vl->push_back(v);
				}
				else if(arrayTokens[0].compare("vn") == 0) // vertex normal
				{
					Vector v;
					v.x = atof(arrayTokens[1].c_str());
					v.y = atof(arrayTokens[2].c_str());
					v.z = atof(arrayTokens[3].c_str());
					vnl->push_back(v);
				}
				else if(arrayTokens[0].compare("vt") == 0) // texture vertex
				{
					Vector v;
					v.x = atof(arrayTokens[1].c_str());
					v.y = atof(arrayTokens[2].c_str());
					v.z = atof(arrayTokens[3].c_str());
					vtl->push_back(v);
				}
				else if(arrayTokens[0].compare("f") == 0) // face
				{
					int size = arrayTokens.size();
					Face face; // create face
					if (size == 5) // we have a quad
						face.setType(FaceType::Quad);
					else if(size == 4) // we have a tri
						face.setType(FaceType::Tri);

					for(int i = 1; i < size; ++i)
					{
						char* str = (char*)arrayTokens[i].c_str(); // from std::string to char*
						std::istringstream stream(str); // stream to iterate over
						std::string num; // output of iterator
						IndexList il; // say hello to IndexList!
						int counter = 0; // to populate the index list (a bit hacky)
						while(getline(stream, num, '/')) // split on '/' and process each IndexList
						{
							switch(counter)
							{
							case 0: // first time around, get first #
								il.v = atof(num.c_str());
								++counter;
								break;
							case 1: // second time around
								il.vn = atof(num.c_str());
								++counter;
								break;
							case 2: // and a third!
								il.vt = atof(num.c_str());
								++counter;
								break;
							}
						} // end while(getline(stream, num, '/'))
						face.add(il);
					} // end for(int i = 1; i < size; ++i)
					fl->push_back(face);
				} // end else if(arrayTokens[0].compare("f") == 0)
				else if(arrayTokens[0].compare("g") == 0) // group
				{
					// TO DO
				}
			} // end if (str.length() > 0 && !(str[0] == '#')) 
		} // end while(mf.good())
	} // end if(mf.is_open())
	mf.close();
	return 0; // TO DO: return real data
}