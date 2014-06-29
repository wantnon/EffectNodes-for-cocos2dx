//
//  scene_tinyWingsTerrain.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-17.
//
//

#ifndef __HelloCpp__scene_tinyWingsTerrain__
#define __HelloCpp__scene_tinyWingsTerrain__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "layer_tinyWingsTerrain.h"
using namespace ens;

class Cscene_tinyWingsTerrain:public CCScene{
public:
    Cscene_tinyWingsTerrain(){};
    virtual~Cscene_tinyWingsTerrain(){}
    bool init(){
        Clayer_tinyWingsTerrain*layer=new Clayer_tinyWingsTerrain();
        layer->autorelease();
        layer->init();
        addChild(layer);
        return true;
    }
};
#endif /* defined(__HelloCpp__scene_tinyWingsTerrain__) */
