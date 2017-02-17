#include "FileIO.h"

FileIO::FileIO()
{
	// Initialize variables.
	pathName = "";
	data = 0;
	nCols = 0;
	nRows = 0;
}

FileIO::~FileIO()
{
	delete [] data;
}

int FileIO::getFileContent(std::string path, std::string & content)
{	
	int res = 0;
	const int iArrSize = 10000;
	char cCharArr[iArrSize];
	content = "";
	std::ifstream ifsFin(path.c_str());
	int iCharsRead;

	if (ifsFin.is_open())
	{
		while (ifsFin.peek() != EOF)
		{
			memset( cCharArr, '\0', iArrSize ); 
			ifsFin.read(cCharArr, iArrSize);
			iCharsRead = (int)ifsFin.gcount();
			content.append(cCharArr, iCharsRead);
		}
	}
	else 
	{
		res = 1;
	}

	ifsFin.close();

	return res;
}

int FileIO::saveAsciiFile(std::string sPath, std::string sFile)
{
	std::ofstream fout;
	fout.open(sPath.c_str());

	if (fout.is_open())
	{
		fout << sFile.c_str();
		fout.close();
		return 0;
	}
	else
	{
		return 1;
	}
}
