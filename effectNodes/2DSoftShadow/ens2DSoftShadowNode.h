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
        ccDrawColor4F(1, 1, 1, 1);
        ccDrawCircle(CCPoint(0,0), m_r, 360, 30, false, 1, 1);
    }
protected:
    float m_r;
};
class CPointType{
public:
    bool m_isLeftUmbraPoint;
    bool m_isRightUmbraPoint;
    bool m_isLeftPenumbraPoint;
    bool m_isRightPenumbraPoint;
    CPointType(){
         m_isLeftUmbraPoint=false;
         m_isRightUmbraPoint=false;
         m_isLeftPenumbraPoint=false;
         m_isRightPenumbraPoint=false;
    }
};

enum eSideType{
    eLeftSide=0,
    eRightSide,
    eOn,
};
class C2DSoftShadowNode:public CCSprite
{
public:
    C2DSoftShadowNode(){
        m_light=NULL;
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
        CCPoint lightPosLocal=getLightPosLocal();
        m_pointTypeList.clear();
        int nPoint=(int)m_polygon.m_pointList.size();
        m_pointTypeList.resize(nPoint);
        for(int i=0;i<nPoint;i++){
            const CCPoint&P=m_polygon.m_pointList[i];
            CPointType&pointType=m_pointTypeList[i];
            vector<CCPoint> LRT=calculateTangentPointsOfPointToCircle(lightPosLocal, m_light->getR(), P);
            if(LRT.empty())return;//P in light circle, return
            CCPoint LT=LRT[0];
            CCPoint RT=LRT[1];
            CCPoint PLT=LT-P;
            CCPoint PRT=RT-P;
            //for each point, determine it is on which side of PLT and PRT
            vector<eSideType> sideTypeList_PLT;
            vector<eSideType> sideTypeList_PRT;
            for(int j=0;j<nPoint;j++){
                const CCPoint&p=m_polygon.m_pointList[j];
                CCPoint Pp=p-P;
                if(j==i){
                    sideTypeList_PLT.push_back(eOn);
                    sideTypeList_PRT.push_back(eOn);
                }else{
                    //sideTypeList_PLT
                    {
                        float cross=ccpCross(PLT, Pp);
                        if(cross==0){
                            sideTypeList_PLT.push_back(eOn);
                        }else if(cross>0){
                            sideTypeList_PLT.push_back(eLeftSide);
                        }else{
                            sideTypeList_PLT.push_back(eRightSide);
                        }
                    }
                    //sideTypeList_PRT
                    {
                        float cross=ccpCross(PRT, Pp);
                        if(cross==0){
                            sideTypeList_PRT.push_back(eOn);
                        }else if(cross>0){
                            sideTypeList_PRT.push_back(eLeftSide);
                        }else{
                            sideTypeList_PRT.push_back(eRightSide);
                        }
                        
                    }
                    
                }
            }//got sideTypeList_PLT and sideTypeList_PRT
            
            
            //see whether P is left umbra point
            //namely see whether all points are on the left side of (or on) PRT
            {
                bool allOnLeftOrOn=true;
                for(int j=0;j<nPoint;j++){
                    eSideType sideType=sideTypeList_PRT[j];
                    if(sideType==eLeftSide||sideType==eOn){
                    }else{
                        allOnLeftOrOn=false;
                        break;
                    }
                }//got allOnLeftOrOn
                if(allOnLeftOrOn)pointType.m_isLeftUmbraPoint=true;
            }
            //see whether P is right umbra point
            //namely see whether all points are on the right side of (or on) PLT
            {
                bool allOnRightOrOn=true;
                for(int j=0;j<nPoint;j++){
                    eSideType sideType=sideTypeList_PLT[j];
                    if(sideType==eRightSide||sideType==eOn){
                    }else{
                        allOnRightOrOn=false;
                        break;
                    }
                }//got allOnRightOrOn
                if(allOnRightOrOn)pointType.m_isRightUmbraPoint=true;
            }
            //see whether P is left penumbra point
            //namely see whether all points are on the left side of (or on) PLT
            {
                bool allOnLeftOrOn=true;
                for(int j=0;j<nPoint;j++){
                    eSideType sideType=sideTypeList_PLT[j];
                    if(sideType==eLeftSide||sideType==eOn){
                    }else{
                        allOnLeftOrOn=false;
                        break;
                    }
                }//got allOnLeftOrOn
                if(allOnLeftOrOn)pointType.m_isLeftPenumbraPoint=true;
            }
            //see whether P is right penumbra point
            //namely see whether all points are on the right side of (or on) PRT
            {
                bool allOnRightOrOn=true;
                for(int j=0;j<nPoint;j++){
                    eSideType sideType=sideTypeList_PRT[j];
                    if(sideType==eRightSide||sideType==eOn){
                    }else{
                        allOnRightOrOn=false;
                        break;
                    }
                }//got allOnRightOrOn
                if(allOnRightOrOn)pointType.m_isRightPenumbraPoint=true;
            }

        }
        
        
        
    
    
    }
    void draw(){
     /*   //----draw mesh
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
            

        }*/
        //----draw wire
        glLineWidth(1);
        ccDrawColor4F(1, 1, 1, 1);
        ccDrawPoly(&m_polygon.m_pointList.front(), (int)m_polygon.m_pointList.size(), true);
        //
        //light pos local
        ccPointSize(4);
        ccDrawColor4F(1, 1, 1, 1);
        ccDrawPoint(getLightPosLocal());

        //
        int nPointType=(int)m_pointTypeList.size();
        ccPointSize(5);
        
        for(int i=0;i<nPointType;i++){
            const CCPoint&P=m_polygon.m_pointList[i];
            CPointType pointType=m_pointTypeList[i];
            if(pointType.m_isLeftPenumbraPoint){
                ccDrawColor4F(1, 0, 0, 1);
                glLineWidth(1);
                ccDrawCircle(P, 5, 360, 10, false, 1, 1);
            }
            if(pointType.m_isRightPenumbraPoint){
                ccDrawColor4F(0, 1, 0, 1);
                glLineWidth(1);
                ccDrawCircle(P, 5, 360, 10, false, 1, 1);
            }
            if(pointType.m_isLeftUmbraPoint){
                ccDrawColor4F(1, 0, 0, 1);
                glLineWidth(5);
                ccDrawPoint(P);
            }
            if(pointType.m_isRightUmbraPoint){
                ccDrawColor4F(0, 1, 0, 1);
                glLineWidth(5);
                ccDrawPoint(P);
            }
            
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
     /*   m_mesh->clear();
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
        
        
        */
        
        
        
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
    vector<CPointType> m_pointTypeList;
    ClightNode *m_light;
    float m_shadowLength;
    CCTexture2D*m_finTexture;
    Cmesh*m_mesh;
    CindexVBO*m_indexVBO;
};




namespace_ens_end
#endif
