

#ifndef HelloWorld_ensNormalMappedNode_h
#define HelloWorld_ensNormalMappedNode_h


#include<iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "ensCommon.h"
namespace_ens_begin
namespace normalMapped {
    
    class ClightSprite:public CCSprite
    {
    public:
        ClightSprite(){
            m_diffuse=ccc4f(1, 1, 1, 1);
            m_ambient=ccc4f(1, 1, 1, 1);
            m_z=0;
        }
        virtual~ClightSprite(){
            
        }
        bool init(const string&texFileName){
            this->CCSprite::initWithFile(texFileName.c_str());
            return true;
        }
        ccColor4F getDiffuse()const{return m_diffuse;}
        void setDiffuse(const ccColor4F&diffuse){
            m_diffuse=diffuse;
            setColor(ccc3(m_diffuse.r*255,m_diffuse.g*255,m_diffuse.b*255));
        }
        ccColor4F getAmbient()const{return m_ambient;}
        void setAmbient(const ccColor4F&ambient){m_ambient=ambient;}
        float getZ()const{return m_z;}
        void setZ(float z){m_z=z;}
    protected:
        ccColor4F m_diffuse;
        ccColor4F m_ambient;
        float m_z;//z value of the light
    };
    class Cmaterial{
    public:
        ccColor4F m_diffuse;
        ccColor4F m_ambient;
        Cmaterial(){
            m_diffuse=ccc4f(1, 1, 1, 1);
            m_ambient=ccc4f(0.5, 0.5, 0.5, 1);
        }
    };
    

}
class CnormalMappedSprite : public CCSprite
//ref to http://www.alkemi-games.com/a-game-of-tricks/
{
public:
	CnormalMappedSprite() {
        m_program=NULL;
        m_normalMap=NULL;
        m_lightSprite=NULL;
        m_kBump=1.0;
    }
	virtual ~CnormalMappedSprite(){
        if(m_program)m_program->release();
        if(m_normalMap)m_normalMap->release();
        if(m_lightSprite)m_lightSprite->release();
    };
	bool init(const string&texFileName,const string&normalMapFileName) ;
    void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
    void setLightSprite(normalMapped::ClightSprite*lightSprite){
        assert(lightSprite);
        if(m_lightSprite==NULL){
            m_lightSprite=lightSprite;
            m_lightSprite->retain();
        }else{
            m_lightSprite->release();
            m_lightSprite=lightSprite;
            m_lightSprite->retain();
        }
        
    }
    normalMapped::ClightSprite* getLightSprite(){return m_lightSprite;}
    void setKBump(float value){m_kBump=value;}
    float getKBump()const{return m_kBump;}
    void onPassUnifoAndBindTex(const Mat4 &transform, uint32_t flags);
protected:
    CGLProgramWithUnifos*m_program;
    CCTexture2D*m_normalMap;
    normalMapped::ClightSprite*m_lightSprite;
    normalMapped::Cmaterial m_material;
    float m_kBump;
    
    
    CustomCommand _customCommand;
    CustomCommand _passUnifoAndBindTexCommand;

};
namespace_ens_end
#endif /* defined(__HelloCpp__ensNormalMappedNode__) */
