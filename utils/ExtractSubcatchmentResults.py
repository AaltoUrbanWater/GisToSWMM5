#!/usr/bin/env python
"""Extract subcatchment runoff summary results from SWMM report file.

Reads subcatchment geometries from a GisToSWMM5 generated subcatchment geometry
file (*_subcatchments.wkt) file and subcatchment runoff results from a SWMM
report (by default .rpt) file. The script merges the information and saves it
as a Well-Known-Text (.wkt) file.

Copyright (C) 2018 Tero Niemi, Aalto University School of Engineering
"""

import os
import sys
import re
import pandas as pd

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
df1 = pd.read_csv(sys.argv[1], sep=';')
df1.drop(df1.columns[3:], axis=1, inplace=True)  # Remove extra columns

# Read subcatchment runoff summary results ...
data = []
with open(sys.argv[2], 'rt') as rpt_file:
    for line in rpt_file:
        if '  Subcatchment Runoff Summary' in line:
            for idx, row in enumerate(rpt_file):
                if idx < 3:     # Skip extra lines after header
                    continue
                if idx == 3:    # Read first line of column headers
                    subcatchment_headers_1 = row.split()
                if idx == 4:    # Read second line of column headers
                    subcatchment_headers_2 = row.split()
                if idx == 5:    # Read column units
                    subcatchment_units = re.split('\s{2,}', row.strip())
                if idx < 7:     # Skip extra lines after header
                    continue
                if row.isspace():    # Stop looking after empty line
                    break
                if row.startswith('  ---'):  # Skip separator lines
                    break
                else:           # Save data
                    data.append(row.split())

            # Create attribute names from header info
            subcatchment_units.pop(0)
            subcatchment_headers = [a + '' + b for a, b in zip(
                subcatchment_headers_1, subcatchment_headers_2)]
            subcatchment_units.append('-')
            subcatchment_headers = [a + '_' + b for a, b in zip(
                subcatchment_headers, subcatchment_units)]
            subcatchment_headers.insert(0, 'name')

# Create dataframe from data
df2 = pd.DataFrame(data, columns=subcatchment_headers)
df2[subcatchment_headers[1:]] = \
    df2[subcatchment_headers[1:]].astype(float, errors='ignore')

# Merge spatial dataframe with data dataframe
df3 = pd.merge(df1, df2, on='name')
df3 = df3.set_index('id')

# Write final dataframe into a wkt file
df3.to_csv(sys.argv[3], sep=';', index_label='id')
print('Saved subcatchment runoff results to ' + sys.argv[3])
# Write a .csvt file describing the field type information of the .wkt file
# (This is only necessary for ogr2ogr conversion of .wkt to shapefile)
with open(os.path.splitext(sys.argv[3])[0] + '.csvt', 'w') as f:
    f.write('''"Integer","WKT","String","Real","Real","Real","Real","Real","Real","Real","Real"''')
print('Saved field type information to ' + os.path.splitext(sys.argv[3])[0] +
      '.csvt')
