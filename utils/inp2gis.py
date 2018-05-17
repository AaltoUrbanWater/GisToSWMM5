#!/usr/bin/env python
"""Extract subcatchment and routing information from SWMM input file to GIS.

Reads subcatchment geometries and routing from a SWMM input (.inp) file and
saves them as shapefiles into the same folder as the SWMM input file.

Copyright (C) 2018 Tero Niemi, Aalto University School of Engineering
"""

import os
import sys
import pandas as pd
import geopandas as gpd
import shapely.wkt


crs = {'init': 'epsg:4326'}  # Default Coordinate Reference System
# Check input parameters
if (len(sys.argv) < 2 or len(sys.argv) > 3):
    print("Usage:\n"
          """$ python inp2gis.py [PATH TO *.inp FILE] 'EPSG:[XXXX]'""")
    sys.exit()
else:
    if (not sys.argv[1].lower().endswith('.inp')):
        print("Usage:\n"
              """$ python inp2gis.py [PATH TO *.inp FILE] 'EPSG:[XXXX]'""")
        sys.exit()
    if (len(sys.argv) == 2):
        print('Using EPSG:4326 as default Coordinate Reference System.')
    elif (len(sys.argv) == 3):
        if not (sys.argv[2].lower().startswith('epsg:')):
            print("Usage:\n"
                  """$ python inp2gis.py [PATH TO *.inp FILE] 'EPSG:[XXXX]'""")
            sys.exit()
        else:
            crs = {'init': sys.argv[2].lower()}  # Custom CRS

subcatchment_data = []
landuse_data = []
subarea_data = []
infiltration_data = []
coordinate_data = []
polygon_data = []
tags_data = []

# Go through the SWMM inp file
with open(sys.argv[1], 'rt', encoding='ISO-8859-1') as inp_file:
    # Check for subcatchment information
    for line in inp_file:
        if '[subcatchments]' in line.lower():
            for idx, row in enumerate(inp_file):
                if row.startswith(';;'):  # Skip comment rows
                    continue
                elif row.isspace():  # Stop looking after empty line
                    break
                elif row.startswith(';'):   # Save landuse
                    try:
                        # TJN 28 Feb 2018 Add .rstrip() here to remove trailing
                        # newline - this has not been tested properly!
                        landuse_data.append(int(row.rstrip().split(";")[1]))
                    except ValueError:
                        landuse_data.append(row.rstrip().split(";")[1])
                else:  # Save data
                    subcatchment_data.append(row.split())

    # Check for subarea information
        if '[subareas]' in line.lower():
            for idx, row in enumerate(inp_file):
                if row.startswith(';'):  # Skip comment rows
                    continue
                elif row.isspace():  # Stop looking after empty line
                    break
                else:  # Save data
                    subarea_data.append(row.split())

    # Check for infiltration information
        if '[infiltration]' in line.lower():
            for idx, row in enumerate(inp_file):
                if row.startswith(';'):  # Skip comment rows
                    continue
                elif row.isspace():  # Stop looking after empty line
                    break
                else:  # Save data
                    infiltration_data.append(row.split())

    # Check for coordinate information
        if '[coordinates]' in line.lower():
            for idx, row in enumerate(inp_file):
                if row.startswith(';'):  # Skip comment rows
                    continue
                elif row.isspace():  # Stop looking after empty line
                    break
                else:  # Save data
                    coordinate_data.append(row.split())
        if '[polygons]' in line.lower():
            for idx, row in enumerate(inp_file):
                if row.startswith(';'):  # Skip comment rows
                    continue
                elif row.isspace():  # Stop looking after empty line
                    break
                else:  # Save data
                    polygon_data.append(row.split())
        # Check for tag information
        if '[tags]' in line.lower():
            for idx, row in enumerate(inp_file):
                if row.startswith(';;'):  # Skip comment rows
                    continue
                elif row.isspace():  # Stop looking after empty line
                    break
                else:  # Save data
                    tags_data.append(row.split(None, 2))

# Create dataframes from data
subcatchment_col_names = ['Name',
                          'Rgage',
                          'OutID',
                          'Area',
                          'Imperv_pct',
                          'Width',
                          'Slope',
                          'Clength']
if (len(subcatchment_data[0]) == 9):
    subcatchment_col_names.append('SPack')
subcatchment_df = pd.DataFrame(subcatchment_data,
                               columns=subcatchment_col_names)
subcatchment_df[['Area',
                 'Imperv_pct',
                 'Width',
                 'Slope',
                 'Clength']] = subcatchment_df[['Area',
                                                'Imperv_pct',
                                                'Width',
                                                'Slope',
                                                'Clength']].astype(float)

subcatchment_df = subcatchment_df.assign(
                    landuse=pd.DataFrame(landuse_data).values)

subarea_col_names = ['Name',
                     'Nimp',
                     'Nperv',
                     'Simp',
                     'Sperv',
                     'Zero_pct',
                     'RouteTo']
if (len(subarea_data[0]) == 8):
    subarea_col_names.append('Routed_pct')
subarea_df = pd.DataFrame(subarea_data, columns=subarea_col_names)
subarea_df[['Nimp',
            'Nperv',
            'Simp',
            'Sperv',
            'Zero_pct']] = subarea_df[['Nimp',
                                       'Nperv',
                                       'Simp',
                                       'Sperv',
                                       'Zero_pct']].astype(float)

if (len(infiltration_data[0]) == 6):    # Horton infiltration
    infiltration_col_names = ['Name',
                              'MaxRate',
                              'MinRate',
                              'Decay',
                              'DryTime',
                              'MaxInf']
    infiltration_df = pd.DataFrame(infiltration_data,
                                   columns=infiltration_col_names)
    infiltration_df[['MaxRate',
                     'MinRate',
                     'Decay',
                     'DryTime',
                     'MaxInf']] = infiltration_df[['MaxRate',
                                                   'MinRate',
                                                   'Decay',
                                                   'DryTime',
                                                   'MaxInf']].astype(float)
elif (len(infiltration_data[0]) == 4):  # Green-Ampt infiltration
    infiltration_col_names = ['Name',
                              'Psi',
                              'Ksat',
                              'IMD']
    infiltration_df = pd.DataFrame(infiltration_data,
                                   columns=infiltration_col_names)
    infiltration_df[['Psi',
                     'Ksat',
                     'IMD']] = infiltration_df[['Psi',
                                                'Ksat',
                                                'IMD']].astype(float)

if tags_data:
    tags_col_names = ['Type',
                      'Name',
                      'Tag']
    tags_df = pd.DataFrame(tags_data, columns=tags_col_names)
    tags_df = tags_df.drop('Type', axis=1)

coordinate_col_names = ['Name', 'X', 'Y']
coordinate_df = pd.DataFrame(coordinate_data, columns=coordinate_col_names)
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
subcatchment_gdf['centroid'] = subcatchment_gdf['geometry'].centroid.map(
                               lambda p: p.x).map(str) + ' ' + \
                               subcatchment_gdf['geometry'].centroid.map(
                               lambda p: p.y).map(str)

# Merge subcatchment dataframes
subcatchment_gdf = subcatchment_gdf.merge(subcatchment_df, on='Name')
subcatchment_gdf = subcatchment_gdf.merge(subarea_df, on='Name')
subcatchment_gdf = subcatchment_gdf.merge(infiltration_df, on='Name')
if tags_data:
    subcatchment_gdf = subcatchment_gdf.merge(tags_df, on='Name')

# Create WKT geometries from junction point information
coordinate_df['centroid'] = coordinate_df['X'].map(str) + ' ' + \
                      coordinate_df['Y'].map(str)

# Create a dictionary of (Name, coordinate) pairs for routing
junction_dict = dict(zip(coordinate_df['Name'], coordinate_df['centroid']))
subcatchment_dict = dict(zip(subcatchment_gdf['Name'],
                             subcatchment_gdf['centroid']))
coordinate_dict = junction_dict.copy()
coordinate_dict.update(subcatchment_dict)

# Create a WKT polyline of routing between subcatchments
subcatchment_df['wktcolumn'] = 'LINESTRING(' + \
                               subcatchment_df['Name'].map(
                               coordinate_dict).map(str) + ',' + \
                               subcatchment_df['OutID'].map(
                               coordinate_dict).map(str) + ')'
# Convert to geodatabase for routing
geometry = subcatchment_df['wktcolumn'].map(shapely.wkt.loads)
subcatchment_df = subcatchment_df.drop('wktcolumn', axis=1)
subcatchment_df = subcatchment_df.drop('Rgage', axis=1)
subcatchment_df = subcatchment_df.drop('Area', axis=1)
subcatchment_df = subcatchment_df.drop('Imperv_pct', axis=1)
subcatchment_df = subcatchment_df.drop('Width', axis=1)
subcatchment_df = subcatchment_df.drop('Slope', axis=1)
subcatchment_df = subcatchment_df.drop('Clength', axis=1)
if 'SPack' in subcatchment_df.columns:
    subcatchment_df = subcatchment_df.drop('SPack', axis=1)
routing_gdf = gpd.GeoDataFrame(subcatchment_df, crs=crs, geometry=geometry)
routing_gdf.rename(index=str, columns={"Name": "from", "OutID": "to"},
                   inplace=True)

# Save subcatchments as shapefile
subcatchment_gdf = subcatchment_gdf.drop('centroid', axis=1)
subcatchment_gdf.to_file(os.path.splitext(sys.argv[1])[0] +
                         '_subcatchments.shp', driver='ESRI Shapefile')
print('Saved subcatchments to ' + os.path.splitext(sys.argv[1])[0] +
      '_subcatchments.shp')
# Save subcatchment routing as shapefile
routing_gdf.to_file(os.path.splitext(sys.argv[1])[0] +
                    '_subcatchment_routing.shp', driver='ESRI Shapefile')
print('Saved subcatchment routing to ' + os.path.splitext(sys.argv[1])[0] +
      '_subcatchment_routing.shp')
