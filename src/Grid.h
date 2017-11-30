// gcc
#ifndef _CSTDLIB_H_
#define _CSTDLIB_H_
#include <cstdlib>
#endif

// gcc
#ifndef _MATH_H_
#define _MATH_H_
#include <math.h>
#endif

#ifndef _SSTREAM_H_
#define _SSTREAM_H_
#include <sstream>
#endif

#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <vector>
#endif

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_
#include "Definitions.h"
#endif

#ifndef _VERTEX_H_
#define _VERTEX_H_
#include "Vertex.h"
#endif

#ifndef _RASTER_H_
#define _RASTER_H_
#include "Raster.h"
#endif

#ifndef _TABLE_H_
#define _TABLE_H_
#include "Table.h"
#endif

#ifndef _CELL_H_
#define _CELL_H_
#include "Cell.h"
#endif

// TJN 18 May 2017 START
#ifndef _STRING_H_
#define _STRING_H_
#include <string>
#endif
// TJN 18 May 2017 END

// TJN 30 Nov 2017 START
#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_
#include <algorithm>
#endif
// TJN 30 Nov 2017 END

class Grid
{
	public:
		// Methods.
		Grid();
		//Grid(int nColsNew, int nRowsNew);
		//Grid(Raster &landuseRaster, Raster &demRaster);
		int create(int gridTypeNew, Raster &landuseRaster, Raster &demRaster);
		int create(int gridTypeNew, double cellSizeMax, int maxSubdivisions, Raster &landuseRaster, Raster &demRaster);
		~Grid();
		void clear();

		void subdivideCell(double cellSize, double llcornerx, double llcornery, int index, int numberOfCells,
			Raster &landuseRaster, int subdivisions, int maxSubdivisions, std::vector<Cell> &cellsAdaptive);
		void setCellNames();
		void computeCellDimensions(double cellSizeNew);
		void computeCellCenterpoints(double xllCornerNew, double yllCornerNew);
		void setCellElevations(Raster &demRaster);
		void setCellFlowdirs(Raster &flowdirRaster);
		void setCellLanduse(Raster &landuseRaster);
		void computeGridExtents();
		void setSubcatchmentProperties(Table &catchPropTable);
		void findCellNeighbours();
		void routeCells();
		void routeCellsReg();   // TJN 29 Sep 2017
		void computeCellSlopes();
		void connectCellsToJunctions(Table &juncTable);
		void routePavedPitAndRooftopCells(Table &juncTable);
		void routePitCells();
		int simplify(std::string path);     // TJN 22 Nov 2017
		void saveRaster(std::string path);
		void saveSubcatchmentPolygon(std::string path); // TJN 17 May 2017
		void saveSubcatchmentRouting(std::string path); // TJN 18 May 2017

		void saveSWMM5File(Table &headerTable, Table &evaporationTable, Table &temperatureTable, Table &inflowsTable, Table &timeseriesTable, Table &reportTable,
			Table &snowpacksTable, Table &raingagesTable, Table &symbolsTable, Table &juncTable, Table &outfallsTable, Table &condTable,
			Table &pumpsTable, Table &pumpCurvesTable, Table &dwfTable, Table &patternsTable, Table &lossesTable, Table &storageTable, Table &xsectionTable, std::string path);
		void printReport(Table &catchPropTable);
		void computeHistogram(std::vector<std::string> &catLabels, std::vector<int> &catCount,
			double lowerLeftCornerX, double lowerLeftCornerY, double cellSizeLoc, Raster &landuseRaster);
		bool isSingleLanduse(double lowerLeftCornerX, double lowerLeftCornerY, double cellSizeLoc, Raster &landuseRaster);

		// Variables.
		Cell * cells;
		int nCols;
		int nRows;
		double cellSize;
		double xllCorner;
		double yllCorner;
		double urcorner[2];
		double llcorner[2];
		int gridType;             // -1 = undefined, 0 = regular grid and 1 = adaptive grid.
};
