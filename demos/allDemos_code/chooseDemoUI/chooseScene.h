//
//  chooseScene.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__chooseScene__
#define __HelloCpp__chooseScene__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "chooseLayer.h"
class CchooseScene:public CCScene{
public:
    CchooseScene(){};
    virtual~CchooseScene(){}
    bool init(){
        CchooseLayer*layer=new CchooseLayer();
        layer->autorelease();
        layer->init();
        addChild(layer);
        return true;
    }
};
#endif /* defined(__HelloCpp__chooseScene__) */
