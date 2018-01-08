@echo off
REM Extract subcatchment simulation results from SWMM report (by default *.rpt) file and save into shapefile with subcatchment geometries from SWMM input (*.inp) file.

REM Activate conda environment (with necessary packages installed)
call activate

REM Run rpt2gis
REM Change the paths and the coordinate reference system code (EPSG:XXXX) as necessary
python "../utils/rpt2gis.py" "../demo_catchment/out/SWMM_in/demo_catchment_adap.inp" "../demo_catchment/out/SWMM_out/demo_catchment_adap.rpt" "EPSG:3879"

REM Deactivate conda environment
call deactivate

REM Wait for user input before closing terminal
pause