#ifndef _STRING_H_
#define _STRING_H_
#include <string>
#endif

#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <vector>
#endif

class Cell
{
	public:
		// Methods.
		Cell();
		~Cell();


		// Variables.
		std::string name;
		double centerCoordX;
		double centerCoordY;
		double elevation;
		int flowdir;
		double cellSize;
		double slope;                // Average surface slope (%)
		double area;                 // Area of subcatchment (ha)
		double flowWidth;            // Width of overland flow (m)
		int landuse;
		double outletCoordX;         // X coordinate of node or another subcatchment that receives runoff      // TJN 18 May 2017
		double outletCoordY;         // Y coordinate of node or another subcatchment that receives runoff      // TJN 18 May 2017
		int outletID;                // Index of the outlet cell    // TJN 23 Nov 2017
		int subcatchmentID;          // Id of the subcatchment this cell belongs to // TJN 23 Nov 2017
		std::string outlet;          // Name of node or another subcatchment that receives runoff
		std::string raingage;        // Rain gage assigned to subcatchment (name)
		std::string imperv;          // Percent of impervious area (%)
		std::string snowPack;        // Name of snow pack parameter set (for snow melt analysis only)
		std::string N_Imperv;        // Mannings N for impervious area (-)
		std::string N_Perv;          // Mannings N for pervious area (-)
		std::string S_Imperv;        // Depth of depression storage on impervious area (mm)
		std::string S_Perv;          // Depth of depression storage on pervious area (mm)
		std::string PctZero;		 		 // percent of impervious area with no depression storage (%)
		std::string RouteTo;         // choice of internal routing between pervious and impervious sub-areas (OUTLET)
		std::string PctRouted;       // Percent of runoff routed between sub-areas (%)
		std::string Suction;         // Soil capillary suction head (mm)
		std::string HydCon;          // Soil saturated hydraulic conductivity (mm/hr)
		std::string IMDmax;          // Difference between soil porosity and initial moisture content (a fraction)
		//int neighCellIndices[8];
		std::vector<int> neighCellIndices;
		//int flowDirection;           // To save diagonal direction.
		std::vector<double> distanceToNeighbours;
		int isSink;                  // Indcator for the cells GisToSWMM determines as local sinks in the catchment // TJN 25 Sep 2017
                                     // By default all cells are routed (0),
                                     // sinks with no routing are marked with 1, and
                                     // sinks with forceful routing (i.e. connected roofs) are marked with 2
};
