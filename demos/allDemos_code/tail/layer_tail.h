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
#include "tail/ensTailNode.h"
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
    
    vector<CtailSprite*> m_tailSpriteList;
    
};

#endif /* defined(__HelloCpp__layer_tail__) */
