#!/usr/bin/env python
"""Extract subcatchment runoff summary results from SWMM report file.

Reads subcatchment geometries from a GisToSWMM5 generated subcatchment geometry
file (*_subcatchments.wkt) file and subcatchment runoff results from a SWMM
report (by default .rpt) file. The script merges the information and saves it
as a Well-Known-Text (.wkt) file.

Copyright (C) 2018 Tero Niemi, Aalto University School of Engineering
"""

import sys
import pandas as pd
import os

# Check input parameters
if (len(sys.argv) != 4):
    print("Usage:\n"
          "./ExtractSubcatchmentResults.py [PATH TO *_subcatchments.wkt FILE] "
          "[PATH TO SWMM *.rpt OUTPUT FILE] [PATH TO OUTPUT FILE AS *.wkt]")
    sys.exit()
elif (not sys.argv[1].lower().endswith('.wkt')):
    print('Error:\n'
          'Second argument has to be a [PATH TO *_subcatchments.wkt FILE]')
    sys.exit()
elif (not sys.argv[2].lower().endswith('.rpt')):
    print('Error:\n'
          'Third argument has to be a [PATH TO SWMM *.rpt OUTPUT FILE]')
    sys.exit()
elif (not sys.argv[3].lower().endswith('.wkt')):
    print('Error:\n'
          'Final argument has to be a [PATH TO OUTPUT FILE AS *.wkt]')
    sys.exit()

# Read subcatchment spatial information from the wkt file into a dataframe
df1 = pd.read_csv(sys.argv[1], sep=';', index_col='id')
df1.drop(df1.columns[2:15], axis=1, inplace=True)  # Remove extra columns

# Read subcatchment runoff summary results ...
data = []
with open(sys.argv[2], 'rt') as rpt_file:
    for line in rpt_file:
        if '  Subcatchment Runoff Summary' in line:
            for idx, row in enumerate(rpt_file):
                if idx < 7:     # Skip extra lines after header
                    continue
                if row.isspace():    # Stop looking after empty line
                    break
                else:           # Save data
                    data.append(row.split())
# ... and create a dataframe from data
df2 = pd.DataFrame(data, columns=['name',
                                  'precip_mm',
                                  'runon_mm',
                                  'evap_mm',
                                  'infil_mm',
                                  'runoff_mm',
                                  'runoff_ML',
                                  'Q_peak_LPS',
                                  'Cr'])

# Merge spatial dataframe with data dataframe
df3 = pd.merge(df1, df2, on='name')

# Write final dataframe into a wkt file
df3.to_csv(sys.argv[3], sep=';', index_label='id')
print('Saved subcatchment runoff results to ' + sys.argv[3])
# Write a .csvt file describing the field type information of the .wkt file
# (This is only necessary for ogr2ogr conversion of .wkt to shapefile)
with open(os.path.splitext(sys.argv[3])[0] + '.csvt', 'w') as f:
    f.write('''"Integer","WKT","String","Real","Real","Real","Real","Real","Real","Real","Real"''')
print('Saved field type information to ' + os.path.splitext(sys.argv[3])[0] +
      '.csvt')
