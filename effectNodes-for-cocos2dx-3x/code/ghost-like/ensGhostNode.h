

#ifndef HelloWorld_ensGhostNode_h
#define HelloWorld_ensGhostNode_h


#include<iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "ensCommon.h"
namespace_ens_begin
namespace ghostlike
{
    const float dft_filterR=16;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32||CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    //PC and Mac is powerful :D
	const float dft_filterSrcScale=1.0;
    const float dft_downSample=1.0;
#else
    //on mobile platform we make a discount
    const float dft_filterSrcScale=0.5;
    const float dft_downSample=0.4;
#endif
}

class CghostSprite : public CCNode
{
public:
    CghostSprite() {
        m_program_filterX=NULL;
        m_program_filterY=NULL;
        m_biggerRT=NULL;
        m_filterXRT=NULL;
        m_filterYRT=NULL;
        m_refSprite=NULL;
        //
        m_filterR=ghostlike::dft_filterR;
        m_fliterSrcScale=ghostlike::dft_filterSrcScale;
        m_downSample=ghostlike::dft_downSample;
        
        m_finalSprite=NULL;
        
       
    }
    virtual ~CghostSprite(){
        if(m_program_filterX)m_program_filterX->release();
        if(m_program_filterY)m_program_filterY->release();
    };

	bool init(const string&texFileName,float filterR=ghostlike::dft_filterR,float fliterSrcScale=ghostlike::dft_filterSrcScale);
	bool init(CCSpriteFrame* spriteFrame,float filterR=ghostlike::dft_filterR,float fliterSrcScale=ghostlike::dft_filterSrcScale);
	bool init(CCSprite* sprite,float filterR=ghostlike::dft_filterR,float fliterSrcScale=ghostlike::dft_filterSrcScale);
    virtual void visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags);
    void setDownSample(float value){m_downSample=value;}
    float getDownSample()const{return m_downSample;}
    CCSprite*getRefSprite(){return m_refSprite;}
    void onBiggerRTPassUnifoAndBindTex(const Mat4 &transform, uint32_t flags);
    void onFilterXRTPassUnifoAndBindTex(const Mat4 &transform, uint32_t flags);
public:
    float m_filterR;
    CGLProgramWithUnifos*m_program_filterX;
    CGLProgramWithUnifos*m_program_filterY;
    CCRenderTexture*m_biggerRT;
    CCRenderTexture*m_filterXRT;
    CCRenderTexture*m_filterYRT;
    CCSprite*m_refSprite;
    float m_fliterSrcScale;//let filter source texture smaller, can make filter op faster
    float m_downSample;
    
    
    
    
    CustomCommand _biggerRTpassUnifoAndBindTexCommand;
    CustomCommand _filterXRTpassUnifoAndBindTexCommand;
    
 
    
    
    CCSprite* m_finalSprite;
    
    

};
namespace_ens_end
#endif
