#include "InstructionFile.h"

InstructionFile::InstructionFile()
{
	// Initialize variables.
	keyString = "";
	skippedLines = 0;
	seriesColumn = 0;
	seriesLength = -1;
	stopString = "";
	seriesName = "undefined";
}

InstructionFile::~InstructionFile()
{
	
}

int InstructionFile::extractInstructions()
{
	int res = 0;
	std::vector<std::string> lines;
	tokenizeLines(fileContent, lines);

	if ((int)lines.size() == INST_FILE_LINES)
	{
		for (int i = 0; i < (int)lines.size(); i++)
		{
			switch (i)
			{
				case 1: keyString = lines[i];
					break;
				case 3: skippedLines = atoi(lines[i].c_str());
					break;
				case 5: seriesColumn = atoi(lines[i].c_str());
					break;
				case 7: seriesLength = atoi(lines[i].c_str());
					break;
				case 9: stopString = lines[i];
					stopString.replace(stopString.find("\\n"),stopString.length(),"\n");
					break;
				case 11: seriesName = lines[i];
					break;
				default:
					break;
			}
		}
	}
	else
	{
		res = 1;
	}

	return res;
}

