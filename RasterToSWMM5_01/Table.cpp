#include "Table.h"

Table::Table()
{
	// Initialize variables.
	
}

Table::~Table()
{
	
}

int Table::load(std::string pathNameNew)
{
	// Initialize variables.
	//empty();

	// Load raster and save it to the struct.
	pathName = pathNameNew;
	std::string content;
	int res = getFileContent(pathName.c_str(), content);

	if (res == 0)
	{
		std::istringstream f(content);
		std::string s;
		
		// Compute table dimensions.
		while (getline(f, s))
		{
			std::istringstream x(s);
			std::string t;
			nCols = 0;

			while (getline(x, t, '\t'))
			{
				if (t != "" && t != " " && t != "\t" && t != "\n" && t != "\r" && t != "\v")
				{					
					nCols++;
				}
			}

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
			std::istringstream fs(content); // why is this redone here?
			unsigned int counter = 0;

			while (getline(fs, s))
			{
				std::istringstream x(s);
				std::string t;

				while (getline(x, t, '\t'))
				{
					if (t != "" && t != " " && t != "\t" && t != "\n" && t != "\r" && t != "\v")
					{					
						data[counter] = t;
						counter++;
					}
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