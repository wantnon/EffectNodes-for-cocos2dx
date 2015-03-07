//
//  layer_lightningBolt.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#include "lightningBolt/layer_lightningBolt.h"
#include "chooseDemoUI/chooseScene.h"

bool Clayer_lightningBolt::init(){
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCPoint start=CCPoint(winSize.width*189/1136,winSize.height/2);
    CCPoint end=CCPoint(winSize.width*947/1136,winSize.height/2);
    
    m_lbNode=new ClightningBoltNode();
    m_lbNode->autorelease();
    m_lbNode->init(start, end);
    addChild(m_lbNode);
    lightningBolt::CflashAction*flash=lightningBolt::CflashAction::create(-1,0.7);//-1 for repeat forever
    m_lbNode->runAction(flash);

	 //debug button
    {
        CCScale9Sprite* btnUp=CCScale9Sprite::create("uiRes/button.png");
        CCScale9Sprite* btnDn=CCScale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("debug", "Helvetica", 30);
        CCControlButton* controlButton=CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(origin.x+controlButton->getContentSize().width/2,winSize.height-150));
        controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&Clayer_lightningBolt::controlButtonEvent_debug), CCControlEventTouchDown);
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
        controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&Clayer_lightningBolt::controlButtonEvent_back), CCControlEventTouchUpInside);
        this->addChild(controlButton);
        m_controlButton_back=controlButton;
    }
    //title
    {
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("Lightning Bolt", "Arial", 45);
            
            // position the label on the center of the screen
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height));
            
            // add the label as a child to this layer
            this->addChild(pLabel, 1);
        }
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("2013-12-11", "Arial", 30);
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height-60));
            this->addChild(pLabel, 1);
        }
        
        
    }
    return true;
}
void Clayer_lightningBolt::controlButtonEvent_back(CCObject *senderz, CCControlEvent controlEvent){
    CchooseScene*scene=new CchooseScene();
    scene->autorelease();
    scene->init();
    CCDirector::sharedDirector()->replaceScene(scene);
}
void Clayer_lightningBolt::controlButtonEvent_debug(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    m_isDebugOn=!m_isDebugOn;
    
    if(m_isDebugOn){
        m_label_debugState->setString("debug: on") ;
        m_lbNode->setIsDrawDebug(true);
        
    }else{
        m_label_debugState->setString("debug: off") ;
        m_lbNode->setIsDrawDebug(false);
    }
    
}


