// gcc
#ifndef _CSTDLIB_H_
#define _CSTDLIB_H_
#include <cstdlib> 
#endif

// gcc
#ifndef _MATH_H_
#define _MATH_H_
#include <math.h>
#endif

#ifndef _FILEIO_H_
#define _FILEIO_H_
#include "FileIO.h"
#endif

// Information about the ESRI .asc file format.
// http://en.wikipedia.org/wiki/Esri_grid

class Raster : public FileIO
{
	public: 
		// Methods.
		Raster();
		~Raster();
		int load(std::string sPath);
		int save(std::string sPath);
		std::string getPixelValue(double centerCoordX, double centerCoordY);
		std::string getPixelValue(int indexX, int indexY);
		void empty();
		void print();
	
		// Variables.
		double xllCorner;
		double yllCorner;
		double cellSize;
		std::string noDataValue;
};
