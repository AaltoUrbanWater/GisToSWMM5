@echo off
REM Extract data from SWMM binaries and save to text file

REM Activate conda environment (with necessary packages installed)
call activate swmmtoolbox

REM Run swmmtoolbox
REM List the objects in output file
echo List objects in output file:
swmmtoolbox catalog "C:\LocalUserData\User-data\tjniemi\GisToSWMM5\demo_catchment\out\SWMM_out\demo_catchment_adap.out"
echo **********

REM List variables available for each type (subcatchment, node, link, pollutant, system)
echo List variables and types in output file:
swmmtoolbox listvariables "C:\LocalUserData\User-data\tjniemi\GisToSWMM5\demo_catchment\out\SWMM_out\demo_catchment_adap.out"
echo **********

REM Extract system runoff and save to new file
echo Extracting system runoff and saving to new file.
swmmtoolbox extract "C:\LocalUserData\User-data\tjniemi\GisToSWMM5\demo_catchment\out\SWMM_out\demo_catchment_adap.out" system,Runoff,4 > system_runoff.txt
REM Extract conduit time series and save to new file, e.g. here flow rate
echo Extracting flow rate timeseries for link c5 and saving to new file.
swmmtoolbox extract "C:\LocalUserData\User-data\tjniemi\GisToSWMM5\demo_catchment\out\SWMM_out\demo_catchment_adap.out" link,c5,0 > c5_flowrate.txt

REM Deactivate conda environment
call deactivate

REM Wait for user input before closing terminal
pause 
