//
//  ens2DSoftShadowNode.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 13-12-16.
//
//

#ifndef HelloWorld_ens2DSoftShadowNode_h
#define HelloWorld_ens2DSoftShadowNode_h
/*
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
        ccDrawColor4F(1, 1, 1, 1);
        ccDrawCircle(CCPoint(0,0), m_r, 360, 10, false, 1, 1);
    }
protected:
    float m_r;
};
class C2DSoftShadowNode:public CCSprite
{
public:
    C2DSoftShadowNode(){
        m_light=NULL;
        m_isLeftUmbraCut=false;
        m_isRightUmbraCut=false;
        m_isLeftPenumbraCut=false;
        m_isRightPenumbraCut=false;
        m_leftTangentPointOfLeftShadowPoint=CCPoint(INFINITY,INFINITY);
        m_leftTangentPointOfRightShadowPoint=CCPoint(INFINITY,INFINITY);
        m_rightTangentPointOfLeftShadowPoint=CCPoint(INFINITY,INFINITY);
        m_rightTangentPointOfRightShadowPoint=CCPoint(INFINITY,INFINITY);
        m_mesh=NULL;
        m_finTexture=NULL;
        m_indexVBO=NULL;
        m_shadowLength=500;//1500;
    }
    virtual~C2DSoftShadowNode(){
        if(m_mesh)m_mesh->release();
        if(m_finTexture)m_finTexture->release();
        if(m_indexVBO)m_indexVBO->release();

    }
    bool init(const Cpolygon&polygon){
        m_polygon=polygon;
        this->CCSprite::init();
        //start update
        this->scheduleUpdate();
        //create mesh
        m_mesh=new Cmesh();
        m_mesh->autorelease();
        m_mesh->retain();
        //create indexVBO
        m_indexVBO=new CindexVBO();
        m_indexVBO->autorelease();
        m_indexVBO->retain();
        //finTexture
        m_finTexture=CCTextureCache::sharedTextureCache()->addImage("demoRes/lightFin.png");
        m_finTexture->retain();

        return true;
    }
    void update(float dt){
        updateShadow();
        updateMesh();
        submit(GL_DYNAMIC_DRAW);
    }
    void updateShadow(){
        if(m_light==NULL)return;
        m_backLightEdgeList.clear();
        m_facingLightEdgeList.clear();
        CCPoint lightPosLocal=getLightPosLocal();
        int nPoint=(int)m_polygon.m_pointList.size();
        vector<bool> isBackLightEdgeList;
        for(int i=0;i<nPoint;i++){
            CCPoint p=m_polygon.m_pointList[i];
            CCPoint pn=m_polygon.m_pointList[(i+1)%nPoint];
            CCPoint rightVec=getRightVector(p,pn);
            vector<CCPoint> leftAndRightTangentPointOfP=calculateTangentPointsOfPointToCircle(lightPosLocal, m_light->getR(), p);
            if(leftAndRightTangentPointOfP.empty())return;//if any vertex is in point circle, return
            CCPoint leftTangentPointOfP=leftAndRightTangentPointOfP[0];
            CCPoint rightTangentPointOfP=leftAndRightTangentPointOfP[1];
            CCPoint lightToLeftTangentPoint=p-leftTangentPointOfP;
            CCPoint lightToRightTangentPoint=p-rightTangentPointOfP;
            
            float dotProduct1=ccpDot(rightVec, lightToLeftTangentPoint);
            float dotProduct2=ccpDot(rightVec, lightToRightTangentPoint);
            
            if(dotProduct1<=0||dotProduct2<=0){//edge(p,pn) is all or partly facing light
                isBackLightEdgeList.push_back(false);
            }else{//edge(p,pn) is all back light
                isBackLightEdgeList.push_back(true);
            }
        }//got isBackLightEdgeList
        //find a edge which is facing the light
        int facingLightEdgeIndex=-1;
        for(int i=0;i<nPoint;i++){
            if(isBackLightEdgeList[i]==false){//facing light
                facingLightEdgeIndex=i;
                break;
            }
        }//got facingLightEdgeIndex
        if(facingLightEdgeIndex==-1)return;//if light is in this shape, facingLightEdgeIndex will be -1
        //start search from facingLightEdgeIndex, find the first back-light edge
        int firstBackLightEdgeIndex=-1;
        for(int ioffset=0;ioffset<nPoint;ioffset++){
            int index=facingLightEdgeIndex+ioffset+1;//start from facingLightEdgeIndex's next point
            index=index%nPoint;
            if(isBackLightEdgeList[index]==true){//back light
                firstBackLightEdgeIndex=index;
                break;
            }
        }//got firstBackLightEdgeIndex
        if(firstBackLightEdgeIndex==-1){//all edges are facing light edge
            int nPoint=(int)m_polygon.m_pointList.size();
            for(int i=0;i<nPoint;i++){
                CCPoint p=m_polygon.m_pointList[i];
                CCPoint pn=m_polygon.m_pointList[(i+1)%nPoint];
                Cedge edge;
                edge.m_start=p;
                edge.m_end=pn;
                m_facingLightEdgeList.push_back(edge);
            }
            return;
        }
        //make m_backLightEdgeList and m_facingLightEdgeList
        for(int ioffset=0;ioffset<nPoint;ioffset++){
            int index=firstBackLightEdgeIndex+ioffset;
            index=index%nPoint;
            CCPoint p=m_polygon.m_pointList[index];
            CCPoint pn=m_polygon.m_pointList[(index+1)%nPoint];
            Cedge edge;
            edge.m_start=p;
            edge.m_end=pn;
            if(isBackLightEdgeList[index]==true){//back light
                m_backLightEdgeList.push_back(edge);
            }else{//facing light
                m_facingLightEdgeList.push_back(edge);
            }
        }//got m_backLightEdgeList and m_facingLightEdgeList
        //calculate:
        // m_leftTangentOfLeftShadowPoint
        // m_leftTangentOfRightShadowPoint
        // m_rightTangentOfLeftShadowPoint
        // m_rightTangentOfRightShadowPoint
        if((int)m_backLightEdgeList.size()!=0)
        {
            CCPoint rightShadowPoint=m_backLightEdgeList[0].m_start;
            CCPoint leftShadowPoint=m_backLightEdgeList[(int)m_backLightEdgeList.size()-1].m_end;
            vector<CCPoint> leftAndRightTangentOfLeftShadowPoint=calculateTangentPointsOfPointToCircle(lightPosLocal, m_light->getR(), leftShadowPoint);
            if(leftAndRightTangentOfLeftShadowPoint.empty()==false){
                m_leftTangentPointOfLeftShadowPoint=leftAndRightTangentOfLeftShadowPoint[0];
                m_rightTangentPointOfLeftShadowPoint=leftAndRightTangentOfLeftShadowPoint[1];
            }else{
                m_leftTangentPointOfLeftShadowPoint=CCPoint(INFINITY,INFINITY);
                m_rightTangentPointOfLeftShadowPoint=CCPoint(INFINITY,INFINITY);
            }
            vector<CCPoint> leftAndRightTangentOfRightShadowPoint=calculateTangentPointsOfPointToCircle(lightPosLocal, m_light->getR(), rightShadowPoint);
            if(leftAndRightTangentOfRightShadowPoint.empty()==false){
                m_leftTangentPointOfRightShadowPoint=leftAndRightTangentOfRightShadowPoint[0];
                m_rightTangentPointOfRightShadowPoint=leftAndRightTangentOfRightShadowPoint[1];
            }else{
                m_leftTangentPointOfRightShadowPoint=CCPoint(INFINITY,INFINITY);
                m_rightTangentPointOfRightShadowPoint=CCPoint(INFINITY,INFINITY);
            }
            
        }else{
            assert(false);
        }
        //calculate:
        // m_isLeftUmbraCut
        // m_isRightUnbraCut
        // m_isLeftPenumbraCut
        // m_isRightPenumbraCut
        if((int)m_backLightEdgeList.size()!=0)
        {
            CCPoint rightShadowPoint=m_backLightEdgeList[0].m_start;
            CCPoint leftShadowPoint=m_backLightEdgeList[(int)m_backLightEdgeList.size()-1].m_end;
            
            if(isPointEqual(m_leftTangentPointOfLeftShadowPoint, CCPoint(INFINITY,INFINITY), 0)==false){
                //left penumbra
                CCPoint leftShadowPointToLeftTangentPoint=m_leftTangentPointOfLeftShadowPoint-leftShadowPoint;
                if((int)m_facingLightEdgeList.size()!=0){
                    Cedge nextEdgeOfLeftShadowPoint=m_facingLightEdgeList[0];
                    Cedge foeEdgeOfLeftShadowPoint=m_backLightEdgeList[(int)m_backLightEdgeList.size()-1];
                    CCPoint leftShadowPointToNext=nextEdgeOfLeftShadowPoint.m_end-nextEdgeOfLeftShadowPoint.m_start;
                    CCPoint leftShadowPointToFoe=foeEdgeOfLeftShadowPoint.m_start-foeEdgeOfLeftShadowPoint.m_end;
                    //see if leftShadowPointToLeftTangentPoint in the middle of leftShadowPointToNext and leftShadowPointToFoe
                    //namely see if cross(leftShadowPointToNext,leftShadowPointToLeftTangentPoint)>0 and cross(leftShadowPointToLeftTangentPoint,leftShadowPointToFoe)>0
                    if(ccpCross(leftShadowPointToNext,leftShadowPointToLeftTangentPoint)>0
                       &&ccpCross(leftShadowPointToLeftTangentPoint,leftShadowPointToFoe)>0){//left penumbra is cut by nextEdgeOfLeftShadowPoint
                        m_isLeftPenumbraCut=true;
                    }else{
                        m_isLeftPenumbraCut=false;
                    }
                }
                
                
            }
            if(isPointEqual(m_leftTangentPointOfRightShadowPoint, CCPoint(INFINITY,INFINITY), 0)==false){
                //right umbra
                CCPoint leftTangentPointToRightShadowPoint=rightShadowPoint-m_leftTangentPointOfRightShadowPoint;
                if((int)m_facingLightEdgeList.size()!=0){
                    Cedge nextEdgeOfRightShadowPoint=m_backLightEdgeList[0];
                    Cedge foeEdgeOfRightShadowPoint=m_facingLightEdgeList[(int)m_facingLightEdgeList.size()-1];
                    CCPoint rightShadowPointToNext=nextEdgeOfRightShadowPoint.m_end-nextEdgeOfRightShadowPoint.m_start;
                    CCPoint rightShadowPointToFoe=foeEdgeOfRightShadowPoint.m_start-foeEdgeOfRightShadowPoint.m_end;
                    //see if leftTangentPointToRightShadowPoint in the middle of rightShadowPointToNext and rightShadowPointToFoe
                    //namely see if cross(rightShadowPointToNext,leftTangentPointToRightShadowPoint)>0 and cross(leftTangentPointToRightShadowPoint,rightShadowPointToFoe)>0
                    if(ccpCross(rightShadowPointToNext,leftTangentPointToRightShadowPoint)>0
                       &&ccpCross(leftTangentPointToRightShadowPoint,rightShadowPointToFoe)>0){//right umbra is cut by nextEdgeOfRightShadowPoint
                        m_isRightUmbraCut=true;
                    }else{
                        m_isRightUmbraCut=false;
                    }
                }
            }
            
            if(isPointEqual(m_rightTangentPointOfLeftShadowPoint, CCPoint(INFINITY,INFINITY), 0)==false){
                //left umbra
                CCPoint rightTangentPointToLeftShadowPoint=leftShadowPoint-m_rightTangentPointOfLeftShadowPoint;
                if((int)m_facingLightEdgeList.size()!=0){
                    Cedge nextEdgeOfLeftShadowPoint=m_facingLightEdgeList[0];
                    Cedge foeEdgeOfLeftShadowPoint=m_backLightEdgeList[(int)m_backLightEdgeList.size()-1];
                    CCPoint leftShadowPointToNext=nextEdgeOfLeftShadowPoint.m_end-nextEdgeOfLeftShadowPoint.m_start;
                    CCPoint leftShadowPointToFoe=foeEdgeOfLeftShadowPoint.m_start-foeEdgeOfLeftShadowPoint.m_end;
                    //see if rightTangentPointToLeftShadowPoint in the middle of leftShadowPointToNext and leftShadowPointToFoe
                    //namely see if cross(leftShadowPointToNext,rightTangentPointToLeftShadowPoint)>0 and cross(rightTangentPointToLeftShadowPoint,leftShadowPointToFoe)>0
                    if(ccpCross(leftShadowPointToNext,rightTangentPointToLeftShadowPoint)>0
                       &&ccpCross(rightTangentPointToLeftShadowPoint,leftShadowPointToFoe)>0){//left umbra is cut by foeEdgeOfLeftShadowPoint
                        m_isLeftUmbraCut=true;
                    }else{
                        m_isLeftUmbraCut=false;
                    }
                }
            }
            
            if(isPointEqual(m_rightTangentPointOfRightShadowPoint, CCPoint(INFINITY,INFINITY), 0)==false){
                //right penumbra
                CCPoint rightShadowPointToRightTangentPoint=m_rightTangentPointOfRightShadowPoint-rightShadowPoint;
                if((int)m_facingLightEdgeList.size()!=0){
                    Cedge nextEdgeOfRightShadowPoint=m_backLightEdgeList[0];
                    Cedge foeEdgeOfRightShadowPoint=m_facingLightEdgeList[(int)m_facingLightEdgeList.size()-1];
                    CCPoint rightShadowPointToNext=nextEdgeOfRightShadowPoint.m_end-nextEdgeOfRightShadowPoint.m_start;
                    CCPoint rightShadowPointToFoe=foeEdgeOfRightShadowPoint.m_start-foeEdgeOfRightShadowPoint.m_end;
                    //see if rightShadowPointToRightTangentPoint in the middle of rightShadowPointToNext and rightShadowPointToFoe
                    //namely see if cross(rightShadowPointToNext,rightShadowPointToRightTangentPoint)>0 and cross(rightShadowPointToRightTangentPoint,rightShadowPointToFoe)>0
                    if(ccpCross(rightShadowPointToNext,rightShadowPointToRightTangentPoint)>0
                       &&ccpCross(rightShadowPointToRightTangentPoint,rightShadowPointToFoe)>0){//right penumbra is cut by foeEdgeOfRightShadowPoint
                        m_isRightPenumbraCut=true;
                    }else{
                        m_isRightPenumbraCut=false;
                    }
                }
            }
          //  cout<<"isXXXXCut: "<<m_isLeftPenumbraCut<<" "<<m_isRightUmbraCut<<" "<<m_isLeftUmbraCut<<" "<<m_isRightPenumbraCut<<endl;
            
        }
        //calculate:
        // m_leftUmbraLine;
        // m_rightUmbraLine;
        // m_leftPenumbraLine;
        // m_rightPenumbraLine;
        {
            // right umbra
            if((int)m_backLightEdgeList.size()!=0)
            {
                if(m_isRightUmbraCut){
                    if((int)m_facingLightEdgeList.size()!=0){
                        Cedge nextEdgeOfRightShadowPoint=m_backLightEdgeList[0];
                        m_rightUmbraLine.m_start=nextEdgeOfRightShadowPoint.m_start;
                        m_rightUmbraLine.m_end=nextEdgeOfRightShadowPoint.m_start+ccpMult(ccpNormalize(nextEdgeOfRightShadowPoint.m_end-nextEdgeOfRightShadowPoint.m_start),m_shadowLength);
                    }
                    
                }else{
                    CCPoint rightShadowPoint=m_backLightEdgeList[0].m_start;
                    m_rightUmbraLine.m_start=rightShadowPoint;
                    m_rightUmbraLine.m_end=rightShadowPoint+ccpMult(ccpNormalize(rightShadowPoint-m_leftTangentPointOfRightShadowPoint), m_shadowLength);
                }
                
            }
            // right penumbra
            if((int)m_backLightEdgeList.size()!=0)
            {
                if(m_isRightPenumbraCut){
                    if((int)m_facingLightEdgeList.size()!=0){
                        Cedge foeEdgeOfRightShadowPoint=m_facingLightEdgeList[(int)m_facingLightEdgeList.size()-1];
                        m_rightPenumbraLine.m_start=foeEdgeOfRightShadowPoint.m_end;
                        m_rightPenumbraLine.m_end=foeEdgeOfRightShadowPoint.m_end+ccpMult(ccpNormalize(foeEdgeOfRightShadowPoint.m_end-foeEdgeOfRightShadowPoint.m_start), m_shadowLength);
                    }
                }else{
                    CCPoint rightShadowPoint=m_backLightEdgeList[0].m_start;
                    m_rightPenumbraLine.m_start=rightShadowPoint;
                    m_rightPenumbraLine.m_end=rightShadowPoint+ccpMult(ccpNormalize(rightShadowPoint-m_rightTangentPointOfRightShadowPoint), m_shadowLength);
                
                }
            }
            // left umbra
            if((int)m_backLightEdgeList.size()!=0)
            {
                if(m_isLeftUmbraCut){
                    if((int)m_facingLightEdgeList.size()!=0){
                        Cedge foeEdgeOfLeftShadowPoint=m_backLightEdgeList[(int)m_backLightEdgeList.size()-1];
                        m_leftUmbraLine.m_start=foeEdgeOfLeftShadowPoint.m_end;
                        m_leftUmbraLine.m_end=foeEdgeOfLeftShadowPoint.m_end+ccpMult(ccpNormalize(foeEdgeOfLeftShadowPoint.m_start-foeEdgeOfLeftShadowPoint.m_end), m_shadowLength);
                        
                    }
                }else{
                    CCPoint leftShadowPoint=m_backLightEdgeList[(int)m_backLightEdgeList.size()-1].m_end;
                    m_leftUmbraLine.m_start=leftShadowPoint;
                    m_leftUmbraLine.m_end=leftShadowPoint+ccpMult(ccpNormalize(leftShadowPoint-m_rightTangentPointOfLeftShadowPoint), m_shadowLength);
                }
                
            }
            // left penumbra
            if((int)m_backLightEdgeList.size()!=0)
            {
                if(m_isLeftPenumbraCut){
                    if((int)m_facingLightEdgeList.size()!=0){
                        Cedge nextEdgeOfLeftShadowPoint=m_facingLightEdgeList[0];
                        m_leftPenumbraLine.m_start=nextEdgeOfLeftShadowPoint.m_start;
                        m_leftPenumbraLine.m_end=nextEdgeOfLeftShadowPoint.m_start+ccpMult(ccpNormalize(nextEdgeOfLeftShadowPoint.m_start-nextEdgeOfLeftShadowPoint.m_end), m_shadowLength);
                    }
                }else{
                    CCPoint leftShadowPoint=m_backLightEdgeList[(int)m_backLightEdgeList.size()-1].m_end;
                    m_leftPenumbraLine.m_start=leftShadowPoint;
                    m_leftPenumbraLine.m_end=leftShadowPoint+ccpMult(ccpNormalize(leftShadowPoint-m_leftTangentPointOfLeftShadowPoint), m_shadowLength);
                    
                }
                
            }

        
        }
        
    
    
    }
    void draw(){
        //----draw mesh
        {
            //----change shader
            ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
            ccGLEnable(m_eGLServerState);
            //pass values for cocos2d-x build-in uniforms
            CCGLProgram*program=this->getShaderProgram();
            program->use();
            program->setUniformsForBuiltins();
            //pass values for my own uniforms
            //...
            //enable attributes
            bool isAttribPositionOn=CindexVBO::isEnabledAttribArray_position();
            bool isAttribColorOn=CindexVBO::isEnabledAttribArray_color();
            bool isAttribTexCoordOn=CindexVBO::isEnabledAttribArray_texCoord();
            CindexVBO::enableAttribArray_position(true);
            CindexVBO::enableAttribArray_color(true);
            CindexVBO::enableAttribArray_texCoord(true);
            //bind texture
            ccGLBindTexture2D( m_finTexture->getName());
            //draw m_indexVBO
            m_indexVBO->setPointer_position();
            m_indexVBO->setPointer_texCoord();
            m_indexVBO->setPointer_color();
            m_indexVBO->draw(GL_TRIANGLES);
            //unbind texture
            ccGLBindTexture2D(0);
            //disable attributes
            CindexVBO::enableAttribArray_position(isAttribPositionOn);
            CindexVBO::enableAttribArray_color(isAttribColorOn);
            CindexVBO::enableAttribArray_texCoord(isAttribTexCoordOn);
            

        }
        //----draw wire
        //draw shape
        glLineWidth(1);
        ccDrawColor4F(1, 1, 1, 1);
        ccDrawPoly(&m_polygon.m_pointList.front(), (int)m_polygon.m_pointList.size(), true);
        //light pos local
        ccPointSize(4);
        ccDrawColor4F(0, 0, 1, 1);
        ccDrawPoint(getLightPosLocal());
        //draw m_backLightEdgeList
        glLineWidth(4);
        ccDrawColor4F(0, 0, 1, 1);
        int nBackLightEdge=(int)m_backLightEdgeList.size();
        for(int i=0;i<nBackLightEdge;i++){
            const Cedge&edge=m_backLightEdgeList[i];
            ccDrawLine(edge.m_start, edge.m_end);
        }
        //draw m_facingLightEdgeList
        glLineWidth(4);
        ccDrawColor4F(1, 0, 0, 1);
        int nFacingLightEdge=(int)m_facingLightEdgeList.size();
        for(int i=0;i<nFacingLightEdge;i++){
            const Cedge&edge=m_facingLightEdgeList[i];
            ccDrawLine(edge.m_start, edge.m_end);
        }

        //draw shadow line
        {
            
            ccDrawColor4F(0, 0, 1, 1);
            CCPoint lightPosLocal=getLightPosLocal();
            int nBackLightEdge=(int)m_backLightEdgeList.size();
            if(nBackLightEdge!=0){
                //light to shadow point
                glLineWidth(1);
                CCPoint rightShadowPoint=m_backLightEdgeList[0].m_start;
                CCPoint leftShadowPoint=m_backLightEdgeList[(int)m_backLightEdgeList.size()-1].m_end;
                ccDrawLine(lightPosLocal, rightShadowPoint);
                ccDrawLine(lightPosLocal, leftShadowPoint);
                //shadow point to infinite
                glLineWidth(1);
                CCPoint lightToRightShadowPoint=rightShadowPoint-lightPosLocal;
                CCPoint lightToLeftShadowPoint=leftShadowPoint-lightPosLocal;
                CCPoint shadowRightEnd=rightShadowPoint+ccpMult(lightToRightShadowPoint, 10000);
                CCPoint shadowLeftEnd=leftShadowPoint+ccpMult(lightToLeftShadowPoint, 10000);
                ccDrawLine(rightShadowPoint, shadowRightEnd);
                ccDrawLine(leftShadowPoint, shadowLeftEnd);
                
            }
            
            
        }
        //draw right umbra
        if((int)m_backLightEdgeList.size()!=0)
        {
            
            glLineWidth(1);
            ccDrawColor4F(0, 0, 1, 1);
            CCPoint rightShadowPoint=m_backLightEdgeList[0].m_start;
            ccDrawLine(rightShadowPoint, m_leftTangentPointOfRightShadowPoint);
            ccDrawLine(rightShadowPoint, rightShadowPoint+ccpMult(rightShadowPoint-m_leftTangentPointOfRightShadowPoint, m_shadowLength));
            
            glLineWidth(1);
            ccDrawColor4F(0, 1, 0, 1);
            ccDrawLine(m_rightUmbraLine.m_start, m_rightUmbraLine.m_end);
            
        }
        //draw right penumbra
        if((int)m_backLightEdgeList.size()!=0)
        {
            glLineWidth(1);
            ccDrawColor4F(0, 0, 1, 1);
            CCPoint rightShadowPoint=m_backLightEdgeList[0].m_start;
            ccDrawLine(rightShadowPoint, m_rightTangentPointOfRightShadowPoint);
            ccDrawLine(rightShadowPoint, rightShadowPoint+ccpMult(rightShadowPoint-m_rightTangentPointOfRightShadowPoint, m_shadowLength));
            
            glLineWidth(1);
            ccDrawColor4F(0, 1, 0, 1);
            ccDrawLine(m_rightPenumbraLine.m_start, m_rightPenumbraLine.m_end);
            
        }
        //draw left umbra
        if((int)m_backLightEdgeList.size()!=0)
        {
            glLineWidth(1);
            ccDrawColor4F(0, 0, 1, 1);
            CCPoint leftShadowPoint=m_backLightEdgeList[(int)m_backLightEdgeList.size()-1].m_end;
            ccDrawLine(leftShadowPoint, m_rightTangentPointOfLeftShadowPoint);
            ccDrawLine(leftShadowPoint, leftShadowPoint+ccpMult(leftShadowPoint-m_rightTangentPointOfLeftShadowPoint, m_shadowLength));
            
            glLineWidth(1);
            ccDrawColor4F(0, 1, 0, 1);
            ccDrawLine(m_leftUmbraLine.m_start, m_leftUmbraLine.m_end);
            
        }
        //draw left penumbra
        if((int)m_backLightEdgeList.size()!=0)
        {
            glLineWidth(1);
            ccDrawColor4F(0, 0, 1, 1);
            CCPoint leftShadowPoint=m_backLightEdgeList[(int)m_backLightEdgeList.size()-1].m_end;
            ccDrawLine(leftShadowPoint, m_leftTangentPointOfLeftShadowPoint);
            ccDrawLine(leftShadowPoint, leftShadowPoint+ccpMult(leftShadowPoint-m_leftTangentPointOfLeftShadowPoint, m_shadowLength));
            
            glLineWidth(1);
            ccDrawColor4F(0, 1, 0, 1);
            ccDrawLine(m_leftPenumbraLine.m_start, m_leftPenumbraLine.m_end);
            
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
    void updateMesh(){
        m_mesh->clear();
        //left penumbra area
        //v0
        Cv2 pos0=ccpTov2(m_leftPenumbraLine.m_start);
        Cv2 texCoord0=Cv2(0,1);
        Cv4 color0=Cv4(1,1,1,1);
        //v1
        Cv2 pos1=ccpTov2(m_leftPenumbraLine.m_end);
        Cv2 texCoord1=Cv2(0,0);
        Cv4 color1=Cv4(1,1,1,1);
        //v2
        Cv2 pos2=ccpTov2(m_leftUmbraLine.m_end);
        Cv2 texCoord2=Cv2(1,0);
        Cv4 color2=Cv4(1,1,1,1);
        //add v0
        m_mesh->vlist.push_back(pos0);
        m_mesh->texCoordList.push_back(texCoord0);
        m_mesh->colorList.push_back(color0);
        int ID0=(int)m_mesh->vlist.size()-1;
        //add v1
        m_mesh->vlist.push_back(pos1);
        m_mesh->texCoordList.push_back(texCoord1);
        m_mesh->colorList.push_back(color1);
        int ID1=(int)m_mesh->vlist.size()-1;
        //add v2
        m_mesh->vlist.push_back(pos2);
        m_mesh->texCoordList.push_back(texCoord2);
        m_mesh->colorList.push_back(color2);
        int ID2=(int)m_mesh->vlist.size()-1;
        //add IDtri
        m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID1,ID2));
        
        
        
        
        
        
        //calculate intersect point of left umbra line and right umbra line
        
    
    }
    void submit(GLenum usage){
        m_indexVBO->submitPos(m_mesh->vlist, usage);
        m_indexVBO->submitTexCoord(m_mesh->texCoordList, usage);
        m_indexVBO->submitColor(m_mesh->colorList, usage);
        m_indexVBO->submitIndex(m_mesh->IDtriList, usage);
    }
    
protected:
    Cpolygon m_polygon;
    ClightNode *m_light;
    vector<Cedge> m_backLightEdgeList;
    vector<Cedge> m_facingLightEdgeList;
    CCPoint m_leftTangentPointOfLeftShadowPoint;
    CCPoint m_leftTangentPointOfRightShadowPoint;
    CCPoint m_rightTangentPointOfLeftShadowPoint;
    CCPoint m_rightTangentPointOfRightShadowPoint;
    bool m_isLeftUmbraCut;
    bool m_isRightUmbraCut;
    bool m_isLeftPenumbraCut;
    bool m_isRightPenumbraCut;
    Cedge m_leftUmbraLine;
    Cedge m_rightUmbraLine;
    Cedge m_leftPenumbraLine;
    Cedge m_rightPenumbraLine;
    float m_shadowLength;
    CCTexture2D*m_finTexture;
    Cmesh*m_mesh;
    CindexVBO*m_indexVBO;
};


*/

namespace_ens_end
#endif
