@echo off
REM This is an example script to run GisToSWMM on Windows

..\bin\RasterToSWMM5.exe ^
..\demo_catchment\data\raster_dem.asc ^
..\demo_catchment\data\raster_landuse.asc ^
..\demo_catchment\data\table_catchment_props.csv ^
..\demo_catchment\data\table_junctions.csv ^
..\demo_catchment\data\table_conduits.csv ^
..\demo_catchment\data\table_outfalls.csv ^
..\demo_catchment\data\table_header.csv ^
..\demo_catchment\data\table_evaporation.csv ^
..\demo_catchment\data\table_temperature.csv ^
..\demo_catchment\data\table_snowpacks.csv ^
..\demo_catchment\data\table_raingages.csv ^
..\demo_catchment\data\table_xsections.csv ^
..\demo_catchment\data\table_report.csv ^
..\demo_catchment\data\table_symbols.csv ^
..\demo_catchment\out\SWMM_in\demo_catchment.asc ^
..\demo_catchment\out\SWMM_in\demo_catchment.inp

REM Wait for user input before closing terminal
pause