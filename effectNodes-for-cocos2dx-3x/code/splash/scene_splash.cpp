#include "scene_splash.h"
#include "CCControlButton.h"
using namespace cocos2d;

bool Cscene_splash::init()
{
    


  
    m_layer_splash=new Clayer_splash();
    m_layer_splash->autorelease();
    m_layer_splash->init();
    addChild(m_layer_splash);
    
       
    

    
    
    return true;
}


