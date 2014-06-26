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
#include "ripple_horizontal/ensRipple_horizontalNode.h"
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
    Cripple_horizontalNode*m_ripple_horizontalNode;
};

#endif
