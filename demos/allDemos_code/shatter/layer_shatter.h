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
#include "shatter/ensShatterNode.h"
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
protected:
    //touch
	virtual void ccTouchesBegan(CCSet* touches , CCEvent* event);
	virtual void ccTouchesMoved(CCSet* touches , CCEvent* event);
	virtual void ccTouchesEnded(CCSet* touches , CCEvent* event);
protected:
    CCControlButton* m_controlButton_back;
    void controlButtonEvent_back(CCObject *senderz, CCControlEvent controlEvent);
protected:
    CshatterSprite*m_shatterSprite;
};

#endif
