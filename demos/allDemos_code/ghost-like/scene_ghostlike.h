//
//  scene_ghostlike.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__scene_ghostlike__
#define __HelloCpp__scene_ghostlike__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "layer_ghostlike.h"
using namespace ens;
class Cscene_ghostlike:public CCScene{
public:
    Cscene_ghostlike(){};
    virtual~Cscene_ghostlike(){}
    bool init(){
        Clayer_ghostlike*layer=new Clayer_ghostlike();
        layer->autorelease();
        layer->init();
        addChild(layer);
        return true;
    }
};
#endif /* defined(__HelloCpp__scene_ghostlike__) */
