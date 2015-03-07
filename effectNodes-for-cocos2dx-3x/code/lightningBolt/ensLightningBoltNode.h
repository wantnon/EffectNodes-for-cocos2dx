

#ifndef __HelloCpp__ensLightningBoltNode__
#define __HelloCpp__ensLightningBoltNode__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "ensCommon.h"
namespace_ens_begin

namespace __lb{
    class ClineSeg{
    public:
        CCPoint startPoint;
        CCPoint endPoint;
        CCPoint branchEndPoint;//only seed seg store this
        int branchID;//which branch the seg belongs to. branchID==0 means belongs to mainBranch
        int branchStartGenerationID;//in which generation the branch this seg belongs to start generated.
        int generationID;//this seg created in which generation, the original seg's generationID is 0
        ClineSeg*father;
        vector<ClineSeg*> childList;
        bool needReGenerate;//whether the branch use this seg as root seg need to regenerate
    public:
        ClineSeg(){
            initParameter();
        }
        ClineSeg(CCPoint start,CCPoint end){
            initParameter();//do not forget this
            init(start, end);
        }
        virtual~ClineSeg(){}
        void init(CCPoint start,CCPoint end);
        bool addChildRef(ClineSeg*child);
        bool removeChildRef(ClineSeg*child);
        float getLen()const;
        CCPoint getDir()const;
        CCPoint getVec()const;
        CCPoint getRightNormal()const;
        CCPoint getMidPoint()const;
    protected:
        void initParameter(){
            branchID=-1;
            branchStartGenerationID=-1;
            generationID=-1;
            father=NULL;
            needReGenerate=false;
        }
    };
}

    

    
class ClightningBoltSprite:public CCSprite
//ref to: http://drilian.com/2009/02/25/lightning-bolts/
{
public:
    ClightningBoltSprite(){
        m_mainSegLen=14;
        m_k_offset=0.13;
        updateNGeneration();
        m_headSeg=NULL;
        m_tailSeg=NULL;
        m_program=NULL;
        m_indexVBO=NULL;
        m_isDrawDebug=false;
        m_percentageOfJitterBranch=0.5;
        m_opacityZoom=1.2;//1.2;
        m_mesh=NULL;
    }
    virtual ~ClightningBoltSprite(){
        if(m_program)m_program->release();
        if(m_indexVBO)m_indexVBO->release();
        if(m_mesh)m_mesh->release();
    }
    void updateNGeneration();
    void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
    void onDraw(const Mat4 &transform, uint32_t flags);
    void onDrawDebug(const Mat4 &transform, uint32_t flags);
    bool init();
    void setStartAndEnd(CCPoint start,CCPoint end);
    void genLighting();
    void doJitter();
    bool getIsDrawDebug()const{return m_isDrawDebug;}
    void setIsDrawDebug(bool value){m_isDrawDebug=value;}
protected:
    //----init program
    void initProgram();
    //----generate lighting
    void genSeges();
    void genMesh();
    vector<__lb::ClineSeg*> genSegesFromBranchSeg(__lb::ClineSeg*branchSeg,int nGeneration);
    vector<__lb::ClineSeg*> splitSeg(__lb::ClineSeg*seg,int generationID);
    //----jitter
    void releaseNeedReGenBranch(vector<__lb::ClineSeg*>&branchList,vector<__lb::ClineSeg*>&segList);
    void releaseNeedReGenBranch_inn(__lb::ClineSeg*seg,vector<__lb::ClineSeg*>&segList);
    //----debug draw
    void debugDraw_segWireFrame(float lineWidth);
    void debugDraw_segLineAndDot(float lineWidth,float pointSize);
protected:
    //shader program
    CGLProgramWithUnifos*m_program;
    //core data of lightning
    CCPoint m_startPoint_parentSpace;//parent space start point
    CCPoint m_endPoint_parentSpace;//parent space end point
    CCPoint m_startPoint;//local space start point
    CCPoint m_endPoint;//local space end point
    vector<__lb::ClineSeg*> m_segList;//hold seges compose the lighting
    int m_nGeneration;//split how many generations
    __lb::ClineSeg*m_headSeg;//the lighting tree's main branch's head seg
    __lb::ClineSeg*m_tailSeg;//the lighting tree's main branch's tail seg
    //model
    Cmesh* m_mesh;
    CindexVBO*m_indexVBO;
    //settings
    float m_k_offset;
    float m_mainSegLen;
    float m_percentageOfJitterBranch;
    float m_opacityZoom;
    bool m_isDrawDebug;
    
    CustomCommand _customCommand;
    CustomCommand _customCommand_debug;
};


class ClightningBoltNode:public CCNode
{
public:
    ClightningBoltNode(){
        m_lbSprite=NULL;
        m_lbSprite2=NULL;
		m_isDrawDebug=false;
    }
    virtual~ClightningBoltNode(){}
    bool init(const CCPoint&start,const CCPoint&end);
    void setStartAndEnd(const CCPoint&start,const CCPoint&end){
        m_start=start;
        m_end=end;
        m_lbSprite->setStartAndEnd(m_start, m_end);
        m_lbSprite2->setStartAndEnd(m_start, m_end);
    }
    CCPoint getStart()const{return m_start;}
    CCPoint getEnd()const{return m_end;}
	bool getIsDrawDebug()const{
		assert(m_lbSprite->getIsDrawDebug()==m_isDrawDebug);
		assert(m_lbSprite2->getIsDrawDebug()==m_isDrawDebug);
		return m_isDrawDebug;
	}
	void setIsDrawDebug(bool value){
		m_isDrawDebug=value;
		m_lbSprite->setIsDrawDebug(value);
		m_lbSprite2->setIsDrawDebug(value);
	}
    ClightningBoltSprite* getLBSprite1(){return m_lbSprite;}
    ClightningBoltSprite* getLBSprite2(){return m_lbSprite2;}
    void runFlashAction();
protected:
    CCPoint m_start;//parent space start point
    CCPoint m_end;//parent space end point
    ClightningBoltSprite*m_lbSprite;
    ClightningBoltSprite*m_lbSprite2;
	bool m_isDrawDebug;


};

namespace_ens_end
#endif /* defined(__HelloCpp__ensLightingSprite__) */
