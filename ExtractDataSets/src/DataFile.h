#ifndef _FILEIO_H_
#define _FILEIO_H_
#include "FileIO.h"
#endif

#ifndef _INSTRUCTIONFILE_H_
#define _INSTRUCTIONFILE_H_
#include "InstructionFile.h"
#endif

class DataFile : public FileIO
{
	public: 
		DataFile();
		~DataFile();
		int extractDataSeries(InstructionFile &insFile);
		void releaseMemory();
		std::string * extractedSeries;
		int lengthOfExtractedSeries;
		//std::vector<std::string> extractedSeries;

};