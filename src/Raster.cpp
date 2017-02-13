#include "Raster.h"

Raster::Raster()
{
	// Initialize variables.
	empty();
}

Raster::~Raster()
{
	empty();
}

void Raster::empty()
{
	pathName = "";
	nCols = 0;
	nRows = 0;
	xllCorner = 0.0f;
	yllCorner = 0.0f;
	cellSize = 0.0f;
	noDataValue = "";
	delete [] data;
	data = 0;
}

int Raster::load(std::string pathName)
{
	// Initialize variables.
	empty();

	// Load raster and save it to the struct.
	std::string content;
	int res = getFileContent(pathName.c_str(), content);

	if (res == 0)
	{
		pathName = pathName;
		std::vector<std::string> strings;
		std::istringstream f(content);
		std::string s;
		unsigned int counter = 0;
		
		while (getline(f, s))
		{
			std::istringstream x(s);
			std::string t;

			while (getline(x, t, ' '))
			{
				if (t != "" && t != " " && t != "\t" && t != "\n" && t != "\r" && t != "\v")
				{					
					if (counter == 1)
					{
						nCols = (unsigned short int)atoi(t.c_str());
					}
					else if (counter == 3)
					{
						nRows = (unsigned short int)atoi(t.c_str());
					}
					else if (counter == 5)
					{
						xllCorner = (double)atof(t.c_str());
					}
					else if (counter == 7)
					{
						yllCorner = (double)atof(t.c_str());
					}
					else if (counter == 9)
					{
						cellSize = (double)atof(t.c_str());
					}
					else if (counter == 11)
					{
						noDataValue = t;

						// Reserve memory for raster data.
						if (nCols > 0 && nRows > 0)
						{
							data = new std::string[nCols * nRows];
						}
						else
						{
							res = 1;
						}
					}
					else if (res == 0)
					{
						if (counter >= 12)
						{
							data[counter - 12] = t;
						}
					}

					counter++;
				}
			}
		}
	}
	else
	{
		res = 1;
	}
	
	return res;
}

int Raster::save(std::string sPath)
{
	std::stringstream sstream;
	sstream.str("");
	sstream << "ncols        " << nCols;
	sstream << "\nnrows        " << nRows;
	sstream << "\nxllcorner    " << std::fixed << xllCorner;
	sstream << "\nyllcorner    " << std::fixed << yllCorner;
	sstream << "\ncellsize     " << cellSize;
	sstream << "\nNODATA_value  " << noDataValue;

	for (int i = 0; i < nRows; i++)
	{
		sstream << "\n";
		
		for (int j = 0; j < nCols; j++)
		{
			sstream << " " << data[ j + i * nCols ];
		}
	}

	int res = saveAsciiFile( sPath, sstream.str() );

	return res;
}

std::string Raster::getPixelValue(double centerCoordX, double centerCoordY)
{
	std::string value = "";

	if (centerCoordX >= xllCorner && centerCoordX < xllCorner + nCols * cellSize 
		&& centerCoordY >= yllCorner && centerCoordY < yllCorner + nRows * cellSize)
	{
		if (nCols > 0 && nRows > 0 && cellSize > 0.0)
		{
			int col = (int)((centerCoordX - xllCorner) / cellSize);
			int row = (int)((centerCoordY - yllCorner) / cellSize);
			row = nRows - 1 - row;
			
			if (col + row * nCols >= 0 && col + row * nCols < nCols * nRows)
			{
				value = data[ col + row * nCols ];
			}
		}
	}
	else
	{
		std::cout << "\nError, a pixel outside raster " << pathName << " was accessed.";
	}

	return value;
}

std::string Raster::getPixelValue(int indexX, int indexY)
{
	std::string value = "";

	if (indexX >= 0 && indexX < nCols 
		&& indexY >= 0 && indexY < nRows)
	{
		if (nCols > 0 && nRows > 0)
		{
			if (indexX + indexY * nCols >= 0 && indexX + indexY * nCols < nCols * nRows)
			{
				value = data[ indexX + indexY * nCols ];
			}
		}
	}
	else
	{
		std::cout << "\nError, a pixel outside raster " << pathName << " was accessed.";
	}

	return value;
}

void Raster::print()
{
	std::cout << "\n\nPath: " << pathName;
	std::cout << "\nColumns: " << nCols;
	std::cout << "\nRows: " << nRows;
	std::cout << "\nLower left corner x: " << xllCorner;
	std::cout << "\nLower left corner y: " << yllCorner;
	std::cout << "\nCell size: " << cellSize;
	std::cout << "\nNo data value: " << noDataValue;

	for (int i = 0; i < nCols * nRows; i++)
	{
		std::cout << "\n" << i << "\t" << data[i];
	}
}

