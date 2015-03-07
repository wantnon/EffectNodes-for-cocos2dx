//
//  layer_splash.cpp
//  HelloWorld
//
//  Created by yang chao (wantnon) on 14/12/15.
//
//

#include "layer_splash.h"
#include "chooseScene.h"

void Clayer_splash::controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent){
    CchooseScene*scene=new CchooseScene();
    scene->autorelease();
    scene->init();
    CCDirector::sharedDirector()->replaceScene(scene);
}

bool Clayer_splash::init(){
    this->CCLayer::init();
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    this->setTouchEnabled(true);
    
    
    CCSprite* backGround=CCSprite::create("demoRes/roughPaper.png");
    backGround->setPosition(ccp(winSize.width/2,winSize.height/2));
    this->addChild(backGround,-1);
    
    m_splashLayer=new CensSplashLayer();
    m_splashLayer->autorelease();
    m_splashLayer->init();
    this->addChild(m_splashLayer);
    m_splashLayer->onStart(ccp(winSize.width/2,winSize.height/2));
    
    //back button
    {
        Scale9Sprite* btnUp=Scale9Sprite::create("uiRes/button.png");
        Scale9Sprite* btnDn=Scale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("back", "Helvetica", 30);
        ControlButton* controlButton=ControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,Control::State::HIGH_LIGHTED);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(origin.x+visibleSize.width-controlButton->getContentSize().width/2,origin.y+controlButton->getContentSize().height/2));
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_splash::controlButtonEvent_back), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(controlButton);
        m_controlButton_back=controlButton;
    }
    
    //title
    {
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("Splash", "Arial", 45);
            
            // position the label on the center of the screen
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height));
            
            // add the label as a child to this layer
            this->addChild(pLabel, 1);
        }
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("2015-3-2", "Arial", 30);
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height-60));
            this->addChild(pLabel, 1);
        }
        
        
    }
    
    return true;
}
void Clayer_splash::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
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
        
        
        
        
        
        
        break;
        
    }
    
    
    
}

void Clayer_splash::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCTouch* touch;
    for(auto it = touches.begin(); it != touches.end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint loc_winSpace = touch->getLocationInView();
        CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
        
        
        
        
        
        break;
        
        
    }
    
}

void Clayer_splash::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCTouch* touch;
    for( auto it = touches.begin(); it != touches.end(); it++)
    {
        touch = (CCTouch*)(*it);
        if(!touch)
            break;
        
        CCPoint loc_winSpace = touch->getLocationInView();
        CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
        
        
        m_splashLayer->onStart(loc_GLSpace);
        
        
        break;
        
        
        
        
        
    }
}