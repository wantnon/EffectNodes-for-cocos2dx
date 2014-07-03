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
    Cedge(){
    
    }
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
class C2DSoftShadowNode:public CCSprite
{
public:
    C2DSoftShadowNode(){
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
    }
    virtual~C2DSoftShadowNode(){
        if(m_mesh)m_mesh->release();
        if(m_finTexture)m_finTexture->release();
        if(m_indexVBO)m_indexVBO->release();
        if(m_program)m_program->release();

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
        //----create and set shader program
        {
            GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/2DSoftShadow.fsh").c_str())->getCString();
            CGLProgramWithUnifos* program = new CGLProgramWithUnifos();
            program->autorelease();
            program->initWithVertexShaderByteArray(ccPositionTextureColor_vert, fragSource);
            //bind attribute
            program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
            program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
            program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
            //link  (must after bindAttribute)
            program->link();
            //get cocos2d-x build-in uniforms
            program->updateUniforms();
            //get my own uniforms
            //...
            //set program
            m_program=program;
            m_program->retain();
            //check gl error
            CHECK_GL_ERROR_DEBUG();
        }


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
                if(allOnLeftOrOn){
                    pointType.m_isLeftUmbraPoint=true;
                    pointType.m_leftUmbraT=RT;
                }
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
                if(allOnRightOrOn){
                    pointType.m_isRightUmbraPoint=true;
                    pointType.m_rightUmbraT=LT;
                }
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
                if(allOnLeftOrOn){
                    pointType.m_isLeftPenumbraPoint=true;
                    pointType.m_leftPenumbraT=LT;
                }
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
                if(allOnRightOrOn){
                    pointType.m_isRightPenumbraPoint=true;
                    pointType.m_rightPenumbraT=RT;
                }
            }

        }//got m_pointTypeList
        //let there is only one left umbra point, only one right umbra point, only one left penumbra point, only one right penumbra point
        int nPointType=(int)m_pointTypeList.size();
        for(int i=0;i<nPointType;i++){
            CPointType&pointType=m_pointTypeList[i];
            CPointType&pointTypen=m_pointTypeList[(i+1)%nPointType];
            if(pointType.m_isLeftPenumbraPoint&&pointTypen.m_isLeftPenumbraPoint){
                pointType.m_isLeftPenumbraPoint=false;
            }
            if(pointType.m_isRightPenumbraPoint&&pointTypen.m_isRightPenumbraPoint){
                pointType.m_isRightPenumbraPoint=false;
            }
            if(pointType.m_isLeftUmbraPoint&&pointTypen.m_isLeftUmbraPoint){
                pointType.m_isLeftUmbraPoint=false;
            }
            if(pointType.m_isRightUmbraPoint&&pointTypen.m_isRightUmbraPoint){
                pointType.m_isRightUmbraPoint=false;
            }
        }
        //make
        //m_leftPenumbraLine
        //m_rightPenumbraLine
        //m_leftUmbraLine
        //m_rightUmbraLine
        //and
        //m_leftPenumbraPointID
        //m_rightPenumbraPointID
        //m_leftUmbraPointID
        //m_rightUmbraPointID
        for(int i=0;i<nPointType;i++){
            const CCPoint&point=m_polygon.m_pointList[i];
            const CPointType&pointType=m_pointTypeList[i];
            if(pointType.m_isLeftPenumbraPoint){
                m_leftPenumbraLine[1]=point;
                m_leftPenumbraLine[0]=pointType.m_leftPenumbraT;
                m_leftPenumbraLine[2]=m_leftPenumbraLine[1]+ccpMult(ccpNormalize(m_leftPenumbraLine[1]-m_leftPenumbraLine[0]), m_shadowLength);
                m_leftPenumbraPointID=i;
            }
            if(pointType.m_isRightPenumbraPoint){
                m_rightPenumbraLine[1]=point;
                m_rightPenumbraLine[0]=pointType.m_rightPenumbraT;
                m_rightPenumbraLine[2]=m_rightPenumbraLine[1]+ccpMult(ccpNormalize(m_rightPenumbraLine[1]-m_rightPenumbraLine[0]), m_shadowLength);
                m_rightPenumbraPointID=i;
            }
            if(pointType.m_isLeftUmbraPoint){
                m_leftUmbraLine[1]=point;
                m_leftUmbraLine[0]=pointType.m_leftUmbraT;
                m_leftUmbraLine[2]=m_leftUmbraLine[1]+ccpMult(ccpNormalize(m_leftUmbraLine[1]-m_leftUmbraLine[0]), m_shadowLength);
                m_leftUmbraPointID=i;
            }
            if(pointType.m_isRightUmbraPoint){
                m_rightUmbraLine[1]=point;
                m_rightUmbraLine[0]=pointType.m_rightUmbraT;
                m_rightUmbraLine[2]=m_rightUmbraLine[1]+ccpMult(ccpNormalize(m_rightUmbraLine[1]-m_rightUmbraLine[0]), m_shadowLength);
                m_rightUmbraPointID=i;
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
            this->setShaderProgram(m_program);
            CGLProgramWithUnifos*program=(CGLProgramWithUnifos*)this->getShaderProgram();
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
        glLineWidth(1);
        ccDrawColor4F(1, 1, 1, 1);
        ccDrawPoly(&m_polygon.m_pointList.front(), (int)m_polygon.m_pointList.size(), true);
        //
        //light pos local
        ccPointSize(4);
        ccDrawColor4F(1, 1, 1, 1);
        ccDrawPoint(getLightPosLocal());


        //m_leftPenumbraLine
        {
            ccDrawColor4F(1, 0, 0, 1);
            glLineWidth(1);
            ccDrawLine(m_leftPenumbraLine[0], m_leftPenumbraLine[2]);
            ccDrawCircle(m_leftPenumbraLine[1], 5, 360, 10, false, 1, 1);
        }
        //m_rightPenumbraLine
        {
            ccDrawColor4F(0, 1, 0, 1);
            glLineWidth(1);
            ccDrawLine(m_rightPenumbraLine[0], m_rightPenumbraLine[2]);
            ccDrawCircle(m_rightPenumbraLine[1], 5, 360, 10, false, 1, 1);
        }
        //m_leftUmbraLine
        {
            ccDrawColor4F(1, 0, 0, 1);
            glLineWidth(1);
            ccDrawLine(m_leftUmbraLine[0], m_leftUmbraLine[2]);
            ccPointSize(5);
            ccDrawPoint(m_leftUmbraLine[1]);
        }
        //m_rightUmbraLine
        {
            ccDrawColor4F(0, 1, 0, 1);
            glLineWidth(1);
            ccDrawLine(m_rightUmbraLine[0], m_rightUmbraLine[2]);
            ccPointSize(5);
            ccDrawPoint(m_rightUmbraLine[1]);
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
        CCPoint lightPosLocal=getLightPosLocal();
        //----right penumbra mesh
        {
            vector<Cedge> edgeList;
            int nPoint=(int)m_polygon.m_pointList.size();
            if(m_rightPenumbraPointID==m_rightUmbraPointID){
                edgeList.push_back(Cedge(m_rightPenumbraLine[1],m_rightPenumbraLine[2]));
                edgeList.push_back(Cedge(m_rightUmbraLine[1],m_rightUmbraLine[2]));
            }else{
                edgeList.push_back(Cedge(m_rightPenumbraLine[1],m_rightPenumbraLine[2]));
                int index=(m_rightPenumbraPointID+1)%nPoint;
                while(1){
                    const CCPoint&p=m_polygon.m_pointList[index];
                    const CCPoint&pf=m_polygon.m_pointList[index-1>=0?index-1:index-1+nPoint];
                    Cedge edge;
                    edge.m_start=pf;
                    edge.m_end=edge.m_start+ccpMult(ccpNormalize(p-pf), m_shadowLength);
                    edgeList.push_back(edge);
                    if(index==m_rightUmbraPointID)break;
                    index=(index+1)%nPoint;
                    
                }
                edgeList.push_back(Cedge(m_rightUmbraLine[1],m_rightUmbraLine[2]));
            }//got edgeList
            vector<float> brightList;
            int nEdge=(int)edgeList.size();
            for(int i=0;i<nEdge;i++){
                const Cedge&edge=edgeList[i];
                //calculate area of the light circle on the right side of the edge
                float PND=PNDisPointToLine(lightPosLocal, edge.m_start, edge.m_end);
                float area=m_light->getR()-PND;
                brightList.push_back(area/(2*m_light->getR()));
            }
            //convert edgeList to mesh
            
            for(int i=0;i<nEdge-1;i++){
                const Cedge&edge=edgeList[i];
                const Cedge&edgen=edgeList[i+1];
                const CCPoint&p0=edge.m_end;
                const CCPoint&p1=edgen.m_end;
                const CCPoint&p2=edgen.m_start;
                float bright=brightList[i];
                float brightn=brightList[i+1];
                //v0
                Cv2 pos0=ccpTov2(p0);
                Cv2 texCoord0=Cv2(0,0);
                Cv4 color0=Cv4(bright,brightn,0,1);
                //v1
                Cv2 pos1=ccpTov2(p1);
                Cv2 texCoord1=Cv2(1,0);
                Cv4 color1=Cv4(bright,brightn,0,1);
                //v2
                Cv2 pos2=ccpTov2(p2);
                Cv2 texCoord2=Cv2(0,1);
                Cv4 color2=Cv4(bright,brightn,0,1);
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
                
            }
        
        }

        //----left penumbra mesh
        {
            vector<Cedge> edgeList;
            int nPoint=(int)m_polygon.m_pointList.size();
            if(m_leftPenumbraPointID==m_leftUmbraPointID){
                edgeList.push_back(Cedge(m_leftPenumbraLine[1],m_leftPenumbraLine[2]));
                edgeList.push_back(Cedge(m_leftUmbraLine[1],m_leftUmbraLine[2]));
            }else{
                edgeList.push_back(Cedge(m_leftPenumbraLine[1],m_leftPenumbraLine[2]));
                int index=(m_leftPenumbraPointID-1>=0?m_leftPenumbraPointID-1:m_leftPenumbraPointID-1+nPoint);
                while(1){
                    const CCPoint&p=m_polygon.m_pointList[index];
                    const CCPoint&pf=m_polygon.m_pointList[(index+1)%nPoint];
                    Cedge edge;
                    edge.m_start=pf;
                    edge.m_end=edge.m_start+ccpMult(ccpNormalize(p-pf), m_shadowLength);
                    edgeList.push_back(edge);
                    if(index==m_leftUmbraPointID)break;
                    index=(index-1>=0?index-1:index-1+nPoint);
                    
                }
                edgeList.push_back(Cedge(m_leftUmbraLine[1],m_leftUmbraLine[2]));
            }//got edgeList
            vector<float> brightList;
            int nEdge=(int)edgeList.size();
            for(int i=0;i<nEdge;i++){
                const Cedge&edge=edgeList[i];
                //calculate area of the light circle on the right side of the edge
                float PND=PNDisPointToLine(lightPosLocal, edge.m_start, edge.m_end);
                float area=m_light->getR()+PND;
                brightList.push_back(area/(2*m_light->getR()));
            }

            //convert edgeList to mesh
            for(int i=0;i<nEdge-1;i++){
                const Cedge&edge=edgeList[i];
                const Cedge&edgen=edgeList[i+1];
                const CCPoint&p0=edge.m_end;
                const CCPoint&p1=edgen.m_start;
                const CCPoint&p2=edgen.m_end;
                float bright=brightList[i];
                float brightn=brightList[i+1];
                //v0
                Cv2 pos0=ccpTov2(p0);
                Cv2 texCoord0=Cv2(0,0);
                Cv4 color0=Cv4(bright,brightn,0,1);
                //v1
                Cv2 pos1=ccpTov2(p1);
                Cv2 texCoord1=Cv2(0,1);
                Cv4 color1=Cv4(bright,brightn,0,1);
                //v2
                Cv2 pos2=ccpTov2(p2);
                Cv2 texCoord2=Cv2(1,0);
                Cv4 color2=Cv4(bright,brightn,0,1);
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
                
            }
            
        }

        
        
   
        
    
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
    vector<CCPoint> m_leftPenumbraLine;//m_leftPenumbraLine have three elements, [0] is start point, [1] is mid point, [2] is end point
    vector<CCPoint> m_rightPenumbraLine;
    vector<CCPoint> m_leftUmbraLine;
    vector<CCPoint> m_rightUmbraLine;
    int m_leftPenumbraPointID;
    int m_rightPenumbraPointID;
    int m_leftUmbraPointID;
    int m_rightUmbraPointID;
    ClightNode *m_light;
    float m_shadowLength;
    CCTexture2D*m_finTexture;
    Cmesh*m_mesh;
    CindexVBO*m_indexVBO;
    CGLProgramWithUnifos*m_program;
};




namespace_ens_end
#endif
