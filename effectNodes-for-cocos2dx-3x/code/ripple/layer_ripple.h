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
#include "ensRippleNode.h"
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
public:
    //touch
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
protected:
    //debug
    ControlButton* m_controlButton_debug;
    void controlButtonEvent_debug(Ref *senderz, Control::EventType controlEvent);
    CCLabelTTF* m_label_debugState;
    bool m_isDebugOn;
    //back
    ControlButton* m_controlButton_back;
    void controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent);
protected:
    CrippleSprite*m_rippleSprite;
};

#endif
