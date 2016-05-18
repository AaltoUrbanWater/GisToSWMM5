#ifndef _FILEIO_H_
#define _FILEIO_H_
#include "FileIO.h"
#endif

class Table : public FileIO
{
	public: 
		// Methods.
		Table();
		~Table();
		int load(std::string sPath);
		int save(std::string sPath);
		void print();
	
		// Variables.
		
};