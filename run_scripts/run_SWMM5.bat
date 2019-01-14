@echo off
REM Test SWMM5 simulation
REM This is an example script to run SWMM5 from command line 
REM Syntax:
REM 		[PATH TO swmm5.exe] [PATH TO SWMM5 INPUT FILE (*.inp)] [PATH TO SWMM5 OUTPUT SUMMARY FILE (*.rpt)] [(OPTIONAL) PATH TO SWMM5 OUTPUT BINARY FILE (*.out)]


REM Save only the summary (text) output file (*.rpt) with given time series at the end of the file
"../../../swmm/swmm5.exe" "../demo_catchment/out/SWMM_in/demo_catchment_adap.inp" "../demo_catchment/out/SWMM_out/demo_catchment_adap.rpt"

REM Save the summary (text) output file (*.rpt) and the binary file (*.out) with detailed simulation results only in the binary file
"../../../swmm/swmm5.exe" "../demo_catchment/out/SWMM_in/demo_catchment_rect.inp" "../demo_catchment/out/SWMM_out/demo_catchment_rect.rpt" "../demo_catchment/out/SWMM_out/demo_catchment_rect.out"

REM Wait for user input before closing terminal
pause


