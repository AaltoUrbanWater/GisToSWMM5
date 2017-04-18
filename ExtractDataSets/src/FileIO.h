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
#include <cstring>		// TJN 20170215 string -> cstring
#endif

// TJN 20170215 START
#ifndef _CSTDLIB_H_
#define _CSTDLIB_H_
#include <cstdlib>
#endif
// TJN 20170215 END

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_
#include "Definitions.h"
#endif

class FileIO
{
	public: 
		// Methods.
		FileIO();
		~FileIO();
		int getFileContent(std::string path, std::string & content);
		int saveAsciiFile(std::string sPath, std::string sFile);
		int setDataFilePath(std::string dataFilePathNew);
		int load(std::string pathNew);
		void print();
		void tokenizeLines(std::string &data, std::vector<std::string> &lines);
		std::string extractLine(int &pos, std::string &data);

		// Variables.
		std::string path;
		std::string dataFilePath;
		std::string fileContent;
};
