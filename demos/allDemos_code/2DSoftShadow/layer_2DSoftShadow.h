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
        m_2DSoftShadowNode=NULL;
        m_2DSoftShadowNode2=NULL;
        m_2DSoftShadowNode3=NULL;
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
    //back
    CCControlButton* m_controlButton_back;
    void controlButtonEvent_back(CCObject *senderz, CCControlEvent controlEvent);
    //debug
    CCControlButton* m_controlButton_debug;
    void controlButtonEvent_debug(CCObject *senderz, CCControlEvent controlEvent);
    CCLabelTTF* m_label_debugState;
    bool m_isDebugOn;
protected:
    CshadowRoot* m_shadowRoot;
    CCSprite*m_sprite;
    CCSprite*m_sprite2;
    CCSprite*m_sprite3;
    CCSprite*m_sprite_light;
    C2DSoftShadowNode*m_2DSoftShadowNode;
    C2DSoftShadowNode*m_2DSoftShadowNode2;
    C2DSoftShadowNode*m_2DSoftShadowNode3;
    ClightNode*m_light;
    CCSprite*m_backGround;
    
};

#endif
