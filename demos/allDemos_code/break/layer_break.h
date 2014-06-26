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
#include "break/ensBreakNode.h"
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
protected:
    //touch
	virtual void ccTouchesBegan(cocos2d::CCSet* touches , cocos2d::CCEvent* event);
	virtual void ccTouchesMoved(cocos2d::CCSet* touches , cocos2d::CCEvent* event);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches , cocos2d::CCEvent* event);
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
    CbreakSprite* m_breakSprite;
};

#endif /* defined(__HelloCpp__layer_break__) */
