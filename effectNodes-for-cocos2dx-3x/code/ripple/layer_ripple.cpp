#include "layer_ripple.h"
#include "chooseScene.h"


bool Clayer_ripple::init()
{
    this->setTouchEnabled(true);
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    
	m_rippleSprite=new CrippleSprite();
    m_rippleSprite->autorelease();
	m_rippleSprite->init("demoRes/frozen_small.png",8);
	m_rippleSprite->setPosition(ccp(winSize.width/2,winSize.height/2));
    m_rippleSprite->scheduleUpdate();
    addChild(m_rippleSprite);

  
    //debug button
    {
        Scale9Sprite* btnUp=Scale9Sprite::create("uiRes/button.png");
        Scale9Sprite* btnDn=Scale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("debug", "Helvetica", 30);
        ControlButton* controlButton=ControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,Control::State::HIGH_LIGHTED);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(origin.x+controlButton->getContentSize().width/2,winSize.height-150));
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_ripple::controlButtonEvent_debug), Control::EventType::TOUCH_DOWN);
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
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_ripple::controlButtonEvent_back), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(controlButton);
        m_controlButton_back=controlButton;
    }
    //title
    {
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("Ripple", "Arial", 45);
            
            // position the label on the center of the screen
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height));
            
            // add the label as a child to this layer
            this->addChild(pLabel, 1);
        }
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("2013-10-20", "Arial", 30);
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height-60));
            this->addChild(pLabel, 1);
        }

        
    }
	
    return true;
}
void Clayer_ripple::controlButtonEvent_debug(Ref *senderz, Control::EventType controlEvent){
    m_isDebugOn=!m_isDebugOn;
    
    if(m_isDebugOn){
        m_label_debugState->setString("debug: on") ;
        m_rippleSprite->setIsDrawDebug(true);
        
    }else{
        m_label_debugState->setString("debug: off") ;
        m_rippleSprite->setIsDrawDebug(false);
    }
    
}

void Clayer_ripple::controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent){
    CchooseScene*scene=new CchooseScene();
    scene->autorelease();
    scene->init();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void Clayer_ripple::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
    //Add a new body/atlas sprite at the touched location
     
    CCTouch* touch;
    
    for(auto  it = touches.begin(); it != touches.end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        
        location = CCDirector::sharedDirector()->convertToGL(location);
        //    cout<<"mos pos:"<<location.x<<" "<<location.y<<endl;
        break;
    }
}
void Clayer_ripple::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
     
    CCTouch* touch;
    for(auto  it = touches.begin(); it != touches.end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        
        location = CCDirector::sharedDirector()->convertToGL(location);
        //    cout<<"mos pos:"<<location.x<<" "<<location.y<<endl;
		m_rippleSprite->doTouch(location, 512, 12);
      //  break;
    }
    
}

void Clayer_ripple:: onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
     
    CCTouch* touch;
	for(auto  it = touches.begin(); it != touches.end(); it++)
    {
        touch = (CCTouch*)(*it);
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        
        location = CCDirector::sharedDirector()->convertToGL(location);
        //  cout<<"mos pos:"<<location.x<<" "<<location.y<<endl;
		m_rippleSprite->doTouch(location, 512, 12);
     //   break;

    }
}
