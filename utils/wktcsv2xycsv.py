#!/usr/bin/env python
"""Convert conduit input file into format required by GisToSWMM5."""

import sys
import pandas as pd

# Check input parameters
if (len(sys.argv) != 2):
    print("Usage:\n"
          """$ python wktcsv2xycsv.py [PATH TO *.csv FILE]""")
    sys.exit()
else:
    if (not sys.argv[1].lower().endswith('.csv')):
        print("Usage:\n"
              """$ python wktcsv2xycsv.py [PATH TO *.csv FILE]'""")

# Read data from csv file
in_data = pd.read_csv(sys.argv[1], sep=';')
# Save backup of old data
in_data.to_csv(sys.argv[1] + '.bak', index=False, sep=';')

# Extract x,y data for linestring begin and end points from geometry
in_data['x0'] = in_data.wkt_geom.str.extract('.*\((.*)\ .*,', expand=False)
in_data['y0'] = in_data.wkt_geom.str.extract('.*\ (.*)\,.*', expand=False)
in_data['x1'] = in_data.wkt_geom.str.extract('.*\,\ (.*)\ .*', expand=False)
in_data['y1'] = in_data.wkt_geom.str.extract('.*\ (.*)\).*', expand=False)

# Remove wkt column
del in_data['wkt_geom']

# Reorder columns
cols = in_data.columns.tolist()
cols = cols[-4:] + cols[:-4]
in_data = in_data[cols]

# Save output as csv over old file
in_data.to_csv(sys.argv[1], index=False)

# Notify user
print('Done!')
