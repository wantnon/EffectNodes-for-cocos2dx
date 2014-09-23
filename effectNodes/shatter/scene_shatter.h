//
//  scene_shatter.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-17.
//
//

#ifndef __HelloCpp__scene_shatter__
#define __HelloCpp__scene_shatter__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "layer_shatter.h"
using namespace ens;

class Cscene_shatter:public CCScene{
public:
    Cscene_shatter(){};
    virtual~Cscene_shatter(){}
    bool init(){
        Clayer_shatter*layer=new Clayer_shatter();
        layer->autorelease();
        layer->init();
        addChild(layer);
        return true;
    }
};
#endif /* defined(__HelloCpp__scene_shatter__) */
