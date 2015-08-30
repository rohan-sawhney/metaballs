#ifndef METABALL_H
#define METABALL_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <vector>
#include <iostream>

class Metaball {
public:
    // overridden default constructor
    Metaball(const Eigen::Vector3d& center0, const double radius0, const double min, const double max);
    
    // resolves collision with boundary
    void resolveBoundaryCollisions();
    
    // moves ball 
    void step();
    
    // member variables;
    Eigen::Vector3d center;
    double radius;

private:
    Eigen::Vector3d direction;
    std::vector<Eigen::Vector3d> planes; // p1, n1, p2, n2 ... p6, n6
    
    bool collision(const Eigen::Vector3d& p, const Eigen::Vector3d& n) const;
};

#endif
