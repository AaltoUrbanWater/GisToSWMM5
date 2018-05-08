@echo off
REM This is an example script to run ExtractSubcatchmentResults.py and
REM adap2shp.py in order to produce a shapefile of SWMM subcatchment 
REM simulation results. 
REM Syntax:
REM 	python ExtractSubcatchmentResults.py [INFILE.wkt] [INFILE.rpt] [OUTFILE.wkt]
REM 	python adap2shp.py [INFILE_subcatchments.asc] [INFILE.wkt] [OUTFILE.shp] 'epsg:XXXX'


REM Activate conda environment (with necessary packages installed)
call activate

REM Example of running ExtractSubcatchmentResults.py
REM '^' is used only to split long lines here
python "../utils/ExtractSubcatchmentResults.py" ^
"../demo_catchment/out/SWMM_in/demo_catchment_adap_subcatchments_attr.wkt" ^
"../demo_catchment/out/SWMM_out/demo_catchment_adap.rpt" ^
"../demo_catchment/out/SWMM_results/demo_catchment_adap_subcatchment_runoffs.wkt"

REM Example of running adap2shp.py
REM '^' is used only to split long lines here
python "../utils/adap2shp.py" ^
"../demo_catchment/out/SWMM_in/demo_catchment_adap_subcatchments.asc" ^
"../demo_catchment/out/SWMM_results/demo_catchment_adap_subcatchment_runoffs.wkt" ^
"../demo_catchment/out/SWMM_results/demo_catchment_adap_subcatchment_runoffs.shp" ^
"epsg:3879"

REM Deactivate conda environment
call deactivate

REM Wait for user input before closing terminal
pause