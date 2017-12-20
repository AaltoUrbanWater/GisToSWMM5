GisToSWMM is a tool for automatically constructing SWMM5 model
descriptions.

The tool was developed by Lassi Warsta (lassi@warsta.net) at Aalto
University School of Engineering and is distributed under MIT-license.

GisToSWMM5 is easiest to use with a script where the input files are
described. An example script is given in 'run_scripts' folder. The
script file should consist of the following lines, which are the inputs
to the GisToSWMM5:

[PATH TO GisToSWMM5 EXECUTABLE] \
[PATH TO DEM RASTER IN .asc FORMAT] \
[PATH TO FLOW DIRECTION RASTER IN .asc FORMAT] \
[PATH TO LANDUSE RASTER IN .asc FORMAT] \
[PATH TO CATCHMENT PROPERTIES TABLE IN .csv FORMAT] \
[PATH TO JUNCTIONS TABLE IN .csv FORMAT] \
[PATH TO CONDUITS TABLE IN .csv FORMAT] \
[PATH TO OUTFALLS TABLE IN .csv FORMAT] \
[PATH TO HEADER TABLE IN .csv FORMAT] \
[PATH TO EVAPORATION TABLE IN .csv FORMAT] \
[PATH TO TEMPERATURE TABLE IN .csv FORMAT] \
[PATH TO SNOWPACK TABLE IN .csv FORMAT] \
[PATH TO RAINGAGE TABLE IN .csv FORMAT] \
[PATH TO INFLOW TABLE IN .csv FORMAT] \
[PATH TO TIMESERIES TABLE IN .csv FORMAT] \
[PATH TO PUMP TABLE IN .csv FORMAT] \
[PATH TO PUMP CURVE TABLE IN .csv FORMAT] \
[PATH TO STORAGE TABLE IN .csv FORMAT] \
[PATH TO DWF TABLE IN .csv FORMAT] \
[PATH TO PATTERNS TABLE IN .csv FORMAT] \
[PATH TO LOSSES TABLE IN .csv FORMAT] \
[PATH TO CROSSECTIONS TABLE IN .csv FORMAT] \
[PATH TO REPORT TABLE IN .csv FORMAT] \
[PATH TO SYMBOLS TABLE IN .csv FORMAT] \
[PATH TO OUTPUT BASE FILENAME] \
[*OPTIONAL* INITIAL GRID CELL SIDE LENGTH] \
[*OPTIONAL* NUMBER OF SUBDIVISIONS FOR GRID CELLS]

NOTES:
DEM, landuse, and flow direction rasters have to be defined with the same
grid. The cell size of these grids defines the minimum
size of the final subcatchment grid size. Flow direction raster uses notation
where flow directions are marked with numbers from 1-8 so that 1 corresponds to
flow to Northeast and numbers increase in counterclockwise direction.

GisToSWMM outputs a SWMM input file ([PATH TO OUTPUT BASE FILENAME].inp)
as well as files defining the subcatchments ([PATH TO OUTPUT BASE
FILENAME]_subcatchments.wkt) and their routing ([PATH TO OUTPUT BASE
FILENAME]_subcatchment_routing.wkt) as defined in SWMM in Well Known
Text (wkt) vector format. E.g. QGIS can natively understand .wkt-files,
for conversion to shapefiles using ogr2ogr a .csvt file defining the
field types of .wkt file is created.


Folder 'utils' contains utility programs for various tasks.

To convert the .wkt files into shapefiles (e.g. for ArcMap) the
wkt2shp.sh tool can be used.

The "[PATH TO OUTPUT BASE FILENAME]_subcatchments.wkt" file has the
following attributes:
"id;";
"wkt;";         // Polygon defining the shape of the  subcatchment
"name;";        // Name of the  subcatchment
"outlet;";      // Name of node or another subcatchment that receives runoff
"area_m2;";     // Area of subcatchment (m2)
"slope_pct;";   // Average surface slope (%)
"landuse;";     // Code for landuse type
"imp_pct;";  	// Percent of impervious area (%)
"n_imp;";     	// Mannings N for impervious area (-)
"n_per;";       // Mannings N for pervious area (-)
"S_imp_mm;";  	// Depth of depression storage on impervious area (mm)
"S_per_mm;";    // Depth of depression storage on pervious area (mm)
"suct_mm;";   	// Soil capillary suction head (mm)
"Ksat_mmhr;";	// Soil saturated hydraulic conductivity (mm/hr)
"IMDmax;";      // Difference between soil porosity and initial moisture content (a fraction)
"isSink";		// An indicator showing if the cell is a local sink in the catchment

The "[PATH TO OUTPUT BASE FILENAME]_subcatchment_routing.wkt" file has
the following attributes:
"id;";
"wkt;";      // Line object defining the route between "from" and "to" subcatchments
"from;";     // Name of the origin subcatchment
"to";        // Name of the target subcatchment

To collate time series data from (several) SWMM5 report file(s) into a
single .csv file the tool ExtractDataSets.py can be used.

To extract subcatchment runoff summary results from SWMM report file and
to present them as attributes of a shapefile of runoff subcatchments the
ExtractSubcatchmentResults.py tool can be used.

To extract subcatchment polygons and routing between subcatchments from
a SWMM input file (*.inp) the tool inp2gis.py can be used. The tool
saves the subcatchments as a polygon shapefile and the routing between
the subcatchment polygon centroids as a line shapefile.


When using the GisToSWMM5 tool please refer to the publication:
Warsta, L., Niemi, T. J., Taka, M., Krebs, G., Haahti, K., Koivusalo,
H., & Kokkonen, T. (2017). Development and application of an automated
subcatchment generator for SWMM using open data. Urban Water Journal.
DOI:10.1080/1573062X.2017.1325496.

Details regarding GisToSWMM5 input files are available from:
Rautiainen, M. (2016, October 17). Hulevesimallinnus ja tulvariskin
arviointi Turun sataman valuma-alueella (Master’s Thesis). Aalto
University, Espoo, Finland. (In Finnish.)
http://urn.fi/URN:NBN:fi:aalto-201611025308
