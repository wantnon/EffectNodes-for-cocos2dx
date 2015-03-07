//
//  layer_splash.h
//  HelloWorld
//
//  Created by yang chao (wantnon) on 14/12/15.
//
//

#ifndef __HelloWorld__layer_splash__
#define __HelloWorld__layer_splash__

#include <stdio.h>

#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace::extension;


#include "__multiSpotLightLayer.h"
#include "ensSplashLayer.h"



class Clayer_splash:public CCLayer{
public:
    Clayer_splash(){
        m_splashLayer=NULL;
      
    }
    virtual~Clayer_splash(){
        
    }
    bool init();
protected:
    //touch
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
protected:
    ControlButton* m_controlButton_back;
    void controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent);
protected:
    CensSplashLayer*m_splashLayer;

    
 


};

#endif /* defined(__HelloWorld__layer_splash__) */
