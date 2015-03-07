#include "layer_colorAdjust.h"
#include "chooseScene.h"

bool Clayer_colorAdjust::init()
{
    
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    //colorAdjustSprite
    m_colorAdjustSprite=new CcolorAdjustSprite();
    m_colorAdjustSprite->autorelease();
    m_colorAdjustSprite->init("demoRes/chameleon.png");
    m_colorAdjustSprite->setAnchorPoint(ccp(0.5,0.5));
    m_colorAdjustSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(m_colorAdjustSprite);
    
    
     //slider
	{
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		ControlSlider *slider = ControlSlider::create("uiRes/sliderTrack_long.png",
                                                          "uiRes/sliderProgress_long.png" ,
                                                          "uiRes/sliderThumb.png");
		slider->setAnchorPoint(ccp(0.5f, 0.5f));
		slider->setMinimumValue(-180); // Sets the min value of range
		slider->setMaximumValue(+180); // Sets the max value of range
		slider->setValue(m_colorAdjustSprite->getDH());
		slider->setPosition(ccp(screenSize.width *0.5, 110));
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_colorAdjust::sliderAction), Control::EventType::VALUE_CHANGED);
		m_pSliderCtl=slider;
		addChild(m_pSliderCtl,100);
		//title
		CCLabelTTF* pLabel = CCLabelTTF::create("Hue ", "Arial", 40);
		pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
        this->addChild(pLabel, 1);
        //label
        m_label=CCLabelTTF::create(numberToStr(m_colorAdjustSprite->getDH()).c_str(), "Arial", 40);
        m_label->setPosition(ccp(slider->getPositionX()+slider->getContentSize().width/2+m_label->getContentSize().width/2, slider->getPositionY()));
        this->addChild(m_label);
	}
	//slider2
    {
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		ControlSlider *slider = ControlSlider::create("uiRes/sliderTrack_long.png",
                                                          "uiRes/sliderProgress_long.png",
                                                          "uiRes/sliderThumb.png");
		slider->setAnchorPoint(ccp(0.5f, 0.5f));
		slider->setMinimumValue(-1); // Sets the min value of range
		slider->setMaximumValue(1); // Sets the max value of range
		slider->setValue(m_colorAdjustSprite->getDS());
		slider->setPosition(ccp(screenSize.width *0.5, 110-40));
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_colorAdjust::sliderAction2), Control::EventType::VALUE_CHANGED);
		m_pSliderCtl2=slider;
		addChild(m_pSliderCtl2,100);
		//title
		CCLabelTTF* pLabel = CCLabelTTF::create("Saturation ", "Arial", 40);
		pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
        this->addChild(pLabel, 1);
        //label2
        m_label2=CCLabelTTF::create(valueToStr(m_colorAdjustSprite->getDH()).c_str(), "Arial", 40);
        m_label2->setPosition(ccp(slider->getPositionX()+slider->getContentSize().width/2+m_label2->getContentSize().width/2, slider->getPositionY()));
        this->addChild(m_label2);
	}
    //slider3
    {
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		ControlSlider *slider = ControlSlider::create("uiRes/sliderTrack_long.png",
                                                          "uiRes/sliderProgress_long.png",
                                                          "uiRes/sliderThumb.png");
		slider->setAnchorPoint(ccp(0.5f, 0.5f));
		slider->setMinimumValue(-1); // Sets the min value of range
		slider->setMaximumValue(1); // Sets the max value of range
		slider->setValue(m_colorAdjustSprite->getDH());
		slider->setPosition(ccp(screenSize.width *0.5, 110-80));
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_colorAdjust::sliderAction3), Control::EventType::VALUE_CHANGED);
		m_pSliderCtl3=slider;
		addChild(m_pSliderCtl3,100);
		//title
		CCLabelTTF* pLabel = CCLabelTTF::create("Lightness ", "Arial", 40);
		pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
        this->addChild(pLabel, 1);
        //label3
        m_label3=CCLabelTTF::create(valueToStr(m_colorAdjustSprite->getDH()).c_str(), "Arial", 40);
        m_label3->setPosition(ccp(slider->getPositionX()+slider->getContentSize().width/2+m_label2->getContentSize().width/2, slider->getPositionY()));
        this->addChild(m_label3);
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
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_colorAdjust::controlButtonEvent_back), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(controlButton);
        m_controlButton_back=controlButton;
    }
    //title
    {
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("Color Adjust", "Arial", 45);
            
            // position the label on the center of the screen
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height));
        
            // add the label as a child to this layer
            this->addChild(pLabel, 1);
        }
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("(imitate Photoshop Hue/Saturation Panel)   2014-6-13", "Arial", 30);
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height-60));
            this->addChild(pLabel, 1);
        }
        
    }


    return true;
}

void Clayer_colorAdjust::controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent){
    CchooseScene*scene=new CchooseScene();
    scene->autorelease();
    scene->init();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void Clayer_colorAdjust::sliderAction(Ref *senderz, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)senderz;
    float value=pSlider->getValue();
    m_colorAdjustSprite->setDH(value);
    m_label->setString(numberToStr(value).c_str());
    m_label->setPosition(ccp(m_pSliderCtl->getPositionX()+m_pSliderCtl->getContentSize().width/2+m_label->getContentSize().width/2, m_pSliderCtl->getPositionY()));
}

void Clayer_colorAdjust::sliderAction2(Ref *senderz, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)senderz;
    float value=pSlider->getValue();
    m_colorAdjustSprite->setDS(value);
    m_label2->setString(valueToStr(value).c_str());
    m_label2->setPosition(ccp(m_pSliderCtl2->getPositionX()+m_pSliderCtl2->getContentSize().width/2+m_label2->getContentSize().width/2, m_pSliderCtl2->getPositionY()));

    
}

void Clayer_colorAdjust::sliderAction3(Ref *senderz, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)senderz;
    float value=pSlider->getValue();
    m_colorAdjustSprite->setDL(value);
    m_label3->setString(valueToStr(value).c_str());
    m_label3->setPosition(ccp(m_pSliderCtl3->getPositionX()+m_pSliderCtl3->getContentSize().width/2+m_label3->getContentSize().width/2, m_pSliderCtl3->getPositionY()));

    
}