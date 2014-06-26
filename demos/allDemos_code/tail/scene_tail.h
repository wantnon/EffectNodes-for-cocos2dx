//
//  scene_tail.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__scene_tail__
#define __HelloCpp__scene_tail__

#include <iostream>
#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "layer_tail.h"
class Cscene_tail:public CCScene
{
public:
    Cscene_tail(){};
    virtual~Cscene_tail(){}
    bool init(){
        Clayer_tail*layer=new Clayer_tail();
        layer->autorelease();
        layer->init();
        addChild(layer);
        return true;
    }

};
#endif /* defined(__HelloCpp__scene_tail__) */
