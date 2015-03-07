//
//  layer_shatter.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__layer_shatter__
#define __HelloCpp__layer_shatter__

#include <iostream>
#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include "ensShatterNode.h"
using namespace ens;

class Clayer_shatter : public CCLayer
{
public:
   
    Clayer_shatter(){
        m_shatterSprite=NULL;
    }
    virtual ~Clayer_shatter(){
    }
    
    bool init();
public:
    //touch
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
protected:
    ControlButton* m_controlButton_back;
    void controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent);
protected:
    CshatterSprite*m_shatterSprite;
};

#endif
