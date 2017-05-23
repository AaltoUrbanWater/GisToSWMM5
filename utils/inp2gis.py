#!/usr/bin/env python
"""Extract subcatchment and routing information from SWMM input file to GIS."""

import sys
import pandas as pd
import geopandas as gpd
import shapely.wkt

# Check input parameters
if (len(sys.argv) != 2):
    print("Usage:\n"
          "$ python inp2gis.py [PATH TO *.inp FILE]")
    sys.exit()
if (not sys.argv[1].lower().endswith('.inp')):
    print("Usage:\n"
          "$ python inp2gis.py [PATH TO *.inp FILE]")
    sys.exit()

subcatchment_data = []
subarea_data = []
infiltration_data = []
coordinate_data = []
polygon_data = []

# Go through the SWMM inp file
with open(sys.argv[1], 'rt', encoding='ISO-8859-1') as inp_file:
    # Check for subcatchment information
    for line in inp_file:
        if '[subcatchments]' in line.lower():
            for idx, row in enumerate(inp_file):
                if row.startswith(';'):  # Skip comment rows
                    continue
                elif row.isspace():  # Stop looking after empty line
                    break
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

coordinate_col_names = ['Name', 'X', 'Y']
coordinate_df = pd.DataFrame(coordinate_data, columns=coordinate_col_names)
polygon_df = pd.DataFrame(polygon_data, columns=coordinate_col_names)

# Create WKT geometries from polygon corner point information
polygon_df['XY'] = polygon_df['X'].map(str) + ' ' + polygon_df['Y'].map(str)
polygon_df = polygon_df.groupby('Name').agg({'XY': lambda x: ','.join(x)})
polygon_df['wktcolumn'] = 'POLYGON((' + polygon_df['XY'].map(str) + '))'
polygon_df = polygon_df.drop('XY', axis=1)
polygon_df.reset_index(inplace=True)

# Convert to geodatabase
geometry = polygon_df['wktcolumn'].map(shapely.wkt.loads)
polygon_df = polygon_df.drop('wktcolumn', axis=1)
crs = {'init': 'epsg:3047'}
subcatchment_gdf = gpd.GeoDataFrame(polygon_df, crs=crs, geometry=geometry)
subcatchment_gdf['centroid'] = subcatchment_gdf['geometry'].centroid

# Merge subcatchment dataframes
subcatchment_gdf = subcatchment_gdf.merge(subcatchment_df, on='Name')
subcatchment_gdf = subcatchment_gdf.merge(subarea_df, on='Name')
subcatchment_gdf = subcatchment_gdf.merge(infiltration_df, on='Name')

# Routing ...

print(subcatchment_df)
print(subarea_df)
print(infiltration_df)
print(coordinate_df)
print(polygon_df)
print(subcatchment_gdf)

# Save subcatchments as shapefile
subcatchment_gdf = subcatchment_gdf.drop('centroid', axis=1)
subcatchment_gdf.to_file('MyGeometries.shp', driver='ESRI Shapefile')
