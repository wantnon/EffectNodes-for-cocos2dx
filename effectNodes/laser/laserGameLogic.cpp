//
//  laserGameLogic.cpp
//  Zuma
//
//  Created by yang chao (wantnon) on 14-8-11.
//
//

#include "laserGameLogic.h"
#include "GamePlayScene.h"
#include "PlaySceneZorder.h"
#include "BallsManager.h"
ClaserGameLogic::ClaserGameLogic(GamePlayScene*gamePlayScene):
m_laserLayer(NULL)
{
    m_gamePlayScene=gamePlayScene;
    m_gamePlayScene->retain();
    init_dft();

}
ClaserGameLogic::~ClaserGameLogic(){
    if(m_gamePlayScene)m_gamePlayScene->release();
    
}
void ClaserGameLogic::onStart(){
    assert(m_laserLayer);
    m_laserLayer->setVisible(true);
    m_isStarted=true;
    //spark_outter
    m_spark_outter->resetSystem();
    m_spark_outter->setVisible(true);
    //spark
    m_spark->resetSystem();
    m_spark->setVisible(true);
    //burst
    m_burst->stopAllActions();
    m_burst->stopSystem();
    m_burst->setVisible(false);
    //smallGlow
    m_smallGlow->stopAllActions();
    m_smallGlow->setVisible(true);

    
}

void ClaserGameLogic::addNewLaser(){
    ClaserLayer*laserLayer=new ClaserLayer();
    laserLayer->autorelease();
    laserLayer->init();
    m_gamePlayScene->addChild(laserLayer,kPlaySceneZorderLaser);
    m_laserLayer=laserLayer;
    laserLayer->setVisible(false);
    
}
void ClaserGameLogic::init_dft(){
    m_hitBallRef=NULL;
    m_isStarted=false;

    //create spark_outter
    m_spark_outter=CCParticleSystemQuad::create("laserRes/spark.plist");
    m_gamePlayScene->addChild(m_spark_outter,kPlaySceneZorderLaser);
    m_spark_outter->stopSystem();
    m_spark_outter->setVisible(false);
    {
        ccBlendFunc blendFunc={GL_ONE,GL_ONE_MINUS_SRC_ALPHA};
        m_spark_outter->setBlendFunc(blendFunc);
    }
    m_spark_outter->setScale(1.5);
    m_spark_outter->setPositionType(kCCPositionTypeGrouped);
    //pre add laser
    if(m_laserLayer==NULL){
        addNewLaser();
        m_laserLayer->setVisible(false);
        m_laserLayer->setTouchEnabled(false);
    }
    //create spark
    m_spark=CCParticleSystemQuad::create("laserRes/spark.plist");
    m_gamePlayScene->addChild(m_spark,kPlaySceneZorderLightningBolt);
    m_spark->stopSystem();
    m_spark->setVisible(false);
    {
        ccBlendFunc blendFunc={GL_ONE,GL_ONE};
        m_spark->setBlendFunc(blendFunc);
    }
    m_spark->setScale(1.1);
    m_spark->setPositionType(kCCPositionTypeGrouped);
    //create burst
    m_burst=CCParticleSystemQuad::create("laserRes/burst.plist");
    m_gamePlayScene->addChild(m_burst,kPlaySceneZorderLightningBolt);
    m_burst->stopSystem();
    m_burst->setVisible(false);
    //create smallGlow
    m_smallGlow=CCSprite::create();
    m_smallGlow->initWithFile("laserRes/smallGlow.png");
    m_gamePlayScene->addChild(m_smallGlow,kPlaySceneZorderLightningBolt);
    m_smallGlow->setVisible(false);
    m_smallGlow->setScale(1.2);
    m_smallGlow->setColor(ccc3(255, 128, 0));
    {
        ccBlendFunc blendFunc={GL_ONE,GL_ONE};
        m_smallGlow->setBlendFunc(blendFunc);
    
    }
}
void ClaserGameLogic::breakHitBall(){
    if(m_hitBallRef){
        m_hitBallRef->markToBeBursted(true);
        m_burst->resetSystem();
        m_burst->setVisible(true);

    }
}
void ClaserGameLogic::onUpdateLaserPos(){
    if(m_laserLayer->isVisible()==false)return;
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    float winWidth = visibleSize.width;
    float winHeight = visibleSize.height;
    CCPoint start,end;
    {
        CCSprite* shooter=m_gamePlayScene->getShooter();
        vector<Ball*> hitBallList;
        vector<Ball*> balls=m_gamePlayScene->getAllBalls();
        //collision test with balls
        int nBalls=(int)balls.size();
        for(int i=0;i<nBalls;i++){
            Ball*ball=balls[i];
            float ballR=ball->circumference()/2;//note: circumference is 2*r
            CCPoint ballPos=ball->getPosition();
            CCPoint shooterPos=shooter->getPosition();
            if(fabsf(ballPos.x-shooterPos.x)<=ballR){
                hitBallList.push_back(ball);
            }
        }//got hitBallList
        //find the hitBall
        Ball*hitBall=NULL;
        float minY=INFINITY;
        int nHitBall=(int)hitBallList.size();
        for(int i=0;i<nHitBall;i++){
            Ball*ball=hitBallList[i];
            if(ball->getPosition().y<minY){
                minY=ball->getPosition().y;
                hitBall=ball;
            }
        }//got hitBall
        if(hitBall){
            m_hitBallRef=hitBall;
            end=CCPoint(shooter->getPosition().x,hitBall->getPosition().y);
        }else{
            m_hitBallRef=NULL;
            end=CCPoint(shooter->getPosition().x,winHeight);
        }
        start=shooter->getPosition();
        m_laserLayer->m_laser->setStart(start);
        m_laserLayer->m_laser->setEnd(end);
        //m_spark position
        if(hitBall){
            const float r=hitBall->circumference()/2;//note: circumference is 2*r
            const float d=fabsf(hitBall->getPosition().x-shooter->getPosition().x);
            m_spark->setPosition(end-ccp(0,sqrtf(r*r-d*d)));
            m_spark_outter->setPosition(m_spark->getPosition());
            m_smallGlow->setPosition(m_spark->getPosition()+ccp(0,r*0.5));
        }else{
            m_spark->setPosition(end);
            m_spark_outter->setPosition(m_spark->getPosition());
            const float r=hitBall->circumference()/2;
            m_smallGlow->setPosition(m_spark->getPosition()+ccp(0,r*0.5));
            
        }
        //m_burst position
        m_burst->setPosition(end);
       
        
    }

}
void ClaserGameLogic::onUpdate(float dt){
    if(m_burst->getParticleCount()==0){
        m_burst->setVisible(false);
        //cout<<"hihi"<<endl;
    }else{
        //cout<<"haha"<<endl;
    }
}
void ClaserGameLogic::onEnd(){
    m_laserLayer->setVisible(false);
    m_spark->stopSystem();
    m_spark->setVisible(false);
    m_spark_outter->stopSystem();
    m_spark_outter->setVisible(false);
    m_smallGlow->setVisible(false);
    m_isStarted=false;

}
