#include "Table.h"

Table::Table()
{
	// Initialize variables.
	
}

Table::~Table()
{
	
}

std::string Table::getData(int row, int col)
{
	if (nRows > 0 && nCols > 0 && row >= 0 && row < nRows && col >= 0 && col < nCols)
	{
		return data[col + row * nCols];
	}
	else
	{
		std::cout << "\n-> Table " << pathName.c_str() << " is not initialized, ";
		std::cout << "or an illegal position was in the table was accessed.";
		
		return "";
	}
}

void Table::writeToStringStream(std::stringstream &sstream)
{
	for (int i = 1; i < nRows; i++)
	{
		sstream << "\n";

		for (int j = 0; j < nCols; j++)
		{	
			sstream << getData(i, j);
			
			if (j < nCols - 1)
			{
				sstream << "   ";
			}
		}
	}
}

int Table::load(std::string pathNameNew)
{
	// Load raster and save it to the struct.
	pathName = pathNameNew;
	std::string content;
	int res = getFileContent(pathName.c_str(), content);

	if (res == 0)
	{
		std::istringstream f(content);
		std::string s;
		
		// Compute number of columns.
		if (getline(f, s))
		{
			std::istringstream x(s);
			std::string t;
			nCols = 0;

			while (getline(x, t, ','))
			{				
				nCols++;
			}
		}
		
		// Compute number of lines.
		nRows = 0;
		f.clear();
		f.seekg(0, std::ios::beg);
		
		while (getline(f, s))
		{
			nRows++;
		}
		
		// Reserve memory. 
		if (nCols > 0 && nRows > 0)
		{
			data = new std::string[nCols * nRows];

			for (int i = 0; i < nCols * nRows; i++)
			{
				data[i] = "empty";
			}
		}
		else
		{
			res = 1;
		}

		// Save data to the table.
		if (res == 0)
		{
			f.clear();
			f.seekg(0, std::ios::beg);

			unsigned int counter = 0;

			while (getline(f, s))
			{
				std::istringstream x(s);
				std::string t;

				while (getline(x, t, ','))
				{					
					data[counter] = t;
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

int Table::save(std::string sPath)
{
	std::stringstream sstream;
	sstream.str("");

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

void Table::print()
{
	std::cout << "\nTable path: " << pathName;
	
	for (int j = 0; j < nRows; j++)
	{
		std::cout << "\n";
		
		for (int i = 0; i < nCols; i++)
		{
			std::cout << data[i + j * nCols];

			if (i < nCols - 1)
			{
				std::cout << "\t";
			}
		}
	}
}
