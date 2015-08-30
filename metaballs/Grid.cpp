#include "Grid.h"

void Grid::init(const double resolution0, const int min0, const int max0)
{
    min = min0;
    max = max0;
    resolution = resolution0;
    
    // build grid
    for (int i = min; i < max; i++) {
        std::vector<std::vector<double>> yValues;
        std::vector<std::vector<Eigen::Vector3d>> yPoints;
        
        for (int j = min; j < max; j++) {
            std::vector<double> zValues;
            std::vector<Eigen::Vector3d> zPoints;
            
            for (int k = min; k < max; k++) {
                zValues.push_back(0.0);
                zPoints.push_back(Eigen::Vector3d(i*resolution, j*resolution, k*resolution));
            }
            yValues.push_back(zValues);
            yPoints.push_back(zPoints);
        }
        
        isovalues.push_back(yValues);
        points.push_back(yPoints);
    }
}

void Grid::addMetaball()
{
    double r = 0.04 + ((double)rand() / RAND_MAX) * (0.07 - 0.04);
    
    double minRes = min * resolution + 0.25;
    double maxRes = max * resolution - 0.25;
    double x = minRes + ((double)rand() / RAND_MAX) * (maxRes - minRes);
    double y = minRes + ((double)rand() / RAND_MAX) * (maxRes - minRes);
    double z = minRes + ((double)rand() / RAND_MAX) * (maxRes - minRes);
    Eigen::Vector3d center(x, y, z);
    
    Metaball metaball(center, r, minRes, maxRes);
    metaballs.push_back(metaball);
}

void Grid::computeIsovalues()
{
    int size = max - min;
    
    // zero out isovalues
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                isovalues[i][j][k] = 0.0;
            }
        }
    }
    
    // compute isovalues
    for (size_t m = 0; m < metaballs.size(); m++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                for (int k = 0; k < size; k++) {
                    isovalues[i][j][k] += metaballs[m].radius /
                                          (points[i][j][k] - metaballs[m].center).norm();
                }
            }
        }
    }
}

void Grid::initializeGridCell(GridCell& gridCell, const int i, const int j, const int k) const
{
    gridCell.points.push_back(points[i][j][k]);
    gridCell.values.push_back(isovalues[i][j][k]);
    
    gridCell.points.push_back(points[i+1][j][k]);
    gridCell.values.push_back(isovalues[i+1][j][k]);
    
    gridCell.points.push_back(points[i+1][j][k+1]);
    gridCell.values.push_back(isovalues[i+1][j][k+1]);
    
    gridCell.points.push_back(points[i][j][k+1]);
    gridCell.values.push_back(isovalues[i][j][k+1]);
    
    gridCell.points.push_back(points[i][j+1][k]);
    gridCell.values.push_back(isovalues[i][j+1][k]);
    
    gridCell.points.push_back(points[i+1][j+1][k]);
    gridCell.values.push_back(isovalues[i+1][j+1][k]);
    
    gridCell.points.push_back(points[i+1][j+1][k+1]);
    gridCell.values.push_back(isovalues[i+1][j+1][k+1]);
    
    gridCell.points.push_back(points[i][j+1][k+1]);
    gridCell.values.push_back(isovalues[i][j+1][k+1]);
}

void Grid::step(const double isolevel, std::vector<Triangle>& triangles)
{
    for (size_t i = 0; i < metaballs.size(); i++) {
        metaballs[i].step();
        metaballs[i].resolveBoundaryCollisions();
    }
    
    computeIsovalues();
    
    // run marching cubes
    int size = max - min;
    for (int i = 0; i < size-1; i++) {
        for (int j = 0; j < size-1; j++) {
            for (int k = 0; k < size-1; k++) {
                GridCell gridCell;
                initializeGridCell(gridCell, i, j, k);

                marchingCubes.polygonize(gridCell, isolevel, triangles);
            }
        }
    }
}