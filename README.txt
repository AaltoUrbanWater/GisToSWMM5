17 Feb 2017 Tero Niemi

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
[PATH TO OUTPUT SUBCATCHMENT POLYGON FILE IN .wkt FORMAT] \
[PATH TO OUTPUT SWMM INPUT FILE IN .inp FORMAT] \
[*OPTIONAL* INITIAL GRID CELL SIDE LENGTH] \
[*OPTIONAL* NUMBER OF SUBDIVISIONS FOR GRID CELLS] \

Note that DEM raster and landuse raster have to be defined with the same
grid and that the cell size of these grids defines the minimum size of 
the final subcatchment grid size.

When using the tool please refer to the publication: 
Warsta, L., Niemi, T. J., Taka, M., Krebs, G., Haahti, K., Koivusalo, 
H., & Kokkonen, T. (2017). Development and application of an automated 
subcatchment generator for SWMM using open data. Urban Water Journal, 
submitted manuscript.

Details regarding GisToSWMM5 input files are available from:
Rautiainen, M. (2016, October 17). Hulevesimallinnus ja tulvariskin 
arviointi Turun sataman valuma-alueella (Master’s Thesis). Aalto 
University, Espoo, Finland. (In Finnish.)
http://urn.fi/URN:NBN:fi:aalto-201611025308
  
