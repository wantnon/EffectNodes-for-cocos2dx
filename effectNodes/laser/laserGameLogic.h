//
//  laserGameLogic.h
//  Zuma
//
//  Created by yang chao (wantnon) on 14-8-11.
//
//

#ifndef __Zuma__laserGameLogic__
#define __Zuma__laserGameLogic__

#include <iostream>
using namespace std;
#include "laserLayer.h"
class GamePlayScene;
class ClaserGameLogic:public CCObject{
public:
    ClaserGameLogic(GamePlayScene*gamePlayScene);
    virtual~ClaserGameLogic();
    void onStart();
    void onUpdate(float dt);
    void onEnd();
    void onUpdateLaserPos();
    bool getIsStarted()const {return m_isStarted;}
    void breakHitBall();
protected:
    void init_dft();
    void addNewLaser();
protected:
    GamePlayScene* m_gamePlayScene;
    ClaserLayer* m_laserLayer;
    bool m_isStarted;
    Ball* m_hitBallRef;
    CCParticleSystemQuad*m_spark;
    CCParticleSystemQuad*m_spark_outter;
    CCParticleSystemQuad*m_burst;
    CCSprite*m_smallGlow;


    
};

#endif /* defined(__Zuma__laserGameLogic__) */
