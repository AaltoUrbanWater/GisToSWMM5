@echo off
REM This is an example script to run ExtractLinkData.py
REM Syntax:
REM 		python ExtractLinkData.py [KEY STRING] [PARAMETER] [PATH(S) TO *.rpt FILE(S)] [PATH TO OUTPUT FILE AS *.csv]
REM Allowed parameters are: 'flow', 'velocity', 'depth', or 'capacity'"

REM Activate conda environment (with necessary packages installed)
REM (here called 'gistoswmm5')
call conda activate gistoswmm5

REM Example of running ExtractLinkData.py
REM '^' is used only to split long lines here
python "..\utils\ExtractLinkData.py" "<<< Link c5 >>>" "flow"^
 "..\demo_catchment\out\SWMM_out\demo_catchment_adap.rpt"^
 "..\demo_catchment\out\SWMM_out\demo_catchment_rect.rpt"^
 "..\demo_catchment\out\SWMM_results\flow_timeseries.csv"

REM Deactivate conda environment
call conda deactivate

REM Wait for user input before closing terminal
pause
