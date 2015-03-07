#ifndef __scene_splash_H__
#define __scene_splash_H__

#include "cocos2d.h"
#include "layer_splash.h"
using namespace cocos2d;

class Cscene_splash : public CCScene
{
public:
    virtual bool init();
public:
    Cscene_splash(){

    }
    virtual~Cscene_splash(){}

public:
    Clayer_splash*m_layer_splash;
    
 
};

#endif // __scene_splash_H__
