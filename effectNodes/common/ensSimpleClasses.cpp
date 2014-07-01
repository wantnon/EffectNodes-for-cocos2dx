//
//  ensSimpleClasses.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 13-12-4.
//
//

#include "common/ensSimpleClasses.h"
namespace_ens_begin
Cpolygon makeRegularPolygon(int r,int n){
    vector<CCPoint> pointList;
    float A=0;
    float dA=360.0/n;
    for(int i=0;i<n;i++){
        float x=r*cosf(A*M_PI/180);
        float y=r*sinf(A*M_PI/180);
        CCPoint point(x,y);
        pointList.push_back(point);
        A+=dA;
    }
    Cpolygon polygon;
    polygon.m_pointList=pointList;
    return polygon;
}
namespace_ens_end