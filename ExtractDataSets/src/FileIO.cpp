#include "FileIO.h"

FileIO::FileIO()
{
	// Initialize variables.
	path = "";
	dataFilePath = "";
	fileContent = "";
}

FileIO::~FileIO()
{

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

int FileIO::setDataFilePath(std::string dataFilePathNew)
{
	int res = 0;

	if (dataFilePathNew != "")
	{
		dataFilePath = dataFilePathNew;
	}
	else
	{
		res = 1;
	}

	return res;
}

int FileIO::load(std::string pathNew)
{
	path = pathNew;
	int res = getFileContent(pathNew, fileContent);

	return res;
}

void FileIO::print()
{
	std::cout << "\n\nFile path: " << path;
	std::cout << "\n" << fileContent;
}

void FileIO::tokenizeLines(std::string &data, std::vector<std::string> &lines)
{
	int posInFile = 0;
	int rowCount = 0;

	while (posInFile < (int)data.length())
	{
		std::string line = extractLine(posInFile, data);
		lines.push_back( line );
		rowCount++;
	}
}

std::string FileIO::extractLine(int &pos, std::string &data)
{
	int iStrLen = (int)data.length() - 1;
	std::string sLine = "";

	while (1) // might be dangerous?
	{
		if (data[pos] != '\n')
		{
			sLine += data[pos];
		}

		if (pos >= iStrLen || data[pos] == '\n')
		{
			pos++;
			break;
		}

		pos++;
	}

	return sLine;
}