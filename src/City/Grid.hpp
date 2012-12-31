#ifndef GRID_HPP
#define GRID_HPP

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>

#include <glm/glm.hpp>

#include "Lot.hpp"
#include "Parcel.hpp"

enum BuildingType{ CIRCULAR, FROM_ORIGIN, RANDOM };

class Grid {
public:
    Grid(){}
    Grid(glm::vec3 leftCorner, float height, float width,int iteration = 3, float parcel_min_size = 1.0, float margin = 1.0);

    std::vector<glm::vec3> getListofNodes(){ return listNodes; }
    std::vector<Parcel *> getListofParcels(){ return listParcels; }

    //subdivides a rectangle in order to create roads
    // We link 4 glm::vec3s 2 by 2 : the first 2 ones form the vertical road
    //                           the last 2 ones form the horizontal road
    void createRoads(glm::vec3 leftCorner, float height, float width, int it);
    void initDrawGrid();
    Lot * getOneLot();

protected:
    float m_parcel_min_size;
    float m_margin;
    size_t indiceFull;

    std::vector<glm::vec3> listNodes;
    std::vector<Parcel * > listParcels;
    float width, height;
};

#endif
