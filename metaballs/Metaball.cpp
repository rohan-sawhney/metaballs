#include "Metaball.h"
#define DAMP 0.05

Metaball::Metaball(const Eigen::Vector3d& center0, const double radius0,
                   const double min, const double max):
center(center0),
radius(radius0)
{    
    planes.push_back(Eigen::Vector3d(min, 0, 0));
    planes.push_back(Eigen::Vector3d(1, 0, 0));
    planes.push_back(Eigen::Vector3d(max, 0, 0));
    planes.push_back(Eigen::Vector3d(-1, 0, 0));
    planes.push_back(Eigen::Vector3d(0, min, 0));
    planes.push_back(Eigen::Vector3d(0, 1, 0));
    planes.push_back(Eigen::Vector3d(0, max, 0));
    planes.push_back(Eigen::Vector3d(0, -1, 0));
    planes.push_back(Eigen::Vector3d(0, 0, min));
    planes.push_back(Eigen::Vector3d(0, 0, 1));
    planes.push_back(Eigen::Vector3d(0, 0, max));
    planes.push_back(Eigen::Vector3d(0, 0, -1));
    
    direction.setRandom();
    direction.normalize();
}

void Metaball::step()
{
    center += direction * DAMP;
}

bool Metaball::collision(const Eigen::Vector3d& p, const Eigen::Vector3d& n) const
{
    Eigen::Vector3d v = center - p;
    double d = v.dot(n);
    
    if(d < radius) {
        return true;
    }
    
    return false;
}

void Metaball::resolveBoundaryCollisions()
{
    for (int i = 0; i < 6; i++) {
        if (collision(planes[2*i+0], planes[2*i+1])) {
            direction -= 2 * planes[2*i+1] * (planes[2*i+1].dot(direction));
            direction.normalize();
            
            break;
        }
    }
}