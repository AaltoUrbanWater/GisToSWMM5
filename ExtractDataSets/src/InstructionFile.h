#ifndef _FILEIO_H_
#define _FILEIO_H_
#include "FileIO.h"
#endif

class InstructionFile : public FileIO
{
	public: 
		InstructionFile();
		~InstructionFile();
		int extractInstructions();
		std::string keyString;
		unsigned int skippedLines;
		unsigned int seriesColumn;
		int seriesLength;
		std::string stopString;
		std::string seriesName;
};