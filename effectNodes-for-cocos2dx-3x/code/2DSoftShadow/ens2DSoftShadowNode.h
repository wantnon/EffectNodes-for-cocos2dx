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
#include "ensCommon.h"

namespace_ens_begin
//this effect was inspired by:
//http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/dynamic-2d-soft-shadows-r2032

class Cedge{
public:
    CCPoint m_start;
    CCPoint m_end;
    Cedge(){}
    Cedge(const CCPoint&start,const CCPoint&end){
        m_start=start;
        m_end=end;
    }
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
    void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags){
        if(m_isDrawDebug){
            
            _customCommand_debug.init(_globalZOrder);
            _customCommand_debug.func = CC_CALLBACK_0(ClightNode::onDrawDebug,this,transform, flags);
            renderer->addCommand(&_customCommand_debug);
        }
        
    }
    void onDrawDebug(const Mat4 &transform, uint32_t flags){
        const Mat4&m=transform;
            glLineWidth(2);
            ccDrawColor4F(0, 0, 1, 1);
            ccDrawCircle(M4TransV2(m,CCPoint(0,0)), m_r, 360, 30, false, 1, 1);
        ccDrawColor4F(1, 1, 1, 1);
    }
    void setIsDrawDebug(bool value){m_isDrawDebug=value;}
    bool getIsDrawDebug()const{return m_isDrawDebug;}
protected:
    float m_r;
    bool m_isDrawDebug;
    
    cocos2d::CustomCommand _customCommand_debug;
};
class CPointType{
public:
    bool m_isLeftUmbraPoint;
    CCPoint m_leftUmbraT;
    bool m_isRightUmbraPoint;
    CCPoint m_rightUmbraT;
    bool m_isLeftPenumbraPoint;
    CCPoint m_leftPenumbraT;
    bool m_isRightPenumbraPoint;
    CCPoint m_rightPenumbraT;
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
class C2DSoftShadowObj:public CCSprite
{
public:
    C2DSoftShadowObj(){
        m_light=NULL;
        m_mesh=NULL;
        m_finTexture=NULL;
        m_indexVBO=NULL;
        m_program=NULL;
        m_shadowLength=1500;
        m_leftPenumbraLine.resize(3);
        m_rightPenumbraLine.resize(3);
        m_leftUmbraLine.resize(3);
        m_rightUmbraLine.resize(3);
        m_leftPenumbraPointID=-1;
        m_rightPenumbraPointID=-1;
        m_leftUmbraPointID=-1;
        m_rightUmbraPointID=-1;
        m_oneDivObjCount=1.0;
        m_isDrawDebug=false;
        m_isDrawNonDebug=true;
        m_isUpdateShadowSucc=false;
    }
    virtual~C2DSoftShadowObj(){
        if(m_mesh)m_mesh->release();
        if(m_finTexture)m_finTexture->release();
        if(m_indexVBO)m_indexVBO->release();
        if(m_program)m_program->release();
    }
    bool init(const Cpolygon&polygon);
    void update(float dt);
    bool updateShadow();
    void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
    void onDraw(const Mat4 &transform, uint32_t flags);
    void onDrawDebug(const Mat4 &transform, uint32_t flags);
    void setLight(ClightNode*light);
    void setOneDivObjCount(float value){m_oneDivObjCount=value;}
    float getOneDivObjCount()const{return m_oneDivObjCount;}
    void setIsDrawDebug(bool value){m_isDrawDebug=value;}
    bool getIsDrawDebug()const{return m_isDrawDebug;}
    void setIsDrawNonDebug(bool value){m_isDrawNonDebug=value;}
    bool getIsDrawNonDebug()const{return m_isDrawNonDebug;}
protected:
    CCPoint getLightPosLocal();
    void makeFullWindowRectMesh();
    void updateMesh();
    void submit(GLenum usage);
protected:
    Cpolygon m_polygon;
    vector<CPointType> m_pointTypeList;
    vector<CCPoint> m_leftPenumbraLine;//m_leftPenumbraLine have three elements, [0] is start point, [1] is mid point, [2] is end point
    vector<CCPoint> m_rightPenumbraLine;
    vector<CCPoint> m_leftUmbraLine;
    vector<CCPoint> m_rightUmbraLine;
    int m_leftPenumbraPointID;
    int m_rightPenumbraPointID;
    int m_leftUmbraPointID;
    int m_rightUmbraPointID;
    CCPoint m_intersectPoint;
    ClightNode *m_light;
    float m_shadowLength;
    CCTexture2D*m_finTexture;
    Cmesh*m_mesh;
    CindexVBO*m_indexVBO;
    CGLProgramWithUnifos*m_program;
    float m_oneDivObjCount;
    bool m_isDrawDebug;
    bool m_isDrawNonDebug;
    bool m_isUpdateShadowSucc;

    
    cocos2d::CustomCommand _customCommand;
    cocos2d::CustomCommand _customCommand_debug;
};


class C2DSoftShadowRoot:public Node
{
public:
    C2DSoftShadowRoot(){
        m_shadowRT=NULL;
        m_program=NULL;
        m_light=NULL;
        m_isDrawDebug=false;
        m_shadowDarkness=0.5;
        
        m_tempSprite=NULL;
    }
    virtual~C2DSoftShadowRoot(){
        if(m_shadowRT)m_shadowRT->release();
        if(m_program)m_program->release();
    }
    bool init();
    void setLight(ClightNode*light);
    void addObj(C2DSoftShadowObj*obj);
    virtual void visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags);
    void setIsDrawDebug(bool value){m_isDrawDebug=value;}
    bool getIsDrawDebug()const{return m_isDrawDebug;}
    void setShadowDarkness(float value){m_shadowDarkness=value;}
    float getShadowDarkness()const{return m_shadowDarkness;}
    CCRenderTexture* getShadowRT(){return m_shadowRT;}
protected:
    CCRenderTexture* m_shadowRT;
    CGLProgramWithUnifos*m_program;
    vector<C2DSoftShadowObj*> m_objList;
    ClightNode*m_light;
    bool m_isDrawDebug;
    float m_shadowDarkness;
    
    CCSprite* m_tempSprite;
    

    
};
namespace_ens_end
#endif
