

#ifndef HelloWorld_ensBreakNode_h
#define HelloWorld_ensBreakNode_h


#include<iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "ensCommon.h"
namespace_ens_begin
namespace breakEffect {

    class CIDquad{
    public:
        int m_vIDLU;
        int m_vIDRU;
        int m_vIDLD;
        int m_vIDRD;
        bool m_isRemoved;
        int m_vIDL;
        int m_vIDR;
        int m_vIDU;
        int m_vIDD;
        CIDquad(){
            m_vIDLU=-1;
            m_vIDRU=-1;
            m_vIDLD=-1;
            m_vIDRD=-1;
            m_isRemoved=false;
            m_vIDL=-1;
            m_vIDR=-1;
            m_vIDU=-1;
            m_vIDD=-1;
        }
    };
    class Ctri{
    public:
        Cv2 m_pos[3];
        Cv2 m_texCoord[3];
        Cv4 m_color[3];
        float m_fallOffDelay;
        float m_vy;
        void setPositions(const Cv2&pos0,const Cv2&pos1,const Cv2&pos2){
            m_pos[0]=pos0;
            m_pos[1]=pos1;
            m_pos[2]=pos2;
        }
        void setColors(const Cv4&color0,const Cv4&color1,const Cv4&color2){
            m_color[0]=color0;
            m_color[1]=color1;
            m_color[2]=color2;
        }
        void setTexCoords(const Cv2&texCoord0,const Cv2&texCoord1,const Cv2&texCoord2){
            m_texCoord[0]=texCoord0;
            m_texCoord[1]=texCoord1;
            m_texCoord[2]=texCoord2;
        }
        Ctri(){
            m_fallOffDelay=0;
            m_vy=0;
            setPositions(Cv2(0,0), Cv2(0,0), Cv2(0,0));
            setTexCoords(Cv2(0,0), Cv2(0,0), Cv2(0,0));
            setColors(Cv4(1,1,1,1), Cv4(1,1,1,1), Cv4(1,1,1,1));
        }
    };
    enum eState{
        eState_well=0,
        eState_crack,
        eState_fallOff,
    };
}
namespace breakEffect {
    class CfallOffAction;
}
class CbreakSprite : public CCSprite
{
public:
    friend class breakEffect::CfallOffAction;
public:
	CbreakSprite() {
        m_mesh=NULL;
        m_meshWire=NULL;
        m_nRow=0;
        m_nCol=0;
        m_indexVBO=NULL;
        m_indexVBO_wire=NULL;
        m_program=NULL;

        m_gridSideLenMax=80;
        m_gridSideLenMin=10;
        m_state=breakEffect::eState_well;
        m_isDrawDebug=false;
    }
	virtual ~CbreakSprite(){
        if(m_mesh)m_mesh->release();
        if(m_meshWire)m_meshWire->release();
        if(m_indexVBO)m_indexVBO->release();
        if(m_indexVBO_wire)m_indexVBO_wire->release();
        if(m_program)m_program->release();
        
    }
	bool init(const string&texFileName) ;
    void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
    void onDrawDebug(const Mat4 &transform, uint32_t flags);
    void onDraw(const Mat4 &transform, uint32_t flags);
    void doCrack(const CCPoint&touchPoint);
    void generateDelayTimes(float maxDelayTime){generateDelayTimeForTris(0,maxDelayTime);}
    float getGridSideLenMax()const{return m_gridSideLenMax;}
    void setGridSideLenMax(float value){m_gridSideLenMax=value;}
    float getGridSideLenMin()const{return m_gridSideLenMin;}
    void setGridSideLenMin(float value){m_gridSideLenMin=value;}
    breakEffect::eState getState()const{return m_state;}
    void reSet();
    void setIsDrawDebug(bool value){m_isDrawDebug=value;}
    bool getIsDrawDebug()const{return m_isDrawDebug;}
protected:
    int getPointInRangeEqualsToTargetPoint(const CCPoint&targetPoint,const vector<CCPoint>&pointList,int imin,int imax,float eps);
    void updateMeshAndSubmitChange();
    void generateDelayTimeForTris(float minDelayTime,float maxDelayTime);
    void updateBreakAction(float time,float dt,float ay);//used by action
    void clear();
protected:
    float m_gridSideLenMax;
    float m_gridSideLenMin;
    CGLProgramWithUnifos* m_program;
    Cmesh*m_mesh;
    CmeshWire*m_meshWire;
    CindexVBO*m_indexVBO;
    CindexVBO*m_indexVBO_wire;
    int m_nRow;//grid row count
    int m_nCol;//grid col count
    int m_nRowPoint;//point row count, m_nRowPoint=m_nRow+1
    int m_nColPoint;//point col count, m_nColPoint=m_nCol+1
    vector<CCPoint> m_pointList;
    vector<breakEffect::CIDquad> m_IDquadList;
    vector<breakEffect::Ctri> m_triList;
    breakEffect::eState m_state;
    bool m_isDrawDebug;
    
    CustomCommand _customCommand;
    CustomCommand _customCommand_debug;
    

};
namespace breakEffect {
    
#define CfallOffAction_dft_ay (-60*10)
    class CfallOffAction : public CCActionInterval
    //ref to CCRotateBy
    //why use action: http://user.qzone.qq.com/350479720/blog/1403318648
    {
    public:
        //creates the action
        static CfallOffAction* create(float fDuration,float ay=CfallOffAction_dft_ay);
        //initializes the action
        bool initWithDuration(float fDuration,float ay=CfallOffAction_dft_ay);
        
        virtual void startWithTarget(CCNode *pTarget);
        virtual void update(float time);
        CfallOffAction(){
            m_ay=CfallOffAction_dft_ay;
            m_timeFoe=0;
            m_timeCur=0;
        }
    protected:
        float m_timeFoe;
        float m_timeCur;
        float m_ay;
        
    };

}
namespace_ens_end
#endif /* defined(__HelloCpp__ensBreakNode__) */
