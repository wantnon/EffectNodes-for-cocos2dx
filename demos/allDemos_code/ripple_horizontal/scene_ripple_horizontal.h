//
//  scene_ripple_horizontal.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-17.
//
//

#ifndef __HelloCpp__scene_ripple_horizontal__
#define __HelloCpp__scene_ripple_horizontal__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "layer_ripple_horizontal.h"
using namespace ens;

class Cscene_ripple_horizontal:public CCScene{
public:
    Cscene_ripple_horizontal(){};
    virtual~Cscene_ripple_horizontal(){}
    bool init(){
        Clayer_ripple_horizontal*layer=new Clayer_ripple_horizontal();
        layer->autorelease();
        layer->init();
        addChild(layer);
        return true;
    }
};
#endif /* defined(__HelloCpp__scene_ripple_horizontal__) */
