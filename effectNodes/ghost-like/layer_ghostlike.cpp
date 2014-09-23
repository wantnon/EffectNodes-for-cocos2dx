#include "ghost-like/layer_ghostlike.h"
#include "chooseDemoUI/chooseScene.h"


bool Clayer_ghostlike::init()
{
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    //backGround
    CCSprite*backGround=CCSprite::create();
    backGround->initWithFile("demoRes/scene.png");
    backGround->setPosition(ccp(winSize.width/2, winSize.height/2));
    addChild(backGround);
    //animation
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("demoRes/fish_frames.plist");
    CCAnimationCache::sharedAnimationCache()->addAnimationsWithFile("demoRes/fish_ani.plist");
    CCAnimation*animation=CCAnimationCache::sharedAnimationCache()->animationByName("fish_ani");//the animation name is written in fish_ani.plist
    //ghost
    m_ghostSprite=new CghostSprite();
    m_ghostSprite->autorelease();
    m_ghostSprite->init(((CCAnimationFrame*)animation->getFrames()->objectAtIndex(0))->getSpriteFrame());//init sprite using first frame
    m_ghostSprite->setPosition(ccp(winSize.width/2, winSize.height/3*2));
    this->addChild(m_ghostSprite,1);
    m_ghostSprite->getRefSprite()->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
    
    //normal sprite (to make contrast)
    CCSprite*sp=CCSprite::create();
    sp->initWithSpriteFrame(((CCAnimationFrame*)animation->getFrames()->objectAtIndex(0))->getSpriteFrame());
    this->addChild(sp);
    sp->setPosition(ccp(winSize.width/2, winSize.height/3));
    sp->runAction(CCRepeatForever::create(CCAnimate::create(animation)));

    //back button
    {
        CCScale9Sprite* btnUp=CCScale9Sprite::create("uiRes/button.png");
        CCScale9Sprite* btnDn=CCScale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("back", "Helvetica", 30);
        CCControlButton* controlButton=CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(origin.x+visibleSize.width-controlButton->getContentSize().width/2,origin.y+controlButton->getContentSize().height/2));
        controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&Clayer_ghostlike::controlButtonEvent_back), CCControlEventTouchUpInside);
        this->addChild(controlButton);
        m_controlButton_back=controlButton;
    }
    //title
    {
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("Ghost-like", "Arial", 45);
            
            // position the label on the center of the screen
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height));
            
            // add the label as a child to this layer
            this->addChild(pLabel, 1);
        }
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("2014-6-12", "Arial", 30);
            pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                    origin.y + visibleSize.height - pLabel->getContentSize().height-60));
            this->addChild(pLabel, 1);
        }

        
    }
    return true;
}
void Clayer_ghostlike::controlButtonEvent_back(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    CchooseScene*scene=new CchooseScene();
    scene->autorelease();
    scene->init();
    CCDirector::sharedDirector()->replaceScene(scene);
}
