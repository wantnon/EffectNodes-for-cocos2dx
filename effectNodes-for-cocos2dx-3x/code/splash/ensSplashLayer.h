//
//  ensSplashLayer.h
//  bubbleGame
//
//  Created by yang chao (wantnon) on 14-12-28.
//
//

#ifndef HelloWorld__ensSplashLayer__
#define HelloWorld__ensSplashLayer__

#include <iostream>
using namespace std;
#include "__multiSpotLightLayer.h"
class CensSplashLayer:public CCLayer{
public:
    CensSplashLayer();
    virtual~CensSplashLayer();
    void onStart(const CCPoint&pos);
    void update(float dt);
    void onEnd();
    bool getIsDone(){return m_isDone;}
    bool init();

protected:
    __CmultiSpotLightLayer* m_multiSpotLightLayer;
    bool m_isDone;


    
};

#endif /* defined(HelloWorld__splashLayer__) */
