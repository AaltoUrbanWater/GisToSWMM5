@echo off
REM Extract subcatchment geometries and attributes from SWMM input (*.inp) file and save into shapefile.

REM Activate conda environment (with necessary packages installed)
call activate

REM Run inp2gis
REM Change the paths and the coordinate reference system code (EPSG:XXXX) as necessary
python "../utils/inp2gis.py" "../demo_catchment/out/SWMM_in/demo_catchment_adap.inp" "EPSG:3879"

REM Deactivate conda environment
call deactivate

REM Wait for user input before closing terminal
pause