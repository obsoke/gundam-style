#include <iostream>
#include <fstream>
#include <sstream>  //for std::istringstream
#include <iterator> //for std::istream_iterator
#include <vector>   //for std::vector
#include "..\Utils.h"
#include "ObjImporter.h"
using namespace std;
iGraphic* ObjImporter::ImportObjModel(const char* fileName)
{
	// open model file
	ifstream mf;
	string str;
	mf.open(fileName);
	if(mf.is_open()) // we did it!
	{
		while(mf.good()) // while not EOF or an error
		{
			getline(mf, str, '\n'); // grab line until newline
			if (str.length() > 0 && !(str[0] == '#')) // dont care for empty  lines or comments
			{
				str = str + "\n";
				//debug(str);
				std::istringstream ss(str);
				std::istream_iterator<std::string> begin(ss), end;

				//putting all the tokens in the vector
				std::vector<std::string> arrayTokens(begin, end); 

				//arrayTokens is containing all the tokens - use it!
				if(arrayTokens[0].compare("v") == 0) // vector
				{
					debug("VECTOR!\n");
				}
				else if(!arrayTokens[0].compare("vn") == 0) // vector normal
				{
					debug("VECTOR NORMAL!\n");
				}
				else if(!arrayTokens[0].compare("vt") == 0) // texture vector
				{
					debug("TEXTURE VECTOR\n");
				}
				else if(!arrayTokens[0].compare("g") == 0) // ???
				{
					debug("G???\n");
				}
				else if(!arrayTokens[0].compare("f") == 0) // face
				{
					debug("FACE!\n");
				}
			}
		}
	}
	// we're done here, close file
	mf.close();
	return 0;
}