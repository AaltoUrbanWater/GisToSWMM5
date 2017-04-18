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

#include <cmath>

class Grid
{
	public:
		// Methods.
		Grid();
		Grid(int nColsNew, int nRowsNew);
		Grid(Raster &landuseRaster, Raster &demRaster);
		~Grid();

		void setCellNames();
		void computeCellDimensions(double cellSizeNew);
		void computeCellCenterpoints(double xllCornerNew, double yllCornerNew);
		void setCellElevations(Raster &demRaster);
		void setCellLanduse(Raster &landuseRaster);
		void computeGridExtents();
		void setSubcatchmentProperties(Table &catchPropTable);
		void findCellNeighbours();
		void routeCells();
		void computeCellSlopes();
		void connectCellsToJunctions(Table &juncTable);
		void routePavedPitAndRooftopCells(Table &juncTable);
		void routePitCells();
		void saveRaster(std::string path);
		void saveSWMM5File(Table &headerTable, Table &evaporationTable, Table &temperatureTable, Table &reportTable, Table &snowpacksTable, Table &raingagesTable, Table &symbolsTable, Table &juncTable, Table &outfallsTable, Table &condTable, Table &xsectionTable, std::string path);
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
};
