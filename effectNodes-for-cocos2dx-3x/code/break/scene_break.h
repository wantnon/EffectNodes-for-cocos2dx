//
//  scene_break.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__scene_break__
#define __HelloCpp__scene_break__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "layer_break.h"
using namespace ens;
class Cscene_break:public CCScene{
public:
    Cscene_break(){};
    virtual~Cscene_break(){}
    bool init(){
        Clayer_break*layer=new Clayer_break();
        layer->autorelease();
        layer->init();
        addChild(layer);
        return true;
    }
};
#endif /* defined(__HelloCpp__scene_break__) */
