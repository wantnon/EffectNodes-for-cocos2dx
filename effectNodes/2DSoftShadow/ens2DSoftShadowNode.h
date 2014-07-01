//
//  ens2DSoftShadowNode.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 13-12-16.
//
//

#ifndef HelloWorld_ens2DSoftShadowNode_h
#define HelloWorld_ens2DSoftShadowNode_h

#include <vector>
#include<iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "common/ensCommon.h"

namespace_ens_begin
class ClightNode:public CCNode{
public:
    bool init(float r){
        m_r=r;
        this->CCNode::init();
        return true;
    }
    float getR()const{return m_r;}
    void draw(){
        ccDrawCircle(CCPoint(0,0), m_r, 360, 10, false, 1, 1);
    }
protected:
    float m_r;
};
class C2DSoftShadowSprite:public CCSprite
{
public:
    
protected:
    Cpolygon *m_polygon;
    ClightNode*m_light;
};




namespace_ens_end
#endif
