//
//  layer_tail.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//
#include "layer_tail.h"
#include "chooseScene.h"

bool Clayer_tail::init()
{
  	setTouchEnabled( true );
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    //debug button
    {
        Scale9Sprite* btnUp=Scale9Sprite::create("uiRes/button.png");
        Scale9Sprite* btnDn=Scale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("debug", "Helvetica", 30);
        ControlButton* controlButton=ControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,Control::State::HIGH_LIGHTED);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(origin.x+controlButton->getContentSize().width/2,winSize.height-150));
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_tail::controlButtonEvent_debug), Control::EventType::TOUCH_DOWN);
        this->addChild(controlButton);
        m_controlButton_debug=controlButton;
        //label
        m_label_debugState = CCLabelTTF::create("debug: off", "Arial", 30);
        m_label_debugState->setPosition(ccp(m_controlButton_debug->getPositionX()+m_controlButton_debug->getContentSize().width/2+m_label_debugState->getContentSize().width/2+10,
                                            m_controlButton_debug->getPositionY()));
        this->addChild(m_label_debugState, 1);
    }
    //back button
    {
        Scale9Sprite* btnUp=Scale9Sprite::create("uiRes/button.png");
        Scale9Sprite* btnDn=Scale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("back", "Helvetica", 30);
        ControlButton* controlButton=ControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,Control::State::HIGH_LIGHTED);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(origin.x+visibleSize.width-controlButton->getContentSize().width/2,origin.y+controlButton->getContentSize().height/2));
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_tail::controlButtonEvent_back), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(controlButton);
        m_controlButton_back=controlButton;
    }

    //title
    {
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("Tail Effect", "Arial", 45);
            
            // position the label on the center of the screen
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height));
            
            // add the label as a child to this layer
            this->addChild(pLabel, 1);
        }
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("2014-6-6", "Arial", 30);
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height-60));
            this->addChild(pLabel, 1);
        }
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("touch and move...", "Arial", 30);
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height-120));
            this->addChild(pLabel, 1);
        }
        
    }

    return true;
}


void Clayer_tail::controlButtonEvent_debug(Ref *senderz, Control::EventType controlEvent){
    m_isDebugOn=!m_isDebugOn;

    if(m_isDebugOn){
        m_label_debugState->setString("debug: on") ;
        int nTailSprite=(int)m_tailSpriteList.size();
        for(int i=0;i<nTailSprite;i++){
            CtailSprite*tailSprite=m_tailSpriteList[i];
            tailSprite->setIsDrawDebug(true);
        }
    }else{
        m_label_debugState->setString("debug: off") ;
        int nTailSprite=(int)m_tailSpriteList.size();
        for(int i=0;i<nTailSprite;i++){
            CtailSprite*tailSprite=m_tailSpriteList[i];
            tailSprite->setIsDrawDebug(false);
        }
    }
        
}

void Clayer_tail::controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent){
    CchooseScene*scene=new CchooseScene();
    scene->autorelease();
    scene->init();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void Clayer_tail::update(float dt){
    //remove empty tailSprites except the last one
    int nTailSprite=(int)m_tailSpriteList.size();
    for(int i=0;i<nTailSprite-1;i++){
        CtailSprite*tailSprite=m_tailSpriteList[i];
        if(tailSprite->getIsEmpty()){
            this->removeChild(tailSprite);
            m_tailSpriteList.erase(m_tailSpriteList.begin()+i);
            i--;
            nTailSprite--;
        }
    }
}
void Clayer_tail::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
     
    CCTouch* touch;
    
    for(auto  it = touches.begin(); it != touches.end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint loc_winSpace = touch->getLocationInView();
        CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
        

        break;//back soon
    }
}
void Clayer_tail::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
     
    CCTouch* touch;
    for(auto  it = touches.begin(); it != touches.end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint loc_winSpace = touch->getLocationInView();
        CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
        

         if(m_tailSpriteList.empty()==false)m_tailSpriteList[(int)m_tailSpriteList.size()-1]->setPosition(loc_GLSpace);
		
    }
    
}

void Clayer_tail:: onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
     
    CCTouch* touch;
	for(auto  it = touches.begin(); it != touches.end(); it++)
    {
        touch = (CCTouch*)(*it);
        if(!touch)
            break;
        
        CCPoint loc_winSpace = touch->getLocationInView();
        CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
        //CCLOG("loc_GLSpace:%f,%f",loc_GLSpace.x,loc_GLSpace.y);

        CtailSprite* tailSprite=new CtailSprite();
        tailSprite->autorelease();
        tailSprite->init("demoRes/quad_dot.png");
        tailSprite->setAnchorPoint(ccp(0.5,0.5));
        tailSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
        tailSprite->setMinDis(14);
        this->addChild(tailSprite);
        tailSprite->scheduleUpdate();
        tailSprite->setPosition(loc_GLSpace);
        tailSprite->setIsDrawDebug(m_isDebugOn);
        m_tailSpriteList.push_back(tailSprite);
        
        break;//back soon
        
    }
}

