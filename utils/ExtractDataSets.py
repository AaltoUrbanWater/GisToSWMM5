#!/usr/bin/env python
"""Collate timeseries data from (many) SWMM5 report file(s) to a .csv file."""

import sys
import pandas as pd

# Check input parameters
inFiles = []
if (len(sys.argv) < 4):
    print("Usage:\n"
          "./ExtractDataSets.py [KEY STRING] [PATH(S) TO *.rpt FILE(S)] "
          "[PATH TO OUTPUT FILE AS *.csv]")
    sys.exit()
for i in range(len(sys.argv)):
    if i < 2:
        continue
    elif (i == len(sys.argv)-1):
        if (not sys.argv[i].lower().endswith('.csv')):
            print('Error:\n'
                  'Final argument has to be a [PATH TO OUTPUT FILE AS *.csv]')
            sys.exit()
    else:
        if (not sys.argv[i].lower().endswith('.rpt')):
            print('Error:\n'
                  'Argument: ' + sys.argv[i] +
                  '\nArgument has to be a [PATH TO *.rpt FILE]')
            sys.exit()
        inFiles.append(sys.argv[i])

# Go through SWMM report files and collect data
cols = ['datetime']  # columns to save (the rest are defined later)
first_file = True
for fileIdx, inFile in enumerate(inFiles):
    with open(inFile, 'rt') as rpt_file:
        data = []
        for line in rpt_file:
            if sys.argv[1] in line:
                for idx, row in enumerate(rpt_file):
                    if idx < 4:  # Skip extra lines after header
                        continue
                    if row.isspace():  # Stop looking after empty line
                        break
                    else:  # Save data
                        data.append(row.split())
        # Create dataframe for the results of this file
        df_single = pd.DataFrame(data, columns=['datetime',
                                                'time',
                                                'flow_lps_' + str(fileIdx),
                                                'vel_ms_' + str(fileIdx),
                                                'depth_m_' + str(fileIdx),
                                                'cap_m_' + str(fileIdx)])
        df_single['datetime'] = pd.to_datetime(df_single['datetime'] + ' ' +
                                               df_single['time'])
        cols.append('flow_lps_' + str(fileIdx))  # Add new columns to save
        if first_file:  # Create dataframe for results from all files
            df_all = df_single.copy()
            print('\nFile indexes in ' + sys.argv[-1] + ':')
            print(str(fileIdx) + '\t' + inFile)
            first_file = False
        else:  # Merge with previous results of previous files
            df_all = pd.merge(df_all, df_single, on='datetime', how='outer')
            print(str(fileIdx) + '\t' + inFile)
print('\n')

# Export all results to csv file
df_all.to_csv(sys.argv[-1], columns=cols, index=False)
print('Saved time series to ' + sys.argv[-1])
