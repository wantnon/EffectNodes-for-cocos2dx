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
class CchooseLayer:public CCLayer
{
public:
    CchooseLayer(){
        
    }
    virtual~CchooseLayer(){
        
    }
    bool init();
protected:
    void controlButtonEvent(CCObject *senderz, CCControlEvent controlEvent);
protected:
    vector<CCControlButton*> m_controlButtonList;
    vector<string> m_demoNameList;
    

};

#endif /* defined(__HelloCpp__chooseLayer__) */
