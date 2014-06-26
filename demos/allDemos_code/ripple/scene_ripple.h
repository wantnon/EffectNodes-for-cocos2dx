//
//  scene_ripple.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-17.
//
//

#ifndef __HelloCpp__scene_ripple__
#define __HelloCpp__scene_ripple__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "layer_ripple.h"
using namespace ens;

class Cscene_ripple:public CCScene{
public:
    Cscene_ripple(){};
    virtual~Cscene_ripple(){}
    bool init(){
        Clayer_ripple*layer=new Clayer_ripple();
        layer->autorelease();
        layer->init();
        addChild(layer);
        return true;
    }
};
#endif /* defined(__HelloCpp__scene_ripple__) */
