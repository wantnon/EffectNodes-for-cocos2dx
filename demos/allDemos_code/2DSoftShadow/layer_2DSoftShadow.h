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
        m_2DSoftShadowNode=NULL;
        m_light=NULL;
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
    CCControlButton* m_controlButton_back;
    void controlButtonEvent_back(CCObject *senderz, CCControlEvent controlEvent);
protected:
    C2DSoftShadowNode*m_2DSoftShadowNode;
    ClightNode*m_light;
};

#endif
