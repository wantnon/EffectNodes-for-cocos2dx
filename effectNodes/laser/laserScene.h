#ifndef __laserScene_H__
#define __laserScene_H__

#include "cocos2d.h"
using namespace cocos2d;
#include "laserLayer.h"
#include "common/ensCommon.h"
class Cscene_laser:public CCScene{
public:
    Cscene_laser(){};
    virtual~Cscene_laser(){}
    bool init(){
        ClaserLayer*layer=new ClaserLayer();
        layer->autorelease();
        layer->init();
        addChild(layer);
        return true;
    }
};

#endif // __laserScene_H__
