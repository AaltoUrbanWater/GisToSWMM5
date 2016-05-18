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
	std::cout << "\n*                     RasterToSWMM5                        *"; 
	std::cout << "\n*    Creates a computational grid for the SWMM5 model.     *"; 
	std::cout << "\n*     The program is developed under the MIT license.      *";
	std::cout << "\n*                (C) Aalto University 2015.                *";
	std::cout << "\n************************************************************";

	// Print commandline arguments.
	std::cout << "\n\nCommand line arguments:";

	for (int i = 0; i < argc; i++)
	{
		std::cout << "\n-> Argument " << i << ": " << cArgv[i];
	}

	if (argc == 16 || argc == 17)
	{		
		// Load the digital elevation map raster file.
		std::cout << "\n\nLoading data:";
		std::cout << "\n-> Loading digital elevation map raster";
		Raster demRaster;
		int resDem = demRaster.load( cArgv[1] );
		
		// Load the landuse raster file
		std::cout << "\n-> Loading landuse raster";
		Raster landuseRaster;
		int resLanduse = landuseRaster.load( cArgv[2] );

		// Load catchment properties table.
		std::cout << "\n-> Loading catchment properties table";
		Table catchPropTable;
		int resCatchProp = catchPropTable.load( cArgv[3] );
		
		// Load network junctions table.
		std::cout << "\n-> Loading network junctions table";
		Table juncTable;
		int resJunc = juncTable.load( cArgv[4] );

		// Load network conduits table.
		std::cout << "\n-> Loading network conduits table";
		Table condTable;
		int resCond = condTable.load( cArgv[5] );
		
		// Load network outfalls table.
		std::cout << "\n-> Loading network outfalls table";
		Table outfallsTable;
		int resOutfalls = outfallsTable.load( cArgv[6] );
		
		// Load SWMM5 input file header.
		std::cout << "\n-> Loading SWMM5 input file header table";
		Table headerTable;
		int resHeader = headerTable.load( cArgv[7] );
		
		// Load SWMM5 input file evaporation settings.
		std::cout << "\n-> Loading SWMM5 input file evaporation table";
		Table evaporationTable;
		int resEvaporation = evaporationTable.load( cArgv[8] );

		// Load SWMM5 input file temperature settings.
		std::cout << "\n-> Loading SWMM5 input file temperature table";
		Table temperatureTable;
		int resTemperature = temperatureTable.load( cArgv[9] );

		// Load snowpacks table.
		std::cout << "\n-> Loading SWMM5 input file snowpacks table";
		Table snowpacksTable;
		int resSnowpacks = snowpacksTable.load( cArgv[10] );

		// Load raingage table.
		std::cout << "\n-> Loading SWMM5 input file raingages table";
		Table raingagesTable;
		int resRaingages = raingagesTable.load( cArgv[11] );

		// Load SWMM5 input file report settings.
		std::cout << "\n-> Loading SWMM5 input file report settings table";
		Table reportTable;
		int resReport = reportTable.load( cArgv[12] );

		// Load SWMM5 input file symbols.
		std::cout << "\n-> Loading SWMM5 input file symbols table";
		Table symbolsTable;
		int resSymbols = symbolsTable.load( cArgv[13] );

		// Create the computational grid.
		std::cout << "\n\nCreating the computational grid:";
		
		if (resDem == 0 && resLanduse == 0 && resCatchProp == 0 && resCatchProp == 0 && 
			resCond == 0 && resOutfalls == 0 && resHeader == 0 && resEvaporation == 0 && 
			resTemperature == 0 && resRaingages == 0 && resReport == 0 && resSymbols == 0
			 && resSnowpacks == 0)
		{
			if (argc == 16)
			{
				// Reserve memory for the grid.
				Grid grid( landuseRaster.nCols, landuseRaster.nRows );
			
				// Set cell names.
				std::cout << "\n-> Setting cell names";
				grid.setCellNames();

				// Set cell dimensions.
				std::cout << "\n-> Setting cell dimensions";
				grid.computeCellDimensions(landuseRaster.cellSize);
			
				// Compute cell centerpoints.
				std::cout << "\n-> Computing cell centerpoint coordinates";
				grid.computeCellCenterpoints(landuseRaster.xllCorner, landuseRaster.yllCorner);
			
				// Set cell elevations.
				std::cout << "\n-> Setting cell elevations";
				grid.setCellElevations(demRaster);
			
				// Set cell landuse.
				std::cout << "\n-> Setting cell landuses";
				grid.setCellLanduse(landuseRaster);
			
				// Compute active grid extents.
				std::cout << "\n-> Computing active grid extents";
				grid.computeGridExtents();
			
				// Save cell properties.
				std::cout << "\n-> Saving cell properties";
				grid.setSubcatchmentProperties(catchPropTable);

				// Find cell neighbours.
				std::cout << "\n-> Finding cell neighbours";
				grid.findCellNeighbours();

				// Route cells to a neighbour cells. Also update flow width parameter of cells.
				std::cout << "\n-> Routing cells to neighbour cells and updating flow width parameter";
				grid.routeCells();

				// Compute cell slopes.
				std::cout << "\n-> Computing cell slopes";
				grid.computeCellSlopes();
			
				// Set the outlets of cells with junctions to the junctions.
				std::cout << "\n-> Connecting cells to junctions";
				grid.connectCellsToJunctions(juncTable);

				// Set outlet of rooftops cells and cells withouth outlets to the nearest junction.
				std::cout << "\n-> Routing rooftop cells and other artificial impermeable pit cells to the nearest junctions";
				grid.routePavedPitAndRooftopCells(juncTable);
			
				// Set outlets of cells without outlets to themselves.
				std::cout << "\n-> Routing natural (rock, vegetation and sand) pit cells to themselves";
				grid.routePitCells();

				// Create and save a raster for inspection.
				std::cout << "\n-> Creating an output raster for inspection";
				grid.saveRaster(cArgv[14]);
			
				// Create the SWMM5 file.
				std::cout << "\n\nCreating the SWMM5 model input file:";
				grid.saveSWMM5File(headerTable, evaporationTable, temperatureTable, reportTable, snowpacksTable, raingagesTable, symbolsTable, juncTable, outfallsTable, condTable, cArgv[15]);

				// Print report.
				std::cout << "\n\nReport:";
				std::cout << "\n-> Running time: ";		
				std::cout << (double)(clock() - iStartClock)/(double)(CLOCKS_PER_SEC * 60) << " minutes";
				grid.printReport(catchPropTable);
				std::cout << "\n\n";
			}
			else if (argc == 17)
			{
				// .
				std::cout << "\n\nDiscretize the area with the adaptive algorithm:";

				// Discretize the area with the adaptive algorithm.
				std::cout << "\n-> Creating grid cells";
				Grid grid(landuseRaster, demRaster);

				// Set cell names.
				std::cout << "\n-> Setting cell names";
				grid.setCellNames();

				// Compute active grid extents.
				std::cout << "\n-> Computing active grid extents";
				grid.computeGridExtents();

				// Save cell properties.
				std::cout << "\n-> Saving cell properties";
				grid.setSubcatchmentProperties(catchPropTable);

				// Create the SWMM5 file.
				std::cout << "\n\nCreating the SWMM5 model input file:";
				grid.saveSWMM5File(headerTable, evaporationTable, temperatureTable, reportTable, snowpacksTable, raingagesTable, symbolsTable, juncTable, outfallsTable, condTable, cArgv[15]);

				// Print report.
				std::cout << "\n\nReport:";
				std::cout << "\n-> Running time: ";		
				std::cout << (double)(clock() - iStartClock)/(double)(CLOCKS_PER_SEC * 60) << " minutes";
				std::cout << "\n\n";
			}
		}
		else
		{
			if (resDem == 1)
			{
				std::cout << "\nError, digital elevation map raster was not succesfully loaded.";
			}

			if (resLanduse == 1)
			{
				std::cout << "\nError, land use raster was not succesfully loaded.";
			}

			if (resCatchProp == 1)
			{
				std::cout << "\nError, catchment properties table was not succesfully loaded.";
			}

			if (resJunc == 1)
			{
				std::cout << "\nError, network junctions table was not succesfully loaded.";
			}

			if (resCond == 1)
			{
				std::cout << "\nError, network conduits table was not succesfully loaded.";
			}

			if (resOutfalls == 1)
			{
				std::cout << "\nError, network outfalls table was not succesfully loaded.";
			}

			if (resHeader == 1)
			{
				std::cout << "\nError, SWMM5 input file header table was not succesfully loaded.";
			}

			if (resEvaporation == 1)
			{
				std::cout << "\nError, SWMM5 input file evaporation table was not succesfully loaded.";
			}

			if (resTemperature == 1)
			{
				std::cout << "\nError, SWMM5 input file temperature table was not succesfully loaded.";
			}

			if (resSnowpacks == 1)
			{
				std::cout << "\nError, SWMM5 input file snowpacks table was not succesfully loaded.";
			}

			if (resRaingages == 1)
			{
				std::cout << "\nError, SWMM5 input file raingages table was not succesfully loaded.";
			}

			if (resReport == 1)
			{
				std::cout << "\nError, SWMM5 input file report table was not succesfully loaded.";
			}

			if (resSymbols == 1)
			{
				std::cout << "\nError, SWMM5 input file symbols table was not succesfully loaded.";
			}
		}
	}
	else
	{
		std::cout << "\nError, number of command line arguments should be 16 or 17.";
	}
	
	return 0;
}
