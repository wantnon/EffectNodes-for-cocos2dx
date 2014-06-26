//
//  layer_ripple.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__layer_ripple__
#define __HelloCpp__layer_ripple__

#include <iostream>
#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include "ripple/ensRippleNode.h"
using namespace ens;

class Clayer_ripple : public CCLayer
{
public:
   
    Clayer_ripple(){
        m_rippleSprite=NULL;
        m_isDebugOn=false;
    }
    virtual ~Clayer_ripple(){
    }
    
    bool init();
protected:
    //touch
	virtual void ccTouchesBegan(CCSet* touches , CCEvent* event);
	virtual void ccTouchesMoved(CCSet* touches , CCEvent* event);
	virtual void ccTouchesEnded(CCSet* touches , CCEvent* event);
protected:
    //debug
    CCControlButton* m_controlButton_debug;
    void controlButtonEvent_debug(CCObject *senderz, CCControlEvent controlEvent);
    CCLabelTTF* m_label_debugState;
    bool m_isDebugOn;
    //back
    CCControlButton* m_controlButton_back;
    void controlButtonEvent_back(CCObject *senderz, CCControlEvent controlEvent);
protected:
    CrippleSprite*m_rippleSprite;
};

#endif
