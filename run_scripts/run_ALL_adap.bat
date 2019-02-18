@echo off
REM This is an example script to demonstrate use of GisToSWMM5, SWMM and utility scripts in adaptive mode on Windows


REM Create SWMM5 model using GisToSWMM5
echo Running GisToSWMM5
call run_GisToSWMM5_adap.bat


REM Convert the subcatchment GIS files from .asc and .wkt into shapefiles.
echo.
echo Running adap2shp.py
call run_adap2shp.bat

REM Create routing shapefiles. 
REM Note that we use inp2gis.py to create the routing shapefile, but because the .inp file only has 
REM catchment center points for adap model, we cannot use it to create the subcatchment shapefile.
REM run_inp2gis.bat shows an example of using inp2gis to create both subcatchment and routing shapefiles.
echo.
echo Running inp2gis.py
call conda activate gistoswmm5
python "../utils/inp2gis.py" "../demo_catchment/out/SWMM_in/demo_catchment_adap.inp" "EPSG:3879"
call conda deactivate
REM Remove the inp2gis created subcatchments-shapefile and move the created routing shapefile to
REM folder SWMM_in/shp
echo.
echo Moving and deleting unnecessary files
FOR %%F IN (".prj",".cpg",".shx",".shp",".dbf") DO (
	del ..\demo_catchment\out\SWMM_in\demo_catchment_adap_subcatchments%%F /f /q
	move ..\demo_catchment\out\SWMM_in\demo_catchment_adap_subcatchment_routing%%F ..\demo_catchment\out\SWMM_in\shp\ )
REM Wait for user input 
pause
	
REM Run SWMM using the created inp-file
echo.
echo Running SWMM
"../../swmm/swmm5.exe" "../demo_catchment/out/SWMM_in/demo_catchment_adap.inp" "../demo_catchment/out/SWMM_out/demo_catchment_adap.rpt"


REM Collect subcatchment results from the SWMM report and save as shapefile
echo.
echo Running ExtractSubcatchmentResults.py
call run_ExtractSubcatchmentResults.bat


REM Collect link time series data from the SWMM report files and save to csv-file
echo.
echo Running ExtractLinkData.py
call run_ExtractLinkData.bat


REM Wait for user input before closing terminal
pause