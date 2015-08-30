#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <vector>

class GridCell {
public:
    std::vector<Eigen::Vector3d> points;
    std::vector<double> values;
};

class Triangle {
public:
    Triangle(const Eigen::Vector3d& a0, const Eigen::Vector3d& b0,
             const Eigen::Vector3d& c0): a(a0), b(b0), c(c0) {}
    
    Eigen::Vector3d a;
    Eigen::Vector3d b;
    Eigen::Vector3d c;
};

// reference: http://paulbourke.net/geometry/polygonise/
class MarchingCubes {
public:
    // polygonizes the isosurface given a grid cell and isosurface
    int polygonize(const GridCell& gridCell, const double isolevel, std::vector<Triangle>& triangles) const;
    
private:
    // linearly interpolate the position where an isosurface cuts an edge between two vertices,
    // each with their own scalar value
    void vertexInterp(const double isolevel, const Eigen::Vector3d& p1,
                      const Eigen::Vector3d& p2, const double value1,
                      const double value2, Eigen::Vector3d& v) const;
};

#endif