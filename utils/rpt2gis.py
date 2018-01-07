#!/usr/bin/env python
"""Extract subcatchment results from SWMM output file to GIS.

Reads subcatchment geometries from a SWMM input (.inp) file and subcatchment
runoff results from a corresponding SWMM report (by default .rpt) file. The
script merges the information and saves it as a shapefile into the same folder
as the SWMM report file.

Copyright (C) 2018 Tero Niemi, Aalto University School of Engineering
"""

import os
import sys
import pandas as pd
import geopandas as gpd
import shapely.wkt


crs = {'init': 'epsg:4326'}  # Default Coordinate Reference System
# Check input parameters
if (len(sys.argv) < 3 or len(sys.argv) > 4):
    print("Usage:\n"
          "$ python inp2gis.py [PATH TO *.inp FILE] [PATH TO *.rpt FILE] "
          "'EPSG:[XXXX]'")
    sys.exit()
else:
    if (not sys.argv[1].lower().endswith('.inp')):
        print("Usage:\n"
              "$ python inp2gis.py [PATH TO *.inp FILE] [PATH TO *.rpt FILE] "
              "'EPSG:[XXXX]'")
        sys.exit()
    if (len(sys.argv) == 3):
        print('Using EPSG:4326 as default Coordinate Reference System.')
    elif (len(sys.argv) == 4):
        if not (sys.argv[3].lower().startswith('epsg:')):
            print("Usage:\n"
                  "$ python inp2gis.py [PATH TO *.inp FILE] [PATH TO *.rpt "
                  "FILE] 'EPSG:[XXXX]'")
            sys.exit()
        else:
            crs = {'init': sys.argv[3].lower()}  # Custom CRS


polygon_data = []

# Go through the SWMM inp file
with open(sys.argv[1], 'rt', encoding='ISO-8859-1') as inp_file:
    # Check for polygon coordinate information
    for line in inp_file:
        if '[polygons]' in line.lower():
            for idx, row in enumerate(inp_file):
                if row.startswith(';'):  # Skip comment rows
                    continue
                elif row.isspace():  # Stop looking after empty line
                    break
                else:  # Save data
                    polygon_data.append(row.split())

coordinate_col_names = ['Name', 'X', 'Y']
polygon_df = pd.DataFrame(polygon_data, columns=coordinate_col_names)

# Create WKT geometries from subcatchment polygon corner point information
polygon_df['XY'] = polygon_df['X'].map(str) + ' ' + polygon_df['Y'].map(str)
polygon_df = polygon_df.groupby('Name').agg({'XY': lambda x: ','.join(x)})
polygon_df['wktcolumn'] = 'POLYGON((' + polygon_df['XY'].map(str) + '))'
# Check that polygons are closed and report if not
for idx, row in polygon_df.iterrows():
    if(row['XY'].split(',')[0] != row['XY'].split(',')[-1]):
        print('Error: ' + str(idx) + ' is not a closed polygon.')
polygon_df = polygon_df.drop('XY', axis=1)
polygon_df.reset_index(inplace=True)

# Convert to geodatabase for subcatchments
geometry = polygon_df['wktcolumn'].map(shapely.wkt.loads)
polygon_df = polygon_df.drop('wktcolumn', axis=1)
subcatchment_gdf = gpd.GeoDataFrame(polygon_df, crs=crs, geometry=geometry)

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
df2 = pd.DataFrame(data, columns=['Name',
                                  'precip_mm',
                                  'runon_mm',
                                  'evap_mm',
                                  'infil_mm',
                                  'runoff_mm',
                                  'runoff_ML',
                                  'Q_peak_LPS',
                                  'Cr'])

# Merge spatial dataframe with data dataframe
subcatchment_gdf = subcatchment_gdf.merge(df2, on='Name')

# Save subcatchment results as shapefile
subcatchment_gdf.to_file(os.path.splitext(sys.argv[2])[0] +
                         '_subcatchment_results.shp', driver='ESRI Shapefile')
print('Saved subcatchment results to ' + os.path.splitext(sys.argv[2])[0] +
      '_subcatchment_results.shp')
