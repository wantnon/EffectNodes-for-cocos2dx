#include "2DSoftShadow/layer_2DSoftShadow.h"
#include "chooseDemoUI/chooseScene.h"
#include "common/common.h"

bool Clayer_2DSoftShadow::init()
{
    this->setTouchEnabled(true);
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    m_backGround=CCSprite::create();
    m_backGround->initWithFile("demoRes/tile.png");
    m_backGround->setTextureRect(CCRect(0,0,winSize.width,winSize.height));
    ccTexParams texParams={GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT};
    m_backGround->getTexture()->setTexParameters(&texParams);
    addChild(m_backGround);
    m_backGround->setPosition(ccp(winSize.width/2,winSize.height/2));
    
    m_light=new ClightNode();
    m_light->autorelease();
    m_light->init(80);//(40);
    addChild(m_light);
    m_light->setPosition(ccp(winSize.width/2,winSize.height/2));
    
    m_2DSoftShadowNode=new C2DSoftShadowNode();
    m_2DSoftShadowNode->autorelease();
    m_2DSoftShadowNode->init(makeRegularPolygon(140,8));
    m_2DSoftShadowNode->setLight(m_light);
    addChild(m_2DSoftShadowNode,10);
    m_2DSoftShadowNode->setPosition(ccp(winSize.width/2,winSize.height/2));
                                                 
    //back button
    {
        CCScale9Sprite* btnUp=CCScale9Sprite::create("uiRes/button.png");
        CCScale9Sprite* btnDn=CCScale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("back", "Helvetica", 30);
        CCControlButton* controlButton=CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(origin.x+visibleSize.width-controlButton->getContentSize().width/2,origin.y+controlButton->getContentSize().height/2));
        controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&Clayer_2DSoftShadow::controlButtonEvent_back), CCControlEventTouchUpInside);
        this->addChild(controlButton);
        m_controlButton_back=controlButton;
    }
  /*  //title
    {
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("2D Soft Shadow", "Arial", 45);
            
            // position the label on the center of the screen
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height));
            
            // add the label as a child to this layer
            this->addChild(pLabel, 1);
        }
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("xxxx-xx-xx", "Arial", 30);
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height-60));
            this->addChild(pLabel, 1);
        }

        
        
    }*/
	
    return true;
}
void Clayer_2DSoftShadow::controlButtonEvent_back(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    CchooseScene*scene=new CchooseScene();
    scene->autorelease();
    scene->init();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void Clayer_2DSoftShadow::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    //Add a new body/atlas sprite at the touched location
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++)
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
void Clayer_2DSoftShadow::ccTouchesMoved(CCSet* touches , CCEvent* event)
{
    CCSetIterator it;
    CCTouch* touch;
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        
        location = CCDirector::sharedDirector()->convertToGL(location);
        //    cout<<"mos pos:"<<location.x<<" "<<location.y<<endl;

        m_light->setPosition(location);
        break;
    }
    
}

void Clayer_2DSoftShadow::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
    CCSetIterator it;
    CCTouch* touch;
	for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        if(!touch)
            break;
        
        CCPoint location = touch->getLocationInView();
        
        location = CCDirector::sharedDirector()->convertToGL(location);
        //  cout<<"mos pos:"<<location.x<<" "<<location.y<<endl;
        m_light->setPosition(location);
        break;
    }
}
