#!/usr/bin/env python
"""Extract subcatchment results from SWMM output file to GIS.

Reads subcatchment geometries from a SWMM input (.inp) file and subcatchment
runoff results from a corresponding SWMM report (by default .rpt) file. The
script merges the information and saves it as a shapefile into the same folder
as the SWMM report file.

Copyright (C) 2018 Tero Niemi, Aalto University School of Engineering

TODO: Add link/conduit results
TODO: Add link/conduit pollutant load results
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


# Go through the SWMM inp file
node_data = []
polygon_data = []
with open(sys.argv[1], 'rt', encoding='ISO-8859-1') as inp_file:
    # Check for node coordinate information
    for line in inp_file:
        if '[coordinates]' in line.lower():
            for idx, row in enumerate(inp_file):
                if row.startswith(';'):  # Skip comment rows
                    continue
                elif row.isspace():  # Stop looking after empty line
                    break
                else:  # Save data
                    node_data.append(row.split())
    # Check for polygon coordinate information
        if '[polygons]' in line.lower():
            for idx, row in enumerate(inp_file):
                if row.startswith(';'):  # Skip comment rows
                    continue
                elif row.isspace():  # Stop looking after empty line
                    break
                else:  # Save data
                    polygon_data.append(row.split())

coordinate_col_names = ['name', 'X', 'Y']
polygon_df = pd.DataFrame(polygon_data, columns=coordinate_col_names)
node_df = pd.DataFrame(node_data, columns=coordinate_col_names)

# Create WKT geometries from subcatchment polygon corner point information
polygon_df['XY'] = polygon_df['X'].map(str) + ' ' + polygon_df['Y'].map(str)
polygon_df = polygon_df.groupby('name').agg({'XY': lambda x: ','.join(x)})
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

# Create WKT geometries from node point information
node_df['XY'] = node_df['X'].map(str) + ' ' + node_df['Y'].map(str)
node_df['wktcolumn'] = 'POINT(' + node_df['XY'].map(str) + ')'
node_df = node_df.drop('XY', axis=1)
node_df.reset_index(inplace=True)
# Convert to geodatabase for junctions
node_geometry = node_df['wktcolumn'].map(shapely.wkt.loads)
node_df = node_df.drop('wktcolumn', axis=1)
node_gdf = gpd.GeoDataFrame(node_df, crs=crs, geometry=node_geometry)

# Go through the SWMM rpt file
subcatchment_data = []
lid_data = []
subcatchmentWash_data = []
nodeDepth_data = []
nodeInflow_data = []
nodeSurcharge_data = []
nodeFlooding_data = []
with open(sys.argv[2], 'rt', encoding='ISO-8859-1') as rpt_file:
    for line in rpt_file:
        # Read simulation start date
        if '  Starting Date ...' in line:
            start_time = pd.to_datetime(line.split()[3], format='%m/%d/%Y')
        # Read subcatchment runoff summary results
        if '  Subcatchment Runoff Summary' in line:
            for idx, row in enumerate(rpt_file):
                if idx < 7:     # Skip extra lines after header
                    continue
                if row.isspace():    # Stop looking after empty line
                    break
                else:           # Save data
                    subcatchment_data.append(row.split())
        # Read LID summary results
        if '  LID Performance Summary' in line:
            for idx, row in enumerate(rpt_file):
                if idx < 7:     # Skip extra lines after header
                    continue
                if row.isspace():    # Stop looking after empty line
                    break
                else:           # Save data
                    lid_data.append(row.split())
        # Read subcatchment washoff summary results
        if '  Subcatchment Washoff Summary' in line:
            for idx, row in enumerate(rpt_file):
                if idx < 3:    # Skip extra lines after header
                    continue
                if idx == 3:   # Read column headers
                    subcatchmentWash_headers = row.split()
                if idx == 4:   # Read column units
                    subcatchmentWash_units = row.split()
                if idx < 6:    # Skip extra lines after header
                    continue
                if row.isspace():    # Stop looking after empty line
                    break
                if row.startswith('  ---'):  # Skip separator lines
                    break
                else:           # Save data
                    subcatchmentWash_data.append(row.split())

            # Create attribute names from header info
            subcatchmentWash_units.pop(0)
            subcatchmentWash_headers = [a + '_' + b for a, b in zip(
                subcatchmentWash_headers, subcatchmentWash_units)]
            subcatchmentWash_headers.insert(0, 'name')
        # Read node depth summary results
        if '  Node Depth Summary' in line:
            for idx, row in enumerate(rpt_file):
                if idx < 7:     # Skip extra lines after header
                    continue
                if row.isspace():    # Stop looking after empty line
                    break
                else:           # Save data
                    nodeDepth_data.append(row.split())
        # Read node inflow summary results
        if '  Node Inflow Summary' in line:
            for idx, row in enumerate(rpt_file):
                if idx < 8:     # Skip extra lines after header
                    continue
                if row.isspace():    # Stop looking after empty line
                    break
                else:           # Save data
                    nodeInflow_data.append(row.split()[0:9])
        # Read node surcharge summary results
        if '  Node Surcharge Summary' in line:
            for idx, row in enumerate(rpt_file):
                if idx < 8:     # Skip extra lines after header
                    continue
                if row.isspace():    # Stop looking after empty line
                    break
                else:           # Save data
                    nodeSurcharge_data.append(row.split())
        # Read node flooding summary results
        if '  Node Flooding Summary' in line:
            for idx, row in enumerate(rpt_file):
                if '  No nodes were flooded.' in row:
                    break
                if idx < 9:     # Skip extra lines after header
                    continue
                if row.isspace():    # Stop looking after empty line
                    break
                else:           # Save data
                    nodeFlooding_data.append(row.split())

# Create dataframes from data
subcatchment_df = pd.DataFrame(subcatchment_data, columns=['name',
                                                           'precip_mm',
                                                           'runon_mm',
                                                           'evap_mm',
                                                           'infil_mm',
                                                           'runoff_mm',
                                                           'runoff_ML',
                                                           'Q_peak_LPS',
                                                           'Cr'])
subcatchment_df[['precip_mm',
                 'runon_mm',
                 'evap_mm',
                 'infil_mm',
                 'runoff_mm',
                 'runoff_ML',
                 'Q_peak_LPS',
                 'Cr']] = subcatchment_df[['precip_mm',
                                           'runon_mm',
                                           'evap_mm',
                                           'infil_mm',
                                           'runoff_mm',
                                           'runoff_ML',
                                           'Q_peak_LPS',
                                           'Cr']].astype(float)

if lid_data:
    lid_df = pd.DataFrame(lid_data, columns=['name',
                                             'LID',
                                             'LQin_mm',
                                             'Levap_mm',
                                             'Linfil_mm',
                                             'LQout_S_mm',
                                             'LQout_D_mm',
                                             'LIniSto_mm',
                                             'LFinSto_mm',
                                             'LError_pct'])
    lid_df[['LQin_mm',
            'Levap_mm',
            'Linfil_mm',
            'LQout_S_mm',
            'LQout_D_mm',
            'LIniSto_mm',
            'LFinSto_mm',
            'LError_pct']] = lid_df[['LQin_mm',
                                     'Levap_mm',
                                     'Linfil_mm',
                                     'LQout_S_mm',
                                     'LQout_D_mm',
                                     'LIniSto_mm',
                                     'LFinSto_mm',
                                     'LError_pct']].astype(float)

if subcatchmentWash_data:
    subcatchmentWash_df = pd.DataFrame(subcatchmentWash_data,
                                       columns=subcatchmentWash_headers)
    subcatchmentWash_df[subcatchmentWash_headers[1:]] = subcatchmentWash_df[
        subcatchmentWash_headers[1:]].astype(float)

node_df = pd.DataFrame(nodeDepth_data, columns=['name',
                                                'type',
                                                'avgDepth',
                                                'maxDepth',
                                                'maxHGL',
                                                'maxHGLT_d',
                                                'maxHGLT_hm',
                                                'repMaxD'])
node_df[['avgDepth',
         'maxDepth',
         'maxHGL',
         'repMaxD']] = node_df[['avgDepth',
                                'maxDepth',
                                'maxHGL',
                                'repMaxD']].astype(float)
node_df['maxHGLTD'] = start_time + \
    pd.to_timedelta(node_df['maxHGLT_d'].astype(float), unit='D')
node_df['maxHGLTT'] = pd.to_datetime(node_df['maxHGLT_hm'],
                                     format='%H:%M').dt.time
node_df['maxDTime'] = node_df.apply(lambda r: pd.datetime.combine(
                                      r['maxHGLTD'], r['maxHGLTT']).strftime(
                                      '%Y-%m-%d %H:%M'), 1)
node_df = node_df.drop('maxHGLT_d', axis=1)
node_df = node_df.drop('maxHGLT_hm', axis=1)
node_df = node_df.drop('maxHGLTD', axis=1)
node_df = node_df.drop('maxHGLTT', axis=1)

if nodeInflow_data:
    nodeInflow_df = pd.DataFrame(nodeInflow_data, columns=['name',
                                                           'type',
                                                           'maxLatIn',
                                                           'maxTotIn',
                                                           'maxInT_d',
                                                           'maxInT_hm',
                                                           'latInVol',
                                                           'totInVol',
                                                           'error_pct'])

nodeInflow_df['maxInD'] = start_time + \
    pd.to_timedelta(nodeInflow_df['maxInT_d'].astype(float), unit='D')
nodeInflow_df['maxInT'] = pd.to_datetime(nodeInflow_df['maxInT_hm'],
                                         format='%H:%M').dt.time
nodeInflow_df['maxInTime'] = nodeInflow_df.apply(lambda r:
                                                 pd.datetime.combine(
                                                    r['maxInD'],
                                                    r['maxInT']).strftime(
                                                    '%Y-%m-%d %H:%M'), 1)
nodeInflow_df = nodeInflow_df.drop('maxInT_d', axis=1)
nodeInflow_df = nodeInflow_df.drop('maxInT_hm', axis=1)
nodeInflow_df = nodeInflow_df.drop('maxInD', axis=1)
nodeInflow_df = nodeInflow_df.drop('maxInT', axis=1)
nodeInflow_df = nodeInflow_df.drop('type', axis=1)

nodeInflow_df[['maxLatIn',
               'maxTotIn',
               'latInVol',
               'totInVol',
               'error_pct']] = nodeInflow_df[['maxLatIn',
                                              'maxTotIn',
                                              'latInVol',
                                              'totInVol',
                                              'error_pct']].astype(float)

if nodeSurcharge_data:
    nodeSurcharge_df = pd.DataFrame(nodeSurcharge_data, columns=['name',
                                                                 'type',
                                                                 'surchgDur',
                                                                 'maxHAC',
                                                                 'minDBR'])
    nodeSurcharge_df[['surchgDur',
                      'maxHAC',
                      'minDBR']] = nodeSurcharge_df[['surchgDur',
                                                     'maxHAC',
                                                     'minDBR']].astype(float)
    nodeSurcharge_df = nodeSurcharge_df.drop('type', axis=1)

if nodeFlooding_data:
    nodeFlooding_df = pd.DataFrame(nodeFlooding_data, columns=['name',
                                                               'floodedDur',
                                                               'maxFldRate',
                                                               'maxFldT_d',
                                                               'maxFldT_hm',
                                                               'totFldVol',
                                                               'maxPondD'])
    nodeFlooding_df[['floodedDur',
                     'maxFldRate',
                     'totFldVol',
                     'maxPondD']] = nodeFlooding_df[['floodedDur',
                                                     'maxFldRate',
                                                     'totFldVol',
                                                     'maxPondD']].astype(float)
    nodeFlooding_df['maxFldTD'] = start_time + \
        pd.to_timedelta(nodeFlooding_df['maxFldT_d'].astype(float), unit='D')
    nodeFlooding_df['maxFldTT'] = pd.to_datetime(nodeFlooding_df['maxFldT_hm'],
                                                 format='%H:%M').dt.time
    nodeFlooding_df['maxFldTime'] = nodeFlooding_df.apply(lambda r:
                                                          pd.datetime.combine(
                                                            r['maxFldTD'],
                                                            r['maxFldTT']).
                                                          strftime(
                                                            '%Y-%m-%d %H:%M'),
                                                          1)
    nodeFlooding_df = nodeFlooding_df.drop('maxFldT_d', axis=1)
    nodeFlooding_df = nodeFlooding_df.drop('maxFldT_hm', axis=1)
    nodeFlooding_df = nodeFlooding_df.drop('maxFldTD', axis=1)
    nodeFlooding_df = nodeFlooding_df.drop('maxFldTT', axis=1)

# Merge geodataframes with data dataframes
subcatchment_gdf = subcatchment_gdf.merge(subcatchment_df, on='name')
if lid_data:
    subcatchment_gdf = subcatchment_gdf.merge(lid_df, on='name', how='left')
if subcatchmentWash_data:
    subcatchment_gdf = subcatchment_gdf.merge(subcatchmentWash_df, on='name',
                                              how='left')

node_gdf = node_gdf.merge(node_df, on='name')
if nodeInflow_data:
    node_gdf = node_gdf.merge(nodeInflow_df, on='name', how='left')
if nodeSurcharge_data:
    node_gdf = node_gdf.merge(nodeSurcharge_df, on='name', how='left')
if nodeFlooding_data:
    node_gdf = node_gdf.merge(nodeFlooding_df, on='name', how='left')
node_gdf = node_gdf.drop('index', axis=1)
node_gdf = node_gdf.drop('X', axis=1)
node_gdf = node_gdf.drop('Y', axis=1)

# Save subcatchment and node results as shapefiles
subcatchment_gdf.to_file(os.path.splitext(sys.argv[2])[0] +
                         '_subcatchment_results.shp', driver='ESRI Shapefile')
print('Saved subcatchment results to ' + os.path.splitext(sys.argv[2])[0] +
      '_subcatchment_results.shp')

node_gdf.to_file(os.path.splitext(sys.argv[2])[0] + '_node_results.shp',
                 driver='ESRI Shapefile')
print('Saved node results to ' + os.path.splitext(sys.argv[2])[0] +
      '_node_results.shp')
