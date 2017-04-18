#include "DataFile.h"

DataFile::DataFile()
{
	// Initialize variables.
	lengthOfExtractedSeries = 0;
	extractedSeries = 0;
}

DataFile::~DataFile()
{
	releaseMemory();
}

void DataFile::releaseMemory()
{
	delete [] extractedSeries;
	extractedSeries = 0;
	lengthOfExtractedSeries = 0;
}

int DataFile::extractDataSeries(InstructionFile &insFile)
{
	load(dataFilePath);
	
	int res = 0;
	std::size_t found = fileContent.find(insFile.keyString);
	int startPos = (int)found;
	found = fileContent.find(insFile.stopString, found);
	int endPos = (int)found;

	if (startPos >= 0 && startPos < (int)fileContent.length() 
		&& endPos >= 0 && endPos < (int)fileContent.length() && startPos < endPos)
	{
		std::string filePart = fileContent.substr (startPos, endPos - startPos);
		std::vector<std::string> lines;
		tokenizeLines(filePart, lines);
		//extractedSeries.clear();
		releaseMemory();
		lengthOfExtractedSeries = (int)lines.size() - insFile.skippedLines + 1;
		extractedSeries = new std::string[ lengthOfExtractedSeries ];

		for (int i = 0; i < lengthOfExtractedSeries; i++)
		{
			extractedSeries[i] = "";
		}

		for (int i = insFile.skippedLines + 1; i < (int)lines.size(); i++)
		{
			std::string dataItem = "";
			std::vector<std::string> words;

			for (int j = 0; j < (int)lines[i].length(); j++)
			{
				if (lines[i][j] != ' ' && lines[i][j] != '\t' && lines[i][j] != '\n' && j < (int)lines[i].length())
				{
					dataItem += lines[i][j];
				}
				else if (dataItem != "")
				{
					words.push_back( dataItem );
					dataItem = "";
				}

				if (j == (int)lines[i].length() - 1)
				{
					words.push_back( dataItem );
					dataItem = "";
				}
			}

			if ((int)words.size() > insFile.seriesColumn)
			{
				//extractedSeries.push_back( words[ insFile.seriesColumn ] );
				int rowIndex = i - (insFile.skippedLines + 1);
				extractedSeries[rowIndex] = words[ insFile.seriesColumn ];
			}
		}
	}
	else
	{
		res = 1;
	}

	fileContent = "";

	return res;
}