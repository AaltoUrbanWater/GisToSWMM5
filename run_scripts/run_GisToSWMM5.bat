@echo off
REM This is an example script to run GisToSWMM on Windows

..\GisToSWMM5\bin\GisToSWMM5.exe ^
..\data\raster_dem.asc ^
..\data\raster_flowdir.asc ^
..\data\raster_landuse.asc ^
..\data\table_catchment_props.csv ^
..\data\table_junctions.csv ^
..\data\table_conduits.csv ^
..\data\table_outfalls.csv ^
..\data\table_header.csv ^
..\data\table_evaporation.csv ^
..\data\table_temperature.csv ^
..\data\table_snowpacks.csv ^
..\data\table_raingages.csv ^
..\data\table_inflows.csv ^
..\data\table_timeseries.csv ^
..\data\table_pumps.csv ^
..\data\table_pumpcurves.csv ^
..\data\table_storage.csv ^
..\data\table_dwf.csv ^
..\data\table_patterns.csv ^
..\data\table_losses.csv ^
..\data\table_xsections.csv ^
..\data\table_report.csv ^
..\data\table_symbols.csv ^
..\out\demo_catchment ^
1

REM Wait for user input before closing terminal
pause