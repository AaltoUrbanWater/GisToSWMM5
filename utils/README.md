# GisToSWMM5 - utils
## inp2gis.py
Extracts subcatchment polygons and routing between subcatchments from a SWMM input file (\*.inp). The tool saves the subcatchments as a polygon shapefile and the routing between the subcatchment polygon centroids as a line shapefile.  
This utility tool is suitable for any SWMM5 input file, i.e., it is not restricted to GisToSWMM5 generated files.

#### Subcatchment polygon shapefile (\*\_subcatchments.shp)  

The subcatchment polygon shapefile has the following attributes:  

| Name | Attribute | Unit |
| -----|-----------|------|
| Name | Name of the subcatchment | - |
| Rgage | Name of rain gage assigned to subcatchment | - |
| OutID | Name of node or subcatchment that receives runoff from subcatchment | - |
| Area   | Subcatchment area | m2 |
| Imperv_pct | Imperviousness of subcatchment | % |
| Width | Characteristic width of subcatchment | m |
| Slope | Subcatchment slope | % |
| Clength | Total curb length | m |
| Nimp | Manning's n for overland flow over the impervious sub-area | - |
| Nperv | Manning's n for overland flow over the pervious sub-area | - |
| Simp | Depression storage for impervious sub-area | mm |
| Sperv | Depression storage for pervious sub-area | mm |
| Zero_pct | Percent of impervious area with no depression storage | % |
| RouteTo | Defines routing between pervious and impervious areas inside the subacathment | - |

If Horton infiltration model is used, the shapefile will also have the following attributes:  

| Name | Attribute | Unit |
| -----|-----------|------|
| MaxRate | Maximum infiltration rate on Horton curve | mm/h |
| MinRate | Minimum infiltration rate on Horton curve | mm/h |
| Decay | Decay rate constant of Horton curve | 1/h |
| DryTime | Time it takes for fully saturated soil to dry | d |
| MaxInf | Maximum infiltration volume possible | mm |

If Green-Ampt infiltration model is used, the shapefile will also have the following attributes:  

| Name | Attribute | Unit |
| -----|-----------|------|
| Psi | Soil capillary suction | mm |
| Ksat | Soil saturated hydraulic conductivity | mm/h |
| IMD | Initial soil moisture deficit | - |

If the landuse for each subcatchment is specified as a comment in the inp-file, the shapefile will also have the following attribute:   

| Name | Attribute |
| -----|-----------|
| landuse | Landuse for the subcatchment |

If there are LID's in use on any of the subcatchments, the shapefile will also have the following attributes:  

| Name | Attribute | Unit |
| -----|-----------|------|
| LID | Name of an LID process | - |
| LIDNumber | Number of replicate LID units deployed | - |
| LIDArea | Area of each replicate unit | m2 |
| LIDWidth | Width of the outflow face of each identical LID unit | m |
| LIDInitSat | Degree of initially water filling in the unit's soil | % |
| LIDFromImp | Percent of the impervious portion of the subcatchment’s non-LID area whose runoff is treated by the LID practice | % |
| LIDToPerv | A value indicating whether (```1```) or not (```0```)the surface and drain flow from the LID unit should be routed back onto the pervious area of the subcatchment that contains it. | - |

If tags have been specified for any of the subcatchments, the shapefile will also have the following attribute:  

| Name | Attribute |
| -----|-----------|
| Tag | Tag for the subcatchment |

#### Subcatchment routing shapefile (\*\_subcatchment_routing.shp)  
The subcatchment routing shapefile has the following attributes:  

| Name | Attribute |
| -----|-----------|
| from | Name of the origin subcatchment |
| to | Name of the target subcatchment/node |

## rpt2gis.py
Extracts subcatchment polygons and node points from a SWMM5 input file (\*.inp) together with subcatchment and node simulation results from the corresponding SWMM5 report file (by default \*.rpt). The tool merges the information and saves the subcatchment results as a polygon shapefile and node results as point shapefile.  
This utility tool is suitable for any SWMM5 input/report file, i.e., it is not restricted to GisToSWMM5 generated files.

#### Subcatchment results shapefile (\*\_subcatchment_result.shp)  

The subcatchment results polygon shapefile has the following attributes:  

| Name | Attribute | Unit |
| -----|-----------|------|
| name | Name of the subcatchment | - |
| precip_mm | Total precipitation | mm |
| runon_mm | Total runon | mm |
| evap_mm | Total evaporation | mm |
| infil_mm | Total infiltration | mm |
| runoff_mm | Total runoff | mm |
| runoff_ML | Total runoff | 10^6 l |
| Q_peak_LPS | Peak runoff | l/s |
| Cr | Runoff coefficient | - ||

If there are LID's in use on any of the subcatchments, the shapefile will also have the following attributes:  

| Name | Attribute | Unit |
| -----|-----------|------|
| LID | LID Control | - |
| LQin_mm | LID total inflow volume | mm |
| Levap_mm | LID evaporation loss | mm |
| Linfil_mm | LID infiltration loss | mm |
| LQout_S_mm | LID surface outflow | mm |
| LQout_D_mm | LID underdrain outflow | mm |
| LIniSto_mm | LID initial storage volume | mm |
| LFinSto_mm | LID final storage volume | mm |
| LError_pct | LID continuity error | % |
*Note: all quantities are expressed as depths (in or mm) over the LID unit’s surface area.*  

If subcatchment washoff has been simulated, the shapefile will also have attributes describing total mass of each pollutant washed off the subcatchment (kg).

#### Node results shapefile (\*\_node_result.shp)

The node results point shapefile has the following attributes:

| Name | Attribute | Unit |
| -----|-----------|------|
| name | Name of the node | - |
| type | Type of the node | - |
| avgDepth | Node average water depth | m |
| maxDepth | Node maximum water depth | m |
| maxHGL | Node maximum hydraulic head (HGL) elevation | m |
| maxDTime | Time of maximum water depth at node | - |
| repMaxD | Node maximum water depth at reporting times | m |
| maxLatIn | Node maximum lateral inflow | l/s |
| maxTotIn | Node maximum total inflow | l/s |
| maxInTime | Time of maximum total inflow at node | - |
| latInVol | Node total lateral inflow volume | 10^6 l |
| totInVol | Node total inflow volume | 10^6 l |
| error_pct | Node flow balance error | % ||
*Note: Total inflow consists of lateral inflow plus inflow from connecting links.*  

If there are any surcharged nodes, the shapefile will also have the following attributes

| Name | Attribute | Unit |
| -----|-----------|------|
| surchgDur | The time node is surcharged | h |
| maxHAC | Maximum height of surcharge above node’s crown | m |
| minDBR | Minimum depth of surcharge below node’s top rim | m |
*Note: surcharging occurs when water rises above the crown of the highest conduit.*  

If there are any flooding nodes, the shapefile will also have the following attributes

| Name | Attribute | Unit |
| -----|-----------|------|
| floodedDur | Time node is flooded | h |
| maxFldRate | Node maximum flooding rate | l/s |
| maxFldTime | Time of maximum flooding at node | - |
| totFldVol | Node total flood volume | 10^6 l |
| maxPondD | Node maximum depth of ponded surface water | m |
*Note: flooding refers to all water that overflows a node, whether it ponds or not.*  

## ExtractLinkData.py

Collates time series data from a given link (in several) SWMM5 report file(s) into a single .csv file.  
This utility tool is suitable for any SWMM5 report file, i.e., it is not restricted to GisToSWMM5 generated files.

## adap2shp.py

Creates a shapefile of adaptive subcatchments with given subcatchment attributes using the information from *"[PATH TO OUTPUT BASE FILENAME]\_subcatchments.asc"* and *"[PATH TO OUTPUT BASE FILENAME]\_subcatchments_attr.wkt"* files.

## ExtractSubcatchmentResults.py
Extracts subcatchment summary results from the SWMM5 report file (by default \*.rpt) and presents them as subcatchment attributes in a Well-Known-Text file.  
Particularly useful for creating subcatchment runoff results shapefile from adaptive model results, as this shapefile cannot be directly produced using the \*.inp and \*.rpt files. To produce the shapefile, first create a \*.wkt file of subcatchment runoff results using *ExtractSubcatchmentResults.py* and then use *adap2shp.py* with the "[PATH TO OUTPUT BASE FILENAME]\_subcatchments.asc" and the newly created \*.wkt file as inputs to produce the final shapefile.

## wkt2shp.sh
Converts all .wkt files in a given folder into shapefiles (e.g. for ArcMap).
