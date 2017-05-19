#!/usr/bin/env bash

# Converts *.wkt files into *.shp files
# The script reads all the *.wkt files in the given directory,
# creates a folder 'shp' if it does not exist already,
# and converts all *.wkt files into *.shp files.
# Change Coordinate Reference System (CSR) as necessary.

# set Coordinate Reference System
CSR="EPSG:3067"

# Show usage information
if [ "$#" -ne 1 ]; then
    echo "Usage:
    ./wkt2shp.sh [PATH TO *.wkt FILES DIRECTORY]"
    exit
fi

# Create a folder for shapefiles
if [[ -d "$1" ]]; then
    mkdir -p $1/shp
# Go through .wkt files and convert to shapefiles
    for f in $1/*.wkt; do
        if [ -f "$f" ]; then
            echo "Processing $f ..."
            path=$(dirname "$f")
            fileName=$(basename "$f")
            oldExtension="${fileName##*.}"
            fileName="${fileName%.*}"
            newExtension=".csv"
            cp $f $path/$fileName$newExtension

            ogr2ogr -f "ESRI Shapefile" $path/shp/$fileName.shp -dialect sqlite -sql "SELECT *, GeomFromText(wkt) FROM $fileName" $path/$fileName$newExtension -a_srs $CSR
            rm $path/$fileName$newExtension
        fi
    done
fi
