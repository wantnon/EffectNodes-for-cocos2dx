#include "layer_shatter.h"
#include "chooseScene.h"


bool Clayer_shatter::init()
{
    this->setTouchEnabled(true);
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    

    //sprite
    m_shatterSprite=new CshatterSprite();
	m_shatterSprite->autorelease();
    m_shatterSprite->init("demoRes/fish2.png");
    m_shatterSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(m_shatterSprite);
  
                                                 
    //back button
    {
        Scale9Sprite* btnUp=Scale9Sprite::create("uiRes/button.png");
        Scale9Sprite* btnDn=Scale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("back", "Helvetica", 30);
        ControlButton* controlButton=ControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,Control::State::HIGH_LIGHTED);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(origin.x+visibleSize.width-controlButton->getContentSize().width/2,origin.y+controlButton->getContentSize().height/2));
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(Clayer_shatter::controlButtonEvent_back), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(controlButton);
        m_controlButton_back=controlButton;
    }
    //title
    {
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("Shatter", "Arial", 45);
            
            // position the label on the center of the screen
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height));
            
            // add the label as a child to this layer
            this->addChild(pLabel, 1);
        }
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("2013-11-11", "Arial", 30);
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height-60));
            this->addChild(pLabel, 1);
        }
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("touch it...", "Arial", 30);
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height-120));
            this->addChild(pLabel, 1);
        }
        
        
    }
	
    return true;
}
void Clayer_shatter::controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent){
    CchooseScene*scene=new CchooseScene();
    scene->autorelease();
    scene->init();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void Clayer_shatter::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
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
void Clayer_shatter::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
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

        
        break;
    }
    
}

void Clayer_shatter::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
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

        if(m_shatterSprite->getOpacity()==0){
            m_shatterSprite->setOpacity(255);
		}else{
           m_shatterSprite->setOpacity(0);
		   m_shatterSprite->stopAllActions();
            shatter::CshatterAction*shatter=shatter::CshatterAction::create(4);
            m_shatterSprite->runAction(shatter);
			
        }
        
        break;
    }
}
