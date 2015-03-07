//
//  scene_colorAdjust.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__scene_colorAdjust__
#define __HelloCpp__scene_colorAdjust__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "layer_colorAdjust.h"
using namespace ens;
class Cscene_colorAdjust:public CCScene{
public:
    Cscene_colorAdjust(){};
    virtual~Cscene_colorAdjust(){}
    bool init(){
        Clayer_colorAdjust*layer=new Clayer_colorAdjust();
        layer->autorelease();
        layer->init();
        addChild(layer);
        return true;
    }
};
#endif /* defined(__HelloCpp__scene_colorAdjust__) */
