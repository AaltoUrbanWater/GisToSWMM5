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
- <sup>1</sup>Xsections table in _.csv_ format
- <sup>1</sup>Report table in _.csv_ format
- <sup>1</sup>Symbols table in _.csv_ format

Except for the raster files and the Catchment properties table, i.e., the first four files, each file defines one section of the SWMM input file.  
Files with <sup>1</sup> match a section from the standard SWMM input file (_.inp_) except for being comma (not space) separated.

## DEM raster
DEM raster defines the elevations across the catchment in the same grid as in the landuse raster.  

File format is _Arc/Info ASCII Grid_. The file can be directly produced using e.g. ArcMap or QGIS.  

The header section needs to have the following information with appropriate values followed by the DEM raster values in a ncols*nrows space-separated list:  
<pre><code>
ncols          [aaa]  
nrows          [bbb]  
xllcorner      [ccc]  
yllcorner      [ddd]  
cellsize       [eee]  
NODATA_value   [fff]
</code></pre>


## Flow direction raster
Flow direction raster defines the flow directions across the catchment in the same grid as in the landuse raster. Flow directions are defined according to D8-method such that each cell can flow into one of the 8 neighbouring cells. Flow direction raster uses a notation where flow directions are marked with integer numbers from 1-8 so that 1 corresponds to flow to Northeast and numbers increase in counterclockwise direction. For cell X, the directions are defined as shown below:  

<table align="center">
    <tr>
        <td align="center"> 3 </td>
        <td align="center"> 2 </td>
        <td align="center"> 1 </td>
        <td align="center">  </td>
        <td align="center"> :arrow_upper_left: </td>
        <td align="center"> :arrow_up: </td>
        <td align="center"> :arrow_upper_right: </td>
    </tr>
    <tr>
        <td align="center"> 4 </td>
        <td align="center"> X </td>
        <td align="center"> 8 </td>
        <td align="center">  </td>
        <td align="center"> :arrow_left: </td>
        <td align="center"> X </td>
        <td align="center"> :arrow_right: </td>
    </tr>
    <tr>
        <td align="center"> 5 </td>
        <td align="center"> 6 </td>
        <td align="center"> 7 </td>
        <td align="center">  </td>
        <td align="center"> :arrow_lower_left: </td>
        <td align="center"> :arrow_down: </td>
        <td align="center"> :arrow_lower_right: </td>
    </tr>
</table>

File format is _Arc/Info ASCII Grid_. The file can be directly produced using e.g. ArcMap or QGIS.  

The header section needs to have the following information with appropriate values followed by the flow direction raster values in a ncols*nrows space-separated list:  
<pre><code>
ncols          [aaa]  
nrows          [bbb]  
xllcorner      [ccc]  
yllcorner      [ddd]  
cellsize       [eee]  
NODATA_value   [fff]
</code></pre>

## Land use raster
Land use raster defines the land use across the catchment in in each grid cell.  

File format is _Arc/Info ASCII Grid_. The file can be directly produced using e.g. ArcMap or QGIS.  

The header section needs to have the following information with appropriate values followed by the landuse raster values in a ncols*nrows space-separated list:  
<pre><code>
ncols          [aaa]  
nrows          [bbb]  
xllcorner      [ccc]  
yllcorner      [ddd]  
cellsize       [eee]  
NODATA_value   0
</code></pre>

GisToSWMM5 currently uses the following codes for different land use classes.
The different integers mark different landuse classes and (optionally) different landuse parameters specified in the Catchment properties table for each landuse class.
The classes are loosely grouped into roofs, built areas, and natural areas, and especially for roofs these groups should be used.
Generally it is good practice to use same subclass values for connected and disconnected roofs to keep things clear.
E.g. 10 should have the same parameters as 20, but 10 are directly connected to junctions whereas 20 are connected to yards. In the same manner 11 should correspond to 21, etc.
For other classes there is no such connection between classes.

| Code        | Landuse           |
| :----------:|:------------------|
| `10-19` | Connected roof    |
| `20-29` | Disconnected roof |
| `30-59` | Built area        |
| `60-`   | Natural area      |

NODATA_value of `0` is used to delineate areas not belonging to the catchment.

## Catchment properties table
Catchment properties table defines the properties of each GisToSWMM5 landuse class.  

File format is comma-separated _.csv_ where each row defines the properties of one landuse type. The file must have a header row followed by rows with attributes defined as below for each landuse type.

**Example:**  
*code*, *imperv*, *dst_imp*, *n_imper*, *dst_per*, *n_per*, *percz_i*, *rain_ga*, *conduct*, *initdef*, *suction*, *Snowpacks*, *[Tag]*

Where the attributes are as follows:  

| Attribute | | Notes |
| :-:|:-|-|
| *code* | Landuse code (see the Landuse  table)    |   |
| *imperv*    | Share of impervious areas (%)           |       |
| *dst_imp*   | Depression storage of impervious areas (mm)   |       |
| *n_imper*   | Roughness coefficient (Manning's n) of impervious areas  |     |
| *dst_per*   | Depression storage of pervious areas (mm)            |       |
| *n_perv*    | Roughness coefficient (Manning's n) of pervious areas     |    |
| *percz_i*   | Share of impervious areas without depression storage (%)     |    |
| *rain_ga*   | Name of the rain gauge for this land-use class      |      |
| *conduct*   | Green-Ampt infiltration model soil saturated hydraulic conductivity (mm/h)  |      |
| *initdef*   | Green-Ampt infiltration model fraction of soil volume that is initially dry (- |   |
| *suction*   | Green-Ampt infiltration model average soil capillary suction (mm)    |   |
| *Snowpacks* | Name for the snow model used for the land use class       |   |
| [*Tag*]     | [Optional tag for the landuse type]                                             | Leave blank if not used ||

## Junctions table
Junctions table defines the junction nodes of the drainage system, i.e., the points where channels and pipes connect together. For sewer systems they can be e.g. manholes/wells or connection fittings.  

File format is comma-separated _.csv_ where each row defines the properties of one junction in the system. The file can be produced using e.g. ArcMap or QGIS. The file must have a header row followed by rows with attributes defined as below for each junction.

**Example:**  
*x0*, *y0*, *name*, *elevation*, *inv_elev*, *max_depth*, *open*, *InitDepth*, *SurDepth*, *Aponded*, *routing*

Where the attributes are as follows:  

| Attribute |              | Notes |
| :--------:|:-------------|-------|
| *x0*        | x-coordinate of the junction ||
| *y0*        | y-coordinate of the junction ||
| *name*      | unique name of the junction (e.g. j1, j2, ..., jn) ||
| *elevation* | elevation of the junction, i.e., ground surface (m) | This is unnecessary parameter to be removed later |
| *inv_elev*  | invert elevation, i.e., the elevation of the channel or manhole bottom ||
| *max_depth* | depth of the well, i.e., elevation - inv_elev ||
| *open*      | `0` or `1` depending on whether the well is open or closed (`0` = closed, `1` = open) ||
| *InitDepth* | water depth at the start of simulation (m; default 0) ||
| *SurDepth*  | maximum additional head above ground elevation that the manhole can sustain under surcharge conditions (m; default 0) ||
| *Aponded*   |ponded surface area when water depth exceeds max_depth (m^2; default 0) ||
| *routing*   | `0` or `1` depending on whether the junction is usable for routing water from roofs and pits in impervious areas (`0` = non-routable, `1` = routable; default `1`) |||

## Conduits table
Conduits table defines the conduits (pipes, links) of the drainage system, i.e., the pipes and channels that convey the water from one node to another.  

File format is comma-separated _.csv_ where each row defines the properties of one conduit in the system. The file can be produced using e.g. ArcMap or QGIS. The file must have a header row followed by rows with attributes defined as below for each conduit.

**Example:**  
*x0*, *y0*, *x1*, *y1*, *name*, *elev_in*, *elev_out*, *length*, *junc_in*, *junc_out*, *roughness*, *elev_in_of*, *elev_ou_of*

Where the attributes are as follows:  

| Attribute |              | Notes |
| :--------:|:-------------|-------|
| *x0* | x-coordinate of the conduit start point ||
| *y0* | y-coordinate of the conduit start point ||
| *x1* | x-coordinate of the conduit end point ||
| *y1* | y-coordinate of the conduit end point ||
| *name* | unique name of the conduit (e.g. c1, c2, ..., cn) ||
| *elev_in* | elevation of upstream end of conduit (m) | This is unnecessary parameter to be removed later |
| *elev_out* | elevation of the downstream end of conduit (m) | This is unnecessary parameter to be removed later |
| *length* | conduit length (m) ||
| *junc_in* | name of upstream node (junction) ||
| *junc_out* | name of downstream node (junction) ||
| *roughness* | Roughness of the conduit (Manning's n) ||
| *elev_in_of* | offset of upstream end of conduit invert (bottom) above the invert (bottom) elevation of the upstream node (junction), i.e., conduit bottom elevation - junction bottom elevation (m) ||
| *elev_ou_of* | offset of downstream end of conduit invert (bottom) above the invert (bottom) elevation of the downstream node (junction), i.e., conduit bottom elevation - junction bottom elevation (m) |||

## Outfalls table
Outfalls table defines the outfall nodes of the drainage system, i.e., the final downstream boundaries, and the corresponding water stage elevation.  

File format is comma-separated _.csv_ where each row defines the properties of one outfall in the system. The file can be produced using e.g. ArcMap or QGIS. The file must have a header row row followed by rows with attributes defined as below for each outfall.

**Example:**  
*x0*, *y0*, *name*, *invert*, *type*, *stage_data*, *gated*

Where the attributes are as follows:  

| Attribute |              |
| :--------:|:-------------|
| *x0* | x-coordinate of the outfall |
| *y0* | y-coordinate of the outfall |
| *name* | unique name of the outfall (e.g. o1, o2, ..., on) |
| *invert* | invert (bottom) elevation of the outfall |
| *type* | type of the outfall (`FREE`, `NORMAL`, `FIXED`, `TIDAL`, or `TIMESERIES`; default `FREE`) |
| *stage_data* | depending on the _type_ of the outfall, either empty, OR elevation of fixed stage outfall (m) OR name of curve containing tidal height OR name of time series describing how outfall stage varies in time; default [empty] |
| *gated* | `YES` or `NO` depending on whether a flap gate is present that prevents reverse flow; default `NO` |

## Header table
Header table provides the values for various analysis options used by SWMM.  

File format is comma-separated _.csv_ where each row defines a value for a single SWMM analysis option. Each row contains the analysis option name and the option value as in the table below separated by comma ','.  

The file must have a header row followed by the necessary parameters from the table below on following rows defining model analysis options.  

Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `OPTIONS`"

| Option | Value             | Notes |
| :--------|:-------------|-------|
| FLOW_UNITS | `CFS` / `GPM` / `MGD` / `CMS` / `LPS` / `MLD` | Choice of flow units. All quantities in the model are according to this option. Default: `CFS` |
| INFILTRATION | `HORTON` / `MODIFIED_HORTON` / `GREEN_AMPT` / `MODIFIED_GREEN_AMPT` / `CURVE_NUMBER` | A model for computing infiltration of rainfall into the upper soil zone of subcatchments. Default: `HORTON` |
| FLOW_ROUTING | `STEADY` / `KINWAVE` / `DYNWAVE` | A method used to route flows through the drainage system. Default: `KINWAVE`|
| LINK_OFFSETS | `DEPTH` / `ELEVATION` | The convention used to specify the position of a link offset above the invert of its connecting node. Default: `DEPTH`, which is also assumed in this document.|
| FORCE_MAIN_EQUATION | `H-W` / `D-W` | Whether the Hazen-Williams (`H-W`) or the Darcy-Weisbach (`D-W`) equation will be used to compute friction losses for pressurized flow in conduits assigned with Circular Force Main cross-section shape. Default: `H-W` |
| IGNORE_RAINFALL | `YES` / `NO` | `YES` if all rainfall data and runoff calculations should be ignored. Default: `NO` |
| IGNORE_SNOWMELT | `YES` / `NO` | `YES` if snowmelt calculations should be ignored. Default: `NO` |
| IGNORE_GROUNDWATER | `YES` / `NO` | `YES` if groundwater calculations should be ignored. Default: `NO` |
| IGNORE_RDII | `YES` / `NO` | `YES` if rainfall dependent inflow/infiltration should be ignored. Default: `NO` |
| IGNORE_ROUTING | `YES` / `NO` | `YES` if only runoff should be computed. Default: `NO` |
| IGNORE_QUALITY | `YES` / `NO` | `YES` if pollutant washoff, routing, and treatment should be ignored. Default: `NO` |
| ALLOW_PONDING | `YES` / `NO` | Whether excess water is allowed to collect atop nodes and be re-introduced into the system as conditions permit. Default: `NO` |
| SKIP_STEADY_STATE | `YES` / `NO` | `YES` if flow routing computations should be skipped during steady state periods of a simulation during which the last set of computed flows will be used. Default: `NO` |
| SYS_FLOW_TOL | value | The maximum percent difference between total system inflow and total system outflow which can occur in order for the `SKIP_STEADY_STATE`  option to take effect. Default: 5%|
| LAT_FLOW_TOL | value | The maximum percent difference between the current and previous lateral inflow at all nodes in the conveyance system in order for the `SKIP_STEADY_STATE` option to take effect. Default: 5%|
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
| VARIABLE_STEP | value | A safety factor applied to a variable time step computed for each time period under dynamic wave flow routing to satisfy the Courant stability criterion while staying below `ROUTING_STEP` value. Default: 0 (no variable time step)|
| MINIMUM_STEP | seconds | The smallest allowed time step for variable time steps used for dynamic wave flow routing. Default: 0.5 s |
| INERTIAL_DAMPING | `NONE` / `PARTIAL` / `FULL` | Indicates handling the inertial terms of the Saint Venant momentum equation under dynamic wave flow routing. Default: ? |
| NORMAL_FLOW_LIMITED | `SLOPE` / `FROUDE` / `BOTH` | Condition to determine if flow in a conduit is supercritical and thus limited to normal flow. Default: `BOTH` |
| MIN_SURFAREA | value | A minimum surface area used at nodes when computing changes in water depth under dynamic wave routing. Default: 0 (uses 12.566 ft², i.e., the area of 4-ft diameter manhole)|
| MIN_SLOPE | value | The minimum value allowed for a conduit’s slope (%). Default: 0 (uses 0.001 ft / 0.00035 m)|
| MAX_TRIALS | value | The maximum number of trials during a time step for reaching convergence when updating hydraulic heads at nodes. Default: 8 |
| HEAD_TOLERANCE | value | The difference in computed head at each node between successive trials below which the flow solution for the current time step is assumed to have converged. Default: 0.005 ft (0.0015 m) |
| THREADS | value | The number of parallel computing threads used for dynamic wave flow routing. Default: 1 |
| TEMPDIR | directory | The name of a file directory (or folder) for SWMM to write temporary files. Default: current directory ||

## Evaporation table
Evaporation table specifies how daily evaporation rates vary with time.  

File format is comma-separated _.csv_ . The file must have a header row followed by a row with one of the `CONSTANT`, `MONTHLY`, `TIMESERIES`, `TEMPERATURE`, or `FILE` with appropriate parameters. In addition, the two last rows with `RECOVERY` and `DRY_ONLY` are optional.  

Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[EVAPORATION]`"

**Formats:**  
`CONSTANT`, *evap*  
`MONTHLY`, *e1*, *e2*, *e3*, *e4*, *e5*, *e6*, *e7*, *e8*, *e9*, *e10*, *e11*, *e12*  
`TIMESERIES`, *Tseries*  
`TEMPERATURE`,   
`FILE`, (*p1*, *p2*, *p3*, *p4*, *p5*, *p6*, *p7*, *p8*, *p9*, *p10*, *p11*, *p12*)  

**Optional rows:**  
`RECOVERY`, *patternID*  
`DRY_ONLY`, `NO` / `YES`  


Where the attributes are as follows:  

| Attribute | |
| :--------|-------|
| *evap* | Constant evaporation rate (mm/day or in/day) |
| *e1*, ...,  *e12* | Evaporation rates from Jan to Dec (mm/day or in/day) |
| *Tseries* | name of time series in `[TIMESERIES]` section with evaporation data. |
| (*p1*, ...,  *p12*)  | (Optional values defining the pan coefficients from Jan to Dec.) |
| *patternID*  | name of a monthly time patternID |

If `TEMPERATURE` is used the evaporation rates are computed from the daily air temperatures contained in an external climate file name provided in the 'Temperature table' section.  
`RECOVERY` defines optional monthly time pattern of multipliers for modifying infiltration recovery rates during dry periods.  
`DRY_ONLY` determines if evaporation only occurs during periods with no precipitation. Default: `NO`.

## Temperature table
The temperature table specifies daily air temperatures, monthly wind speed, and various snowmelt parameters for the study area. The file is required when snowmelt is being modeled or when evaporation rates are computed from daily temperatures or are read from an external climate file.  

File format is comma-separated _.csv_. The file must have a header row followed by appropriate rows from below defining model analysis options.

Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[TEMPERATURE]`"

**Formats:**  
`TIMESERIES`, *Tseries*  
OR  
`FILE`, *Fname*, (*Start*)  
`WINDSPEED MONTHLY`, *s1*, *s2*, *s3*, *s4*, *s5*, *s6*, *s7*, *s8*, *s9*, *s10*, *s11*, *s12*  
OR  
`WINDSPEED FILE`  
`SNOWMELT`, *Stemp*, *ATIwt*, *RNM*, *Elev*, *Lat*, *DTLon*  
`ADC IMPERVIOUS`, *f.0*, *f.1*, *f.2*, *f.3*, *f.4*, *f.5*, *f.6*, *f.7*, *f.8*, *f.9*    
`ADC PERVIOUS`, *f.0*, *f.1*, *f.2*, *f.3*, *f.4*, *f.5*, *f.6*, *f.7*, *f.8*, *f.9*     

Where the attributes are as follows:  

| Attribute |  |
| :--------|--|
| *Tseries* | Name of time series in `[TIMESERIES]` section with temperature data |
| *Fname* | Name of external Climate file with temperature data |
| (*Start*) | Date to begin reading from the file (mm/dd/yyyy); Default: beginning of file |
| *s1*, ..., *s12* | Average wind speed values from Jan to Dec |
| *Stemp* | Air temperature at which precipitation falls as snow (deg F or C) |
| *ATIwt* | Antecedent temperature index weight; Default = 0.5 |
| *RNM* | Negative melt ratio; Default = 0.6 |
| *Elev* | Average elevation of study area above mean sea level (ft or m); Default = 0 |
| *Lat* | Latitude of the study area in degrees North; Default = 50 |
| *DTLon* | Correction (min) between true solar time and the standard clock time; Default = 0 |
| *f.0*, ... *f.9* | Fractions of area covered by snow on pervious and impervious subareas ||  

If `WINDSPEED FILE` is used, the wind speed is specified by the same Climate file used for air temperature.  

## Snowpack table
The snowpacks table specifies how snowfall accumulates and melts on the plowable, impervious and pervious surfaces of subcatchments.  

File format is comma-separated _.csv_. The file must have a header row followed by appropriate rows from below defining model analysis options.

Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[SNOWPACKS]`"

**Formats:**  
*Name*, `PLOWABLE`, *Cmin*, *Cmax*, *Tbase*, *FWF*, *SD0*, *FW0*, *SNN0*  
*Name*, `IMPERVIOUS`, *Cmin*, *Cmax*, *Tbase*, *FWF*, *SD0*, *FW0*, *SD100*  
*Name*, `PERVIOUS`, *Cmin*, *Cmax*, *Tbase*, *FWF*, *SD0*, *FW0*, *SD100*  
*Name*, `REMOVAL`, *Dplow*, *Fout*, *Fimp*, *Fperv*, *Fimelt*, (*Fsub*, *Scatch*)  

Where the attributes are as follows:  

| Attribute | |
| :---| :--- |
| *Name* | Name of the snowpacks parameter set |
| *Cmin* | min melt coefficient (mm/h-deg C or in/h-deg C) |
| *Cmax* | max melt coefficient (mm/h-deg C or in/h-deg C) |
| *Tbase* | snow melt base temperature (deg C or deg F) |
| *FWF* | free-water-holding-capacity to snow-depth fraction |
| *SD0* | initial snow depth (mm or in water equivalent) |
| *FW0* | initial free water in snowpack (mm or in) |
| *SNN0* | fraction of plowable impervious area |
| *SD100* | snow depth above which there is 100% cover (mm or in water equivalent) |
| *Dplow* | depth of snow on plowable areas at which snow removal begins (mm or in) |
| *Fout* | fraction of snow on plowable area transferred out of watershed |
| *Fimp* | fraction of snow on plowable area transferred to impervious area by plowing |
| *Fperv* | fraction of snow on plowable area transferred to pervious area by plowing |
| *Fimelt* | fraction of snow on plowable area converted into immediate melt |
| (*Fsub*) | (fraction of snow on plowable area transferred to pervious area in another subcatchment) |
| (*Scatch*) | (name of subcatchment receiving the Fsub fraction of transferred snow) ||  

Snow pack parameter sets are associated with specific subcatchments in the `[SUBCATCHMENTS]` section. Each snow pack parameter set requires one set of `PLOWABLE`, `IMPERVIOUS`, and `PERVIOUS` lines. Multiple subcatchments can share the same set of snow pack parameters.

## Raingages table
Raingages table identifies each rain gage that provides rainfall data for the study area.  

File format is comma-separated _.csv_ where each row defines the properties of one rain gage in the system. The file must have a header row followed by appropriate rows from below defining model analysis options.

Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[RAINGAGES]`"

**Formats:**  
*Name*, *Form*, *Intvl*, *SCF*, `TIMESERIES`, *Tseries*  
*Name*, *Form*, *Intvl*, *SCF*, `FILE`, *Fname*, *Sta*, *Units*   

Where the attributes are as follows:  

| Attribute | |
| :--|:-------------|
| *Name* | name of the rain gage |
| *Form* | form of recorded rainfall (`INTENSITY`, `VOLUME` or `CUMULATIVE`) |
| *Intvl* | interval between gage readings (hh:mm) |
| *SCF* | snow catch deficiency correction factor (default: 1.0, i.e., no correction) |
| *Tseries* | name of the timeseries in `[TIMESERIES]` section with rainfall data |
| *Fname* | name of external file with rainfall data |
| *Sta* | name of recording station used in the rain file |
| *Units* | rain depth units used in the rain file (`IN` [inches] or `MM` [millimeters]) ||

## Inflows table
Inflows table specifies external hydrographs and pollutographs that enter the drainage system at specific nodes.  

File format is comma-separated _.csv_ where each row defines options for a single node in the system. The file must have a header row followed by appropriate rows from below defining model analysis options.

Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[INFLOWS]`"

**Formats:**  
*Node*, `FLOW`, *Tseries*, (`FLOW`, (*1.0*, *Sfactor*, *Base*, *Pat*))  
*Node*, *Pollut*, *Tseries*, (*Type*, (*Mfactor*, *Sfactor*, *Base*, *Pat*))  

Where the attributes are as follows:  

| Attribute | |
| :---|:---|
| *Node* | name of node where external inflow enters |
| *Pollut* | name of pollutant |
| *Tseries* | name of time series in `[TIMESERIES]` section describing external flow or pollutant loading varying with time |
| (*Type*) | (`CONCEN` if pollutant inflow is concentration, `MASS` if mass flow rate; Default: `CONCEN`)) |
| (*Mfactor*) | (factor converting inflow’s mass flow rate units into project’s mass units per second; Default: 1.0) |
| (*Sfactor*) | (scaling factor multiplying recorded time series values; Default: 1.0) |
| (*Base*) | (constant baseline value added to the time series value; Default: 0.0) |
| (*Pat*) | (name of optional time pattern in `[PATTERNS]` section to adjust the baseline value on a periodic basis) ||

## Timeseries table
Timeseries table describes how various quantities vary over time.  

File format is comma-separated _.csv_ where each row defines one time series. The file must have a header row followed by appropriate rows from below.


Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[TIMESERIES]`"

**Formats:**  
*Name*, (*Date*), *Hour*, *Value*, ...  
*Name*, *Time*, *Value*, ...  
*Name*, `FILE`, *Fname*  

Where the attributes are as follows:  

| Attribute |  |
| :---|:---|
| *Name* | name of the time series |
| (*Date*) | (date (MM/DD/YYYY)) |
| *Hour* | hour relative to the last date specified (24h time, HH:MM) |
| *Time* | hours since start of simulation (HH:MM) |
| *Value* | value corresponding to given date and time |
| *Fname* | name of the file containing the time series data ||

## Pump table
Pump table identifies each pump link of the drainage system.  

File format is comma-separated _.csv_ where each row defines options for a single pump in the system. The file must have a header row followed by rows formatted as below.

Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[PUMPS]`"

**Format:**  
*Name*, *Node1*, *Node2*, *Pcurve*, (*Status*, *Startup*, *Shutoff*)

Where the attributes are as follows:  

| Attribute |  |
| :---|:--- |
| *Name* | name of the pump link |
| *Node1* | name of the node on inlet side of pump |
| *Node2* | name of the node on outlet side of pump |
| *Pcurve* | name of pump curve listed in the `[CURVES]` section |
| (*Status*) | (status at start of simulation (`ON` or `OFF`); Default: `ON`) |
| (*Startup*) | (depth at the inlet node when pump turns on (m or ft); Default: 0) |
| (*Shutoff*) | (depth at the inlet node when pump shuts off (m or ft); Default: 0)  ||

## Curve table
Curve table describes a relationship between two variables (e.g. pump curve, rating curve, etc.) in tabular format.  

File format is comma-separated _.csv_ where each row defines a single curve. The file must have a header row followed by rows formatted as below.

Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[CURVES]`"

**Format:**  
*Name*, *Type*, *X-value*, *Y-value*  

Where the attributes are as follows:  

| Attribute |  |
| :---|:--- |
| *Name* | name of the curve |
| *Type* | `STORAGE` / `SHAPE` / `DIVERSION` / `TIDAL` / `PUMP1` / `PUMP2` / `PUMP3` / `PUMP4` / `RATING` / `CONTROL` |
| *X-value* | an x (independent variable) value |
| *Y-value* | the y (dependent variable) value corresponding to x |  

Multiple pairs of x-y values can appear on a line. The x-values must be entered in increasing order.

## Storage table
**NOTE: THIS IS OUTDATED AND NEEDS TO BE UPDATED IN GisToSWMM5!**

Storage table identifies each storage node of the drainage system.

File format is comma-separated _.csv_ where each row defines options for a single storage node in the system. The file must have a header row followed by rows formatted as below.

More details of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[STORAGE]`"

**Format:**  
*x0*, *y0*, *Name*, *Elev*, *Ymax*, *Y0*, *Curve*, *Params*, *Evap.Factor*, *Seepage loss*  

Where the attributes are as follows:  

| Attribute |  |
| :---|:---|
| x0 | x-coordinate of the storage node |
| y0 | y-coordinate of the storage node |
| Name | name of the storage node |
| Elev | invert elevation (m or ft) |
| Ymax | max possible water depth (m or ft) |
| Y0 | water depth at the start of the simulation (m or ft) |
| Curve |  |
| Params |  |
| Evap.Factor |  |
| Seepage loss |  ||

## Dry weather flow (DWF) table
Dry weather flow (DWF) table specifies dry weather flow and its quality entering the drainage system at specific nodes.  

File format is comma-separated _.csv_ where each row defines options for a single node where dry weather flow enters the system. The file must have a header row followed by rows formatted as below.  

Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[DWF]`"

**Format:**  
*Node*, *Type*, *Base*, (*Pat1*, *Pat2*, *Pat3*, *Pat4*)  

Where the attributes are as follows:  

| Attribute |  |
| :---|:---|
| *Node* | name of node where dry weather flow enters |
| *Type* | `FLOW` for flow or pollutant name for quality constituent |
| *Base* | average baseline value for constituent (flow or concentration units) |
| (*Pat1*, *Pat2*), etc. | (names of up to four time patterns appearing in the `[PATTERNS]` section) ||

## Patterns table
Patterns table specifies time pattern of dry weather flow or quality in the form of adjustment factors applied as multipliers to baseline values.  

File format is comma-separated _.csv_ where each row defines one time pattern. The file must have a header row followed by appropriate rows from below.

Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[PATTERNS]`"

**Formats:**  
*Name*, `MONTHLY`, *Factor1*, *Factor2*, ... *Factor12*  
*Name*, `DAILY`, *Factor1*, *Factor2*, ... *Factor7*  
*Name*, `HOURLY`, *Factor1*, *Factor2*, ... *Factor24*  
*Name*, `WEEKEND`, *Factor1*, *Factor2*, ... *Factor24*  

Where the attributes are as follows:  

| Attribute |  |
| :---|:---|
| *Name* | Name to identify the pattern |
| *Factor1*, *Factor2*, etc. | multiplier values ||

## Losses table
Losses table specifies minor head loss coefficients, flap gates, and seepage rates for conduits.

File format is comma-separated _.csv_ where each row defines minor head loss options for a single conduit in the system. The file must have a header row followed by rows formatted as below.  

Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[LOSSES]`"

**Format:**  
*Conduit*, *Kentry*, *Kexit*, *Kavg*, (*Flap*, *Seepage*)

Where the attributes are as follows:  

| Attribute |  |
| :---|:---|
| *Conduit* | name of conduit |
| *Kentry* | entrance minor head loss coefficient |
| *Kexit* | exit minor head loss coefficient |
| *Kavg* | average minor head loss coefficient across length of conduit |
| *Flap* | `YES` if conduit has a flap valve that prevents back flow, `NO` otherwise; Default: `NO` |
| *Seepage* | Rate of seepage loss into surrounding soil (mm/hr or in/hr) ||

## Xsections table
Xsections table defines the geometries for all the conduits (pipes, channels, junctions) and regulator links (orifices, weirs) in the drainage system.  

File format is comma-separated _.csv_ where each row defines the cross section of one conduit in the system. The file must have a header row followed by appropriate rows from below.

Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[XSECTIONS]`"

**Formats:**  
*Link*, *Shape*, *Geom1*, *Geom2*, *Geom3*, *Geom4*, (*Barrels*, *Culvert*)  
*Link*, `CUSTOM`, *Geom1*, *Curve*, (*Barrels*)  
*Link*, `IRREGULAR`, *Tsect*   

Where the attributes are as follows:  

| Attribute |  |
| :---|---|
| *Link* | unique name of the conduit, orifice or weir |
| *Shape* | cross-secion shape  |
| *Geom1* | full height of the cross-section (m or ft) |
| *Geom2* ... *Geom4* | optional parameters depending on geometry type |
| (*Barrels*) | (number of barrels (i.e. number of parallel pipes with identical properties) associated with a conduit; Default: 1)|
| (*Culvert*) | (optional code number for the conduit's inlet geometry if it is a culvert) |
| *Curve* | name of a Shape Curve in the `[CURVES]` section defining how width varies with depth |
| *Tsect* | name of an entry in the `[TRANSECTS]` section describing the cross-section geometry of an irregular channel ||

## Report table
Report table describes the contents of the report file that is produced.  

File format is comma-separated _.csv_ where each row defines a value for a single SWMM analysis option. Each row contains the analysis option name and the option value as in the table below separated by comma ','.  

The file must have a header row and the necessary parameters from the following table defining model analysis options.  

Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[REPORT]`"

| Option | Value | Notes |
| :--------|:-------------|-------|
| `INPUT` | `YES` / `NO` | Specifies if a summary of the input data is provided in the output report. Default: `NO` |
| `CONTINUITY` | `YES` / `NO` | Specifies if continuity checks are reported. Default: `YES` |
| `FLOWSTATS` | `YES` / `NO` | Specifies if summary flow statistics are reported. Default: `YES` |
| `CONTROLS` | `YES` / `NO` | Specifies if all control actions taken during a simulation are listed. Default: `NO` |
| `SUBCATCHMENTS` | `ALL` / `NONE` / &lt;list of subcatchment names&gt; | A list of subcatchments whose results are reported. Default: `NONE` |
| `NODES` | `ALL` / `NONE` / &lt;list of node names&gt; | A list of nodes whose results are reported. Default: `NONE` |
| `LINKS` | `ALL` / `NONE` / &lt;list of link names&gt; | A list of links whose results are reported. Default: `NONE` |
| `LID` | _Name_ _Subcatch_ _Fname_ | Specifies that a detailed performance report for the LID control _Name_ in subcatchment _Subcatch_ is written to file _Fname_.||

## Symbols table
Symbols table assigns X,Y coordinates to rain gage symbols.  

File format is comma-separated _.csv_ where each row defines coordinates for a single rain gage. The file must have a header row followed by rows formatted as below.

Format of these options are explained in SWMM user manual Appendix D, section D.2 Input File Format, "Section: `[SYMBOLS]`"

**Format:**  
*Gage*, *Xcoord*, *Ycoord*

Where the attributes are as follows:  

| Attribute |  |
| :---|:---|
| *Gage* | Name of rain gage |
| *Xcoord* | Horizontal coordinate relative to origin in lower left of map |
| *Ycoord* | Vertical coordinate relative to origin in lower left of map ||
