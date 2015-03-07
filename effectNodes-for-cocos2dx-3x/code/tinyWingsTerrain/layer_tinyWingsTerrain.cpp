#include "layer_tinyWingsTerrain.h"
#include "chooseScene.h"


bool Clayer_tinyWingsTerrain::init()
{
    this->setTouchEnabled(true);
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    
	m_tinyWingsTerrainSprite=new CtinyWingsTerrainSprite();
    const float controlPointDis=180;
    const float terrainWidth=visibleSize.width;
    const float terrainHeight=150;
	m_tinyWingsTerrainSprite->init(terrainWidth,terrainHeight,(int)(visibleSize.width/controlPointDis));
	m_tinyWingsTerrainSprite->setPosition(ccp(origin.x,winSize.height/2-120));
    addChild(m_tinyWingsTerrainSprite);
  
    //debug button
    {
        Scale9Sprite* btnUp=Scale9Sprite::create("uiRes/button.png");
        Scale9Sprite* btnDn=Scale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("debug", "Helvetica", 30);
        ControlButton* controlButton=ControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,Control::State::HIGH_LIGHTED);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(origin.x+controlButton->getContentSize().width/2,winSize.height-150));
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_tinyWingsTerrain::controlButtonEvent_debug), Control::EventType::TOUCH_DOWN);
        this->addChild(controlButton);
        m_controlButton_debug=controlButton;
        //label
        m_label_debugState = CCLabelTTF::create("debug: off", "Arial", 30);
        m_label_debugState->setPosition(ccp(m_controlButton_debug->getPositionX()+m_controlButton_debug->getContentSize().width/2+m_label_debugState->getContentSize().width/2+10,
                                            m_controlButton_debug->getPositionY()));
        this->addChild(m_label_debugState, 1);
    }
    //slider
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		ControlSlider *slider = ControlSlider::create("uiRes/sliderTrack.png",
                                                          "uiRes/sliderProgress.png" ,
                                                          "uiRes/sliderThumb.png");
		slider->setAnchorPoint(ccp(0.5f, 0.5f));
		slider->setMinimumValue(0); // Sets the min value of range
		slider->setMaximumValue(1); // Sets the max value of range
		slider->setValue(m_tinyWingsTerrainSprite->getTexNonlinearFactor());
		slider->setPosition(ccp(winSize.width *0.5, 100));
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_tinyWingsTerrain::sliderAction), Control::EventType::VALUE_CHANGED);
		m_pSliderCtl=slider;
		addChild(m_pSliderCtl,100);
		//title
		CCLabelTTF* pLabel = CCLabelTTF::create("texNonlinearFactor ", "Arial", 40);
		pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
        this->addChild(pLabel, 1);
        //label
        m_label=CCLabelTTF::create(valueToStr(m_tinyWingsTerrainSprite->getTexNonlinearFactor()).c_str(), "Arial", 40);
        m_label->setPosition(ccp(20+slider->getPositionX()+slider->getContentSize().width/2+m_label->getContentSize().width/2, slider->getPositionY()));
        this->addChild(m_label);
        
	}
    //ribbonRotation slider
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		ControlSlider *slider = ControlSlider::create("uiRes/sliderTrack.png",
                                                          "uiRes/sliderProgress.png" ,
                                                          "uiRes/sliderThumb.png");
		slider->setAnchorPoint(ccp(0.5f, 0.5f));
		slider->setMinimumValue(0); // Sets the min value of range
		slider->setMaximumValue(180); // Sets the max value of range
		slider->setValue(m_tinyWingsTerrainSprite->getRibbonRotation());
		slider->setPosition(ccp(winSize.width *0.5, 60));
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_tinyWingsTerrain::sliderAction_ribbonRotation), Control::EventType::VALUE_CHANGED);
		addChild(slider,100);
        m_pSliderCtl_ribbonRotation=slider;
		//title
		CCLabelTTF* pLabel = CCLabelTTF::create("ribbonRotation ", "Arial", 40);
		pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
        this->addChild(pLabel, 1);
        //label
        CCLabelTTF*label=CCLabelTTF::create(valueToStr(m_tinyWingsTerrainSprite->getRibbonRotation()).c_str(), "Arial", 40);
        label->setPosition(ccp(
                                                20+slider->getPositionX()+slider->getContentSize().width/2+label->getContentSize().width/2,
                                                slider->getPositionY()));
        this->addChild(label);
        m_label_ribbonRotation=label;
        
	}
    //ribbonRepeat slider
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		ControlSlider *slider = ControlSlider::create("uiRes/sliderTrack.png",
                                                          "uiRes/sliderProgress.png" ,
                                                          "uiRes/sliderThumb.png");
		slider->setAnchorPoint(ccp(0.5f, 0.5f));
		slider->setMinimumValue(0); // Sets the min value of range
		slider->setMaximumValue(30); // Sets the max value of range
		slider->setValue(m_tinyWingsTerrainSprite->getRibbonRepeat());
		slider->setPosition(ccp(winSize.width *0.5, 20));
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_tinyWingsTerrain::sliderAction_ribbonRepeat), Control::EventType::VALUE_CHANGED);
		addChild(slider,100);
        m_pSliderCtl_ribbonRepeat=slider;
		//title
		CCLabelTTF* pLabel = CCLabelTTF::create("ribbonRepeat ", "Arial", 40);
		pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
        this->addChild(pLabel, 1);
        //label
        CCLabelTTF*label=CCLabelTTF::create(valueToStr(m_tinyWingsTerrainSprite->getRibbonRepeat()).c_str(), "Arial", 40);
        label->setPosition(ccp(
                               20+slider->getPositionX()+slider->getContentSize().width/2+label->getContentSize().width/2,
                               slider->getPositionY()));
        this->addChild(label);
        m_label_ribbonRepeat=label;
        
	}
    //reGenerate button
    {
        Scale9Sprite* btnUp=Scale9Sprite::create("uiRes/button.png");
        Scale9Sprite* btnDn=Scale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("reGenerate", "Helvetica", 30);
        ControlButton* controlButton=ControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,Control::State::HIGH_LIGHTED);
        controlButton->setPreferredSize(CCSize(150,50));
        controlButton->setPosition(ccp(winSize.width/2,150));
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_tinyWingsTerrain::controlButtonEvent_reGenerate), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(controlButton);
        m_controlButton_reGenerate=controlButton;
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
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_tinyWingsTerrain::controlButtonEvent_back), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(controlButton);
        m_controlButton_back=controlButton;
    }
    //title
    {
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("Tiny Wings Terrain", "Arial", 45);
            
            // position the label on the center of the screen
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height));
            
            // add the label as a child to this layer
            this->addChild(pLabel, 1);
        }
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("2014-6-29", "Arial", 30);
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height-60));
            this->addChild(pLabel, 1);
        }

        
    }
	
    return true;
}
void Clayer_tinyWingsTerrain::sliderAction(Ref *senderz, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)senderz;
    float value=pSlider->getValue();
    m_tinyWingsTerrainSprite->setTexNonlinearFactor(value);
    m_label->setString(valueToStr(value).c_str());
    m_label->setPosition(ccp(
                                            20+m_pSliderCtl->getPositionX()+m_pSliderCtl->getContentSize().width/2+m_label->getContentSize().width/2,
                                            m_pSliderCtl->getPositionY()));

}
void Clayer_tinyWingsTerrain::sliderAction_ribbonRotation(Ref *senderz, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)senderz;
    float value=pSlider->getValue();
    m_tinyWingsTerrainSprite->setRibbonRotation(value);
    m_label_ribbonRotation->setString(valueToStr(value).c_str());
    m_label_ribbonRotation->setPosition(ccp(
                                            20+m_pSliderCtl_ribbonRotation->getPositionX()+m_pSliderCtl_ribbonRotation->getContentSize().width/2+m_label_ribbonRotation->getContentSize().width/2,
                                            m_pSliderCtl_ribbonRotation->getPositionY()));
    
}
void Clayer_tinyWingsTerrain::sliderAction_ribbonRepeat(Ref *senderz, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)senderz;
    float value=pSlider->getValue();
    m_tinyWingsTerrainSprite->setRibbonRepeat(value);
    m_label_ribbonRepeat->setString(valueToStr(value).c_str());
    m_label_ribbonRepeat->setPosition(ccp(
                                            20+m_pSliderCtl_ribbonRepeat->getPositionX()+m_pSliderCtl_ribbonRepeat->getContentSize().width/2+m_label_ribbonRepeat->getContentSize().width/2,
                                            m_pSliderCtl_ribbonRepeat->getPositionY()));
    
}

void Clayer_tinyWingsTerrain::controlButtonEvent_debug(Ref *senderz, Control::EventType controlEvent){
    m_isDebugOn=!m_isDebugOn;
    
    if(m_isDebugOn){
        m_label_debugState->setString("debug: on") ;
        m_tinyWingsTerrainSprite->setIsDrawDebug(true);
        
    }else{
        m_label_debugState->setString("debug: off") ;
        m_tinyWingsTerrainSprite->setIsDrawDebug(false);
    }
    
}

void Clayer_tinyWingsTerrain::controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent){
    CchooseScene*scene=new CchooseScene();
    scene->autorelease();
    scene->init();
    CCDirector::sharedDirector()->replaceScene(scene);
}
void Clayer_tinyWingsTerrain::controlButtonEvent_reGenerate(Ref *senderz, Control::EventType controlEvent){
    m_tinyWingsTerrainSprite->reGenerateShape(m_tinyWingsTerrainSprite->getInsertControlPointCount());
    m_tinyWingsTerrainSprite->reGenerateColors();
    
}


