//
//  layer_break.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__layer_break__
#define __HelloCpp__layer_break__
#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include "ensBreakNode.h"
using namespace ens;
class Clayer_break : public CCLayer
{
public:
    Clayer_break(){
        m_breakSprite=NULL;
        m_isDebugOn=false;
    };
    virtual~Clayer_break(){}
    
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
    CbreakSprite* m_breakSprite;
};

#endif /* defined(__HelloCpp__layer_break__) */
