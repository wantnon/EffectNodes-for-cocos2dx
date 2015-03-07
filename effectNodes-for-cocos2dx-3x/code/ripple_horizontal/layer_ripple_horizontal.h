//
//  layer_ripple_horizontal.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__layer_ripple_horizontal__
#define __HelloCpp__layer_ripple_horizontal__

#include <iostream>
#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include "ensRipple_horizontalNode.h"
using namespace ens;

class Clayer_ripple_horizontal : public CCLayer
{
public:
   
    Clayer_ripple_horizontal(){
        m_ripple_horizontalNode=NULL;
        m_isDebugOn=false;
    }
    virtual ~Clayer_ripple_horizontal(){
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
    Cripple_horizontalNode*m_ripple_horizontalNode;
};

#endif
