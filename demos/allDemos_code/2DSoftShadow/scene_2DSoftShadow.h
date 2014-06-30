//
//  scene_2DSoftShadow.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-17.
//
//

#ifndef __HelloCpp__scene_2DSoftShadow__
#define __HelloCpp__scene_2DSoftShadow__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "layer_2DSoftShadow.h"
using namespace ens;

class Cscene_2DSoftShadow:public CCScene{
public:
    Cscene_2DSoftShadow(){};
    virtual~Cscene_2DSoftShadow(){}
    bool init(){
        Clayer_2DSoftShadow*layer=new Clayer_2DSoftShadow();
        layer->autorelease();
        layer->init();
        addChild(layer);
        return true;
    }
};
#endif /* defined(__HelloCpp__scene_2DSoftShadow__) */
