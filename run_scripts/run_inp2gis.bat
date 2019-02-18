@echo off
REM Extract subcatchment geometries and attributes from SWMM input (*.inp) file
REM and save into shapefile.
REM Syntax:
REM 	python inp2gis.py [PATH TO *.inp FILE] 'EPSG:[XXXX]'


REM Activate conda environment (with necessary packages installed)
REM (here called 'gistoswmm5')
call conda activate gistoswmm5

REM Example of running inp2gis.py
REM '^' is used only to split long lines here
python "../utils/inp2gis.py"^
 "../demo_catchment/out/SWMM_in/demo_catchment_rect.inp"^
 "EPSG:3879"

REM Deactivate conda environment
call conda deactivate

REM Wait for user input before closing terminal
pause
