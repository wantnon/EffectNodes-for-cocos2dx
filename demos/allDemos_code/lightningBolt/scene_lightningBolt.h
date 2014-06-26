//
//  scene_lightningBolt.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__scene_lightningBolt__
#define __HelloCpp__scene_lightningBolt__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "layer_lightningBolt.h"
using namespace ens;
class Cscene_lightningBolt:public CCScene{
public:
    Cscene_lightningBolt(){};
    virtual~Cscene_lightningBolt(){}
    bool init(){
        Clayer_lightningBolt*layer=new Clayer_lightningBolt();
        layer->autorelease();
        layer->init();
        addChild(layer);
        return true;
    }
};
#endif /* defined(__HelloCpp__scene_lightningBolt__) */
