#ifndef _MAIN_H_
#define _MAIN_H_
#include "main.h"
#endif

int main (int argc, char* cArgv[])
{
	// Introduce timing related objects.
	int iStartClock = clock(); // For Windows version

	// Print header.
	std::cout << "\n\n************************************************************";
	std::cout << "\n*                    ExtractDataSets                       *"; 
	std::cout << "\n*   Extracts time series data from a larger output file.   *"; 
	std::cout << "\n*     The program is developed under the MIT license.      *";
	std::cout << "\n*                (C) Aalto University 2015.                *";
	std::cout << "\n************************************************************";

	// Print commandline arguments.
	std::cout << "\n\nCommand line arguments:";

	for (int i = 0; i < argc; i++)
	{
		std::cout << "\n-> Argument " << i << ": " << cArgv[i];
	}

	if (argc > 2 && (argc - 2) % 2 == 0)
	{
		std::vector<InstructionFile> insFiles;
		insFiles.resize( (argc - 2) / 2 );
		//std::vector<DataFile> dataFiles;
		//dataFiles.resize( (argc - 2) / 2 );
		DataFile * dataFiles;
		int numOfDataFiles = (argc - 2) / 2;
		dataFiles = new DataFile [numOfDataFiles];
		int resInsFile = 0;
		int resDataFile = 0;
		int insFileNum = -1;
		int dataFileNum = -1;
		
		for (int i = 1; i < argc - 1; i++) // leave out the exe and the csv file
		{
			// Instruction file.
			if (i % 2 != 0)
			{
				int resLoc = insFiles[ (i - 1) / 2 ].load(cArgv[i]);

				if (resInsFile == 0 && resLoc == 1)
				{
					resInsFile = resLoc;
					insFileNum = (i - 1) / 2;
					break;
				}
			}
			// Data file.
			else
			{
				//int resLoc = dataFiles[ (i - 1) / 2 ].load(cArgv[i]);
				int resLoc = dataFiles[ (i - 1) / 2 ].setDataFilePath(cArgv[i]);
				
				if (resDataFile == 0 && resLoc == 1)
				{
					resDataFile = resLoc;
					dataFileNum = (i - 1) / 2;
					break;
				}
			}
		}
		/*
		std::string ** dataArray;
		dataArray = new std::string*[27];

		for (int i = 0; i < 27; i++)
		{
			dataArray[i] = new std::string[1379700];
		}

		for (int i = 0; i < 27; i++)
		{
			for (int j = 0; j < 1379700; j++)
			{
				dataArray[i][j] = "test";
			}
		}

		for (int i = 0; i < 27; i++)
		{	
			delete [] dataArray[i];
		}

		delete [] dataArray;
		*/
		
		// Continue if the instruction files were read succesfully
		//if (resInsFile == 0 && resDataFile == 0 && (int)insFiles.size() == (int)dataFiles.size())
		if (resInsFile == 0 && resDataFile == 0 && (int)insFiles.size() == numOfDataFiles)
		{
			// Extract instructions from the content.
			for (int i = 0; i < (int)insFiles.size(); i++)
			{
				int res = insFiles[i].extractInstructions();
			}

			// Extract data series from the content according to the instructions.
			#pragma omp parallel for num_threads(4)
			//for (int i = 0; i < (int)dataFiles.size(); i++)
			for (int i = 0; i < numOfDataFiles; i++)
			{
				int res = dataFiles[i].extractDataSeries( insFiles[i] );
			}
			
			// Find the longest extracted series length.
			int seriesMaxLength = 0;

			//for (int i = 0; i < (int)dataFiles.size(); i++)
			for (int i = 0; i < numOfDataFiles; i++)
			{
				//if (seriesMaxLength < (int)dataFiles[i].extractedSeries.size())
				if (seriesMaxLength < (int)dataFiles[i].lengthOfExtractedSeries)
				{
					//seriesMaxLength = (int)dataFiles[i].extractedSeries.size();
					seriesMaxLength = (int)dataFiles[i].lengthOfExtractedSeries;
				}
			}

			// Write series headers.
			std::stringstream sstream;

			for (int i = 0; i < (int)insFiles.size(); i++)
			{
				std::stringstream indexNumber;

				if (i < 9)
				{
					indexNumber << "0" << i;
				}
				else
				{
					indexNumber << i;
				}
				
				sstream << insFiles[i].seriesName << "_" << indexNumber.str();

				if (i < (int)insFiles.size() - 1)
				{
					sstream  << ", ";
				}
			}

			// Write series data.
			for (int j = 0; j < seriesMaxLength; j++)
			{
				sstream << "\n";
					
				//for (int i = 0; i < (int)dataFiles.size(); i++)
				for (int i = 0; i < numOfDataFiles; i++)
				{
					//if (j < (int)dataFiles[i].extractedSeries.size())
					if (j < (int)dataFiles[i].lengthOfExtractedSeries)
					{
						sstream << dataFiles[i].extractedSeries[j];
					}
					else
					{
						sstream << " ";
					}

					//if (i < (int)dataFiles.size() - 1)
					if (i < numOfDataFiles)
					{
						sstream << ", ";
					}
				}
			}

			// Write the csv file.
			FileIO fileio;
			fileio.saveAsciiFile( cArgv[ argc - 1 ], sstream.str());
			
		}
		else
		{
			std::cout << "\nError: ";

			if (resInsFile != 0)
			{	
				std::cout << "-> Instruction file " << insFileNum << " was not succesfully loaded";
			}

			if (resDataFile != 0)
			{
				std::cout << "-> Data file " << dataFileNum << " was not succesfully loaded";
			}

			//if ((int)insFiles.size() != (int)dataFiles.size())
			if ((int)insFiles.size() != numOfDataFiles)
			{
				std::cout << "\n-> The number of instruction and data files does not match";
			}
		}

		// Release memory.
		for (int i = 0; i < numOfDataFiles; i++)
		{
			dataFiles[i].releaseMemory();
		}

		delete [] dataFiles;
		
	}
	else
	{
		std::cout << "\nError: ";
		std::cout << "-> Provide an instruction file path for each data file path and only one csv output file path";
	}
	
	// Print report.
	std::cout << "\n\nReport:";
	std::cout << "\n-> Running time: ";		
	std::cout << (double)(clock() - iStartClock)/(double)(CLOCKS_PER_SEC * 60) << " minutes\n\n";
	
	return 0;
}
