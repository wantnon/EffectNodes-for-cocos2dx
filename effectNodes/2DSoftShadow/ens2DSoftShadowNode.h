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
class Cedge{
public:
    CCPoint m_start;
    CCPoint m_end;
};
class ClightNode:public CCNode{
public:
    ClightNode(){
        m_r=0;
    }
    bool init(float r){
        m_r=r;
        this->CCNode::init();
        return true;
    }
    float getR()const{return m_r;}
    void draw(){
        glLineWidth(1);
        ccDrawCircle(CCPoint(0,0), m_r, 360, 10, false, 1, 1);
    }
protected:
    float m_r;
};
class C2DSoftShadowNode:public CCNode
{
public:
    C2DSoftShadowNode(){
        m_light=NULL;
    }
    virtual~C2DSoftShadowNode(){

    }
    bool init(const Cpolygon&polygon){
        m_polygon=polygon;
        this->CCNode::init();
        this->scheduleUpdate();
        return true;
    }
    void update(float dt){
        updateShadow();
    }
    void updateShadow(){
        if(m_light==NULL)return;
        m_backLightEdgeList.clear();
        CCPoint lightPosLocal=getLightPosLocal();
        int nPoint=(int)m_polygon.m_pointList.size();
        //find a edge which is facing the light
        int facingLightEdgeIndex=-1;
        for(int i=0;i<nPoint;i++){
            CCPoint p=m_polygon.m_pointList[i];
            CCPoint pn=m_polygon.m_pointList[(i+1)%nPoint];
            CCPoint rightVec=getRightVector(p,pn);
            CCPoint lightToP=p-lightPosLocal;
            float dotProduct=ccpDot(rightVec, lightToP);
            if(dotProduct<=0){//facing light
                facingLightEdgeIndex=i;
                break;
            }
        }//got facingLightEdgeIndex
        //cout<<"facinglightEdgeIndex:"<<facingLightEdgeIndex<<endl;
        if(facingLightEdgeIndex==-1)return;//if light is in this shape, facingLightEdgeIndex will be -1
        //start search from facingLightEdgeIndex, find all back-light edges
        for(int ioffset=0;ioffset<nPoint;ioffset++){
            int index=facingLightEdgeIndex+ioffset+1;//start from facingLightEdgeIndex's next point
            index=index%nPoint;
            CCPoint p=m_polygon.m_pointList[index];
            CCPoint pn=m_polygon.m_pointList[(index+1)%nPoint];
            CCPoint rightVec=getRightVector(p,pn);
            CCPoint lightToP=p-lightPosLocal;
            float dotProduct=ccpDot(rightVec, lightToP);
            if(dotProduct>0){//back light
                Cedge edge;
                edge.m_start=p;
                edge.m_end=pn;
                m_backLightEdgeList.push_back(edge);
            }
        }//got m_backLightEdgeList
        //calculate:
        // m_leftTangentOfLeftShadowPoint;
        // m_leftTangentOfRightShadowPoint;
        // m_rightTangentOfLeftShadowPoint;
        // m_rightTangentOfRightShadowPoint;
        if((int)m_backLightEdgeList.size()!=0)
        {
            CCPoint leftShadowPoint=m_backLightEdgeList[0].m_start;
            CCPoint rightShadowPoint=m_backLightEdgeList[(int)m_backLightEdgeList.size()-1].m_end;
            vector<CCPoint> leftAndRightTangentOfLeftShadowPoint=calculateTangentPoints(lightPosLocal, m_light->getR(), leftShadowPoint);
            m_leftTangentOfLeftShadowPoint=leftAndRightTangentOfLeftShadowPoint[0];
            m_rightTangentOfLeftShadowPoint=leftAndRightTangentOfLeftShadowPoint[1];
            vector<CCPoint> leftAndRightTangentOfRightShadowPoint=calculateTangentPoints(lightPosLocal, m_light->getR(), rightShadowPoint);
            m_leftTangentOfRightShadowPoint=leftAndRightTangentOfRightShadowPoint[0];
            m_rightTangentOfRightShadowPoint=leftAndRightTangentOfRightShadowPoint[1];
            
        }else{
            assert(false);
        }
    
    }
    void draw(){
        //draw shape
        glLineWidth(1);
        ccDrawPoly(&m_polygon.m_pointList.front(), (int)m_polygon.m_pointList.size(), true);
        //light pos local
        ccPointSize(4);
        ccDrawPoint(getLightPosLocal());
        //draw m_backLightEdgeList
        glLineWidth(4);
        int nBackLightEdge=(int)m_backLightEdgeList.size();
        for(int i=0;i<nBackLightEdge;i++){
            const Cedge&edge=m_backLightEdgeList[i];
            ccDrawLine(edge.m_start, edge.m_end);
        }
        //draw shadow line
        {
            
            
            CCPoint lightPosLocal=getLightPosLocal();
            int nBackLightEdge=(int)m_backLightEdgeList.size();
            if(nBackLightEdge!=0){
                //light to shadow point
                glLineWidth(1);
                CCPoint leftShadowPoint=m_backLightEdgeList[0].m_start;
                CCPoint rightShadowPoint=m_backLightEdgeList[(int)m_backLightEdgeList.size()-1].m_end;
                ccDrawLine(lightPosLocal, leftShadowPoint);
                ccDrawLine(lightPosLocal, rightShadowPoint);
                //shadow point to infinite
                glLineWidth(4);
                CCPoint lightToLeftShadowPoint=leftShadowPoint-lightPosLocal;
                CCPoint lightToRightShadowPoint=rightShadowPoint-lightPosLocal;
                CCPoint shadowStartInfinite=leftShadowPoint+ccpMult(lightToLeftShadowPoint, 10000);
                CCPoint shadowEndInfinite=rightShadowPoint+ccpMult(lightToRightShadowPoint, 10000);
                ccDrawLine(leftShadowPoint, shadowStartInfinite);
                ccDrawLine(rightShadowPoint, shadowEndInfinite);
                
            }
            
            
        }
        //draw left umbra
        if((int)m_backLightEdgeList.size()!=0){
            glLineWidth(1);
             CCPoint leftShadowPoint=m_backLightEdgeList[0].m_start;
            ccDrawLine(leftShadowPoint, m_leftTangentOfLeftShadowPoint);
            
        }
    }
    void setLight(ClightNode*light){
        if(m_light==NULL){
            m_light=light;
            m_light->retain();
        }else{
            m_light->release();
            m_light=light;
            m_light->retain();
        }
    
    }
protected:
    CCPoint getLightPosLocal(){
        CCPoint lightPosWorld=m_light->convertToWorldSpaceAR(ccp(0,0));
        CCPoint lightPosLocal=this->convertToNodeSpace(lightPosWorld);
        return lightPosLocal;
    }
protected:
    Cpolygon m_polygon;
    ClightNode *m_light;
    vector<Cedge> m_backLightEdgeList;
    CCPoint m_leftTangentOfLeftShadowPoint;
    CCPoint m_leftTangentOfRightShadowPoint;
    CCPoint m_rightTangentOfLeftShadowPoint;
    CCPoint m_rightTangentOfRightShadowPoint;
};




namespace_ens_end
#endif
