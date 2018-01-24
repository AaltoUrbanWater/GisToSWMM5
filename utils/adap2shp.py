#!/usr/bin/env python
"""Create a subcatchment shapefile for adaptive grid.

Converts the ARC/INFO ASCII GRID (.asc) raster file to polygon shapefile and
joins it with the attribute data from the Well-Known-Text (.wkt) file.

Copyright (C) 2018 Tero Niemi, Aalto University School of Engineering
"""

import pandas as pd
import geopandas as gpd
import rasterio
from rasterio.features import shapes
import sys

crs = {'init': 'epsg:4326'}  # Default Coordinate Reference System
# Check input parameters
if (len(sys.argv) < 4 or len(sys.argv) > 5):
    print("Usage:\n"
          """$ python asc2shp.py [PATH TO INPUT *_subcatchments.asc FILE] """
          """[PATH TO INPUT *.wkt FILE] [PATH TO OUTPUT """
          """.shp FILE] 'EPSG:[XXXX]'""")
    sys.exit(1)
else:
    if (not sys.argv[1].lower().endswith('.asc')):
        print("Usage:\n"
              """$ python asc2shp.py [PATH TO INPUT *_subcatchments.asc """
              """FILE] [PATH TO INPUT *.wkt FILE] [PATH TO OUTPUT """
              """.shp FILE] 'EPSG:[XXXX]'""")
        sys.exit(1)
    if (not sys.argv[2].lower().endswith('.wkt')):
        print("Usage:\n"
              """$ python asc2shp.py [PATH TO INPUT *_subcatchments.asc """
              """FILE] [PATH TO INPUT *.wkt FILE] [PATH TO OUTPUT """
              """.shp FILE] 'EPSG:[XXXX]'""")
        sys.exit(1)
    if (not sys.argv[3].lower().endswith('.shp')):
        print("Usage:\n"
              """$ python asc2shp.py [PATH TO INPUT *_subcatchments.asc """
              """FILE] [PATH TO INPUT *.wkt FILE] [PATH TO OUTPUT """
              """.shp FILE] 'EPSG:[XXXX]'""")
        sys.exit(1)
    if (len(sys.argv) == 4):
        print('NOTE: Using default (EPSG:4326) Coordinate Reference System.')
    elif (len(sys.argv) == 5):
        if not (sys.argv[4].lower().startswith('epsg:')):
            print("Usage:\n"
                  """$ python asc2shp.py [PATH TO INPUT *_subcatchments.asc """
                  """FILE] [PATH TO INPUT *.wkt FILE] [PATH TO OUTPUT """
                  """.shp FILE] 'EPSG:[XXXX]'""")
            sys.exit(1)
        else:
            crs = {'init': sys.argv[4].lower()}  # Custom CRS
        print('Using ' + sys.argv[4].upper() +
              ' as Coordinate Reference System.')


# Read in the raster data and polygonize using rasterio
mask = None
with rasterio.drivers():
    with rasterio.open(sys.argv[1]) as src:
        image = src.read(1)  # first band
        results = (
            {'properties': {'id': v}, 'geometry': s}
            for i, (s, v)
            in enumerate(shapes(image, mask=mask, transform=src.affine)))

geoms = list(results)

# Create a GeoDataFrame from polygonized data
gdf = gpd.GeoDataFrame.from_features(geoms)
gdf.crs = crs
gdf['id'] = gdf['id'].astype(int)
gdf = gdf[gdf['id'] > 0]

# Read in the attribute data
attr = pd.read_csv(sys.argv[2], ';')
attr = attr.drop('wkt', axis=1)

# Join attribute tables
gdf = gdf.merge(attr, on='id')

# Save the shapefile
gdf.to_file(sys.argv[3], driver='ESRI Shapefile')
print('Saved subcatchments to ' + sys.argv[3])
