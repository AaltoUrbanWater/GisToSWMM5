# GisToSWMM5 input file instructions

The following input files are required to build SWMM5 models using GisToSWMM5:
- DEM raster in _.asc_ format
- Flow direction raster in _.asc_ format
- Landuse raster in _.asc_ format
- Catchment properties table in _.csv_ format
- Junctions table in _.csv_ format
- Conduits table in _.csv_ format
- Outfalls table in _.csv_ format
- <sup>1</sup>Header table in _.csv_ format
- <sup>1</sup>Evaporation table in _.csv_ format
- <sup>1</sup>Temperature table in _.csv_ format
- <sup>1</sup>Snowpack table in _.csv_ format
- <sup>1</sup>Raingage table in _.csv_ format
- <sup>1</sup>Inflow table in _.csv_ format
- <sup>1</sup>Timeseries table in _.csv_ format
- <sup>1</sup>Pump table in _.csv_ format
- <sup>1</sup>Curve table in _.csv_ format
- Storage table in _.csv_ format
- <sup>1</sup>Dry weather flow (DWF) table in _.csv_ format
- <sup>1</sup>Patterns table in _.csv_ format
- <sup>1</sup>Losses table in _.csv_ format
- <sup>1</sup>Crossections table in _.csv_ format
- <sup>1</sup>Report table in _.csv_ format
- <sup>1</sup>Symbols table in _.csv_ format

<sup>1</sup>Corresponds to section from the standard SWMM input file (_.inp_)

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

GisToSWMM5 currently uses the following codes for different land use classes.
The different integers mark different landuse classes and (optionally) different landuse parameters specified in the Catchment properties table for each landuse class.
The classes are loosely grouped into roofs, built areas, and natural areas, and especially for roofs these groups should be used.
Generally it is good practice to use same subclass values for connected and unconnected roofs to keep things clear.
E.g. 10 should have the same parameters as 20 but 10 are directly connected to junctions whereas 20 are connected to yards, in the same manner 11 should correspond to 21, etc.
For other classes there is no such connection between classes.

| Code | Landuse       |
| :-----:|:-------------|
| ```10-19```   | Connected roof |
| ```20-29```   | Unconnected roof |
| ```30-59```   | Built area|
| ```60-```   | Natural area |

Previously (before March 2018) the following codes were used

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
| id        | Landuse code (see the Landuse table) ) |
| imperv    | Share of impervious areas (%)|
| dst_imp   | Depression storage of impervious areas (mm)  |
| n_imper   | Roughness coefficient (Manning's n) of impervious areas |
| dst_per   | Depression storage of pervious areas (mm) |
| n_perv    | Roughness coefficient (Manning's n) of pervious areas |
| percz_i   | Share of impervious areas without depression storage (%) |
| rain_ga   | Name of the rain gauge for this land-use class|
| conduct   | Green-Ampt infiltration model soil saturated hydraulic conductivity (mm/h) |
| initdef   | Green-Ampt infiltration model fraction of soil volume that is initially dry (-) |
| suction   | Green-Ampt infiltration model average soil capillary suction (mm) |
| Snowpacks | Name for the snow model used for the land use class |
| [Tag]     | [Optional tag for the landuse type] |

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
| routing   | ```0``` or ```1``` depending on whether the junction is usable for routing water from roofs and pits in impervious areas (```0``` = non-routable, ```1``` = routable; default ```1```) |||

## Conduits table
Conduits table defines the conduits (pipes, links) of the drainage system, i.e., the pipes and channels that convey the water from one node to another.  
File format is _.csv_ where each row defines the properties of one conduit in the system. The file can be directly produced using e.g. ArcMap or QGIS.    
The file must have a header row with the following attributes that are defined on following rows for each conduit.

| Attribute |              | Notes |
| :--------:|:-------------|-------|
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

| Attribute |              |
| :--------:|:-------------|
| x-coord | x-coordinate of the outfall |
| y-coord | y-coordinate of the outfall |
| Outfall | unique name of the outfall (e.g. o1, o2, ..., on) |
| Invert | invert (bottom) elevation of the outfall |
| Type | type of the outfall (```FREE```, ```NORMAL```, ```FIXED```, ```TIDAL```, or ```TIMESERIES```; default ```FREE```) |
| Stage_data | depending on the _Type_ of outfall, either elevation of fixed stage outfall (m) OR name of curve containing tidal height OR name of time series describing how outfall stage varies in time; default [empty] |
| Gated | ```YES``` or ```NO``` depending on whether a flap gate is present that prevents reverse flow; default ```NO``` |

## Header table
Header table provides the values for various analysis options used by SWMM.  
File format is _.csv_ where each row defines a value for a single SWMM analysis option.
Each row contains the analysis option name and the option value as in the table below separated by comma ','.  
The file must have a header row and the necessary parameters from the following table defining model analysis options.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```OPTIONS```"

| Option | Value             | Notes |
| :--------|:-------------|-------|
| FLOW_UNITS | ```CFS``` / ```GPM``` / ```MGD``` / ```CMS``` / ```LPS``` / ```MLD``` | Choice of flow units. All quantities in the model are according to this option. Default: ```CFS``` |
| INFILTRATION | ```HORTON``` / ```MODIFIED_HORTON``` / ```GREEN_AMPT``` / ```MODIFIED_GREEN_AMPT``` / ```CURVE_NUMBER``` | A model for computing infiltration of rainfall into the upper soil zone of subcatchments. Default: ```HORTON``` |
| FLOW_ROUTING | ```STEADY``` / ```KINWAVE``` / ```DYNWAVE``` | A method used to route flows through the drainage system. Default: ```KINWAVE```|
| LINK_OFFSETS | ```DEPTH``` / ```ELEVATION``` | The convention used to specify the position of a link offset above the invert of its connecting node. Default: ```DEPTH```, which is also assumed in this document.|
| FORCE_MAIN_EQUATION | ```H-W``` / ```D-W``` | Whether the Hazen-Williams (```H-W```) or the Darcy-Weisbach (```D-W```) equation will be used to compute friction losses for pressurized flow in conduits assigned with Circular Force Main cross-section shape. Default: ```H-W``` |
| IGNORE_RAINFALL | ```YES``` / ```NO``` | ```YES``` if all rainfall data and runoff calculations should be ignored. Default: ```NO``` |
| IGNORE_SNOWMELT | ```YES``` / ```NO``` | ```YES``` if snowmelt calculations should be ignored. Default: ```NO``` |
| IGNORE_GROUNDWATER | ```YES``` / ```NO``` | ```YES``` if groundwater calculations should be ignored. Default: ```NO``` |
| IGNORE_RDII | ```YES``` / ```NO``` | ```YES``` if rainfall dependent inflow/infiltration should be ignored. Default: ```NO``` |
| IGNORE_ROUTING | ```YES``` / ```NO``` | ```YES``` if only runoff should be computed. Default: ```NO``` |
| IGNORE_QUALITY | ```YES``` / ```NO``` | ```YES``` if pollutant washoff, routing, and treatment should be ignored. Default: ```NO``` |
| ALLOW_PONDING | ```YES``` / ```NO``` | Whether excess water is allowed to collect atop nodes and be re-introduced into the system as conditions permit. Default: ```NO``` |
| SKIP_STEADY_STATE | ```YES``` / ```NO``` | ```YES``` if flow routing computations should be skipped during steady state periods of a simulation during which the last set of computed flows will be used. Default: ```NO``` |
| SYS_FLOW_TOL | value | The maximum percent difference between total system inflow and total system outflow which can occur in order for the ```SKIP_STEADY_STATE```  option to take effect. Default: 5%|
| LAT_FLOW_TOL | value | The maximum percent difference between the current and previous lateral inflow at all nodes in the conveyance system in order for the ```SKIP_STEADY_STATE``` option to take effect. Default: 5%|
| START_DATE | month/day/year | The date when the simulation begins.|
| START_TIME | hours:minutes | The time of day on the starting date when the simulation begins. Default: midnight (0:00:00)|
| END_DATE | month/day/year | The date when the simulation is to end. Default: start date |
| END_TIME | hours:minutes | The time of day on the ending date when the simulation will end. Default: 24:00:00 |
| REPORT_START_DATE | month/day/year | The date when reporting of results is to begin. Default: simulation start date|
| REPORT_START_TIME | hours:minutes | The time of day on the report starting date when reporting is to begin. Default: simulation start time|
| SWEEP_START | month/day | The date (month/day) when street sweeping operations begin. Default: 1/1 |
| SWEEP_END | month/day | The date (month/day) when street sweeping operations end. Default: 12/31 |
| DRY_DAYS | days | The number of days with no rainfall prior to the start of the simulation. Default: 0|
| REPORT_STEP | hours:minutes:seconds | The time interval for reporting of computed results. Default: 0:15:00|
| WET_STEP | hours:minutes:seconds | The time step for runoff computations during periods of rainfall or when ponded water still remains on the surface. Default: 0:05:00 |
| DRY_STEP | hours:minutes:seconds | The time step for runoff computations during periods with no rainfall and no ponded water. Default: 1:00:00 |
| ROUTING_STEP | seconds | The time step for routing through the conveyance system. Default: 600 s (5 min). Should be shorter for dynamic wave routing.|
| LENGTHENING_STEP | seconds | The time step for lengthening conduits under dynamic wave routing, in order to meet the Courant stability criterion under full-flow conditions. Default: 0 (no lengthening)|
| VARIABLE_STEP | value | A safety factor applied to a variable time step computed for each time period under dynamic wave flow routing to satisfy the Courant stability criterion while staying below ```ROUTING_STEP``` value. Default: 0 (no variable time step)|
| MINIMUM_STEP | seconds | The smallest allowed time step for variable time steps used for dynamic wave flow routing. Default: 0.5 s |
| INERTIAL_DAMPING | ```NONE``` / ```PARTIAL``` / ```FULL``` | Indicates handling the inertial terms of the Saint Venant momentum equation under dynamic wave flow routing. Default: ? |
| NORMAL_FLOW_LIMITED | ```SLOPE``` / ```FROUDE``` / ```BOTH``` | Condition to determine if flow in a conduit is supercritical and thus limited to normal flow. Default: ```BOTH``` |
| MIN_SURFAREA | value | A minimum surface area used at nodes when computing changes in water depth under dynamic wave routing. Default: 0 (uses 12.566 ft², i.e., the area of 4-ft diameter manhole)|
| MIN_SLOPE | value | The minimum value allowed for a conduit’s slope (%). Default: 0 (uses 0.001 ft / 0.00035 m)|
| MAX_TRIALS | value | The maximum number of trials during a time step for reaching convergence when updating hydraulic heads at nodes. Default: 8 |
| HEAD_TOLERANCE | value | The difference in computed head at each node between successive trials below which the flow solution for the current time step is assumed to have converged. Default: 0.005 ft (0.0015 m) |
| THREADS | value | The number of parallel computing threads used for dynamic wave flow routing. Default: 1 |
| TEMPDIR | directory | The name of a file directory (or folder) for SWMM to write temporary files. Default: current directory ||

## Evaporation table
Evaporation table specifies how daily evaporation rates vary with time.  
File format is .csv where each row defines a value for a single SWMM analysis option. Each row contains the analysis option name and the option value as in the table below separated by comma ','.
The file must have a header row and one of the below formats (```CONSTANT```, ```MONTHLY```, ```TIMESERIES```, ```TEMPERATURE```, or ```FILE```) from the following table defining model analysis options with two more optional parameters.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[EVAPORATION]```"

| Option | Value | Notes |
| :--------|:-------------|-------|
| ```CONSTANT``` | value ||
| ```MONTHLY``` | values ||
| ```TIMESERIES``` | name of timeserie ||
| ```TEMPERATURE``` |  | Evaporation rates are computed from the daily air temperatures contained in an external climate file name provided in the 'Temperature table' section. |
| ```FILE``` | values | Evaporation data is read directly from the external climate file name provided in the 'Temperature table' section. |
| ```RECOVERY``` | value | Optional monthly time pattern of multipliers for modifying infiltration recovery rates during dry periods. |
| ```DRY_ONLY``` | ```NO``` / ```YES``` | Determines if evaporation only occurs during periods with no precipitation. Default: ```NO``` ||

## Temperature table
The temperature table specifies daily air temperatures, monthly wind speed, and various snowmelt parameters for the study area. The file is required only when snowmelt is being modeled or when evaporation rates are computed from daily temperatures or are read from an external climate file.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[TEMPERATURE]```"


| Option | Value | Notes |
| :--------:|:-------------|-------|
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Snowpack table
The snowpacks table specifies how snowfall accumulates and melts on the plowable, impervious and pervious surfaces of subcatchments.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[SNOWPACKS]```"

| Option | Value | Notes |
| :--------:|:-------------|-------|
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Raingages table
Raingages table identifies each rain gage that provides rainfall data for the study area.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[RAINGAGES]```"

| Option | Value | Notes |
| :--------:|:-------------|-------|
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Inflows table
Inflows table specifies external hydrographs and pollutographs that enter the drainage system at specific nodes.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[INFLOWS]```"

| Option | Value | Notes |
| :--------:|:-------------|-------|
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Timeseries table
Timeseries table describes how various quantities vary over time.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[TIMESERIES]```"

| Option | Value | Notes |
| :--------:|:-------------|-------|
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Pump table
Pump table identifies each pump link of the drainage system.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[PUMPS]```"

| Attribute |              | Notes |
| :--------:|:-------------|-------|
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Curve table
Curve table describes a relationship between two variables (e.g. pump curve, rating curve, etc.) in tabular format.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[CURVES]```"

| Option | Value | Notes |
| :--------:|:-------------|-------|
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Storage table
Storage table identifies each storage node of the drainage system.
More details of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[STORAGE]```"

| Attribute |              | Notes |
| :--------:|:-------------|-------|
| x0 |  ||
| y0 |  ||
| Name |  ||
| Elev |  ||
| Ymax |  ||
| Yinit |  ||
| Curve |  ||
| Params |  ||
| Evap.Factor |  ||
| Seepage loss |  |||

## Dry weather flow (DWF) table
Dry weather flow (DWF) table specifies dry weather flow and its quality entering the drainage system at specific nodes.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[DWF]```"

| Option | Value | Notes |
| :--------:|:-------------|-------|
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Patterns table
Patterns table specifies time pattern of dry weather flow or quality in the form of adjustment factors applied as multipliers to baseline values.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[PATTERNS]```"

| Option | Value | Notes |
| :--------:|:-------------|-------|
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Losses table
Losses table specifies minor head loss coefficients, flap gates, and seepage rates for conduits.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[LOSSES]```"


| Option | Value | Notes |
| :--------:|:-------------|-------|
|  |  ||
|  |  ||
|  |  ||
|  |  |||

## Cross sections table
Cross sections table defines the geometries for all the conduits (pipes, channels, junctions) and regulator links (orifices, weirs) in the drainage system. File format is _.csv_ where each row defines the properties of one conduit in the system. The file can be directly produced using e.g. ArcMap or QGIS.  
The file must have a header row with the following attributes that are defined on the following rows for each conduit.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[XSECTIONS]```"

| Attribute |              | Notes |
| :--------:|:-------------|-------|
| Link | unique name of the conduit, orifice or weir ||
| Cshape | cross-secion shape (```CIRCULAR```, ```FORCE_MAIN```, ```FILLED_CIRCULAR```, ```RECT_CLOSED```, ```RECT_OPEN```, ```TRAPEZOIDAL```, ```TRIANGULAR```, ```HORIZ_ELLIPSE```, ```VERT_ELLIPSE```, ```ARCH```, ```PARABOLIC```, ```POWER```, ```RECT_TRIANGULAR```, ```RECT_ROUND```, ```MODBASKETHANDLE```, ```EGG```, ```HORSESHOE```, ```GOTHIC```, ```CATENARY```, ```SEMIELLIPTIC```, ```BASKETHANDLE```, ```SEMICIRCULAR```) ||
| Geom1 | full height of the cross-section (m) ||
| Geom2 | optional parameter depending on geometry type | leave blank if not used |
| Geom3 | optional parameter depending on geometry type |leave blank if not used |
| Geom4 | optional parameter depending on geometry type |leave blank if not used |
| Barrels | number of barrels (i.e. number of parallel pipes with identical properties) associated with a conduit (default ```1```)||
| Culvert | optional code number for the conduit's inlet geometry if it is a culvert | leave blank if not used ||

## Report table
Report table describes the contents of the report file that is produced.  
File format is _.csv_ where each row defines a value for a single SWMM analysis option.
Each row contains the analysis option name and the option value as in the table below separated by comma ','.  
The file must have a header row and the necessary parameters from the following table defining model analysis options.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[REPORT]```"

| Option | Value | Notes |
| :--------|:-------------|-------|
| ```INPUT``` | ```YES``` / ```NO``` | Specifies if a summary of the input data is provided in the output report. Default: ```NO``` |
| ```CONTINUITY``` | ```YES``` / ```NO``` | Specifies if continuity checks are reported. Default: ```YES``` |
| ```FLOWSTATS``` | ```YES``` / ```NO``` | Specifies if summary flow statistics are reported. Default: ```YES``` |
| ```CONTROLS``` | ```YES``` / ```NO``` | Specifies if all control actions taken during a simulation are listed. Default: ```NO``` |
| ```SUBCATCHMENTS``` | ```ALL``` / ```NONE``` / &lt;list of subcatchment names&gt; | A list of subcatchments whose results are reported. Default: ```NONE``` |
| ```NODES``` | ```ALL``` / ```NONE``` / &lt;list of node names&gt; | A list of nodes whose results are reported. Default: ```NONE``` |
| ```LINKS``` | ```ALL``` / ```NONE``` / &lt;list of link names&gt; | A list of links whose results are reported. Default: ```NONE``` |
| ```LID``` | _Name_ _Subcatch_ _Fname_ | Specifies that a detailed performance report for the LID control _Name_ in subcatchment _Subcatch_ is written to file _Fname_.||

## Symbols table
Symbols table assigns X,Y coordinates to rain gage symbols.  
File format is _.csv_ where each row defines the properties of one conduit in the system. The file must have a header. The following attributes are then defined on the subsequent rows for each rain gage.  
Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: ```[SYMBOLS]```"

| Attribute |  |
| :--------:|:-------------|
| Gage | Name of rain gage |
| Xcoord | Horizontal coordinate relative to origin in lower left of map |
| Ycoord | Vertical coordinate relative to origin in lower left of map ||
