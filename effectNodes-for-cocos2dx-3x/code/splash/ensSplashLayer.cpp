//
//  ensSplashLayer.cpp
//  bubbleGame
//
//  Created by yang chao (wantnon) on 14-12-28.
//
//

#include "ensSplashLayer.h"
#include "layer_splash.h"
CensSplashLayer::CensSplashLayer()
{
    m_isDone=true;
    m_multiSpotLightLayer=NULL;
    


}
CensSplashLayer::~CensSplashLayer(){
    
}
void CensSplashLayer::onStart(const CCPoint&pos){

    assert(m_multiSpotLightLayer);

    m_multiSpotLightLayer->setVisible(true);
    
    m_isDone=false;
    
    vector<__Cspot> spotList;
    const int nSpot=30;
    for(int i=0;i<nSpot;i++){
        ccColor4F color=ccc4f(1, 1, 1, 1);
        __Cspot spot;
        float x=pos.x;
        float y=pos.y;
        CCPoint c=ccp(x,y) ;
        CCPoint velocity;
        float r;
        if(i==0){
            r=130;
        }else{
            r=ens::randomInRange(20, /*50*/80);//200;//ens::randomInRange(100, 200);
        }

        float A;
        if(i==0){
            A=0;
            velocity=ccp(0,0);
        }else{
            A=ens::randomInRange(0, 360);
            CCPoint dir=ccp(cosf(A*M_PI/180),sinf(A*M_PI/180));
            float speed=MIN(400,40+ens::randomInRange(30, 200)*1.0/r*100);
            velocity=dir*speed;
        }
        float damping;
        if(i==0){
            damping=0.99498743710662;
        }else{
            damping=ens::randomInRange(0.894427190999916, 0.948683298050514);
        }
                string texFileName;
        if(i==0){
            texFileName="demoRes/splash/splashSpot1.png";
        }else{
            texFileName="demoRes/splash/splashSpot2.png";
        }
        
        
        spot.init(c, r, color);
        spot.m_velocity=velocity;
        spot.m_damping=damping;
        spot.m_A=A;
        spot.m_texFileName=texFileName;
        spotList.push_back(spot);
        
    }
    
    m_multiSpotLightLayer->m_spotList=spotList;
    
    m_multiSpotLightLayer->getRenderMaskSprite()->m_highlight=1.0;
    
}

bool CensSplashLayer::init(){

    
    //m_multiSpotLightLayer
    m_multiSpotLightLayer=new __CmultiSpotLightLayer();
    m_multiSpotLightLayer->autorelease();
    m_multiSpotLightLayer->init();
    this->addChild(m_multiSpotLightLayer);
    m_multiSpotLightLayer->setVisible(false);
    m_multiSpotLightLayer->m_maskColor=ccc4f(0.15*0.5, 0.15*0.5, 0.15*0.5, 1.0);
    
   
 
    
    this->scheduleUpdate();

    return true;
    
}
void CensSplashLayer::update(float dt){
    if(m_isDone)return;
    

    __Cspot&centerSpot=m_multiSpotLightLayer->m_spotList[0];
    centerSpot.m_r=MAX(0,centerSpot.m_r-40*dt);

    const int nSpot=m_multiSpotLightLayer->m_spotList.size();
    bool isAllOthersOutOfCenterSpot=true;
    const float oneDivSixty=1.0/60;
    for(int i=1;i<nSpot;i++){
        __Cspot&spot=m_multiSpotLightLayer->m_spotList[i];
        spot.m_c+=spot.m_velocity*dt;
        if(i==0){
            const float f=dt/oneDivSixty;
        ////    cout<<"f1:"<<f<<endl;
            spot.m_r*=powf(spot.m_damping,f);
           
        }else{
            const float disFromCenter=ccpDistance(spot.m_c, centerSpot.m_c);
            if(disFromCenter>centerSpot.m_r){
                const float f=dt/oneDivSixty;
          ////      cout<<"f2:"<<f<<endl;
                spot.m_r*=powf(spot.m_damping,f);
                if(spot.m_r<15){
                    spot.m_r=0;
                }
            }
            if(i!=0&&disFromCenter+spot.m_r<centerSpot.m_r){
                isAllOthersOutOfCenterSpot=false;
                
            
            }
        }
        
    }
    if(isAllOthersOutOfCenterSpot){
        
        m_multiSpotLightLayer->getRenderMaskSprite()->m_highlight*=0.92;
   
    }
    
    

}

void CensSplashLayer::onEnd(){
    
    
    m_multiSpotLightLayer->setVisible(false);
    
    m_isDone=true;
}

