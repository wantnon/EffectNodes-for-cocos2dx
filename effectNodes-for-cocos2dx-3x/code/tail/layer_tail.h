//
//  layer_tail.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__layer_tail__
#define __HelloCpp__layer_tail__
#include <iostream>
#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include "ensTailNode.h"
using namespace ens;
class Clayer_tail : public CCLayer
{
public:
    Clayer_tail(){
        m_isDebugOn=false;
    }
    virtual~Clayer_tail(){}
public:
    bool init();
    void update(float dt);
protected:
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
    
    vector<CtailSprite*> m_tailSpriteList;
    
};

#endif /* defined(__HelloCpp__layer_tail__) */
