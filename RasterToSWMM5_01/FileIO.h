#ifndef _FSTREAM_H_
#define _FSTREAM_H_
#include <fstream>
#endif

#ifndef _SSTREAM_H_
#define _SSTREAM_H_
#include <sstream>
#endif

#ifndef _IOSTREAM_H_
#define _IOSTREAM_H_
#include <iostream>
#endif

#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <vector>
#endif

#ifndef _STRING_H_
#define _STRING_H_
#include <string>
#endif

class FileIO
{
	public: 
		// Methods.
		FileIO();
		~FileIO();
		int getFileContent(std::string path, std::string & content);
		int saveAsciiFile(std::string sPath, std::string sFile);
	
		// Variables.
		std::string pathName;
		std::string * data;
		unsigned short int nCols;
		unsigned short int nRows;
};