//
//  ensTinyWingsTerrainSprite.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 13-12-16.
//
//

#ifndef __HelloCpp__ensTinyWingsTerrainNode__
#define __HelloCpp__ensTinyWingsTerrainNode__

#include <iostream>
#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "ensCommon.h"
namespace_ens_begin
//ref to: http://www.cnblogs.com/andyque/archive/2011/07/01/2095479.html
//and https://github.com/diwu/Tiny-Wings-Remake-on-Android
class CtinyWingsTerrainSprite:public CCSprite{
public:
    CtinyWingsTerrainSprite(){
        m_dx=10;
        m_mesh=NULL;
        m_indexVBO=NULL;
        m_isDrawDebug=false;
        m_texture=NULL;
        m_program=NULL;
        m_texNonlinearFactor=0.7;
        m_ribbonRotation=75.0;
        m_ribbonRepeat=7.5;
        m_width=0;
        m_height=0;
        m_nRow=4;
        m_insertControlPointCount=4;
    }
    virtual~CtinyWingsTerrainSprite(){
        if(m_mesh)m_mesh->release();
        if(m_indexVBO)m_indexVBO->release();
        if(m_texture)m_texture->release();
        if(m_program)m_program->release();
    }
    void init(float width,float height,int insertControlPointCount);
    void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
    void onDraw(const Mat4 &transform, uint32_t flags);
    void onDrawDebug(const Mat4 &transform, uint32_t flags);
    void drawWire();
    void reGenerateShape(int insertControlPointCount);
    void reGenerateColors();
    
    bool getIsDrawDebug()const {return m_isDrawDebug;}
    void setIsDrawDebug(bool value){m_isDrawDebug=value;}
    void setTexNonlinearFactor(float value){m_texNonlinearFactor=value;}
    float getTexNonlinearFactor()const{return m_texNonlinearFactor;}
    void setRibbonRotation(float value){m_ribbonRotation=value;}
    float getRibbonRotation()const{return m_ribbonRotation;}
    void setRibbonRepeat(float value){m_ribbonRepeat=value;}
    float getRibbonRepeat()const{return m_ribbonRepeat;}
    float getWidth()const{return m_width;}
    float getHeight()const{return m_height;}
    int getInsertControlPointCount()const{return m_insertControlPointCount;}
protected:
    void submit();
    void updateMesh();
protected:
    CCTexture2D*m_texture;
    vector<vector<CCPoint> > m_pointMat;//in local space
    float m_dx;
    int m_nRow;//must >=2
    Cmesh*m_mesh;
    CindexVBO*m_indexVBO;
    bool m_isDrawDebug;
    CGLProgramWithUnifos*m_program;
    float m_texNonlinearFactor;
    vector<ccColor4F> m_colorList;
    float m_ribbonRotation;
    float m_ribbonRepeat;
    float m_width;
    float m_height;
    int m_insertControlPointCount;
    
    CustomCommand _customCommand;
    CustomCommand _customCommand_debug;
};

namespace_ens_end
#endif /* defined(__HelloCpp__ensTinyWingsTerrainNode__) */
