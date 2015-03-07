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
#include "ens2DSoftShadowNode.h"
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
public:
    //touch
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
protected:
    //slider
    ControlSlider* m_pSliderCtl;
    void sliderAction(Ref *senderz, Control::EventType controlEvent);
    CCLabelTTF* m_label;
    //back
    ControlButton* m_controlButton_back;
    void controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent);
    //debug
    ControlButton* m_controlButton_debug;
    void controlButtonEvent_debug(Ref *senderz, Control::EventType controlEvent);
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
