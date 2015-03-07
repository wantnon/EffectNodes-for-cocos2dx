//
//  layer_ghostlike.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__layer_ghostlike__
#define __HelloCpp__layer_ghostlike__

#include <iostream>
#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include "ensGhostNode.h"
#include "CCControlButton.h"
#include "CCControl.h"
using namespace ens;
class Clayer_ghostlike : public CCLayer
{
public:
    bool init();
protected:
    ControlButton* m_controlButton_back;
    void controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent);
protected:
    CghostSprite* m_ghostSprite;
};

#endif
