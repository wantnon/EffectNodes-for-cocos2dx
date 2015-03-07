//
//  layer_lightningBolt.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__layer_lightningBolt__
#define __HelloCpp__layer_lightningBolt__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include "ensLightningBoltNode.h"
using namespace ens;
class Clayer_lightningBolt:public CCLayer
{
public:
    Clayer_lightningBolt(){
        m_lbNode=NULL;
		m_isDebugOn=false;
    }
    virtual~Clayer_lightningBolt(){
        
    }
    bool init();

protected:
	//debug
    ControlButton* m_controlButton_debug;
    void controlButtonEvent_debug(Ref *senderz, cocos2d::extension::Control::EventType controlEvent);
    CCLabelTTF* m_label_debugState;
    bool m_isDebugOn;
	//back
    void controlButtonEvent_back(Ref *senderz, cocos2d::extension::Control::EventType controlEvent);
    ControlButton* m_controlButton_back;
protected:
    ClightningBoltNode* m_lbNode;
};
#endif /* defined(__HelloCpp__layer_lightningBolt__) */
