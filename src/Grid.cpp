#include "Grid.h"

Grid::Grid()
{
    clear();
}

Grid::~Grid()
{
    clear();
}

void Grid::clear()
{
    cells = 0;
    nCols = 0;
    nRows = 0;
    cellSize = 0.0;
    xllCorner = 0.0;
    yllCorner = 0.0;
    urcorner[0] = urcorner[1] = 0.0;
    llcorner[0] = llcorner[1] = 0.0;
    gridType = -1;
    delete [] cells;
    cells = 0;
}

// Create normal grid by treating each grid cell as a subcatchment
int Grid::create(int gridTypeNew, Raster &landuseRaster, Raster &demRaster)
{
    if (gridTypeNew == 0)
    {
        clear();
        gridType = gridTypeNew; // gridType is set to -1 in clear() method
        nCols = landuseRaster.nCols;
        nRows = landuseRaster.nRows;
        cells = new Cell[ nCols * nRows ];

        return 0;
    }
    else
    {
        return 1;
    }
}

// Create adaptive grid by subdividing cells with the same landuse
int Grid::create(int gridTypeNew, double cellSizeMax, int maxSubdivisions, Raster &landuseRaster, Raster &demRaster)
{
    if (gridTypeNew == 1 && cellSizeMax > 0.0 && maxSubdivisions >= 0)
    {
        clear(); // gridType is set to -1 in clear() method
        gridType = gridTypeNew;

        // Create cells.
        int numberOfCellsX;
        int numberOfCellsY;
        //double cellSizeMax = 16.0;
        numberOfCellsX = (int)((double)landuseRaster.nCols * landuseRaster.cellSize / cellSizeMax + 0.5);
        numberOfCellsY = (int)((double)landuseRaster.nRows * landuseRaster.cellSize / cellSizeMax + 0.5);
        double lowerLeftCornerX =  landuseRaster.xllCorner;
        double lowerLeftCornerY =  landuseRaster.yllCorner;
        int subdivisions = 0;
        //int maxSubdivisions = 3; // 16 -> 8 -> 4 -> 2
        std::vector<Cell> cellsAdaptive;

        for (int i = 0; i < numberOfCellsX * numberOfCellsY; i++)
        {
            subdivideCell(cellSizeMax, lowerLeftCornerX, lowerLeftCornerY, i, numberOfCellsX,
                          landuseRaster, subdivisions, maxSubdivisions, cellsAdaptive);
        }

        // Transfer adaptive cell data to the pointer array.
        nCols = (int)cellsAdaptive.size();
        nRows = 1;
        cells = new Cell[nRows * nCols];

        for (int i = 0; i < nRows * nCols; i++ )
        {
            cells[i].cellSize = cellsAdaptive[i].cellSize;
            cells[i].area = cells[i].cellSize * cells[i].cellSize; // Temp. place
            cells[i].centerCoordX = cellsAdaptive[i].centerCoordX;
            cells[i].centerCoordY = cellsAdaptive[i].centerCoordY;
            cells[i].landuse = cellsAdaptive[i].landuse;
        }

        return 0;
    }
    else
    {
        std::cout << "\n-> Error, when using the adaptive discretiztion algorithm ";
        std::cout << "check that the maximum cell width is above zero and maximum number of subdivisions ";
        std::cout << "is equal to or above zero.";

        return 1;
    }
}

void Grid::subdivideCell(double cellSize, double llcornerx, double llcornery, int index, int numberOfCells,
                         Raster &landuseRaster, int subdivisions, int maxSubdivisions, std::vector<Cell> &cellsAdaptive)
{
    int cellIndexX = index % numberOfCells;
    int cellIndexY = index / numberOfCells;
    llcornerx = llcornerx + cellIndexX * cellSize;
    llcornery = llcornery + cellIndexY * cellSize;

    if (isSingleLanduse(llcornerx, llcornery, cellSize, landuseRaster) || subdivisions >= maxSubdivisions)
    {
        Cell cell;
        cell.cellSize = cellSize;
        cell.centerCoordX = llcornerx + cell.cellSize * 0.5;
        cell.centerCoordY = llcornery + cell.cellSize * 0.5;
        std::string landuse = landuseRaster.getPixelValue(cell.centerCoordX, cell.centerCoordY);
        cell.landuse = atoi(landuse.c_str());
        cellsAdaptive.push_back(cell);
    }
    else
    {
        subdivisions++;

        for (int i = 0; i < 4; i++)
        {
            subdivideCell(0.5 * cellSize, llcornerx, llcornery, i, 2,
                          landuseRaster, subdivisions, maxSubdivisions, cellsAdaptive);
        }
    }
}

void Grid::computeHistogram(std::vector<std::string> &catLabels, std::vector<int> &catCount,
                            double lowerLeftCornerX, double lowerLeftCornerY, double cellSizeLoc, Raster &landuseRaster)
{
    catLabels.clear();
    catCount.clear();
    int indexMinX = (int)((lowerLeftCornerX - landuseRaster.xllCorner) / landuseRaster.cellSize + 0.5);
    int indexMaxX = (int)((lowerLeftCornerX + cellSizeLoc - landuseRaster.xllCorner) / landuseRaster.cellSize + 0.5);
    int indexMinY = (int)((lowerLeftCornerY - landuseRaster.yllCorner) / landuseRaster.cellSize + 0.5);
    int indexMaxY = (int)((lowerLeftCornerY + cellSizeLoc - landuseRaster.yllCorner) / landuseRaster.cellSize + 0.5);

    if (indexMinX < 0)
    {
        indexMinX = 0;
    }
    if (indexMinY < 0)
    {
        indexMinY = 0;
    }
    if (indexMaxX > landuseRaster.nCols)
    {
        indexMaxX = landuseRaster.nCols;
    }
    if (indexMaxY > landuseRaster.nRows)
    {
        indexMaxY = landuseRaster.nRows;
    }

    for (int j = indexMinY; j < indexMaxY; j++)
    {
        for (int i = indexMinX; i < indexMaxX; i++)
        {
            double coordinateX = landuseRaster.xllCorner + i * landuseRaster.cellSize + landuseRaster.cellSize * 0.5;
            double coordinateY = landuseRaster.yllCorner + j * landuseRaster.cellSize + landuseRaster.cellSize * 0.5;
            std::string landuse = landuseRaster.getPixelValue(coordinateX, coordinateY);
            bool landuseCatFound = false;

            for (int k = 0; k < (int)catLabels.size(); k++)
            {
                if (catLabels[k] == landuse)
                {
                    catCount[k]++;
                    landuseCatFound = true;
                }
            }

            if (landuseCatFound == false)
            {
                catLabels.push_back(landuse);
                catCount.push_back(1);
            }
        }
    }
}

bool Grid::isSingleLanduse(double lowerLeftCornerX, double lowerLeftCornerY, double cellSizeLoc, Raster &landuseRaster)
{
    std::vector<std::string> catLabels;
    std::vector<int> catCount;
    computeHistogram(catLabels, catCount, lowerLeftCornerX, lowerLeftCornerY, cellSizeLoc, landuseRaster);

    int numberOfCategories = 0;

    for (int i = 0; i < (int)catCount.size(); i++)
    {
        if (catCount[i] > 0)
        {
            numberOfCategories++;
        }
    }

    if (numberOfCategories == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Grid::setCellNames()
{
    for (int i = 0; i < nCols * nRows; i++)
    {
        std::stringstream sstream("");
        sstream << "s" << i;
        cells[i].name = sstream.str();
    }
}

void Grid::computeCellDimensions(double cellSizeNew)
{
    cellSize = cellSizeNew;

    for (int i = 0; i < nCols * nRows; i++)
    {
        cells[i].area = cellSize * cellSize;
        cells[i].cellSize = cellSize;
    }
}

void Grid::computeCellCenterpoints(double xllCornerNew, double yllCornerNew)
{
    xllCorner = xllCornerNew; // is this needed anymore here?
    yllCorner = yllCornerNew; // is this needed anymore here?

    for (int j = 0; j < nRows; j++)
    {
        for (int i = 0; i < nCols; i++)
        {
            double centerCoordX = (double)i * cellSize + 0.5 * cellSize + xllCorner;
            double centerCoordY = (double)j * cellSize + 0.5 * cellSize + yllCorner;
            cells[ i + j * nCols ].centerCoordX = centerCoordX;
            cells[ i + j * nCols ].centerCoordY = centerCoordY;
        }
    }
}

void Grid::setCellElevations(Raster &demRaster)
{
    // Regular grid.
    if (gridType == 0)
    {
        for (int i = 0; i < nCols * nRows; i++)
        {
            std::string elevation = demRaster.getPixelValue( cells[i].centerCoordX, cells[i].centerCoordY );
            cells[i].elevation = atof(elevation.c_str());
        }
    }
    // Adaptive grid.
    else if (gridType == 1)
    {
        for (int i = 0; i < nRows * nCols; i++ )
        {
            std::vector<std::string> catLabels;
            std::vector<int> catCount;
            computeHistogram(catLabels, catCount, cells[i].centerCoordX - 0.5 * cells[i].cellSize,
                             cells[i].centerCoordY - 0.5 * cells[i].cellSize, cells[i].cellSize, demRaster);
            double elevation = 0;
            int count = 0;

            for (int j = 0; j < (int)catLabels.size(); j++)
            {
                elevation += atof( catLabels[j].c_str() ) * (double)catCount[j];
                count += catCount[j];
            }

            if (count > 0)
            {
                elevation /= (double)count;
                cells[i].elevation = elevation;
            }
            else
            {
                std::cout << "\n-> Error, no elevation data was found for the cell " << i;
            }
        }
    }
}


// TJN 29 Sep 2017 START
void Grid::setCellFlowdirs(Raster &flowdirRaster)
{
    for (int i = 0; i < nCols * nRows; i++)
    {
        std::string flowdir = flowdirRaster.getPixelValue( cells[i].centerCoordX, cells[i].centerCoordY );
        cells[i].flowdir = atoi(flowdir.c_str());
    }
}
// TJN 29 Sep 2017 END

void Grid::setCellLanduse(Raster &landuseRaster)
{
    for (int j = 0; j < nRows; j++)
    {
        for (int i = 0; i < nCols; i++)
        {
            std::string landuseType = landuseRaster.getPixelValue( i, nRows - 1 - j );
            cells[ i + j * nCols ].landuse = atoi(landuseType.c_str());
        }
    }
}

void Grid::computeGridExtents()
{
    if (nCols * nRows > 0)
    {
        urcorner[0] = llcorner[0] = cells[0].centerCoordX;
        urcorner[1] = llcorner[1] = cells[0].centerCoordY;
    }

    for (int i = 0; i < nCols * nRows; i++)
    {
        if (cells[i].landuse != LANDUSE_NONE)
        {
            if (cells[i].centerCoordX < llcorner[0])
            {
                llcorner[0] = cells[i].centerCoordX;
            }

            if (cells[i].centerCoordX > urcorner[0])
            {
                urcorner[0] = cells[i].centerCoordX;
            }

            if (cells[i].centerCoordY < llcorner[1])
            {
                llcorner[1] = cells[i].centerCoordY;
            }

            if (cells[i].centerCoordY > urcorner[1])
            {
                urcorner[1] = cells[i].centerCoordY;
            }
        }
    }
}

void Grid::setSubcatchmentProperties(Table &catchPropTable)
{
    for (int i = 0; i < nCols * nRows; i++)
    {
        for (int j = 1; j < catchPropTable.nRows; j++)
        {
            if (cells[i].landuse == atoi(catchPropTable.data[j * catchPropTable.nCols].c_str()))
            {
                cells[i].raingage = catchPropTable.data[j * catchPropTable.nCols + 7];
                cells[i].imperv = catchPropTable.data[j * catchPropTable.nCols + 1].c_str();
                cells[i].N_Imperv = catchPropTable.data[j * catchPropTable.nCols + 3].c_str();
                cells[i].N_Perv = catchPropTable.data[j * catchPropTable.nCols + 5].c_str();
                cells[i].S_Imperv = catchPropTable.data[j * catchPropTable.nCols + 2].c_str();
                cells[i].S_Perv = catchPropTable.data[j * catchPropTable.nCols + 4].c_str();
                cells[i].PctZero = catchPropTable.data[j * catchPropTable.nCols + 6].c_str();
                cells[i].Suction = catchPropTable.data[j * catchPropTable.nCols + 10].c_str();
                cells[i].HydCon = catchPropTable.data[j * catchPropTable.nCols + 8].c_str();
                cells[i].IMDmax = catchPropTable.data[j * catchPropTable.nCols + 9].c_str();
                cells[i].snowPack = catchPropTable.data[j * catchPropTable.nCols + 11];

                break;
            }
        }
    }
}

void Grid::findCellNeighbours()
{
    // Regular grid.
    if (gridType == 0)
    {
        //std::cout << "\n-> Grid type " << gridType;

        for (int j = 0; j < nRows; j++)
        {
            for (int i = 0; i < nCols; i++)
            {
                int currentCell = i + j * nCols;
                cells[currentCell].neighCellIndices.assign(8, -1);
                cells[currentCell].distanceToNeighbours.assign(8, 0.0 );

                // TJN 2 Oct 2017 START
                // Change routing of cells so that flow directions go from (1-8
                // [here 0-8]) with 0 pointing Northeast and numbers increasing
                // in counterclockwise direction
                if (j < nRows - 1)
                {
                    cells[currentCell].neighCellIndices[1] = currentCell + nCols;
                }

                if (j < nRows - 1 && i > 0)
                {
                    cells[currentCell].neighCellIndices[2] = currentCell + nCols - 1;
                }

                if (i > 0)
                {
                    cells[currentCell].neighCellIndices[3] = currentCell - 1;
                }

                if (j > 0 && i > 0)
                {
                    cells[currentCell].neighCellIndices[4] = currentCell - nCols - 1;
                }

                if (j > 0)
                {
                    cells[currentCell].neighCellIndices[5] = currentCell - nCols;
                }

                if (i < nCols - 1 && j > 0)
                {
                    cells[currentCell].neighCellIndices[6] = currentCell + 1 - nCols;
                }

                if (i < nCols - 1)
                {
                    cells[currentCell].neighCellIndices[7] =  currentCell + 1;
                }

                if (i < nCols - 1 && j < nRows - 1)
                {
                    cells[currentCell].neighCellIndices[0] = currentCell + nCols + 1;
                }
                // TJN 2 Oct 2017 END
            }
        }
    }
    // Adaptive grid. This section could be optimized.
    else if (gridType == 1)
    {
        double thresholdDistance = 0.5; // magic number !

        for (int i = 0; i < nRows * nCols; i++ )
        {
            std::vector<int> neighbours;

            for (int j = 0; j < nRows * nCols; j++ )
            {
                if
                (
                    // Corner points.
                    (
                        (
                            fabs((cells[i].centerCoordX + 0.5 * cells[i].cellSize) - (cells[j].centerCoordX - 0.5 * cells[j].cellSize)) < thresholdDistance
                            ||
                            fabs((cells[i].centerCoordX - 0.5 * cells[i].cellSize) - (cells[j].centerCoordX + 0.5 * cells[j].cellSize)) < thresholdDistance
                        )
                        &&
                        (
                            fabs((cells[i].centerCoordY + 0.5 * cells[i].cellSize) - (cells[j].centerCoordY - 0.5 * cells[j].cellSize)) < thresholdDistance
                            ||
                            fabs((cells[i].centerCoordY - 0.5 * cells[i].cellSize) - (cells[j].centerCoordY + 0.5 * cells[j].cellSize)) < thresholdDistance
                        )
                    )
                    ||
                    // Side points.
                    (
                        (
                            (
                                fabs((cells[i].centerCoordX + 0.5 * cells[i].cellSize) - (cells[j].centerCoordX - 0.5 * cells[j].cellSize)) < thresholdDistance
                                ||
                                fabs((cells[i].centerCoordX - 0.5 * cells[i].cellSize) - (cells[j].centerCoordX + 0.5 * cells[j].cellSize)) < thresholdDistance
                            )
                            &&
                            (
                                (cells[i].centerCoordY + 0.5 * cells[i].cellSize >= cells[j].centerCoordY && cells[i].centerCoordY - 0.5 * cells[i].cellSize <= cells[j].centerCoordY)
                                ||
                                (cells[j].centerCoordY + 0.5 * cells[j].cellSize >= cells[i].centerCoordY && cells[j].centerCoordY - 0.5 * cells[j].cellSize <= cells[i].centerCoordY)
                            )
                        )

                        ||

                        (
                            (
                                fabs((cells[i].centerCoordY + 0.5 * cells[i].cellSize) - (cells[j].centerCoordY - 0.5 * cells[j].cellSize)) < thresholdDistance
                                ||
                                fabs((cells[i].centerCoordY - 0.5 * cells[i].cellSize) - (cells[j].centerCoordY + 0.5 * cells[j].cellSize)) < thresholdDistance
                            )
                            &&
                            (
                                (cells[i].centerCoordX + 0.5 * cells[i].cellSize >= cells[j].centerCoordX && cells[i].centerCoordX - 0.5 * cells[i].cellSize <= cells[j].centerCoordX)
                                ||
                                (cells[j].centerCoordX + 0.5 * cells[j].cellSize >= cells[i].centerCoordX && cells[j].centerCoordX - 0.5 * cells[j].cellSize <= cells[i].centerCoordX)
                            )
                        )
                    )
                )
                {
                    if (i != j && cells[j].landuse != LANDUSE_NONE)
                    {
                        neighbours.push_back(j);
                    }
                }
            }

            cells[i].neighCellIndices = neighbours;
            cells[i].distanceToNeighbours.assign( (int)neighbours.size(), 0.0 );
        }
    }
}

// TJN 29 Sep 2017 START
// Route cells in old adaptive grid
void Grid::routeCells()
{
    for (int i = 0; i < nCols * nRows; i++)
    {
        double slope = 0.0;             // TJN 20170908 Use slope instead of elevation to route flow
        double maxSlope = -1.0;         // TJN 20170908 Use slope instead of elevation to route flow
        double elevation = cells[i].elevation;
        int neighCellIndex = -1;
        cells[i].flowWidth = cells[i].cellSize; //Modified 20160909
        int flowDirection = -1;

        // Compute distances between neighbour cells.
        for (int j = 0; j < (int)cells[i].neighCellIndices.size(); j++)
        {
            if (cells[i].neighCellIndices[j] != -1)
            {
                double distance = sqrt( (cells[i].centerCoordX - cells[ cells[i].neighCellIndices[j] ].centerCoordX)
                                        * (cells[i].centerCoordX - cells[ cells[i].neighCellIndices[j] ].centerCoordX)
                                        + (cells[i].centerCoordY - cells[ cells[i].neighCellIndices[j] ].centerCoordY)
                                        * (cells[i].centerCoordY - cells[ cells[i].neighCellIndices[j] ].centerCoordY) );

                if (distance > 0.0)
                {
                    cells[i].distanceToNeighbours[j] = distance;
                }
                else
                {
                    std::cout << "\n-> Error, distance between cell " << i << " and cell " << j << " is zero or below zero";
                }
            }
        }

        // Find outlet.
        for (int k = 0; k < (int)cells[i].neighCellIndices.size(); k++)
        {
            if (cells[i].neighCellIndices[k] != -1 && cells[ cells[i].neighCellIndices[k] ].landuse != LANDUSE_ROOF_CONNECTED
                    && cells[ cells[i].neighCellIndices[k] ].landuse != LANDUSE_ROOF_UNCONNECTED
                    && cells[ cells[i].neighCellIndices[k] ].landuse != LANDUSE_NONE)
            {
                // TJN 20170908 START
                // Bug fix: Use slope instead of elevation to route flow
                double distance = cells[i].distanceToNeighbours[k];
                if (distance > 0.0)
                    slope = fabs((cells[i].elevation - cells[ cells[i].neighCellIndices[k] ].elevation) / distance);

                if (cells[ cells[i].neighCellIndices[k] ].elevation < elevation && slope > maxSlope)
                {
//                    elevation = cells[ cells[i].neighCellIndices[k] ].elevation;  // TJN 29 Sep 2017 Bug fix
                    neighCellIndex = cells[i].neighCellIndices[k];
                    flowDirection = k;
                    maxSlope = slope;
                }

//                if (cells[ cells[i].neighCellIndices[k] ].elevation < elevation)
//                {
//                    elevation = cells[ cells[i].neighCellIndices[k] ].elevation;
//                    neighCellIndex = cells[i].neighCellIndices[k];
//                    flowDirection = k;
//                }
                // TJN 20170908 END
            }
        }

        // Save the outlet name and compute flow width.
        if (neighCellIndex != -1)
        {
            cells[i].outlet = cells[neighCellIndex].name;
            // TJN 18 May 2017 START
            cells[i].outletCoordX = cells[neighCellIndex].centerCoordX;
            cells[i].outletCoordY = cells[neighCellIndex].centerCoordY;
            // TJN 18 May 2017 END

            if (cells[i].distanceToNeighbours[flowDirection] > 0.0)
            {
                cells[i].flowWidth = cells[i].area / cells[i].distanceToNeighbours[flowDirection];
            }
            //else  //Modified 20160909
            //{
            //	cells[i].flowWidth = 0.0;
            //}

            // Update flow width if the flow direction is in a diagonal direction.
            //if (gridType == 0 && flowDirection != -1 && flowDirection % 2 != 0)
            //{
            //	cells[i].flowWidth = cells[i].cellSize / 1.4142135624;
            //}

        }
    }
}

// Use flow direction raster to route cells instead of DEM raster when grid is regular
// TJN 22 Nov 2017 This could be simplified by removing unnecessary distance computations between cells other than neighbour
void Grid::routeCellsReg()
{
    for (int i = 0; i < nCols * nRows; i++)
    {
        int neighCellIndex = -1;
        cells[i].flowWidth = cells[i].cellSize; //Modified 20160909
        int flowDirection = -1;

        // Compute distances between neighbour cells.
        for (int j = 0; j < (int)cells[i].neighCellIndices.size(); j++)
        {
            if (cells[i].neighCellIndices[j] != -1)
            {
                double distance = sqrt( (cells[i].centerCoordX - cells[ cells[i].neighCellIndices[j] ].centerCoordX)
                                        * (cells[i].centerCoordX - cells[ cells[i].neighCellIndices[j] ].centerCoordX)
                                        + (cells[i].centerCoordY - cells[ cells[i].neighCellIndices[j] ].centerCoordY)
                                        * (cells[i].centerCoordY - cells[ cells[i].neighCellIndices[j] ].centerCoordY) );

                if (distance > 0.0)
                {
                    cells[i].distanceToNeighbours[j] = distance;
                }
                else
                {
                    std::cout << "\n-> Error, distance between cell " << i << " and cell " << j << " is zero or below zero";
                }
            }
        }

        // Find outlet.
        flowDirection = cells[i].flowdir - 1;
        if (cells[i].neighCellIndices[flowDirection] != -1 && cells[ cells[i].neighCellIndices[flowDirection] ].landuse != LANDUSE_ROOF_CONNECTED
                && cells[ cells[i].neighCellIndices[flowDirection] ].landuse != LANDUSE_ROOF_UNCONNECTED
                && cells[ cells[i].neighCellIndices[flowDirection] ].landuse != LANDUSE_NONE)
        {
            neighCellIndex = cells[i].neighCellIndices[flowDirection];
        }

        // Save the outlet name and compute flow width.
        if (neighCellIndex != -1)
        {
            cells[i].outletID = neighCellIndex;
            cells[i].outlet = cells[neighCellIndex].name;
            cells[neighCellIndex].inletIDs.push_back(i);    // TJN 7 Dec 2017
            cells[i].outletCoordX = cells[neighCellIndex].centerCoordX;
            cells[i].outletCoordY = cells[neighCellIndex].centerCoordY;

            if (cells[i].distanceToNeighbours[flowDirection]  > 0.0)
            {
                cells[i].flowWidth = cells[i].area / cells[i].distanceToNeighbours[flowDirection] ;
            }
        }
    }
}
// TJN 29 Sep 2017 END

void Grid::computeCellSlopes()
{
    for (int i = 0; i < nCols * nRows; i++)
    {
        double slope = 0.0;
        int slopeCount = 0;

        //if (cells[i].landuse != LANDUSE_NONE && (int)cells[i].neighCellIndices.size() > 0)
        //{
        //	std::cout << "\n-> Cell " << i << " neighbour count: " <<  (int)cells[i].neighCellIndices.size();
        //}

        for (int k = 0; k < (int)cells[i].neighCellIndices.size(); k++)
        {
            if (cells[i].neighCellIndices[k] != -1 && cells[ cells[i].neighCellIndices[k] ].landuse != LANDUSE_ROOF_CONNECTED
                    && cells[ cells[i].neighCellIndices[k] ].landuse != LANDUSE_ROOF_UNCONNECTED
                    && cells[ cells[i].neighCellIndices[k] ].landuse != LANDUSE_NONE)
            {
                // Compute slope between cells.
                //double distance = 0.0;

                //if (gridType == 0)
                //{
                //	distance = cells[i].cellSize;
                //
                //	if (k % 2 != 0)
                //	{
                //		distance *= 1.4142135624; // approximate of square root of two
                //	}
                //}
                // Compute distance between cells in an adaptive grid.
                //else if (gridType == 1)
                //{
                //	distance = 0.5 * (cells[i].cellSize + cells[ cells[i].neighCellIndices[k] ].cellSize);
                //}

                double distance = cells[i].distanceToNeighbours[k];

                if (distance > 0.0)
                {
                    slope += fabs((cells[i].elevation - cells[ cells[i].neighCellIndices[k] ].elevation) / distance);
                    slopeCount++;
                }
                else
                {
                    std::cout << "\n-> Error, distance between cell " << i << " and " << cells[i].neighCellIndices[k] << " is zero or below zero.";
                }
            }
        }

        // Save the average slope.
        if (slopeCount > 0)
        {
            cells[i].slope = slope / (double)slopeCount;
        }

        // Fix slope for the rooftops.
        if (cells[i].landuse == LANDUSE_ROOF_CONNECTED || cells[i].landuse == LANDUSE_ROOF_UNCONNECTED)
        {
            cells[i].slope = 0.05;
        }

        // Set minimum slope
        if (cells[i].slope < 0.001)
        {
            cells[i].slope = 0.001;
        }
    }
}


// TJN 20170908 This relies (implicitly) that the cell connected to junction is a local
// pit in the DEM, if it is not (and e.g. the next cell is) most of the water flows past
// the junction...
// TJN 23 Oct 2017 UPDATE: Using 3x3 area surrounding junctions as the collecting area for water
void Grid::connectCellsToJunctions(Table &juncTable)
{
    for (int k = 1; k < juncTable.nRows; k++) // pass the header line
    {
        double juncPosX = atof( juncTable.data[k * juncTable.nCols].c_str() );
        double juncPosY = atof( juncTable.data[k * juncTable.nCols + 1].c_str() );
        int isOpen = atoi( juncTable.data[k * juncTable.nCols + 6].c_str() );

        // Regular grid.
        if (gridType == 0)
        {
            if (juncPosX >= xllCorner && juncPosX < xllCorner + nCols * cellSize
                    && juncPosY >= yllCorner && juncPosY < yllCorner + nRows * cellSize && isOpen == 1) // pass closed junctions
            {
                if (nCols > 0 && nRows > 0 && cellSize > 0.0)
                {
                    int col = (int)((juncPosX - xllCorner) / cellSize);
                    int row = (int)((juncPosY - yllCorner) / cellSize);

                    if (col + row * nCols >= 0 && col + row * nCols < nCols * nRows)
                    {
                        cells[ col + row * nCols ].outletID =  col + row * nCols;   // TJN 23 Nov 2017: outletID is the outletID of junction cell
                        cells[ col + row * nCols ].outlet = juncTable.data[k * juncTable.nCols + 2];
                        cells[col + row * nCols].inletIDs.push_back(col + row * nCols);     // TJN 7 Dec 2017   This is unnecessary?
                        cells[ col + row * nCols ].flowWidth = cells[ col + row * nCols ].cellSize; //Modified 20160909
                        // TJN 18 May 2017 START
                        cells[ col + row * nCols ].outletCoordX = stod(juncTable.data[k * juncTable.nCols + 0]);
                        cells[ col + row * nCols ].outletCoordY = stod(juncTable.data[k * juncTable.nCols + 1]);
                        // TJN 18 May 2017 END

                        // TJN 23 Oct 2017 START
                        // Use the 3x3 cell area surrounding each stormwater inlet as the collecting area for the inlet.
                        // This should help with misplacement of stormwater inlets when not just the cell with the inlet is used
                        // to collect water
                        for (int i = 0; i < (int)cells[ col + row * nCols ].neighCellIndices.size(); i++)
                        {
                            if (cells[ col + row * nCols ].neighCellIndices[i] != -1
                                    && cells[ cells[ col + row * nCols ].neighCellIndices[i] ].landuse != LANDUSE_ROOF_CONNECTED
                                    && cells[ cells[ col + row * nCols ].neighCellIndices[i] ].landuse != LANDUSE_ROOF_UNCONNECTED
                                    && cells[ cells[ col + row * nCols ].neighCellIndices[i] ].landuse != LANDUSE_NONE)
                            {
                                cells[ cells[ col + row * nCols ].neighCellIndices[i] ].outletID =  col + row * nCols;   // TJN 23 Nov 2017: outletID is the outletID of junction cell
                                cells[ cells[ col + row * nCols ].neighCellIndices[i] ].outlet = juncTable.data[k * juncTable.nCols + 2];
                                cells[col + row * nCols].inletIDs.push_back(cells[ col + row * nCols ].neighCellIndices[i]);     // TJN 7 Dec 2017   This is unnecessary?
                                cells[ cells[ col + row * nCols ].neighCellIndices[i] ].flowWidth =  cells[ cells[ col + row * nCols ].neighCellIndices[i] ].cellSize; // This is unnecessary?
                                cells[ cells[ col + row * nCols ].neighCellIndices[i] ].outletCoordX = stod(juncTable.data[k * juncTable.nCols + 0]);
                                cells[ cells[ col + row * nCols ].neighCellIndices[i] ].outletCoordY = stod(juncTable.data[k * juncTable.nCols + 1]);
                            }
                        }
                        // TJN 23 Oct 2017 END
                    }
                }
            }
        }
        // Irregular grid.
        else if (gridType == 1)
        {
            for (int i = 0; i < nCols * nRows; i++)
            {
                if (cells[i].centerCoordX - 0.5 * cells[i].cellSize <= juncPosX // modified
                        && cells[i].centerCoordX + 0.5 * cells[i].cellSize > juncPosX
                        && cells[i].centerCoordY - 0.5 * cells[i].cellSize <= juncPosY // modified
                        && cells[i].centerCoordY + 0.5 * cells[i].cellSize > juncPosY
                        && isOpen == 1      // TJN 20170908 pass closed junctions
                   )
                {
                    cells[i].outlet = juncTable.data[k * juncTable.nCols + 2];
                    cells[i].flowWidth = cells[i].cellSize; //Modified 20160909
                    // TJN 18 May 2017 START
                    cells[i].outletCoordX = stod(juncTable.data[k * juncTable.nCols + 0]);
                    cells[i].outletCoordY = stod(juncTable.data[k * juncTable.nCols + 1]);
                    // TJN 18 May 2017 END
                }
            }
        }
    }
}

void Grid::routePavedPitAndRooftopCells(Table &juncTable)
{
    // Route paved pit cells and connected rooftop cells to junctions.
    double distanceMaxSquared = (urcorner[0] - llcorner[0]) * (urcorner[0] - llcorner[0]) +
                                (urcorner[1] - llcorner[1]) * (urcorner[1] - llcorner[1]);

    for (int i = 0; i < nCols * nRows; i++)
    {
        // TJN 22 Nov 2017
        // Only route connected roofs to nearest junction, route other pits to themselves
        if (cells[i].landuse == LANDUSE_ROOF_CONNECTED) // should this be here?
        {
            double distanceSquared = distanceMaxSquared;

            for (int j = 1; j < juncTable.nRows; j++) // pass the header line
            {
                double juncPosX = atof( juncTable.data[j * juncTable.nCols].c_str() );
                double juncPosY = atof( juncTable.data[j * juncTable.nCols + 1].c_str() );
                double dx = juncPosX - cells[i].centerCoordX;
                double dy = juncPosY - cells[i].centerCoordY;
                int isroutable = atoi( juncTable.data[j * juncTable.nCols + 10].c_str() ); // modified 20160812.

                int col = (int)((juncPosX - xllCorner) / cellSize);     // TJN 24 Nov 2017
                int row = (int)((juncPosY - yllCorner) / cellSize);     // TJN 24 Nov 2017

                if (dx * dx + dy * dy < distanceSquared && isroutable == 1)  // modified 20160812.
                {
                    distanceSquared = dx * dx + dy * dy;
                    cells[i].outletID = col + row * nCols;   // TJN 24 Nov 2017: outletID is the outletID of junction cell
                    cells[i].outlet = juncTable.data[j * juncTable.nCols + 2];
                    cells[col + row * nCols].inletIDs.push_back(i);     // TJN 7 Dec 2017
                    cells[i].flowWidth = cells[i].cellSize; //Modified 20160909, compute by area / sqrt (distanceSquared) ?
                    // TJN 18 May 2017 START
                    cells[i].outletCoordX = stod(juncTable.data[j * juncTable.nCols + 0]);
                    cells[i].outletCoordY = stod(juncTable.data[j * juncTable.nCols + 1]);
                    // TJN 18 May 2017 END
                }
            }

            //cells[i].outlet = cells[i].name;

            // TJN 25 Sep 2017
            // Mark local pits where water is routed forcefully
            cells[i].isSink = 2;
        }
        else if ((cells[i].landuse == LANDUSE_ASPHALT_STREET && cells[i].outlet ==  "*") // the last condition is repeated...
                 || (cells[i].landuse > 40 && cells[i].landuse < 45 && cells[i].outlet ==  "*") // this line was added 25.06.2016
                 || (cells[i].landuse == LANDUSE_TILES && cells[i].outlet ==  "*"))
        {
            cells[i].outletID = i;   // TJN 24 Nov 2017
            cells[i].outlet = cells[i].name;
            cells[i].inletIDs.push_back(i);     // TJN 7 Dec 2017   This is unnecessary?
            cells[i].outletCoordX = cells[i].centerCoordX;
            cells[i].outletCoordY = cells[i].centerCoordY;

            // Mark local pits where water is not routed
            cells[i].isSink = 1;
        }
    }

    // Route unconnected rooftop cells to the nearest non-roof cells.
    for (int i = 0; i < nCols * nRows; i++)
    {
        if (cells[i].landuse == LANDUSE_ROOF_UNCONNECTED)
        {
            double distanceSquared = distanceMaxSquared;

            for (int j = 0; j < nCols * nRows; j++)
            {
                if (cells[j].landuse != LANDUSE_ROOF_CONNECTED && cells[j].landuse != LANDUSE_ROOF_UNCONNECTED
                        && cells[j].landuse != LANDUSE_NONE)
                {
                    double dx = cells[i].centerCoordX - cells[j].centerCoordX;
                    double dy = cells[i].centerCoordY - cells[j].centerCoordY;

                    if (dx * dx + dy * dy < distanceSquared)
                    {
                        distanceSquared = dx * dx + dy * dy;
                        cells[i].outletID = j;  // TJN 23 Nov 2017
                        cells[i].outlet = cells[j].name;
                        cells[j].inletIDs.push_back(i);     // TJN 7 Dec 2017
                        cells[i].flowWidth = cells[i].cellSize; //Modified 20160909, compute by area / sqrt (distanceSquared) ?
                        // TJN 18 May 2017 START
                        cells[i].outletCoordX = cells[j].centerCoordX;
                        cells[i].outletCoordY = cells[j].centerCoordY;
                        // TJN 18 May 2017 END
                    }
                }
            }
        }
    }
}

void Grid::routePitCells()
{
    for (int i = 0; i < nCols * nRows; i++)
    {
        if (cells[i].outlet ==  "*" &&
                (cells[i].landuse == LANDUSE_SAND || cells[i].landuse == LANDUSE_VEGETATION
                 || cells[i].landuse == LANDUSE_WATER || cells[i].landuse == LANDUSE_BEDROCK))
        {
            cells[i].outletID = i;  // TJN 24 Nov 2017
            cells[i].outlet = cells[i].name;
            cells[i].inletIDs.push_back(i);     // TJN 7 Dec 2017   This is unnecessary?

            // Set depression storage of pit cells in permeable areas to a very high value ...
            // ... to prevent loss of water from the system.
            cells[i].S_Imperv = "50000";
            cells[i].S_Perv = "50000";

            // TJN 18 May 2017 START
            cells[i].outletCoordX = cells[i].centerCoordX;
            cells[i].outletCoordY = cells[i].centerCoordY;
            // TJN 18 May 2017 END

            // TJN 25 Sep 2017
            // Mark local pits where water is not routed
            cells[i].isSink = 1;
        }
    }
}

// TJN 22 Nov 2017
// Simplify grid based on common flow direction and landuse
//
/*
TJN 4 Dec 2017
Taa on turhan monimutkainen tapa tehda taa!
Kopioi flow trace QGIS:sta tanne. Koska tunnetaan tarkalleen routtaus loppupisteelle
pitaisi olla jopa helpompaa traceta taalla kuin QGIS puolella. Eli voidaan kayttaa
suoraan neighCellIndices ja outlet tietoa, eika tarvitse laskea vektorin alku ja
loppupisteita ja tarkistaa miten lahella toisiaan ne on.
Tahan pitaisi pystya lisaamaan suht iisisti myos adaptiivisuus.
Subcatchmenttien keskipisteet saadaan suoraan mukana olevien solujen koordinaattien
keskiarvoina.
Taa antaa myos suoraan koko valuma-alueen rajauksen, koska vaan ne solut jotka kontribuoi
loppupisteelle on mukana.
Saako tasta myos flow width arvioitua suoraan laskemalla kaikkien subcatchmentin
sisaisten routtausten pituuden, ottamalla niista keskiarvon ja jakamalla subcathmentin
pinta-alan routtausten (flow lenghtien) keskiarvolla?
*/
int Grid::simplify(std::string path)
{
    bool newSubcathcments = true;
    int subcatchmentIdx = 0;

    // Connect cells into subcatchments starting from lower left corner
    while (newSubcathcments)
    {
        newSubcathcments = false;
        for (int i = 0; i < nCols * nRows; i++)
        {
            if (cells[i].landuse != LANDUSE_NONE && cells[i].landuse != LANDUSE_ROOF_UNCONNECTED && cells[i].landuse != LANDUSE_ROOF_CONNECTED && cells[i].outletID > -1)     //  Check that cells have routable land-use and outlet
            {
                int j = i;
                // Connect cells with outlet that have the same land-use
                while (cells[j].outletID > -1
                        &&
                        cells[j].landuse == cells[cells[j].outletID].landuse
                        &&
                        j != cells[j].outletID)
                {
                    // Connect cell into existing subcatchment
                    if ((cells[j].subcatchmentID > 0 || cells[cells[j].outletID].subcatchmentID > 0)
                            &&
                            cells[j].subcatchmentID != cells[cells[j].outletID].subcatchmentID)
                    {
                        int oldSubcatchmentIdx = 0;
                        if (cells[cells[j].outletID].subcatchmentID > 0)
                        {
                            oldSubcatchmentIdx = cells[cells[j].outletID].subcatchmentID;
                            cells[j].subcatchmentID = oldSubcatchmentIdx;
                        }
                        else
                        {
                            oldSubcatchmentIdx = cells[j].subcatchmentID;
                            cells[cells[j].outletID].subcatchmentID = oldSubcatchmentIdx;
                        }
                    }
                    // Create a new subcatchment
                    else if (cells[j].subcatchmentID == 0
                             &&
                             cells[cells[j].outletID].subcatchmentID == 0)
                    {
                        newSubcathcments = true;
                        subcatchmentIdx++;
                        cells[j].subcatchmentID = subcatchmentIdx;
                        cells[cells[j].outletID].subcatchmentID = subcatchmentIdx;
                    }
                    j = cells[j].outletID;
                }
                // Create a new subcatchment for individual cells that are not connected to same land-use
                if (cells[i].outletID > -1
                        &&
                        cells[i].subcatchmentID < 1
                        &&
                        cells[i].landuse != cells[cells[i].outletID].landuse
                        &&
                        cells[i].landuse != LANDUSE_ROOF_CONNECTED
                        &&
                        cells[i].landuse != LANDUSE_ROOF_UNCONNECTED)
                {
                    subcatchmentIdx++;
                    cells[i].subcatchmentID = subcatchmentIdx;
                }
            }
            // Roofs
            else
            {
                for (int k = 0; k < (int)cells[i].neighCellIndices.size(); k++)
                {
                    // Combine unconnected roof cells with same outlet into one subcatchment
                    if (cells[i].neighCellIndices[k] != -1
                            &&
                            cells[ cells[i].neighCellIndices[k] ].landuse == LANDUSE_ROOF_UNCONNECTED
                            &&
                            cells[i].outletID == cells[ cells[i].neighCellIndices[k] ].outletID)
                    {
                        // Check if subcatchment exists
                        if ((cells[i].subcatchmentID > 0 || cells[ cells[i].neighCellIndices[k] ].subcatchmentID > 0)
                            &&
                            cells[i].subcatchmentID != cells[cells[i].outletID].subcatchmentID)
                        {
                            int oldSubcatchmentIdx = 0;
                            if (cells[i].subcatchmentID > 0)
                            {
                                oldSubcatchmentIdx = cells[i].subcatchmentID;
                                cells[ cells[i].neighCellIndices[k] ].subcatchmentID = oldSubcatchmentIdx;
                            }
                            else
                            {
                                oldSubcatchmentIdx = cells[ cells[i].neighCellIndices[k] ].subcatchmentID;
                                cells[i].subcatchmentID = oldSubcatchmentIdx;
                            }
                        }
                        // Subcathment does not exits - create a new subcatchment
                        else if (cells[i].subcatchmentID == 0
                                 &&
                                 cells[cells[i].outletID].subcatchmentID == 0)
                        {
                            subcatchmentIdx++;
                            cells[i].subcatchmentID = subcatchmentIdx;
                            cells[ cells[i].neighCellIndices[k] ].subcatchmentID = subcatchmentIdx;
                        }
                    }
                    // Combine connected roof cells with same outlet into one subcatchment
                    if (cells[i].neighCellIndices[k] != -1
                            &&
                            cells[ cells[i].neighCellIndices[k] ].landuse == LANDUSE_ROOF_CONNECTED
                            &&
                            cells[i].outletID == cells[ cells[i].neighCellIndices[k] ].outletID)
                    {
                        // Check if subcatchment exists
                        if ((cells[i].subcatchmentID > 0 || cells[ cells[i].neighCellIndices[k] ].subcatchmentID > 0)
                             &&
                             cells[i].subcatchmentID != cells[cells[i].outletID].subcatchmentID)
                        {
                            int oldSubcatchmentIdx = 0;
                            if (cells[i].subcatchmentID > 0)
                            {
                                oldSubcatchmentIdx = cells[i].subcatchmentID;
                                cells[ cells[i].neighCellIndices[k] ].subcatchmentID = oldSubcatchmentIdx;
                            }
                            else
                            {
                                oldSubcatchmentIdx = cells[ cells[i].neighCellIndices[k] ].subcatchmentID;
                                cells[i].subcatchmentID = oldSubcatchmentIdx;
                            }
                        }
                        // Subcathment does not exits - create a new subcatchment
                        else if (cells[i].subcatchmentID == 0
                                 &&
                                 cells[cells[i].outletID].subcatchmentID == 0)
                        {
                            subcatchmentIdx++;
                            cells[i].subcatchmentID = subcatchmentIdx;
                            cells[ cells[i].neighCellIndices[k] ].subcatchmentID = subcatchmentIdx;
                        }
                    }
                }
                // Treat individual roof cells
                if (cells[i].outletID > -1
                        &&
                        cells[i].subcatchmentID < 1
                        &&
                        cells[i].landuse != cells[cells[i].outletID].landuse
                        &&
                        (cells[i].landuse == LANDUSE_ROOF_UNCONNECTED || cells[i].landuse == LANDUSE_ROOF_CONNECTED))
                {
                    subcatchmentIdx++;
                    cells[i].subcatchmentID = subcatchmentIdx;
                }
            }
        }
    }

    // Connect remainig cells into subcatchments starting from upper right corner
    newSubcathcments = true;
    while (newSubcathcments)
    {
        newSubcathcments = false;
        for (int i = nCols * nRows; i >= 0; i--)
        {
            if (cells[i].landuse != LANDUSE_NONE
                    &&
                    cells[i].landuse != LANDUSE_ROOF_UNCONNECTED
                    &&
                    cells[i].landuse != LANDUSE_ROOF_CONNECTED
                    &&
                    cells[i].outletID > -1)
            {
                int j = i;

                while (cells[j].outletID > -1
                        &&
                        cells[j].landuse == cells[cells[j].outletID].landuse
                        &&
                        j != cells[j].outletID)
                {
                    if ((cells[j].subcatchmentID > 0 || cells[cells[j].outletID].subcatchmentID > 0)
                            &&
                            cells[j].subcatchmentID != cells[cells[j].outletID].subcatchmentID)
                    {
                        int oldSubcatchmentIdx = 0;
                        if (cells[cells[j].outletID].subcatchmentID > 0)
                        {
                            oldSubcatchmentIdx = cells[cells[j].outletID].subcatchmentID;
                            cells[j].subcatchmentID = oldSubcatchmentIdx;
                        }

                        else
                        {
                            oldSubcatchmentIdx = cells[j].subcatchmentID;
                            cells[cells[j].outletID].subcatchmentID = oldSubcatchmentIdx;
                        }
                    }
                    else if (cells[j].subcatchmentID == 0
                             &&
                             cells[cells[j].outletID].subcatchmentID == 0)
                    {
                        newSubcathcments = true;
                        subcatchmentIdx++;
                        cells[j].subcatchmentID = subcatchmentIdx;
                        cells[cells[j].outletID].subcatchmentID = subcatchmentIdx;
                    }
                    j = cells[j].outletID;
                }
                // Create a new subcatchment for individual cells that are not connected to same land-use
                if (cells[i].outletID > -1 && cells[i].subcatchmentID < 1 && cells[i].landuse != cells[cells[i].outletID].landuse)
                {
                    subcatchmentIdx++;
                    cells[i].subcatchmentID = subcatchmentIdx;
                }
            }
        }
    }

    // Save subcatchment raster before destroying it
    saveRaster(path);

    // Create a copy of subcathments to be sorted
    // This could be done in a more sophisticated way, but for now just loop through
    // all cells...
    Cell * sortedCells = new Cell[ nCols * nRows ];
    for (int i = 0; i < nRows*nCols; i++)
    {
        sortedCells[i].name = cells[i].name;
        sortedCells[i].centerCoordX = cells[i].centerCoordX;
        sortedCells[i].centerCoordY = cells[i].centerCoordY;
        sortedCells[i].elevation = cells[i].elevation;
        sortedCells[i].flowdir = cells[i].flowdir;
        sortedCells[i].cellSize = cells[i].cellSize;
        sortedCells[i].slope = cells[i].slope;
        sortedCells[i].area = cells[i].area;
        sortedCells[i].flowWidth = cells[i].flowWidth;
        sortedCells[i].landuse = cells[i].landuse;
        sortedCells[i].outletCoordX = cells[i].outletCoordX;
        sortedCells[i].outletCoordY = cells[i].outletCoordY;
        sortedCells[i].outletID = cells[i].outletID;
        sortedCells[i].subcatchmentID = cells[i].subcatchmentID;
        sortedCells[i].outlet = cells[i].outlet;
        sortedCells[i].raingage = cells[i].raingage;
        sortedCells[i].imperv = cells[i].imperv;
        sortedCells[i].snowPack = cells[i].snowPack;
        sortedCells[i].N_Imperv = cells[i].N_Imperv;
        sortedCells[i].N_Perv = cells[i].N_Perv;
        sortedCells[i].S_Imperv = cells[i].S_Imperv;
        sortedCells[i].S_Perv = cells[i].S_Perv;
        sortedCells[i].PctZero = cells[i].PctZero;
        sortedCells[i].RouteTo = cells[i].RouteTo;
        sortedCells[i].PctRouted = cells[i].PctRouted;
        sortedCells[i].Suction = cells[i].Suction;
        sortedCells[i].HydCon = cells[i].HydCon;
        sortedCells[i].IMDmax = cells[i].IMDmax;
        sortedCells[i].neighCellIndices = cells[i].neighCellIndices;
        sortedCells[i].distanceToNeighbours = cells[i].distanceToNeighbours;
        sortedCells[i].isSink = cells[i].isSink;
    }

    // Create a vector for storing merged subcatchments
    std::vector<Cell> mergedCells;

    // Sort cells according to subcatchmentID
    std::sort(sortedCells, sortedCells + (nCols * nRows),
    [](Cell const & a, Cell const & b) -> bool
    { return a.subcatchmentID < b.subcatchmentID; } );

    // Go through sorted cells and keep increasing subcatchment size as long as subcatchmentID remains constant
    for (int i = 0; i < nCols * nRows; i++)
    {
        if (sortedCells[i].outletID > -1)
        {
            // Create a new subcatchment
            Cell newSubcatchment;

            // Non-pit subcatchments
            if (sortedCells[i].subcatchmentID > 0)
            {
                int oldID = sortedCells[i].subcatchmentID;
                int j = i;
                int cellCount = 0;

                // Give name for this subcatchment
                std::stringstream subcatchmentName("");
                subcatchmentName << "s" << sortedCells[i].subcatchmentID;
                // Give remaining parameters that are constant for the entire subcatchment
                newSubcatchment.name = subcatchmentName.str();
                newSubcatchment.landuse = sortedCells[i].landuse;
                newSubcatchment.raingage = sortedCells[i].raingage;
                newSubcatchment.imperv = sortedCells[i].imperv;
                newSubcatchment.snowPack = sortedCells[i].snowPack;
                newSubcatchment.N_Imperv = sortedCells[i].N_Imperv;
                newSubcatchment.N_Perv = sortedCells[i].N_Perv;
                newSubcatchment.S_Imperv = sortedCells[i].S_Imperv;
                newSubcatchment.S_Perv = sortedCells[i].S_Perv;
                newSubcatchment.PctZero = sortedCells[i].PctZero;
                newSubcatchment.RouteTo = sortedCells[i].RouteTo;
                newSubcatchment.PctRouted = sortedCells[i].PctRouted;
                newSubcatchment.Suction = sortedCells[i].Suction;
                newSubcatchment.HydCon = sortedCells[i].HydCon;
                newSubcatchment.IMDmax = sortedCells[i].IMDmax;

                // Add cells to the current subcatchment and give parameter values depenging on subcatchment size
                while (sortedCells[j].subcatchmentID == oldID)
                {
                    if (sortedCells[j].isSink > 0)
                    {
                        newSubcatchment.isSink = sortedCells[j].isSink;
                        std::stringstream outletName("");
                        outletName << "s" << sortedCells[j].subcatchmentID;
                        newSubcatchment.outlet = outletName.str();
                    }
                    newSubcatchment.elevation += sortedCells[j].elevation;
                    newSubcatchment.slope += sortedCells[j].slope;
                    newSubcatchment.area += sortedCells[j].area;
                    // Find outlet
                    if (sortedCells[j].outletID > -1 && sortedCells[j].subcatchmentID != cells[sortedCells[j].outletID].subcatchmentID)
                    {
                        // Subcatchment outlet is a pit
                        if (cells[sortedCells[j].outletID].subcatchmentID == 0)
                        {
                            std::stringstream outletName("");
                            outletName << "s" << sortedCells[j].subcatchmentID;
                            newSubcatchment.outlet = outletName.str();
                        }
                        // Subcatchment outlet is another subcatchment
                        else
                        {
                            std::stringstream outletName("");
                            outletName << "s" << cells[sortedCells[j].outletID].subcatchmentID;
                            newSubcatchment.outlet = outletName.str();
                        }
                    }
                    else if (sortedCells[j].outletID > -1 && (sortedCells[j].outlet.compare(0,1,"j") == 0))
                    {
                        newSubcatchment.outlet = sortedCells[j].outlet;
                    }

                    // outletCoordX
                    // outletCoordY
                    cellCount++;
                    j++;
                }
                newSubcatchment.elevation /= (double) cellCount;
                newSubcatchment.slope /= (double) cellCount;
                newSubcatchment.flowWidth = 0.7 * std::sqrt(newSubcatchment.area);    // Krebs et al. (2014)
                i = j - 1;
            }
// TJN 4 Dec 2017: Leave out individual pit cells since they do not contribute to outlet in any case
//                 (Also, naming them is difficult...)
//            // Individual pit cell subcatchments
//            else
//            {
//                subcatchmentCount++;
//
//                std::stringstream subcatchmentName("");
//                subcatchmentName << "s" << sortedCells[i].subcatchmentID;
//                newSubcatchment.name = subcatchmentName.str();
//                std::stringstream outletName("");
//                outletName << "s" << sortedCells[i].outletID;
//                newSubcatchment.outlet = outletName.str();
//                newSubcatchment.landuse = sortedCells[i].landuse;
//                newSubcatchment.raingage = sortedCells[i].raingage;
//                newSubcatchment.isSink = sortedCells[i].isSink;
//                newSubcatchment.imperv = sortedCells[i].imperv;
//                newSubcatchment.snowPack = sortedCells[i].snowPack;
//                newSubcatchment.N_Imperv = sortedCells[i].N_Imperv;
//                newSubcatchment.N_Perv = sortedCells[i].N_Perv;
//                newSubcatchment.S_Imperv = sortedCells[i].S_Imperv;
//                newSubcatchment.S_Perv = sortedCells[i].S_Perv;
//                newSubcatchment.PctZero = sortedCells[i].PctZero;
//                newSubcatchment.RouteTo = sortedCells[i].RouteTo;
//                newSubcatchment.PctRouted = sortedCells[i].PctRouted;
//                newSubcatchment.Suction = sortedCells[i].Suction;
//                newSubcatchment.HydCon = sortedCells[i].HydCon;
//                newSubcatchment.IMDmax = sortedCells[i].IMDmax;
//                newSubcatchment.elevation = sortedCells[i].elevation;
//                newSubcatchment.slope = sortedCells[i].slope;
//                newSubcatchment.area = sortedCells[i].area;
//                newSubcatchment.outletCoordX = sortedCells[i].outletCoordX;
//                newSubcatchment.outletCoordY = sortedCells[i].outletCoordY;
//                newSubcatchment.flowWidth = std::sqrt(newSubcatchment.area);    // Is there a better value for this?
//
////                std::cout << "\nsortedCells[i].isSink = " << sortedCells[i].isSink;
//            }
            mergedCells.push_back(newSubcatchment);
        }
    }

    // Transfer adaptive cell data to the pointer array.
    clear();
    nCols = (int)mergedCells.size();
    nRows = 1;
    cells = new Cell[nRows * nCols];

    for (int i = 0; i < nRows * nCols; i++ )
    {
        cells[i].name = mergedCells[i].name;
        cells[i].landuse = mergedCells[i].landuse;
        cells[i].raingage = mergedCells[i].raingage;
        cells[i].isSink = mergedCells[i].isSink;
        cells[i].imperv = mergedCells[i].imperv;
        cells[i].snowPack = mergedCells[i].snowPack;
        cells[i].N_Imperv = mergedCells[i].N_Imperv;
        cells[i].N_Perv = mergedCells[i].N_Perv;
        cells[i].PctZero = mergedCells[i].PctZero;
        cells[i].RouteTo = mergedCells[i].RouteTo;
        cells[i].PctRouted = mergedCells[i].PctRouted;
        cells[i].Suction = mergedCells[i].Suction;
        cells[i].HydCon = mergedCells[i].HydCon;
        cells[i].IMDmax = mergedCells[i].IMDmax;
        cells[i].elevation = mergedCells[i].elevation;
        cells[i].slope = mergedCells[i].slope;
        cells[i].flowWidth = mergedCells[i].flowWidth;
        cells[i].area = mergedCells[i].area;
        cells[i].outlet = mergedCells[i].outlet;
        cells[i].outletCoordX = mergedCells[i].outletCoordX;
        cells[i].outletCoordY = mergedCells[i].outletCoordY;
    }

    return 0;
}

// TJN 5 Dec 2017
// Find cells routed to outlet using the algorithm from GQIS flowTrace plugin.
// This can be extended to handle subcatchment simplification in future.
void Grid::findRouted(Table &outfallsTable, Table &condTable, Table &juncTable)
{
    // Empty vectors for final list of routed subcatchments and for the temporary working list
    std::vector<Cell> final_list;
    std::vector<Cell> selection_list;

    std::vector<std::string> outfalls(outfallsTable.nRows-1);
    std::vector<std::string> conduits(condTable.nRows-1);
    std::vector<std::string> inJunctions(condTable.nRows-1);
    std::vector<std::string> outJunctions(condTable.nRows-1);
    std::vector<std::string> junctions(juncTable.nRows-1);

    // Go through outfalls
    for (int i = 1; i < outfallsTable.nRows; i++)
    {
        outfalls[i-1] = outfallsTable.getData(i, 2);    // Outfall name
    }

    // Go through conduits
    for (int i = 1; i < condTable.nRows; i++)
    {
        conduits[i-1] = condTable.getData(i, 4);        // Conduit name
        inJunctions[i-1] = condTable.getData(i, 8);     // From junction
        outJunctions[i-1] = condTable.getData(i, 9);    // To junction
    }

    // Go through junctions
    for (int i = 1; i < juncTable.nRows; i++)
    {
        junctions[i-1] = juncTable.getData(i, 2);       // Junction name
    }

    // For each outfall follow conduit network upstream and add open junctions into final and selection list
    for (int i = 0; i < outfalls.size(); i++)
    {
        for (int j = 0; j < outJunctions.size(); j++)
        {
            if (outJunctions[j].compare(outfalls[i]) == 0)  // junctions[j] is routed to outfalls[i]
            {
                // TAHAN JOKU while LOOPPI flowTrace koodista
            }
        }
    }

    // Add outlet to the lists
//    final_list.push_back();
//    selection_list.push_back();
    // ONKO ONGELMA KUN PITAIS ROUTATA VALUMA-ALUEITA MUTTA VIRTAUS MENEE OSIN PUTKISSA?
    // KATO ONKO HOIDETTU JO AIEMMIN NIIN, ETTA ROUTATAAN SUBCATCHMENTIT NIIN KUIN OLIS PUTKET MUKANA
}

void Grid::saveRaster(std::string path)
{
    path += "_subcatchments.asc";       // TJN 23 Nov 2017

    Raster outputRaster;
    outputRaster.pathName = path;
    outputRaster.nCols = nCols;
    outputRaster.nRows = nRows;
    outputRaster.xllCorner = xllCorner;
    outputRaster.yllCorner = yllCorner;
    outputRaster.cellSize = cellSize;
    std::string noValue = "-1";
    outputRaster.noDataValue = noValue;
    outputRaster.data = new std::string[outputRaster.nCols * outputRaster.nRows];

    for (int j = 0; j < outputRaster.nRows; j++)
    {
        for (int i = 0; i < outputRaster.nCols; i++)
        {
            std::stringstream sstream;
            sstream << std::fixed;
            sstream.precision(8);

            if (cells[ i + j * nCols ].landuse != LANDUSE_NONE)
            {
                sstream << cells[ i + j * nCols ].subcatchmentID;        // TJN 23 Nov 2017
//                sstream << cells[ i + j * nCols ].slope * 100.0;
            }
            else
            {
                sstream << noValue.c_str();
            }

            int flippedIndex = outputRaster.nRows - 1 - j;
            outputRaster.data[ i + flippedIndex * outputRaster.nCols ] = sstream.str();
        }
    }

    outputRaster.save( outputRaster.pathName );
}

// TJN 17 May 2017 START
void Grid::saveSubcatchmentPolygon(std::string path)
{
    std::stringstream sstream;
    std::stringstream sstream_csvt;
    sstream << std::fixed;

    sstream << "id;";
    sstream << "wkt;";          // Polygon defining the shape of the  subcatchment
    sstream << "name;";         // Name of the  subcatchment
    sstream << "outlet;";       // Name of node or another subcatchment that receives runoff
    sstream << "area_m2;";      // Area of subcatchment (m2)
    sstream << "slope_pct;";    // Average surface slope (%)
    sstream << "elevation;";    // Elevation of the subcatchment
    sstream << "landuse;";      // Code for landuse type
    sstream << "imp_pct;";      // Percent of impervious area (%)
    sstream << "n_imp;";        // Mannings N for impervious area (-)
    sstream << "n_per;";        // Mannings N for pervious area (-)
    sstream << "S_imp_mm;";     // Depth of depression storage on impervious area (mm)
    sstream << "S_per_mm;";     // Depth of depression storage on pervious area (mm)
    sstream << "suct_mm;";      // Soil capillary suction head (mm)
    sstream << "Ksat_mmhr;";    // Soil saturated hydraulic conductivity (mm/hr)
    sstream << "IMDmax;";       // Difference between soil porosity and initial moisture content (a fraction)
    sstream << "isSink;";       // Cell is a local sink

    // Create a .csvt file defining the field types of the .wkt file for ogr2ogr conversion to shapefile
    sstream_csvt << "Integer,";
    sstream_csvt << "WKT,";
    sstream_csvt << "String,";
    sstream_csvt << "String,";
    sstream_csvt << "Real,";
    sstream_csvt << "Real,";
    sstream_csvt << "Real,";
    sstream_csvt << "Real,";
    sstream_csvt << "Real,";
    sstream_csvt << "Real,";
    sstream_csvt << "Real,";
    sstream_csvt << "Real,";
    sstream_csvt << "Real,";
    sstream_csvt << "Real,";
    sstream_csvt << "Real,";
    sstream_csvt << "Real,";
    sstream_csvt << "Integer";


    // Write polygon vertex coordinates.
    int polyId = 0;
    for (int i = 1; i < nRows * nCols; i++)
    {
        if (cells[i].landuse != LANDUSE_NONE)
        {
            sstream << "\n" << polyId << ";POLYGON((";
            sstream.precision(2);
            sstream << std::fixed << cells[i].centerCoordX - 0.5 * cells[i].cellSize << " ";
            sstream << std::fixed << cells[i].centerCoordY + 0.5 * cells[i].cellSize;
            sstream << ",";
            sstream << std::fixed << cells[i].centerCoordX - 0.5 * cells[i].cellSize << " ";
            sstream << std::fixed << cells[i].centerCoordY - 0.5 * cells[i].cellSize;
            sstream << ",";
            sstream << std::fixed << cells[i].centerCoordX + 0.5 * cells[i].cellSize << " ";
            sstream << std::fixed << cells[i].centerCoordY - 0.5 * cells[i].cellSize;
            sstream << ",";
            sstream << std::fixed << cells[i].centerCoordX + 0.5 * cells[i].cellSize << " ";
            sstream << std::fixed << cells[i].centerCoordY + 0.5 * cells[i].cellSize;
            sstream << ",";
            sstream << std::fixed << cells[i].centerCoordX - 0.5 * cells[i].cellSize << " ";
            sstream << std::fixed << cells[i].centerCoordY + 0.5 * cells[i].cellSize;
            sstream << "));" << cells[i].name;
            sstream <<  ";" << cells[i].outlet;
            sstream.precision(5);
            sstream <<  ";" << cells[i].area;
            sstream <<  ";" << cells[i].slope * 100.0; // convert fraction to percentage * 100.0
            sstream <<  ";" << cells[i].elevation;
            sstream <<  ";" << cells[i].landuse;
            sstream <<  ";" << cells[i].imperv;
            sstream.precision(3);
            sstream <<  ";" << cells[i].N_Imperv;
            sstream <<  ";" << cells[i].N_Perv;
            sstream.precision(2);
            sstream <<  ";" << cells[i].S_Imperv;
            sstream <<  ";" << cells[i].S_Perv;
            sstream <<  ";" << cells[i].Suction;
            sstream <<  ";" << cells[i].HydCon;
            sstream <<  ";" << cells[i].IMDmax;
            sstream <<  ";" << cells[i].isSink;

            polyId++;
        }
    }

    // Write files to disk.
    FileIO fileio;
    std::string path_csvt = path + "_subcatchments.csvt";
    path += "_subcatchments.wkt";
    int res = fileio.saveAsciiFile( path, sstream.str() );
    res = fileio.saveAsciiFile( path_csvt, sstream_csvt.str() );
}
// TJN 17 May 2017 END

// TJN 18 May 2017 START
void Grid::saveSubcatchmentRouting(std::string path)
{
    std::stringstream sstream;
    std::stringstream sstream_csvt;
    sstream << std::fixed;

    sstream << "id;";
    sstream << "wkt;";      // Line object defining the route between "from" and "to" subcatchments
    sstream << "from;";     // Name of the origin subcatchment
    sstream << "to";        // Name of the target subcatchment

    // Create a .csvt file defining the field types of the .wkt file for ogr2ogr conversion to shapefile
    sstream_csvt << "Integer,";
    sstream_csvt << "WKT,";
    sstream_csvt << "String,";
    sstream_csvt << "String,";

    // Write polygon vertex coordinates.
    int lineId = 0;
    for (int i = 1; i < nRows * nCols; i++)
    {
        if (cells[i].landuse != LANDUSE_NONE)
        {
            sstream << "\n" << lineId << ";LINESTRING(";
            sstream.precision(2);
            sstream << std::fixed << cells[i].centerCoordX << " ";
            sstream << std::fixed << cells[i].centerCoordY;
            sstream << ",";
            sstream << std::fixed << cells[i].outletCoordX << " ";
            sstream << std::fixed << cells[i].outletCoordY;
            sstream << ");" << cells[i].name;
            sstream <<  ";" << cells[i].outlet;
            lineId++;
        }
    }

    // Write files to disk.
    FileIO fileio;
    std::string path_csvt = path + "_subcatchment_routing.csvt";
    path += "_subcatchment_routing.wkt";
    int res = fileio.saveAsciiFile( path, sstream.str() );
    res = fileio.saveAsciiFile( path_csvt, sstream_csvt.str() );
}
// TJN 18 May 2017 END

void Grid::saveSWMM5File(Table &headerTable, Table &evaporationTable, Table &temperatureTable, Table &inflowsTable, Table &timeseriesTable, Table &reportTable,
                         Table &snowpacksTable, Table &raingagesTable, Table &symbolsTable, Table &juncTable, Table &outfallsTable, Table &condTable, Table &pumpsTable, Table &pumpCurvesTable, Table &dwfTable, Table &patternsTable, Table &lossesTable, Table &storageTable, Table &xsectionTable, std::string path)
{
    std::stringstream sstream;
    sstream << std::fixed;

    // If the output file is a PEST template file, write an identifier (ptf #) into the begining of the file
    if (path.substr ((int)path.length() - 3, 3) == "TPL")
    {
        std::cout << "\n-> test print: " << path.substr ((int)path.length() - 3, 3);
        sstream << "ptf #\n";
    }

    // Write the header table.
    std::cout << "\n-> Writing header table";
    sstream << "[TITLE]";
    sstream << "\n;;Project Title/Notes";
    sstream << "\n;;Generated with the GisToSWMM5 program";
    sstream << "\n";
    sstream << "\n[OPTIONS]";
    sstream << "\n;Option Value";
    headerTable.writeToStringStream(sstream);
    sstream << "\n";

    // Write the evaporation table.
    std::cout << "\n-> Writing evaporation table";
    sstream << "\n[EVAPORATION]";
    sstream << "\n;;Evap Data      Parameters";
    sstream << "\n;;-------------- ----------------";
    evaporationTable.writeToStringStream(sstream);
    sstream << "\n";

    // Write the temperature table.
    std::cout << "\n-> Writing temperature table";
    sstream << "\n[TEMPERATURE]";
    sstream << "\n;;Temp/Wind/Snow   Source/Data";
    temperatureTable.writeToStringStream(sstream);
    sstream << "\n";

    // Write raingage properties.
    std::cout << "\n-> Writing raingages";
    sstream << "\n[RAINGAGES]";
    sstream << "\n;;Gage           Format    Interval SCF      Source";
    sstream << "\n;;-------------- --------- ------ ------ ----------";
    raingagesTable.writeToStringStream(sstream);
    sstream << "\n";

    // Write subcatchment properties.
    std::cout << "\n-> Writing subcatchment properties";
    sstream << "\n[SUBCATCHMENTS]";
    sstream << "\n;;Subcatchment   Rain Gage        Outlet           Area     %Imperv  Width    %Slope   CurbLen  Snow Pack       ";
    sstream << "\n;;-------------- ---------------- ---------------- -------- -------- -------- -------- -------- ----------------";

    for (int i = 0; i < nRows * nCols; i++)
    {
        if (cells[i].landuse != LANDUSE_NONE)
        {
            sstream << "\n;" << cells[i].landuse;
            sstream << "\n" << cells[i].name << "   ";
            sstream << cells[i].raingage << "   ";
            sstream << cells[i].outlet << "   ";
            sstream.precision(5);
            sstream << cells[i].area / 10000.0 << "   "; // convert m2 to ha
            sstream.precision(2);
            sstream << cells[i].imperv << "   ";
            sstream << cells[i].flowWidth << "   ";
            sstream.precision(4);
            sstream << cells[i].slope * 100.0 << "   "; // convert fraction to percentage * 100.0
            sstream.precision(2);
            sstream << cells[i].cellSize << "   ";
            sstream << cells[i].snowPack << "   ";
        }
    }

    sstream << "\n";

    // Write subarea properties.
    std::cout << "\n-> Writing subarea properties";
    sstream << "\n[SUBAREAS]";
    sstream << "\n;;Subcatchment   N-Imperv   N-Perv     S-Imperv   S-Perv     PctZero    RouteTo    PctRouted ";
    sstream << "\n;;-------------- ---------- ---------- ---------- ---------- ---------- ---------- ----------";

    for (int i = 0; i < nRows * nCols; i++)
    {
        if (cells[i].landuse != LANDUSE_NONE)
        {
            sstream << "\n" << cells[i].name << "   ";
            //sstream.precision(3);
            sstream << cells[i].N_Imperv << "   ";
            sstream << cells[i].N_Perv << "   ";
            //sstream.precision(2);
            sstream << cells[i].S_Imperv << "   ";
            sstream << cells[i].S_Perv << "   ";
            sstream << cells[i].PctZero << "   ";
            sstream << cells[i].RouteTo << "   ";
            sstream << cells[i].PctRouted << "   ";
        }
    }

    sstream << "\n";

    // Write infiltration properties.
    std::cout << "\n-> Writing infiltration properties";
    sstream << "\n[INFILTRATION]";
    sstream << "\n;;Subcatchment   Suction    HydCon     IMDmax    ";
    sstream << "\n;;-------------- ---------- ---------- ----------";

    for (int i = 0; i < nRows * nCols; i++)
    {
        if (cells[i].landuse != LANDUSE_NONE)
        {
            sstream << "\n" << cells[i].name << "   ";
            //sstream.precision(2);
            sstream << cells[i].Suction << "   ";
            //sstream.precision(4);
            sstream << cells[i].HydCon << "   ";
            //sstream.precision(2);
            sstream << cells[i].IMDmax << "   ";
        }
    }

    sstream << "\n";

    // Write the snowpacks table.
    std::cout << "\n-> Writing snowpacks table";
    sstream << "\n[SNOWPACKS]";
    sstream << "\n;;Name           Surface    Parameters";
    sstream << "\n;;-------------- ---------- ----------";
    snowpacksTable.writeToStringStream(sstream);
    sstream << "\n";

    // Write junction properties.
    std::cout << "\n-> Writing junction properties";
    sstream << "\n[JUNCTIONS]";
    sstream << "\n;;Junction       Invert     MaxDepth   InitDepth  SurDepth   Aponded   ";
    sstream << "\n;;-------------- ---------- ---------- ---------- ---------- ----------";

    for (int i = 1; i < juncTable.nRows; i++)
    {
        sstream << "\n" << juncTable.getData(i, 2) << "   ";        // Junction name
        sstream << juncTable.getData(i, 4) << "   ";                // Invert elevation
        sstream << juncTable.getData(i, 5) << "   ";                // Max depth
        sstream << juncTable.getData(i, 7) << "   ";                // Initial depth
        sstream << juncTable.getData(i, 8) << "   ";                // Surcharge dept
        sstream << juncTable.getData(i, 9) << "   ";                // Ponded area
    }

    sstream << "\n";

    // Write outfall properties.
    std::cout << "\n-> Writing outfalls properties";
    sstream << "\n[OUTFALLS]";
    sstream << "\n;;Outfall        Invert     Type       Stage Data       Gated   ";
    sstream << "\n;;-------------- ---------- ---------- ---------------- --------";

    for (int i = 1; i < outfallsTable.nRows; i++)
    {
        sstream << "\n" << outfallsTable.data[i * outfallsTable.nCols + 2] << "   ";    // Outfall name
        sstream << outfallsTable.data[i * outfallsTable.nCols + 3] << "   ";            // Invert height
        sstream << outfallsTable.data[i * outfallsTable.nCols + 4] << "   ";            // Type
        sstream << outfallsTable.data[i * outfallsTable.nCols + 5] << "   ";            // Stage data
        sstream << outfallsTable.data[i * outfallsTable.nCols + 6] << "   ";            // Gated
    }

    sstream << "\n";

    // Write storage properties.
    std::cout << "\n-> Writing storage properties";
    sstream << "\n[STORAGE]";
    sstream << "\n;;Name           Elev.    MaxDepth   InitDepth  Shape      Curve Name/Params                     Fevap    Psi      Ksat     IMD    ";
    sstream << "\n;;-------------- -------- ---------- ----------- ---------- ---------------------------- -------- --------          -------- --------";

    for (int i = 1; i < storageTable.nRows; i++)
    {
        sstream << "\n" << storageTable.getData(i, 2) << "   ";
        sstream << storageTable.getData(i, 3) << "   ";
        sstream << storageTable.getData(i, 4) << "   ";
        sstream << storageTable.getData(i, 5) << "   ";
        sstream << storageTable.getData(i, 6) << "   ";
        sstream << storageTable.getData(i, 7) << "   ";
        sstream << storageTable.getData(i, 8) << "   ";
    }

    sstream << "\n";

    // Write conduits properties.
    std::cout << "\n-> Writing conduits properties";
    sstream << "\n[CONDUITS]";
    sstream << "\n;;Conduit        From Node        To Node          Length     Roughness  InOffset   OutOffset  InitFlow   MaxFlow   ";
    sstream << "\n;;-------------- ---------------- ---------------- ---------- ---------- ---------- ---------- ---------- ----------";

    for (int i = 1; i < condTable.nRows; i++)
    {
        sstream << "\n" << condTable.data[i * condTable.nCols + 4] << "   ";    // Conduit name
        sstream << condTable.data[i * condTable.nCols + 8] << "   ";            // From node
        sstream << condTable.data[i * condTable.nCols + 9] << "   ";            // To node
        sstream << condTable.data[i * condTable.nCols + 7] << "   ";            // Length
        sstream << condTable.data[i * condTable.nCols + 10] << "   ";           // roughness
        sstream << condTable.data[i * condTable.nCols + 11] << "   ";           // InOffset
        sstream << condTable.data[i * condTable.nCols + 12] << "   ";           // OutOffset
        sstream << 0.0 << "   ";        // InitFlow
        sstream << 0.0 << "   ";        // MaxFlow
    }

    sstream << "\n";

    // Write pump properties.
    std::cout << "\n-> Writing pumps";
    sstream << "\n[PUMPS]";
    sstream << "\n;;Name	From node	To node	Pump curve	Status	Startup	Shutoff";
    sstream << "\n;;------	------	------	------	------	------	------";
    pumpsTable.writeToStringStream(sstream);
    sstream << "\n";

    // Write conduit cross section properties.
    std::cout << "\n-> Writing conduit cross section properties";
    sstream << "\n[XSECTIONS]";
    sstream << "\n;;Link           Shape        Geom1            Geom2      Geom3      Geom4      Barrels   ";
    sstream << "\n;;-------------- ------------ ---------------- ---------- ---------- ---------- ----------";
    xsectionTable.writeToStringStream(sstream);
    sstream << "\n";

    // Write losses table.
    std::cout << "\n-> Writing losses table";
    sstream << "\n[LOSSES]";
    sstream << "\n;;Link           Kentry     Kexit      Kavg       Flap Gate  Seepage   ";
    sstream << "\n;;-------------- ---------- ---------- ---------- ---------- ----------";
    lossesTable.writeToStringStream(sstream);
    sstream << "\n";

    // Write pump curve properties.
    std::cout << "\n-> Writing pump curves";
    sstream << "\n[CURVES]";
    sstream << "\n;;Name	Type	X-value	Y-value";
    sstream << "\n;;------	------	------	------";
    pumpCurvesTable.writeToStringStream(sstream);
    sstream << "\n";

    // Write inflows table.
    std::cout << "\n-> Writing inflows table";
    sstream << "\n[INFLOWS]";
    sstream << "\n;;Node           Constituent      Time Series      Type     Mfactor  Sfactor  Baseline Pattern";
    sstream << "\n;;-------------- ---------------- ---------------- -------- -------- -------- -------- --------";
    inflowsTable.writeToStringStream(sstream);
    sstream << "\n";

    // Write time series table.
    std::cout << "\n-> Writing time series table";
    sstream << "\n[TIMESERIES]";
    sstream << "\n;;Name           Date       Time       Value";
    sstream << "\n;;-------------- ---------- ---------- ----------";
    timeseriesTable.writeToStringStream(sstream);
    sstream << "\n";

    // Write dry weather flow table.
    std::cout << "\n-> Writing dry weather flow table";
    sstream << "\n[DWF]";
    sstream << "\n;;Node           Constituent      Baseline   Patterns ";
    sstream << "\n;;-------------- ---------------- ---------- ----------";
    dwfTable.writeToStringStream(sstream);
    sstream << "\n";

    // Write patterns table.
    std::cout << "\n-> Writing patterns table";
    sstream << "\n[PATTERNS]";
    sstream << "\n;;Name           Type       Multipliers";
    sstream << "\n;;-------------- ---------- -----------";
    patternsTable.writeToStringStream(sstream);
    sstream << "\n";

    // Write reporting settings.
    std::cout << "\n-> Writing reporting settings";
    sstream << "\n[REPORT]";
    sstream << "\n;;Reporting Options";
    reportTable.writeToStringStream(sstream);
    sstream << "\n";

    // Write tags.
    sstream << "\n[TAGS]";
    sstream << "\n";

    // Write map settings.
    std::cout << "\n-> Writing map settings";
    sstream << "\n[MAP]";
    sstream << "\nDIMENSIONS ";
    sstream.precision(2);
    sstream << llcorner[0] * 0.99999 << " ";
    sstream << llcorner[1] * 0.99999 << " ";
    sstream << urcorner[0] * 1.00001 << " ";
    sstream << urcorner[1] * 1.00001;
    sstream << "\nUnits      Meters";
    sstream << "\n";

    // Write node coordinates.
    std::cout << "\n-> Writing node coordinates";
    sstream << "\n[COORDINATES]";
    sstream << "\n;;Node           X-Coord            Y-Coord           ";
    sstream << "\n;;-------------- ------------------ ------------------";

    for (int i = 1; i < juncTable.nRows; i++)
    {
        sstream << "\n" << juncTable.data[i * juncTable.nCols + 2] << "   ";
        sstream << juncTable.data[i * juncTable.nCols + 0] << "   ";
        sstream << juncTable.data[i * juncTable.nCols + 1] << "   ";
    }

    for (int i = 1; i < outfallsTable.nRows; i++)
    {
        sstream << "\n" << outfallsTable.data[i * outfallsTable.nCols + 2] << "   ";
        sstream << outfallsTable.data[i * outfallsTable.nCols] << "   ";
        sstream << outfallsTable.data[i * outfallsTable.nCols + 1] << "   ";
    }

    for (int i = 1; i < storageTable.nRows; i++)
    {
        sstream << "\n" << storageTable.getData(i, 2) << "   ";
        sstream << storageTable.getData(i, 0) << "   ";
        sstream << storageTable.getData(i, 1) << "   ";
    }

    sstream << "\n";
    sstream << "\n[VERTICES]";
    sstream << "\n;;Link           X-Coord            Y-Coord           ";
    sstream << "\n;;-------------- ------------------ ------------------";
    sstream << "\n";

    // Write polygon vertex coordinates.
    std::cout << "\n-> Writing polygon vertex coordinates";
    sstream << "\n[Polygons]";
    sstream << "\n;;Subcatchment   X-Coord            Y-Coord           ";
    sstream << "\n;;-------------- ------------------ ------------------";

    for (int i = 1; i < nRows * nCols; i++)
    {
        if (cells[i].landuse != LANDUSE_NONE)
        {
            sstream.precision(2);
            sstream << "\n" << cells[i].name << "   ";
            sstream << std::fixed << cells[i].centerCoordX - 0.5 * cells[i].cellSize << "   ";
            sstream << std::fixed << cells[i].centerCoordY + 0.5 * cells[i].cellSize;
            sstream << "\n" << cells[i].name << "   ";
            sstream << std::fixed << cells[i].centerCoordX - 0.5 * cells[i].cellSize << "   ";
            sstream << std::fixed << cells[i].centerCoordY - 0.5 * cells[i].cellSize;
            sstream << "\n" << cells[i].name << "   ";
            sstream << std::fixed << cells[i].centerCoordX + 0.5 * cells[i].cellSize << "   ";
            sstream << std::fixed << cells[i].centerCoordY - 0.5 * cells[i].cellSize;
            sstream << "\n" << cells[i].name << "   ";
            sstream << std::fixed << cells[i].centerCoordX + 0.5 * cells[i].cellSize << "   ";
            sstream << std::fixed << cells[i].centerCoordY + 0.5 * cells[i].cellSize;
            sstream << "\n" << cells[i].name << "   ";      // TJN 1 Jun 2017
            sstream << std::fixed << cells[i].centerCoordX - 0.5 * cells[i].cellSize << "   ";  // TJN 1 Jun 2017
            sstream << std::fixed << cells[i].centerCoordY + 0.5 * cells[i].cellSize;  // TJN 1 Jun 2017
        }
    }

    sstream << "\n";

    // Write symbols.
    std::cout << "\n-> Writing symbols";
    sstream << "\n[SYMBOLS]";
    sstream << "\n;;Gage           X-Coord            Y-Coord";
    sstream << "\n;;-------------- ------------------ ------------------";
    symbolsTable.writeToStringStream(sstream);
    sstream << "\n";
    sstream << "\n";

    // Write the file to disk.
    std::cout << "\n-> Writing the file to disk";
    FileIO fileio;
    path += ".inp";
    int res = fileio.saveAsciiFile( path, sstream.str() );
}

void Grid::printReport(Table &catchPropTable)
{
    double elevationAverage = 0.0;
    double slopeAverage = 0.0;
    int numOfLandUseClasses = catchPropTable.nRows - 1;

    if (numOfLandUseClasses < 0)
    {
        numOfLandUseClasses = 0;
    }

    std::vector<int> cellsInlandUseClass;
    cellsInlandUseClass.assign(numOfLandUseClasses, 0);
    std::vector<int> landUseClassIds;
    landUseClassIds.assign(numOfLandUseClasses, 0);
    std::vector<double> areasInlandUseClass;
    areasInlandUseClass.assign(numOfLandUseClasses, 0.0);

    for (int i = 0; i < (int)landUseClassIds.size(); i++)
    {
        landUseClassIds[i] = atoi(catchPropTable.data[(i + 1) * catchPropTable.nCols].c_str());
    }

    int numOfActiveCells = 0;
    int numOfActiveCellsNoRoofs = 0;

    for (int i = 0; i < nCols * nRows; i++)
    {
        if (cells[i].landuse != LANDUSE_NONE)
        {
            numOfActiveCells += 1;
            elevationAverage += cells[i].elevation;

            if (cells[i].landuse != LANDUSE_ROOF_CONNECTED || cells[i].landuse != LANDUSE_ROOF_UNCONNECTED)
            {
                slopeAverage += cells[i].slope;
                numOfActiveCellsNoRoofs += 1;
            }

            for (int j = 0; j < (int)landUseClassIds.size(); j++)
            {
                if (landUseClassIds[j] == cells[i].landuse)
                {
                    cellsInlandUseClass[j] += 1;
                    areasInlandUseClass[j] += cells[i].area;
                    break;
                }
            }
        }
    }

    double area = 0.0;

    for (int i = 0; i < numOfLandUseClasses; i++)
    {
        area += areasInlandUseClass[i];
    }

    if (numOfActiveCells > 0)
    {
        elevationAverage /= (double)(numOfActiveCells);
    }

    if (numOfActiveCellsNoRoofs > 0)
    {
        slopeAverage /= (double)(numOfActiveCellsNoRoofs);
    }

    std::cout << "\n-> Total number of cells: " << nCols * nRows;
    std::cout << "\n-> Number of active cells: " << numOfActiveCells;
    std::cout << "\n-> Catchment area: " << area / 10000.0 << " ha";
    std::cout << "\n-> Catchment average elevation: " << elevationAverage << " m";
    std::cout << "\n-> Catchment average slope: " << slopeAverage;
    std::cout << "\n-> Landuse information (code, number of cells, area in ha, % of catchment):";

    for (int i = 0; i < (int)cellsInlandUseClass.size(); i++)
    {
        std::cout << "\n"
                  << "\t" << landUseClassIds[i]
                  << "\t" << cellsInlandUseClass[i]
                  << "\t" << areasInlandUseClass[i] / 10000.0
                  << "\t" << areasInlandUseClass[i] / area * 100.0;
    }
}

