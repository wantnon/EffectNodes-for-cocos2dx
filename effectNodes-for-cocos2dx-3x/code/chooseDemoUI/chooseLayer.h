//
//  chooseLayer.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__chooseLayer__
#define __HelloCpp__chooseLayer__

#include <iostream>
#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include "CCControlButton.h"
class CchooseLayer:public CCLayer
{
public:
    CchooseLayer(){
        
    }
    virtual~CchooseLayer(){
        //show frame rate
        CCDirector::sharedDirector()->setDisplayStats(true);
    }
    bool init();
protected:
    void controlButtonEvent(Ref *senderz, cocos2d::extension::Control::EventType controlEvent);
protected:
    vector<ControlButton*> m_controlButtonList;
    vector<string> m_demoNameList;
    

};

#endif /* defined(__HelloCpp__chooseLayer__) */
