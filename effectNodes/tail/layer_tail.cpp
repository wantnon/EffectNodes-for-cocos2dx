//
//  layer_tail.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//
#include "tail/layer_tail.h"
#include "chooseDemoUI/chooseScene.h"

bool Clayer_tail::init()
{
  	setTouchEnabled( true );
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    //debug button
    {
        CCScale9Sprite* btnUp=CCScale9Sprite::create("uiRes/button.png");
        CCScale9Sprite* btnDn=CCScale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("debug", "Helvetica", 30);
        CCControlButton* controlButton=CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(origin.x+controlButton->getContentSize().width/2,winSize.height-150));
        controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&Clayer_tail::controlButtonEvent_debug), CCControlEventTouchDown);
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
        CCScale9Sprite* btnUp=CCScale9Sprite::create("uiRes/button.png");
        CCScale9Sprite* btnDn=CCScale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("back", "Helvetica", 30);
        CCControlButton* controlButton=CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(origin.x+visibleSize.width-controlButton->getContentSize().width/2,origin.y+controlButton->getContentSize().height/2));
        controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&Clayer_tail::controlButtonEvent_back), CCControlEventTouchUpInside);
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


void Clayer_tail::controlButtonEvent_debug(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    m_isDebugOn=!m_isDebugOn;

    if(m_isDebugOn){
        m_label_debugState->setString("debug: on") ;
        int nTailSprite=(int)m_tailSpriteList.size();
        for(int i=0;i<nTailSprite;i++){
            CtailSprite*tailSprite=m_tailSpriteList[i];
            tailSprite->setIsShowWire(true);
        }
    }else{
        m_label_debugState->setString("debug: off") ;
        int nTailSprite=(int)m_tailSpriteList.size();
        for(int i=0;i<nTailSprite;i++){
            CtailSprite*tailSprite=m_tailSpriteList[i];
            tailSprite->setIsShowWire(false);
        }
    }
        
}

void Clayer_tail::controlButtonEvent_back(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
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
void Clayer_tail::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint loc_winSpace = touch->getLocationInView();
        CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
        

    }
}
void Clayer_tail::ccTouchesMoved(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSetIterator it;
    CCTouch* touch;
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint loc_winSpace = touch->getLocationInView();
        CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
        

         if(m_tailSpriteList.empty()==false)m_tailSpriteList[(int)m_tailSpriteList.size()-1]->setPosition(loc_GLSpace);
		
    }
    
}

void Clayer_tail::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSetIterator it;
    CCTouch* touch;
	for( it = touches->begin(); it != touches->end(); it++)
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
        tailSprite->setIsShowWire(m_isDebugOn);
        m_tailSpriteList.push_back(tailSprite);
        
        
        
    }
}

