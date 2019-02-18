@echo off
REM Extract subcatchment simulation results from SWMM report (by default *.rpt) file
REM and save into shapefile with subcatchment geometries from SWMM input (*.inp) file.
REM Syntax:
REM 	python rpt2gis.py [PATH TO *.inp FILE] [PATH TO *.rpt FILE] 'EPSG:[XXXX]'


REM Activate conda environment (with necessary packages installed)
REM (here called 'gistoswmm5')
call conda activate gistoswmm5

REM Example of running rpt2gis.py
REM '^' is used only to split long lines here
python "../utils/rpt2gis.py"^
 "../demo_catchment/out/SWMM_in/demo_catchment_rect.inp"^
 "../demo_catchment/out/SWMM_out/demo_catchment_rect.rpt"^
 "EPSG:3879"

REM Deactivate conda environment
call conda deactivate

REM Wait for user input before closing terminal
pause
