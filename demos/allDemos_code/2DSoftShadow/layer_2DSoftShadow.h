//
//  layer_2DSoftShadow.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__layer_2DSoftShadow__
#define __HelloCpp__layer_2DSoftShadow__

#include <iostream>
#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include "2DSoftShadow/ens2DSoftShadowNode.h"
using namespace ens;

class Clayer_2DSoftShadow : public CCLayer
{
public:
   
    Clayer_2DSoftShadow(){
        m_sprite=NULL;
        m_sprite2=NULL;
        m_sprite3=NULL;
        m_sprite_light=NULL;
        m_shadowObj=NULL;
        m_shadowObj2=NULL;
        m_shadowObj3=NULL;
        m_light=NULL;
        m_backGround=NULL;
        m_shadowRoot=NULL;
        m_isDebugOn=false;
        
    }
    virtual ~Clayer_2DSoftShadow(){
    }
    
    bool init();
protected:
    //touch
	virtual void ccTouchesBegan(CCSet* touches , CCEvent* event);
	virtual void ccTouchesMoved(CCSet* touches , CCEvent* event);
	virtual void ccTouchesEnded(CCSet* touches , CCEvent* event);
protected:
    //slider
    CCControlSlider* m_pSliderCtl;
    void sliderAction(CCObject* sender, CCControlEvent controlEvent);
    CCLabelTTF* m_label;
    //back
    CCControlButton* m_controlButton_back;
    void controlButtonEvent_back(CCObject *senderz, CCControlEvent controlEvent);
    //debug
    CCControlButton* m_controlButton_debug;
    void controlButtonEvent_debug(CCObject *senderz, CCControlEvent controlEvent);
    CCLabelTTF* m_label_debugState;
    bool m_isDebugOn;
protected:
    CCSprite*m_backGround;
    CCSprite*m_sprite;
    CCSprite*m_sprite2;
    CCSprite*m_sprite3;
    CCSprite*m_sprite_light;
    C2DSoftShadowRoot* m_shadowRoot;
    C2DSoftShadowObj*m_shadowObj;
    C2DSoftShadowObj*m_shadowObj2;
    C2DSoftShadowObj*m_shadowObj3;
    ClightNode*m_light;
    
    
};

#endif
