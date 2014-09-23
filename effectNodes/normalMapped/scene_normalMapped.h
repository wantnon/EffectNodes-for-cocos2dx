//
//  scene_normalMapped.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__scene_normalMapped__
#define __HelloCpp__scene_normalMapped__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "layer_normalMapped.h"
using namespace ens;
class Cscene_normalMapped:public CCScene{
public:
    Cscene_normalMapped(){};
    virtual~Cscene_normalMapped(){}
    bool init(){
        Clayer_normalMapped*layer=new Clayer_normalMapped();
        layer->autorelease();
        layer->init();
        addChild(layer);
        return true;
    }
};
#endif /* defined(__HelloCpp__scene_normalMapped__) */
