#ifndef __laserLayer_H__
#define __laserLayer_H__

#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include "common/ensCommon.h"
class ChalfLaser:public CCSprite{
public:
    ChalfLaser(){
        m_program=NULL;
        m_maskTex=NULL;
        m_maskTex2=NULL;
        m_noiseTex=NULL;
        m_noiseTex2=NULL;
        m_highlight=1.0;
        m_time=0;
        m_PorN=1;
        m_kLightColor=1.0;
        m_kGlowColor=1.0;
        m_noiseScale=1.0;
    }
    virtual~ChalfLaser(){
        if(m_program)m_program->release();
        if(m_maskTex)m_maskTex->release();
        if(m_maskTex2)m_maskTex2->release();
        if(m_noiseTex)m_noiseTex->release();
        if(m_noiseTex2)m_noiseTex2->release();

    }
    void setBlendFunc(GLenum src,GLenum dst);
    void updateStartAndEnd(){
        
        CCPoint mid=ccpMult(m_start+m_end, 0.5) ;
        CCPoint dir=ccpNormalize(m_end-m_start);
        float A;
        if(dir.y<=0){
            A=acosf(dir.x)*180/M_PI;
        }else{
            A=(2*M_PI-acosf(dir.x))*180/M_PI;
        }
        this->setPosition(mid);
        this->setRotation(A);
        
        float targetLen=ccpDistance(m_start, m_end);
        float contentLen=this->getContentSize().width;
        float scaleX=targetLen/contentLen;
        this->setScaleX(scaleX);
   
        
        
    }
    void setStart(const CCPoint&start){
        m_start=start;
        updateStartAndEnd();
    }
    void setEnd(const CCPoint&end){
        m_end=end;
        updateStartAndEnd();
    }
    bool init();
    void draw();
    void update(float dt);
    void setPorN(float value){m_PorN=value;}
    ens::CGLProgramWithUnifos*m_program;
    CCTexture2D*m_maskTex;
    CCTexture2D*m_maskTex2;
    CCTexture2D*m_noiseTex;
    CCTexture2D*m_noiseTex2;
    float m_highlight;
    float m_time;
    float m_PorN;
    CCPoint m_start;
    CCPoint m_end;
    float m_kLightColor;
    float m_kGlowColor;
    float m_noiseScale;


};
class Claser:public CCNode
{
public:
    ChalfLaser*m_halfLaser1;
    ChalfLaser*m_halfLaser2;
    ChalfLaser*m_slimHalfLaser1;
    ChalfLaser*m_slimHalfLaser2;
    CCPoint m_start;
    CCPoint m_end;
    Claser(){
        m_halfLaser1=NULL;
        m_halfLaser2=NULL;
        m_slimHalfLaser1=NULL;
        m_slimHalfLaser2=NULL;

    }
    bool init(){
        //halfLaser1
        m_halfLaser1=new ChalfLaser();
        m_halfLaser1->autorelease();
        m_halfLaser1->init();
        m_halfLaser1->setPorN(1);
        m_halfLaser1->setBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        m_halfLaser1->setScaleY(1.3);//(1.0);
        m_halfLaser1->m_kGlowColor=1.6;//1.0;
        m_halfLaser1->m_kLightColor=0.0;
        m_halfLaser1->m_noiseScale=2.2;//0.5;
        addChild(m_halfLaser1);
        //halfLaser2
        m_halfLaser2=new ChalfLaser();
        m_halfLaser2->autorelease();
        m_halfLaser2->init();
        m_halfLaser2->setPorN(-1);
        m_halfLaser2->setBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        m_halfLaser2->setScaleY(1.3);//(1.0);
        m_halfLaser2->m_kGlowColor=1.6;//1.0;
        m_halfLaser2->m_kLightColor=0.0;
        m_halfLaser2->m_noiseScale=2.2;//0.5;
        addChild(m_halfLaser2);
        //slimHalfLaser1
        m_slimHalfLaser1=new ChalfLaser();
        m_slimHalfLaser1->autorelease();
        m_slimHalfLaser1->init();
        m_slimHalfLaser1->setPorN(1);
        m_slimHalfLaser1->setBlendFunc(GL_ONE, GL_ONE);
        m_slimHalfLaser1->setScaleY(0.4);//(0.6);//(0.1);
        m_slimHalfLaser1->m_kGlowColor=2.5;//1.5;
        m_slimHalfLaser1->m_kLightColor=0.5;//0.4;//0.3;
        m_slimHalfLaser1->m_noiseScale=1.5;
        addChild(m_slimHalfLaser1);
        //slimHalfLaser2
        m_slimHalfLaser2=new ChalfLaser();
        m_slimHalfLaser2->autorelease();
        m_slimHalfLaser2->init();
        m_slimHalfLaser2->setPorN(-1);
        m_slimHalfLaser2->setBlendFunc(GL_ONE, GL_ONE);
        m_slimHalfLaser2->setScaleY(0.4);//(0.6);//(0.1);
        m_slimHalfLaser2->m_kGlowColor=2.5;//1.5;
        m_slimHalfLaser2->m_kLightColor=0.5;//0.4;//0.3;
        m_slimHalfLaser2->m_noiseScale=1.5;
        addChild(m_slimHalfLaser2);
        
        return true;
    }
    void setStart(const CCPoint&start){
        m_start=start;
        m_halfLaser1->setStart(m_start);
        m_halfLaser2->setStart(m_start);
        m_slimHalfLaser1->setStart(m_start);
        m_slimHalfLaser2->setStart(m_start);


    
    }
    void setEnd(const CCPoint&end){
        m_end=end;
        m_halfLaser1->setEnd(m_end);
        m_halfLaser2->setEnd(m_end);
        m_slimHalfLaser1->setEnd(m_end);
        m_slimHalfLaser2->setEnd(m_end);

    }


};

class ClaserLayer : public cocos2d::CCLayer
{
public:
    bool init();
public:
    //touch
	virtual void ccTouchesBegan(cocos2d::CCSet* touches , cocos2d::CCEvent* event);
	virtual void ccTouchesMoved(cocos2d::CCSet* touches , cocos2d::CCEvent* event);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches , cocos2d::CCEvent* event);
public:
    //back
    void controlButtonEvent_back(CCObject *senderz, CCControlEvent controlEvent);
    CCControlButton* m_controlButton_back;
public:
    ClaserLayer(){

    }
    virtual ~ ClaserLayer(){
    }
    void update(float dt);
public:
    Claser* m_laser;



 };

#endif // __laserLayer_H__
