# GisToSWMM5 input file instructions

The following input files are required to build SWMM5 models using GisToSWMM5:
- DEM raster in _.asc_ format
- Flow direction raster in _.asc_ format
- Landuse raster in _.asc_ format
- Catchment properties table in _.csv_ format
- Junctions table in _.csv_ format
- Conduits table in _.csv_ format
- Outfalls table in _.csv_ format
- Header table in _.csv_ format
- Evaporation table in _.csv_ format
- Temperature table in _.csv_ format
- Snowpack table in _.csv_ format
- Raingage table in _.csv_ format
- Inflow table in _.csv_ format
- Timeseries table in _.csv_ format
- Pump table in _.csv_ format
- Pump curve table in _.csv_ format
- Storage table in _.csv_ format
- DWF table in _.csv_ format
- Patterns table in _.csv_ format
- Losses table in _.csv_ format
- Crossections table in _.csv_ format
- Report table in _.csv_ format
- Symbols table in _.csv_ format

## DEM raster
DEM raster defines the elevations across the catchment in the same grid as in the landuse raster.  
File format is _Arc/Info ASCII Grid_. The file can be directly produced using e.g. ArcMap or QGIS.  
The header section needs to have the following information with appropriate values followed by the DEM raster values in a ncols*nrows space-separated list:  
ncols&emsp;&emsp;&emsp;&ensp;[aaa]  
nrows&emsp;&emsp;&ensp;&ensp;[bbb]  
xllcorner&emsp;&ensp;&ensp;[ccc]  
yllcorner&emsp;&ensp;&ensp;[ddd]  
cellsize&emsp;&emsp;&ensp;[eee]  
NODATA_value&emsp;[fff]  

## Flow direction raster
Flow direction raster defines the flow directions across the catchment in the same grid as in the landuse raster. Flow directions are defined according to D8-method such that each cell can flow into one of the 8 neighbouring cells. Directions are represented by integers and are defined as such for cell _X_:  

| |  |  |  | |
|: |:|:|:|:|
| | ```3``` | ```2``` | ```1``` | |
| | ```4``` |   _X_   | ```8``` | |
| | ```5``` | ```6``` | ```7``` | |   
| | | | | | |

File format is _Arc/Info ASCII Grid_. The file can be directly produced using e.g. ArcMap or QGIS.  
The header section needs to have the following information with appropriate values followed by the flow direction raster values in a ncols*nrows space-separated list:  
ncols&emsp;&emsp;&emsp;&ensp;[aaa]  
nrows&emsp;&emsp;&ensp;&ensp;[bbb]  
xllcorner&emsp;&ensp;&ensp;[ccc]  
yllcorner&emsp;&ensp;&ensp;[ddd]  
cellsize&emsp;&emsp;&ensp;[eee]  
NODATA_value&emsp;[fff]

## Land use raster
Land use raster defines the land use across the catchment in in each grid cell.  
File format is _Arc/Info ASCII Grid_. The file can be directly produced using e.g. ArcMap or QGIS.  
The header section needs to have the following information with appropriate values followed by the landuse raster values in a ncols*nrows space-separated list:  
ncols&emsp;&emsp;&emsp;&ensp;[aaa]  
nrows&emsp;&emsp;&ensp;&ensp;[bbb]  
xllcorner&emsp;&ensp;&ensp;[ccc]  
yllcorner&emsp;&ensp;&ensp;[ddd]  
cellsize&emsp;&emsp;&ensp;[eee]  
NODATA_value&emsp;```0```

GisToSWMM5 currently uses the following codes for different land use classes

| Code | Landuse       |
| :-----:|:-------------|
| ```10```   | Rock outcrops |
| ```20```   | Roofs connected to storm water network|
| ```21```   | Roofs not connected to storm water network|
| ```30```   | Sand and gravel |
| ```40```   | Asphalt |
| ```50```   | Stone paver |
| ```60```   |  Vegetation |
| ```70```   | Water |

NODATA_value of ```0``` is used to delineate areas not belonging to the catchment.

## Catchment properties table
Catchment properties table defines the properties of each GisToSWMM5 landuse class.  
File format is _.csv_ where each row defines the properties of one landuse type.  
The file must have a header row with the following attributes that are defined on following rows for each landuse type.

| Attribute |              |
| :--------:|:-------------|
| id        | landuse code (see theLanduse table) ) |
| imperv    | share of impervious areas (%)|
| dst_imp   |depression storage of impervious areas (mm)  |
| n_imper   | Roughness coefficient (Manning's n) of impervious areas |
| dst_per   | depression storage of pervious areas (mm) |
| n_perv    | Roughness coefficient (Manning's n) of pervious areas |
| percz_i   | share of impervious areas without depression storage (%) |
| rain_ga   | name of the rain gauge for this land-use class|
| conduct   | Green-Ampt infiltration model soil saturated hydraulic conductivity (mm/h) |
| initdef   | Green-Ampt infiltration model fraction of soil volume that is initially dry (-) |
| suction   | Green-Ampt infiltration model average soil capillary suction (mm) |
| Snowpacks | Name for the snow model used for the land use class |

## Junctions table
Junctions table defines the junction nodes of the drainage system, i.e., the points where channels and pipes connect together. For sewer systems they can be e.g. manholes/wells or connection fittings.  
File format is _.csv_ where each row defines the properties of one junction in the system. The file can be directly produced using e.g. ArcMap or QGIS.    
The file must have a header row with the following attributes that are defined on following rows for each junction.

| Attribute |              | Notes |
| :--------:|:-------------|-------|
| x0        | x-coordinate of the junction ||
| y0        | y-coordinate of the junction ||
| name      | unique name of the junction (e.g. j1, j2, ..., jn) ||
| elevation | elevation of the junction, i.e., ground surface (m) | This is unnecessary! |
| inv_elev  | invert elevation, i.e., the elevation of the channel or manhole bottom ||
| max_depth | depth of the well, i.e., elevation - inv_elev ||
| open      | ```0``` or ```1``` depending on whether the well is open or closed (```0``` = closed, ```1``` = open) ||
| InitDepth | water depth at the start of simulation (m; default 0) ||
| SurDepth  | maximum additional head above ground elevation that the manhole can sustain under surcharge conditions (m; default 0) ||
| Aponded   |ponded surface area when water depth exceeds max_depth (m^2; default 0) ||
| routing   | ```0``` or ```1``` depending on whether the junction is usable for routing water from roofs and pits in impervious areas (```0``` = non-routable, ```1``` = routable; default ```0```) |||

## Conduits table
Conduits table defines the conduits (pipes, links) of the drainage system, i.e., the pipes and channels that convey the water from one node to another.  
File format is _.csv_ where each row defines the properties of one conduit in the system. The file can be directly produced using e.g. ArcMap or QGIS.    
The file must have a header row with the following attributes that are defined on following rows for each conduit.

| Attribute |              | Notes |
| :--------:|:-------------||
| x0 | x-coordinate of the conduit start point ||
| y0 | y-coordinate of the conduit start point ||
| x1 | x-coordinate of the conduit end point ||
| y1 | y-coordinate of the conduit end point ||
| name | unique name of the conduit (e.g. c1, c2, ..., cn) ||
| elev_in | elevation of upstream end of conduit (m) | This is unnecessary! |
| elev_out | elevation of the downstream end of conduit (m) | This is unnecessary! |
| length | conduit length (m) ||
| junc_in | name of upstream node (junction) ||
| junc_out | name of downstream node (junction) ||
| roughness | Roughness of the conduit (Manning's n) ||
| elev_in_of | offset of upstream end of conduit invert (bottom) above the invert (bottom) elevation of the upstream node (junction), i.e., conduit bottom elevation - junction bottom elevation (m) ||
| elev_ou_of | offset of downstream end of conduit invert (bottom) above the invert (bottom) elevation of the downstream node (junction), i.e., conduit bottom elevation - junction bottom elevation (m) |||

## Outfalls table
Outfalls table defines the outfall nodes of the drainage system, i.e., the final downstream boundaries, and the corresponding water stage elevation.
File format is _.csv_ where each row defines the properties of one outfall in the system. The file can be directly produced using e.g. ArcMap or QGIS.    
The file must have a header row with the following attributes that are defined on following rows for each outfall.

| Attribute |              | Notes |
| :--------:|:-------------||
| x-coord | x-coordinate of the outfall ||
| y-coord | y-coordinate of the outfall ||
| Outfall | unique name of the outfall (e.g. o1, o2, ..., on) ||
| Invert | invert (bottom) elevation of the outfall ||
| Type | type of the outfall (```FREE```, ```NORMAL```, ```FIXED```, ```TIDAL```, or ```TIMESERIES```; default ```FREE```) ||
| Stage_data | depending on the _Type_ of outfall, either elevation of fixed stage outfall (m) OR name of curve containing tidal height OR name of time series describing how outfall stage varies in time; default [empty] ||
| Gated | ```YES``` or ```NO``` depending on whether a flap gate is present that prevents reverse flow; default ```NO``` |||

## Header table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Evaporation table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Temperature table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Snowpack table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Raingage table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Inflow table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Timeseries table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Pump table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Pump curve table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Storage table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## DWF table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Patterns table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Losses table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Crossections table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Report table
| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Symbols table


| Attribute |              | Notes |
| :--------:|:-------------||
|  |  ||
|  |  ||
|  |  ||
|  |  |||
