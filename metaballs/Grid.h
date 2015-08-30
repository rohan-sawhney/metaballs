#ifndef GRID_H
#define GRID_H

#include "Metaball.h"
#include "MarchingCubes.h"

class Grid {
public:
    // initialize grid
    void init(const double resolution0, const int min0, const int max0);
    
    // add a metaball to the grid
    void addMetaball();
    
    // steps system
    void step(const double isolevel, std::vector<Triangle>& triangles);

private:    
    // computes isovalues
    void computeIsovalues();
    
    // sets grid cell points and values
    void initializeGridCell(GridCell& gridCell, const int i, const int j, const int k) const;
    
    // member variables
    int min;
    int max;
    double resolution;
    std::vector<Metaball> metaballs;
    MarchingCubes marchingCubes;
    std::vector<std::vector<std::vector<Eigen::Vector3d>>> points;
    std::vector<std::vector<std::vector<double>>> isovalues;
};

#endif
