#ifndef _MAIN_H_
#define _MAIN_H_
#include "main.h"
#endif

// Number of command line parameters.
const int regGridParams = 25;       // TJN 18 May 2017 25 -> 24; TJN 29 Sep 2017 24 -> 25
const int adapGridParams = 26;      // TJN 21 Nov 2017: Enable new adaptive grid
const int adapGridParamsOld = 27;      // TJN 18 May 2017 27 -> 26; TJN 29 Sep 2017 26 -> 27

int main (int argc, char* cArgv[])
{
    // Introduce timing related objects.
    int iStartClock = clock(); // For Windows version

    // Print header.
    std::cout << "\n\n************************************************************";
    std::cout << "\n*                       GisToSWMM5                         *";
    std::cout << "\n*           Creates a SWMM5 computational model.           *";
    std::cout << "\n*     The program is developed under the MIT license.      *";
    std::cout << "\n*                (C) Aalto University 2016.                *";
    std::cout << "\n************************************************************";

    // Print commandline arguments.
    std::cout << "\n\nCommand line arguments:";

    for (int i = 0; i < argc; i++)
    {
        std::cout << "\n-> Argument " << i << ": " << cArgv[i];
    }

    if (argc == regGridParams || argc == adapGridParams || argc == adapGridParamsOld)
    {
        std::cout << "\n\nLoading data:";

        // Load the digital elevation map raster file.
        std::cout << "\n-> Loading digital elevation map raster";
        Raster demRaster;
        int resDem = demRaster.load( cArgv[1] );

        // TJN 29 Sep 2017 START
        // Load the flow direction raster file.
        std::cout << "\n-> Loading flow direction raster";
        Raster flowdirRaster;
        int resFlowdirRaster = flowdirRaster.load( cArgv[2] );

        // Load the landuse raster file
        std::cout << "\n-> Loading landuse raster";
        Raster landuseRaster;
        int resLanduse = landuseRaster.load( cArgv[3] );

        // Load catchment properties table.
        std::cout << "\n-> Loading catchment properties table";
        Table catchPropTable;
        int resCatchProp = catchPropTable.load( cArgv[4] );

        // Load network junctions table.
        std::cout << "\n-> Loading network junctions table";
        Table juncTable;
        int resJunc = juncTable.load( cArgv[5] );

        // Load network conduits table.
        std::cout << "\n-> Loading network conduits table";
        Table condTable;
        int resCond = condTable.load( cArgv[6] );

        // Load network outfalls table.
        std::cout << "\n-> Loading network outfalls table";
        Table outfallsTable;
        int resOutfalls = outfallsTable.load( cArgv[7] );

        // Load input file header.
        std::cout << "\n-> Loading input file header table";
        Table headerTable;
        int resHeader = headerTable.load( cArgv[8] );

        // Load input file evaporation settings.
        std::cout << "\n-> Loading input file evaporation table";
        Table evaporationTable;
        int resEvaporation = evaporationTable.load( cArgv[9] );

        // Load input file temperature settings.
        std::cout << "\n-> Loading input file temperature table";
        Table temperatureTable;
        int resTemperature = temperatureTable.load( cArgv[10] );

        // Load snowpacks table.
        std::cout << "\n-> Loading input file snowpacks table";
        Table snowpacksTable;
        int resSnowpacks = snowpacksTable.load( cArgv[11] );

        // Load raingage table.
        std::cout << "\n-> Loading input file raingages table";
        Table raingagesTable;
        int resRaingages = raingagesTable.load( cArgv[12] );

        // Load inflows table.
        std::cout << "\n-> Loading inflows table";
        Table inflowsTable;
        int resInflows = inflowsTable.load(cArgv[13]);

        // Load time series table.
        std::cout << "\n-> Loading time series table";
        Table timeseriesTable;
        int resTimeseries = timeseriesTable.load(cArgv[14]);

        // Load pump table.
        std::cout << "\n-> Loading pump table";
        Table pumpsTable;
        int resPumps = pumpsTable.load(cArgv[15]);

        // Load pump curves table.
        std::cout << "\n-> Loading pump curves table";
        Table pumpCurvesTable;
        int resPumpCurves = pumpCurvesTable.load(cArgv[16]);

        // Load storage table.
        std::cout << "\n-> Loading storage table";
        Table storageTable;
        int resStorages = storageTable.load(cArgv[17]);

        // Load dwf table.
        std::cout << "\n-> Loading dwf table";
        Table dwfTable;
        int resDwf = dwfTable.load(cArgv[18]);

        // Load patterns table.
        std::cout << "\n-> Loading patterns table";
        Table patternsTable;
        int resPatterns = patternsTable.load(cArgv[19]);

        // Load losses table.
        std::cout << "\n-> Loading losses table";
        Table lossesTable;
        int resLosses = lossesTable.load(cArgv[20]);

        // Load xsections table.
        std::cout << "\n-> Loading xsection table";
        Table xsectionTable;
        int resXsections = xsectionTable.load(cArgv[21]);

        // Load input file report settings.
        std::cout << "\n-> Loading input file report settings table";
        Table reportTable;
        int resReport = reportTable.load( cArgv[22] );

        // Load input file symbols.
        std::cout << "\n-> Loading input file symbols table";
        Table symbolsTable;
        int resSymbols = symbolsTable.load( cArgv[23] );

        if (resDem == 0 && resLanduse == 0 && resCatchProp == 0 && resCatchProp == 0 &&
                resCond == 0 && resOutfalls == 0 && resHeader == 0 && resEvaporation == 0 &&
                resTemperature == 0 && resRaingages == 0 && resInflows == 0 && resTimeseries == 0
                && resPumps == 0 && resPumpCurves == 0 && resStorages == 0 && resDwf == 0
                && resPatterns == 0 && resLosses == 0 && resXsections == 0 && resReport == 0
                && resSymbols == 0 && resSnowpacks == 0)
        {
            // Create the computational grid.
            std::cout << "\n\nCreating the computational grid:";
            int gridType = -1;
            Grid grid;
            int resDiscretization = 1;

            if (argc == regGridParams || argc == adapGridParams)
            {
                // Print discretization method.
                std::cout << "\n-> Discretizing the area with a regular grid";
                gridType = 0;
                resDiscretization = grid.create(gridType, landuseRaster, demRaster);
            }
            else if (argc == adapGridParamsOld)
            {
                // Print discretization method.
                std::cout << "\n-> Discretizing the area with a rectangular adaptive grid:";
                std::cout << "\n-> Initial grid cell size: " << cArgv[25] << "x" << cArgv[25] << "m^2";
                std::cout << "\n-> Number of subdivisions: " << cArgv[26];
                gridType = 1;
                resDiscretization = grid.create(gridType, atof(cArgv[25]), atoi(cArgv[26]), landuseRaster, demRaster);
            }

            if (resDiscretization != 0)
            {
                std::cout << "\n-> Error in the discretization stage of the grid creation.";

                return 1;
            }

            // Set cell names.
            std::cout << "\n-> Setting cell names";
            grid.setCellNames();

            if (gridType == 0)
            {
                // Set cell dimensions.
                std::cout << "\n-> Setting cell dimensions";
                grid.computeCellDimensions(landuseRaster.cellSize);

                // Compute cell centerpoints.
                std::cout << "\n-> Computing cell centerpoint coordinates";
                grid.computeCellCenterpoints(landuseRaster.xllCorner, landuseRaster.yllCorner);
            }

            // Set cell elevations.
            std::cout << "\n-> Setting cell elevations";
            grid.setCellElevations(demRaster);

            if (gridType == 0)
            {
                // TJN 29 Sep 2017 START
                // Set cell flow directions
                std::cout << "\n-> Setting cell flow directions";
                grid.setCellFlowdirs(flowdirRaster);
                // TJN 29 Sep 2017 END

                // Set cell landuse.
                std::cout << "\n-> Setting cell landuses";
                grid.setCellLanduse(landuseRaster);
            }

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
            // TJN 29 Sep 2017 START
            // Use different routing for regular and adaptive grids
            if (gridType == 0)
            {
                std::cout << "\n-> Routing cells to neighbour cells using flow direction raster and updating flow width parameter";
                grid.routeCellsReg();
            }
            else if (gridType == 1)
            {
                std::cout << "\n-> Routing cells to neighbour cells and updating flow width parameter";
                grid.routeCells();
            }
            // TJN 29 Sep 2017 END

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

            // TJN 22 Nov 2017 START
            if (argc == adapGridParams)
            {
                // TJN 7 Dec 2017
                grid.findRouted(condTable, juncTable);

//                int resSimplifying = 1;
//
//                std::cout << "\n-> Simplifying subcatchments based on  landuse and routing";
//                resSimplifying = grid.simplify(cArgv[24]);
//
//                if (resSimplifying != 0)
//                {
//                    std::cout << "\n-> Error in the simplifying stage of the grid creation.";
//                    return 1;
//                }
            }

            // TJN 17 May 2017 START
            // Create a vector output of the SWMM subcatchment grid instead of raster grid

//			if (argc == regGridParams || adapGridParams)
//			{
//				// Create and save a raster for inspection.
//				std::cout << "\n-> Creating an output raster for inspection";
//				grid.saveRaster(cArgv[24]);
//			}

            // Create and save a WKT vector file of subcatchment polygons
            std::cout << "\n-> Creating a subcatchment polygon file for inspection";
            grid.saveSubcatchmentPolygon(cArgv[24]);
            // TJN 17 May 2017 END

            // TJN 18 May 2017 START
            // Create and save a WKT vector file of subcatchment routing
            std::cout << "\n-> Creating a subcatchment routing file for inspection";
            grid.saveSubcatchmentRouting(cArgv[24]);
            // TJN 18 May 2017 END

//            // TJN 8 Dec 2017 START
//            // Create and save a WKT vector file of subcatchment routing
//            std::cout << "\n-> Creating a network routing file for inspection";
//            grid.saveNetworkRouting(cArgv[24], condTable);
//            // TJN 8 Dec 2017 END

            // Create the SWMM5 file.
            std::cout << "\n\nCreating the SWMM5 model input file:";
            grid.saveSWMM5File(headerTable, evaporationTable, temperatureTable, inflowsTable, timeseriesTable,
                               reportTable, snowpacksTable, raingagesTable, symbolsTable, juncTable, outfallsTable, condTable, pumpsTable, pumpCurvesTable, dwfTable, patternsTable, lossesTable, storageTable, xsectionTable, cArgv[24]);

            // Print report.
            std::cout << "\n\nReport:";
            std::cout << "\n-> Running time: ";
            std::cout << (double)(clock() - iStartClock)/(double)(CLOCKS_PER_SEC * 60) << " minutes";
            grid.printReport(catchPropTable);
            std::cout << "\n\n";
        }
        else
        {
            if (resDem > 0)
            {
                std::cout << "\nError, digital elevation map raster was not succesfully loaded.";
            }

            if (resFlowdirRaster > 0)
            {
                std::cout << "\nError, flow direction raster was not succesfully loaded.";
            }

            if (resLanduse > 0)
            {
                std::cout << "\nError, land use raster was not succesfully loaded.";
            }

            if (resCatchProp > 0)
            {
                std::cout << "\nError, catchment properties table was not succesfully loaded.";
            }

            if (resJunc > 0)
            {
                std::cout << "\nError, network junctions table was not succesfully loaded.";
            }

            if (resCond > 0)
            {
                std::cout << "\nError, network conduits table was not succesfully loaded.";
            }

            if (resOutfalls > 0)
            {
                std::cout << "\nError, network outfalls table was not succesfully loaded.";
            }

            if (resHeader > 0)
            {
                std::cout << "\nError, input file header table was not succesfully loaded.";
            }

            if (resEvaporation > 0)
            {
                std::cout << "\nError, input file evaporation table was not succesfully loaded.";
            }

            if (resTemperature > 0)
            {
                std::cout << "\nError, input file temperature table was not succesfully loaded.";
            }

            if (resSnowpacks > 0)
            {
                std::cout << "\nError, input file snowpacks table was not succesfully loaded.";
            }

            if (resRaingages > 0)
            {
                std::cout << "\nError, input file raingages table was not succesfully loaded.";
            }

            if (resInflows > 0)
            {
                std::cout << "\nError, inflows table was not succesfully loaded.";
            }

            if (resTimeseries > 0)
            {
                std::cout << "\nError, time series table was not succesfully loaded.";
            }

            if (resPumps > 0)
            {
                std::cout << "\nError, pump table was not succesfully loaded.";
            }

            if (resPumpCurves > 0)
            {
                std::cout << "\nError, pump curve table was not succesfully loaded.";
            }

            if (resStorages > 0)
            {
                std::cout << "\nError, storage table was not succesfully loaded.";
            }

            if (resDwf > 0)
            {
                std::cout << "\nError, dwf table was not succesfully loaded.";
            }

            if (resPatterns > 0)
            {
                std::cout << "\nError, patterns table was not succesfully loaded.";
            }

            if (resLosses > 0)
            {
                std::cout << "\nError, losses table was not succesfully loaded.";
            }

            if (resXsections > 0)
            {
                std::cout << "\nError, xsection table was not succesfully loaded.";
            }

            if (resReport > 0)
            {
                std::cout << "\nError, input file report table was not succesfully loaded.";
            }

            if (resSymbols > 0)
            {
                std::cout << "\nError, input file symbols table was not succesfully loaded.";
            }
        }
    }
    else
    {
        std::cout << "\nError, number of command line arguments should be " << regGridParams << " or " <<  adapGridParamsOld;
    }

    return 0;
}

