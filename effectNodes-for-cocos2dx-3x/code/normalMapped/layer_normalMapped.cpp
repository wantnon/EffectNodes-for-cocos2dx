#include "layer_normalMapped.h"
#include "chooseScene.h"

bool Clayer_normalMapped::init()
{
    this->setTouchEnabled(true);
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    //lightSprite
    m_lightSprite=new normalMapped::ClightSprite();
    m_lightSprite->autorelease();
    m_lightSprite->init("demoRes/light.png");
    m_lightSprite->setPosition(ccp(winSize.width/4, winSize.height/4*3));
    this->addChild(m_lightSprite);
    m_lightSprite->setZ(50);
    
    //normlMappedSprite
    m_normalMappedSprite=new CnormalMappedSprite();
    m_normalMappedSprite->autorelease();
    m_normalMappedSprite->init("demoRes/fish2.png","demoRes/fish2_normal.png");
    m_normalMappedSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(m_normalMappedSprite);
    m_normalMappedSprite->setLightSprite(m_lightSprite);
    

    //slider
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		ControlSlider *slider = ControlSlider::create("uiRes/sliderTrack.png",
                                                          "uiRes/sliderProgress.png" ,
                                                          "uiRes/sliderThumb.png");
		slider->setAnchorPoint(ccp(0.5f, 0.5f));
		slider->setMinimumValue(0); // Sets the min value of range
		slider->setMaximumValue(1); // Sets the max value of range
		slider->setValue(m_lightSprite->getDiffuse().r);
		slider->setPosition(ccp(winSize.width *0.5, 180));
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_normalMapped::sliderAction), Control::EventType::VALUE_CHANGED);
		m_pSliderCtl=slider;
		addChild(m_pSliderCtl,100);
		//title
		CCLabelTTF* pLabel = CCLabelTTF::create("R ", "Arial", 40);
		pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
        this->addChild(pLabel, 1);
  
	}
	//slider2
    {
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		ControlSlider *slider = ControlSlider::create("uiRes/sliderTrack.png",
                                                          "uiRes/sliderProgress.png",
                                                          "uiRes/sliderThumb.png");
		slider->setAnchorPoint(ccp(0.5f, 0.5f));
		slider->setMinimumValue(0); // Sets the min value of range
		slider->setMaximumValue(1); // Sets the max value of range
		slider->setValue(m_lightSprite->getDiffuse().g);
		slider->setPosition(ccp(winSize.width *0.5, 180-40));
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_normalMapped::sliderAction2), Control::EventType::VALUE_CHANGED);
		m_pSliderCtl2=slider;
		addChild(m_pSliderCtl2,100);
		//title
		CCLabelTTF* pLabel = CCLabelTTF::create("G ", "Arial", 40);
		pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
        this->addChild(pLabel, 1);

	}
    //slider3
    {
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		ControlSlider *slider = ControlSlider::create("uiRes/sliderTrack.png",
                                                          "uiRes/sliderProgress.png",
                                                          "uiRes/sliderThumb.png");
		slider->setAnchorPoint(ccp(0.5f, 0.5f));
		slider->setMinimumValue(0); // Sets the min value of range
		slider->setMaximumValue(1); // Sets the max value of range
		slider->setValue(m_lightSprite->getDiffuse().b);
		slider->setPosition(ccp(winSize.width *0.5, 180-80));
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_normalMapped::sliderAction3), Control::EventType::VALUE_CHANGED);
		m_pSliderCtl3=slider;
		addChild(m_pSliderCtl3,100);
		//title
		CCLabelTTF* pLabel = CCLabelTTF::create("B ", "Arial", 40);
		pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
        this->addChild(pLabel, 1);
	}
    
    //slider4
    {
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		ControlSlider *slider = ControlSlider::create("uiRes/sliderTrack.png",
                                                          "uiRes/sliderProgress.png",
                                                          "uiRes/sliderThumb.png");
		slider->setAnchorPoint(ccp(0.5f, 0.5f));
		slider->setMinimumValue(1); // Sets the min value of range
		slider->setMaximumValue(4); // Sets the max value of range
		slider->setValue(m_normalMappedSprite->getKBump());
		slider->setPosition(ccp(winSize.width *0.5, 180-120));
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_normalMapped::sliderAction4), Control::EventType::VALUE_CHANGED);
		m_pSliderCtl4=slider;
		addChild(m_pSliderCtl4,100);
		//title
		CCLabelTTF* pLabel = CCLabelTTF::create("kBump ", "Arial", 40);
		pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
        this->addChild(pLabel, 1);
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
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_normalMapped::controlButtonEvent_back), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(controlButton);
        m_controlButton_back=controlButton;
    }
    //title
    {
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("Normal Mapped", "Arial", 45);
            
            // position the label on the center of the screen
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height));
            
            // add the label as a child to this layer
            this->addChild(pLabel, 1);
        }
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("2013-11-4", "Arial", 30);
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height-60));
            this->addChild(pLabel, 1);
        }
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("move the light...", "Arial", 30);
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height-120));
            this->addChild(pLabel, 1);
        }
        
    }

    return true;
}

void Clayer_normalMapped::controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent){
    CchooseScene*scene=new CchooseScene();
    scene->autorelease();
    scene->init();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void Clayer_normalMapped::sliderAction(Ref *senderz, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)senderz;
    float value=pSlider->getValue();
    ccColor4F diffuse=m_lightSprite->getDiffuse();
    diffuse.r=value;
    m_lightSprite->setDiffuse(diffuse);
  }

void Clayer_normalMapped::sliderAction2(Ref *senderz, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)senderz;
    float value=pSlider->getValue();
    ccColor4F diffuse=m_lightSprite->getDiffuse();
    diffuse.g=value;
    m_lightSprite->setDiffuse(diffuse);

    
}
void Clayer_normalMapped::sliderAction3(Ref *senderz, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)senderz;
    float value=pSlider->getValue();
    ccColor4F diffuse=m_lightSprite->getDiffuse();
    diffuse.b=value;
    m_lightSprite->setDiffuse(diffuse);
    
}
void Clayer_normalMapped::sliderAction4(Ref *senderz, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)senderz;
    float value=pSlider->getValue();
    m_normalMappedSprite->setKBump(value);
    
}
void Clayer_normalMapped::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
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
        
        m_lightSprite->setPosition(loc_GLSpace);
        
    }
}
void Clayer_normalMapped::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
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
        
        m_lightSprite->setPosition(loc_GLSpace);
        
    }
    
}

void Clayer_normalMapped:: onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
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
        
        m_lightSprite->setPosition(loc_GLSpace);
    
        break;
        
        
    }
}
