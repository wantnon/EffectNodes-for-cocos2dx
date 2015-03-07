//
//  chooseLayer.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#include "chooseLayer.h"
#include "scene_lightningBolt.h"
#include "scene_tail.h"
#include "scene_ghostlike.h"
#include "scene_ripple.h"
#include "scene_break.h"
#include "scene_colorAdjust.h"
#include "scene_normalMapped.h"
#include "scene_shatter.h"
#include "scene_ripple_horizontal.h"
#include "scene_tinyWingsTerrain.h"
#include "scene_2DSoftShadow.h"
#include "laserScene.h";
#include "scene_splash.h"
bool CchooseLayer::init(){
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    //enable touch
	setTouchEnabled( true );
    //hide frame rate
    CCDirector::sharedDirector()->setDisplayStats(false);
    
    //sinSize
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize
    //head line
    {
        CCLabelTTF* title = CCLabelTTF::create("Effect Nodes Demos", "Arial", 50);
        title->setPosition(ccp(winSize.width/2,winSize.height-50));
        this->addChild(title, 1);
    }
    
    m_demoNameList.push_back("lightningBolt");
    m_demoNameList.push_back("tail");
    m_demoNameList.push_back("ghost-like");
    m_demoNameList.push_back("ripple");
    m_demoNameList.push_back("ripple_horizontal");
    m_demoNameList.push_back("break");
    m_demoNameList.push_back("colorAdjust");
    m_demoNameList.push_back("normalMapped");
    m_demoNameList.push_back("shatter");
    m_demoNameList.push_back("tinyWingsTerrain");
    m_demoNameList.push_back("2DSoftShadow");
    m_demoNameList.push_back("laser");
    m_demoNameList.push_back("splash");
    //head line height
    float headlineHeight=80;
    //button size and margin
    CCSize buttonSize=CCSize(140,140);
    CCSize buttonMargin=CCSize(20,40);
    CCSize buttonSizeEx=CCSize(buttonSize.width+buttonMargin.width,buttonSize.height+buttonMargin.height);
    //how many Button one row
    int nButtonPerRow=floorf(visibleSize.width/buttonSizeEx.width);
    //controlButton
    int nDemo=(int)m_demoNameList.size();
    for(int i=0;i<nDemo;i++)
    {
        int rowID=i/nButtonPerRow;
        int colID=i%nButtonPerRow;
        float x=origin.x+buttonSizeEx.width*colID+buttonSizeEx.width/2;
        float y=origin.y+visibleSize.height-(buttonSizeEx.height*rowID+buttonSizeEx.height/2)-headlineHeight;
        string demoName=m_demoNameList[i];
        Scale9Sprite* btnUp=Scale9Sprite::create("uiRes/button.png");
        Scale9Sprite* btnDn=Scale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("", "Helvetica", 30);
        ControlButton* controlButton=ControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,Control::State::HIGH_LIGHTED);
        controlButton->setPreferredSize(buttonSize);
        controlButton->setPosition(ccp(x,y));
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CchooseLayer::controlButtonEvent), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(controlButton);
        m_controlButtonList.push_back(controlButton);
        CCLabelTTF*label=CCLabelTTF::create(demoName.c_str(), "Arial", 18);
        addChild(label);
        label->setPosition(ccp(x,y-controlButton->getContentSize().height/2-label->getContentSize().height/2));
        //thumbnail
        CCSprite*thumbnail=CCSprite::create((string("screenshots/")+demoName+".png").c_str());
        thumbnail->setScale((1-0.05)*MIN(controlButton->getContentSize().width/thumbnail->getContentSize().width,
                                controlButton->getContentSize().height/thumbnail->getContentSize().height));
        thumbnail->setPosition(ccp(x,y));
        addChild(thumbnail);
        
        
    }
    
    return true;
    
}

void CchooseLayer::controlButtonEvent(Ref *senderz, Control::EventType controlEvent){
    int index=-1;
    int nButton=(int)m_controlButtonList.size();
    for(int i=0;i<nButton;i++){
        ControlButton*button=m_controlButtonList[i];
        if(button==senderz){
            index=i;
            break;
        }
    }
    assert(index!=-1);
    string demoName=m_demoNameList[index];
    //cout<<"demoName:"<<demoName<<endl;
    if(demoName=="lightningBolt"){
        Cscene_lightningBolt *scene=new Cscene_lightningBolt();
        scene->autorelease();
        scene->init();
        CCDirector::sharedDirector()->replaceScene(scene);
    }else if(demoName=="tail"){
        Cscene_tail *scene=new Cscene_tail();
        scene->autorelease();
        scene->init();
        CCDirector::sharedDirector()->replaceScene(scene);
    }else if(demoName=="colorAdjust"){
        Cscene_colorAdjust *scene=new Cscene_colorAdjust();
        scene->autorelease();
        scene->init();
        CCDirector::sharedDirector()->replaceScene(scene);
    
    }else if(demoName=="ghost-like"){
        Cscene_ghostlike *scene=new Cscene_ghostlike();
        scene->autorelease();
        scene->init();
        CCDirector::sharedDirector()->replaceScene(scene);
        
    }else if(demoName=="ripple"){
        Cscene_ripple *scene=new Cscene_ripple();
        scene->autorelease();
        scene->init();
        CCDirector::sharedDirector()->replaceScene(scene);
        
    }else if(demoName=="break"){
        Cscene_break *scene=new Cscene_break();
        scene->autorelease();
        scene->init();
        CCDirector::sharedDirector()->replaceScene(scene);
    }else if(demoName=="normalMapped"){
        Cscene_normalMapped *scene=new Cscene_normalMapped();
        scene->autorelease();
        scene->init();
        CCDirector::sharedDirector()->replaceScene(scene);
    
    }else if(demoName=="shatter"){
        Cscene_shatter *scene=new Cscene_shatter();
        scene->autorelease();
        scene->init();
        CCDirector::sharedDirector()->replaceScene(scene);
    }else if(demoName=="ripple_horizontal"){
        Cscene_ripple_horizontal *scene=new Cscene_ripple_horizontal();
        scene->autorelease();
        scene->init();
        CCDirector::sharedDirector()->replaceScene(scene);
    }else if(demoName=="tinyWingsTerrain"){
        Cscene_tinyWingsTerrain *scene=new Cscene_tinyWingsTerrain();
        scene->autorelease();
        scene->init();
        CCDirector::sharedDirector()->replaceScene(scene);
        
    }else if(demoName=="2DSoftShadow"){
        Cscene_2DSoftShadow *scene=new Cscene_2DSoftShadow();
        scene->autorelease();
        scene->init();
        CCDirector::sharedDirector()->replaceScene(scene);
        
    }else if(demoName=="laser"){
        Cscene_laser *scene=new Cscene_laser();
        scene->autorelease();
        scene->init();
        CCDirector::sharedDirector()->replaceScene(scene);
    }else if(demoName=="splash"){
        Cscene_splash *scene=new Cscene_splash();
        scene->autorelease();
        scene->init();
        CCDirector::sharedDirector()->replaceScene(scene);
    }else{
        assert(false);
    }
    
}