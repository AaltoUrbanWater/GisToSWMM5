#!/usr/bin/env bash

# Creates a shapefile of adaptive subcatchments with subcatchment attributes.

# Show usage information
if [ "$#" -ne 2 ]; then
    echo "Usage:
    ./adap2gis.sh [PATH TO GisToSWMM5 OUTPUT *_subcatchments.asc FILE] 'EPSG:[XXXX]'"
    exit
fi

# Create path and file names
exePath=$(dirname "$0")
path=$(dirname "$1")
fileName=$(basename "$1")
oldExtension="${fileName##*.}"
fileName="${fileName%.*}"

shapeBaseName=${fileName}_geom
shapeFile=$shapeBaseName.shp
shapePath=$path/shp/$shapeFile
csvBaseName=${fileName}_attr
csvFile=${csvBaseName}.csv
csvPath=$path/$csvFile

# Create a folder for shapefiles if it does not exist
if [[ -d "$path" ]]; then
  mkdir -p $path/shp
fi

# Create a temporary shapefile of adaptive subcatchments usign asc2shp.py script
python ${exePath}/asc2shp.py $1 $shapePath $2

# Exit if something went wrong when creating the shapefile
rc=$?
if [[ $rc != 0 ]]; then
  echo ""
  echo "Error in using asc2shp.py"
  exit $rc;
fi

# Create a temporary .csv copy of the .wkt file
cp $path/${csvBaseName}.wkt $path/${csvFile}

# Join attribute table from the .wkt file to the shapefile
ogr2ogr -sql "select * from ${shapeBaseName} left join '${csvPath}'.${csvBaseName} on ${shapeBaseName}.id = ${csvBaseName}.id" $path/shp/${fileName}.shp $shapePath

# Remove temp files
echo "Removing temp file: $path/shp/$shapeBaseName.*"
rm $path/shp/$shapeBaseName.*
echo "Removing temp file: $csvPath"
rm $csvPath
