17 Feb 2017 Tero Niemi

ExtractDataSets is a tool to collate time series data from (several) 
SWMM5 report file(s) to single .csv file. 

Usage:
	Preferred method to use the tool is to modify the script 
	run_ExtractDataSets as necessary and then run the script using
	$ ./run_ExtractDataSets

	ExtractDataSets uses instruction files (.INS) and SWMM5 report files
	(usually .rpt or .DAT) as input. .INS files specify the 
	column that is copied to the .csv file. There are .INS files for 
	date column (date.INS), time column (time.INS) and discharge
	colum (discharge.INS). Using a combination of .INS and report files 
	the results from several individual results files can be collated 
	into one .csv file.
	
	Example:
	$ ./ExtractDataSets date.INS test1.rpt time.INS test1.rpt \
	discharge.INS test1.rpt simulation_results.csv 

Tool was developed by Lassi Warsta (lassi@warsta.net)
