#!/usr/bin/env python
"""Convert ARC/INFO ASCII GRID (.asc) raster file to polygon shapefile."""

from osgeo import gdal
from osgeo import ogr
from osgeo import osr
import sys
gdal.UseExceptions()    # allow GDAL to throw Python Exceptions

crs = {'init': 'epsg:4326'}  # Default Coordinate Reference System
# Check input parameters
if (len(sys.argv) < 3 or len(sys.argv) > 4):
    print("Usage:\n"
          """$ python asc2shp.py [PATH TO INPUT .asc FILE] [PATH TO OUTPUT """
          """.shp FILE] 'EPSG:[XXXX]'""")
    sys.exit(1)
else:
    if (not sys.argv[1].lower().endswith('.asc')):
        print("Usage:\n"
              """$ python asc2shp.py [PATH TO INPUT .asc FILE] [PATH TO """
              """OUTPUT .shp FILE] 'EPSG:[XXXX]'""")
        sys.exit(1)
    if (not sys.argv[2].lower().endswith('.shp')):
        print("Usage:\n"
              """$ python asc2shp.py [PATH TO INPUT .asc FILE] PATH TO """
              """OUTPUT .shp FILE] 'EPSG:[XXXX]'""")
        sys.exit(1)
    if (len(sys.argv) == 3):
        print('NOTE: Using default (EPSG:4326) Coordinate Reference System.')
    elif (len(sys.argv) == 4):
        if not (sys.argv[3].lower().startswith('epsg:')):
            print("Usage:\n"
                  """$ python asc2shp.py [PATH TO INPUT .asc FILE]"""
                  """ [PATH TO OUTPUT .shp FILE] 'EPSG:[XXXX]'""")
            sys.exit(1)
        else:
            crs = {'init': sys.argv[3].lower()}  # Custom CRS
        print('Using ' + sys.argv[3].upper() +
              ' as Coordinate Reference System.')

# Open raster dataset
try:
    raster_ds = gdal.Open(sys.argv[1], gdal.GA_ReadOnly)
except RuntimeError as err:
    print('Unable to open ' + sys.argv[1])
    print(err)
    sys.exit(1)

# Get raster data
try:
    raster_data = raster_ds.GetRasterBand(1)
except RuntimeError as err:
    print(err)
    sys.exit(1)

# Convert to polygon
dst_layername = sys.argv[2]
driver = ogr.GetDriverByName("ESRI Shapefile")
polygon_ds = driver.CreateDataSource(dst_layername)
srs = osr.SpatialReference()
srs.ImportFromEPSG(int(sys.argv[3].split(':')[1]))
dst_layer = polygon_ds.CreateLayer(dst_layername, srs)
newField = ogr.FieldDefn('id', ogr.OFTInteger)
dst_layer.CreateField(newField)

# Save polygon to shapefile
try:
    gdal.Polygonize(raster_data, raster_data, dst_layer, 0, [], callback=None)
    print('Saved ' + dst_layername)
except RuntimeError as err:
    print('Unable to create ' + dst_layername)
    print(err)
    sys.exit(1)
