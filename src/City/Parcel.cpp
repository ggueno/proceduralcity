#include "Parcel.hpp"

double frandd_a_b(double a, double b){
    return ( rand()/(double)RAND_MAX ) * (b-a) + a;
}

void Parcel::divide(glm::vec3 leftCorner, float height, float width, int it){

        if( it == 0) return;

        if( height / 2.0 < m_lotMinSize || width / 2.0 < m_lotMinSize) {
            //std::cout << height<< "<" << m_lotMinSize << std::endl;
            //std::cout << width / 2.0<< "<" << m_lotMinSize << std::endl;
            m_listLots.push_back(
                new Lot(
                    glm::vec3(leftCorner.x, 0.0, leftCorner.z),
                    glm::vec3(leftCorner.x, 0.0, leftCorner.z+height),
                    glm::vec3(leftCorner.x+width, 0.0, leftCorner.z+height),
                    glm::vec3(leftCorner.x+width, 0.0, leftCorner.z),
                    m_buildingHeightMax,
                    m_buildingHeightMin
                )
            );
        }else{

            float r = frandd_a_b(0.3,0.7);

            glm::vec3 tmp1 = glm::vec3(leftCorner.x+r*width, 0.0, leftCorner.z); //E

            //r = frandd_a_b(0.3,0.7);
            glm::vec3 tmp2 = glm::vec3(tmp1.x, 0.0,tmp1.z+height);

            r = frandd_a_b(0.3,0.7);
            glm::vec3 tmp3 = glm::vec3(leftCorner.x, 0.0, leftCorner.z+r*height);

            //r = frandd_a_b(0.3,0.7);
            glm::vec3 tmp4 = glm::vec3(tmp3.x+width, 0.0, tmp3.z);

            //Callback of the function for the topLeft terrain
            glm::vec3 intersect = glm::vec3(tmp1.x, 0.0, tmp3.z);

            if(it == 1){
                //Margin

                //Top left parcel
                m_listLots.push_back(
                    new Lot(
                        glm::vec3(leftCorner.x, 0.0, leftCorner.z),
                        glm::vec3(tmp3.x, 0.0, tmp3.z),
                        glm::vec3(intersect.x, 0.0, intersect.z),
                        glm::vec3(tmp1.x, 0.0, tmp1.z),
                        m_buildingHeightMax,
                        m_buildingHeightMin
                    )
                );


                //Top right parcel
                m_listLots.push_back(
                    new Lot(
                        glm::vec3(tmp1.x, 0.0, tmp1.z),
                        glm::vec3(intersect.x, 0.0, intersect.z),
                        glm::vec3(tmp4.x, 0.0, tmp4.z),
                        glm::vec3(leftCorner.x+width, 0.0, leftCorner.z),
                        m_buildingHeightMax,
                        m_buildingHeightMin
                    )
                );

                //Bottom right parcel
                m_listLots.push_back(
                    new Lot(
                        glm::vec3(intersect.x,0.0, intersect.z),
                        glm::vec3(tmp2.x ,0.0, tmp2.z ),
                        glm::vec3(leftCorner.x + width ,0.0, leftCorner.z + height ),
                        glm::vec3(tmp4.x ,0.0, tmp4.z ),
                        m_buildingHeightMax,
                        m_buildingHeightMin
                    )
                );

                //Bottom left parcel
                m_listLots.push_back(
                    new Lot(
                        glm::vec3(tmp3.x,0.0, tmp3.z),
                        glm::vec3(leftCorner.x ,0.0, leftCorner.z + height ),
                        glm::vec3(tmp2.x ,0.0, tmp2.z ),
                        glm::vec3(intersect.x,0.0, intersect.z),
                        m_buildingHeightMax,
                        m_buildingHeightMin
                    )
                );
        }

        float width1 = fabs(tmp1.x-leftCorner.x);
        float width2 = fabs(leftCorner.x+width-tmp1.x);
        float height1 = fabs(tmp3.z - leftCorner.z);
        float height2 = fabs(leftCorner.z+height-tmp3.z);

        divide(leftCorner, height1, width1, it-1); //AGIE
        divide(tmp3, height2, width1 , it-1); //GBFI
        divide(intersect, height2, width2, it-1); //IFCH
        divide(tmp1, height1, width2, it-1); //EIH
    }
}