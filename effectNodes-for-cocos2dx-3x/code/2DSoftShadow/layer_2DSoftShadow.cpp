#include "layer_2DSoftShadow.h"
#include "chooseScene.h"

bool Clayer_2DSoftShadow::init()
{
    this->setTouchEnabled(true);
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
   
       
    m_backGround=CCSprite::create("demoRes/tableCloth.png");
     this->addChild(m_backGround);
     m_backGround->setPosition(ccp(winSize.width/2,winSize.height/2));
  ////  m_backGround->setVisible(false);

    
    
    m_shadowRoot=new C2DSoftShadowRoot();
    m_shadowRoot->autorelease();
    m_shadowRoot->init();
    addChild(m_shadowRoot);
    
    
    
    m_light=new ClightNode();
    m_light->autorelease();
    m_light->init(48);
    m_shadowRoot->setLight(m_light);
    m_light->setPosition(ccp(winSize.width/4*3,winSize.height/3*2));
    
    m_shadowObj=new C2DSoftShadowObj();
    m_shadowObj->autorelease();
    m_shadowObj->init(makeRegularPolygon(84,8));
    m_shadowObj->setLight(m_light);
    m_shadowRoot->addObj(m_shadowObj);
    m_shadowObj->setPosition(ccp(winSize.width/2,winSize.height/2));
    
    m_shadowObj2=new C2DSoftShadowObj();
    m_shadowObj2->autorelease();
    m_shadowObj2->init(makeRegularPolygon(20, 8));
    m_shadowObj2->setLight(m_light);
    m_shadowRoot->addObj(m_shadowObj2);
    m_shadowObj2->setPosition(ccp(winSize.width/3*2,winSize.height/3));
    
    m_shadowObj3=new C2DSoftShadowObj();
    m_shadowObj3->autorelease();
    m_shadowObj3->init(makeRectPolygon(32,47.5));
    m_shadowObj3->setLight(m_light);
    m_shadowRoot->addObj(m_shadowObj3);
    m_shadowObj3->setPosition(ccp(winSize.width/2,winSize.height/6*5));


    m_sprite=CCSprite::create("demoRes/dish.png");
    addChild(m_sprite,20);
    m_sprite->setRotation(360.0/8/2);
    m_sprite->setPosition(ccp(winSize.width/2,winSize.height/2));
    
    m_sprite2=CCSprite::create("demoRes/chessPiece.png");
    addChild(m_sprite2,20);
    m_sprite2->setPosition(ccp(winSize.width/3*2,winSize.height/3));
    
    m_sprite3=CCSprite::create("demoRes/cigaretteBox.png");
    addChild(m_sprite3,20);
    m_sprite3->setPosition(ccp(winSize.width/2,winSize.height/6*5));
    
    m_sprite_light=CCSprite::create("demoRes/light2.png");
    addChild(m_sprite_light,20);
    m_sprite_light->setPosition(ccp(winSize.width/4*3,winSize.height/3*2));
    m_sprite_light->setScale(4.5*m_light->getR()/(m_sprite_light->getContentSize().width/2));
    ccBlendFunc blendFunc={GL_ONE,GL_ONE};
    m_sprite_light->setBlendFunc(blendFunc);
    
    //slider
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		ControlSlider *slider = ControlSlider::create("uiRes/sliderTrack.png",
                                                          "uiRes/sliderProgress.png" ,
                                                          "uiRes/sliderThumb.png");
		slider->setAnchorPoint(ccp(0.5f, 0.5f));
		slider->setMinimumValue(0); // Sets the min value of range
		slider->setMaximumValue(1); // Sets the max value of range
		slider->setValue(m_shadowRoot->getShadowDarkness());
		slider->setPosition(ccp(winSize.width *0.5, 80));
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_2DSoftShadow::sliderAction), Control::EventType::VALUE_CHANGED);
		m_pSliderCtl=slider;
		addChild(m_pSliderCtl,100);
		//title
		CCLabelTTF* pLabel = CCLabelTTF::create("shadowDark ", "Arial", 40);
		pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
        this->addChild(pLabel, 1);
        //label
        m_label=CCLabelTTF::create(valueToStr(m_shadowRoot->getShadowDarkness()).c_str(), "Arial", 40);
        m_label->setPosition(ccp(20+slider->getPositionX()+slider->getContentSize().width/2+m_label->getContentSize().width/2, slider->getPositionY()));
        this->addChild(m_label);
        
	}
    //debug button
    {
        Scale9Sprite* btnUp=Scale9Sprite::create("uiRes/button.png");
        Scale9Sprite* btnDn=Scale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("debug", "Helvetica", 30);
        ControlButton* controlButton=ControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,Control::State::HIGH_LIGHTED);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(origin.x+controlButton->getContentSize().width/2,winSize.height-150));
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_2DSoftShadow::controlButtonEvent_debug), Control::EventType::TOUCH_DOWN);
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
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_2DSoftShadow::controlButtonEvent_back), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(controlButton);
        m_controlButton_back=controlButton;
    }
    //instruction
    {
        CCLabelTTF* pLabel = CCLabelTTF::create("move the light...", "Arial", 30);
        
        // position the label on the center of the screen
        pLabel->setPosition(ccp(winSize.width *0.25, winSize.height/3));
        
        // add the label as a child to this layer
        this->addChild(pLabel, 100);
    }
    //title
    {

        {
            CCLabelTTF* pLabel = CCLabelTTF::create("2D Soft Shadow", "Arial", 45);
            
            // position the label on the center of the screen
            pLabel->setPosition(ccp(origin.x +pLabel->getContentSize().width/2+30,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height));
            
            // add the label as a child to this layer
            this->addChild(pLabel, 100);

        }
        {
            {
                CCLabelTTF* pLabel = CCLabelTTF::create("2014-7-6", "Arial", 30);
                pLabel->setPosition(ccp(origin.x +pLabel->getContentSize().width/2+30,
                                        origin.y + visibleSize.height - pLabel->getContentSize().height-60));
                this->addChild(pLabel, 100);
            }

        }

  
        
    }
	
    return true;
}

void Clayer_2DSoftShadow::sliderAction(Ref *senderz, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)senderz;
    float value=pSlider->getValue();
    m_shadowRoot->setShadowDarkness(value);
    m_label->setString(valueToStr(value).c_str());
    m_label->setPosition(ccp(
                             20+m_pSliderCtl->getPositionX()+m_pSliderCtl->getContentSize().width/2+m_label->getContentSize().width/2,
                             m_pSliderCtl->getPositionY()));
    
}

void Clayer_2DSoftShadow::controlButtonEvent_debug(Ref *senderz, Control::EventType controlEvent){
    m_isDebugOn=!m_isDebugOn;
    
    if(m_isDebugOn){
        m_label_debugState->setString("debug: on") ;
        m_shadowRoot->setIsDrawDebug(true);
        
    }else{
        m_label_debugState->setString("debug: off") ;
        m_shadowRoot->setIsDrawDebug(false);
    }
    
}
void Clayer_2DSoftShadow::controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent){
    CchooseScene*scene=new CchooseScene();
    scene->autorelease();
    scene->init();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void Clayer_2DSoftShadow::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
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
void Clayer_2DSoftShadow::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
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

        m_light->setPosition(location);
        m_sprite_light->setPosition(location);
        break;
    }
    
}

void Clayer_2DSoftShadow:: onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
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
        m_light->setPosition(location);
        m_sprite_light->setPosition(location);
        break;
    }
}
