#include "laserLayer.h"
#include "chooseDemoUI/chooseScene.h"

bool ChalfLaser::init(){
    
    
    //lightTex
    this->CCSprite::initWithFile("demoRes/laserRes/light2.png");
    {
        ccTexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
        this->getTexture()->setTexParameters(&texParams);
    }
    //maskTex
    m_maskTex=CCTextureCache::sharedTextureCache()->addImage("demoRes/laserRes/mask.png");
    m_maskTex->retain();
    {
        ccTexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
        m_maskTex->setTexParameters(&texParams);
    }
    //maskTex2
    m_maskTex2=CCTextureCache::sharedTextureCache()->addImage("demoRes/laserRes/mask2.png");
    m_maskTex2->retain();
    {
        ccTexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
        m_maskTex2->setTexParameters(&texParams);
    }

    //noiseTex
    m_noiseTex=CCTextureCache::sharedTextureCache()->addImage("demoRes/laserRes/noise.png");
     m_noiseTex->retain();
    {
        ccTexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
        m_noiseTex->setTexParameters(&texParams);
    }
    //noiseTex2
    m_noiseTex2=CCTextureCache::sharedTextureCache()->addImage("demoRes/laserRes/noise2.png");
    m_noiseTex2->retain();
    {
        ccTexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
        m_noiseTex2->setTexParameters(&texParams);
    }

    //create and set shader program
    {
        GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/laser.fsh").c_str())->getCString();
        ens::CGLProgramWithUnifos* program = new ens::CGLProgramWithUnifos();
        program->autorelease();
        program->initWithVertexShaderByteArray(ccPositionTextureColor_vert, fragSource);
        //bind attribute
        program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        //link  (must after bindAttribute)
        program->link();
        //get cocos2d-x build-in uniforms
        program->updateUniforms();
        //get my own uniforms
        program->attachUniform("u_maskTex");
        program->attachUniform("u_maskTex2");
        program->attachUniform("u_noiseTex");
        program->attachUniform("u_noiseTex2");
        program->attachUniform("u_spriteSize");
        program->attachUniform("u_LUPos");
        program->attachUniform("u_noiseSize");
        program->attachUniform("u_highlight");
        program->attachUniform("u_time");
        program->attachUniform("u_PorN");
        program->attachUniform("u_scale");
        program->attachUniform("u_kLightColor");
        program->attachUniform("u_kGlowColor");
        program->attachUniform("u_noiseScale");
        //set program
        m_program=program;
        m_program->retain();
        
        //check gl error
        CHECK_GL_ERROR_DEBUG();
    }
    //update
    this->scheduleUpdate();
 

    return true;

}
void ChalfLaser::setBlendFunc(GLenum src,GLenum dst){
    //
    ccBlendFunc blendFunc={src,dst};
    this->CCSprite::setBlendFunc(blendFunc);
    
}
void ChalfLaser::update(float dt){
    m_time+=dt;
    if(m_time>=10000)m_time=0;
    
}
void ChalfLaser::draw(){
    {

        float spriteSize_tmp[2]={this->getContentSize().width*this->getScaleX(),
        this->getContentSize().height*this->getScaleY()};//because this sprite may rotate, so we use contentSize*scale instead of boundingBox().size
        float LUPos_tmp[2]={0,0};//if we use {boundingBox().getMinX(),boundingBox().getMaxY()} instead of {0,0}, when this sprite scale, the noiseTex will move, cause uncontinuous effect. to avoid this, so we should use {0,0}
        float noiseSize_tmp[2]={m_noiseTex->getContentSize().width,m_noiseTex->getContentSize().height};
        float scale_tmp[2]={this->getScaleX(),this->getScaleY()};
        //----change shader
        ccGLEnable(m_eGLServerState);
        //pass values for cocos2d-x build-in uniforms
        this->setShaderProgram(m_program);
        ens::CGLProgramWithUnifos*program=(ens::CGLProgramWithUnifos*)this->getShaderProgram();
        program->use();
        program->setUniformsForBuiltins();
        //pass values for my own uniforms
        program->passUnifoValue1i("u_maskTex", 1);
        program->passUnifoValue1i("u_noiseTex", 2);
        program->passUnifoValue1i("u_noiseTex2", 4);
        program->passUnifoValue1i("u_maskTex2", 3);
        program->passUnifoValueNfv("u_spriteSize", spriteSize_tmp, 2);
        program->passUnifoValueNfv("u_LUPos", LUPos_tmp, 2);
        program->passUnifoValueNfv("u_noiseSize", noiseSize_tmp, 2);
        program->passUnifoValue1f("u_highlight", m_highlight);
        program->passUnifoValue1f("u_time", m_time);
        program->passUnifoValue1f("u_PorN", m_PorN);
        program->passUnifoValueNfv("u_scale", scale_tmp, 2);
        program->passUnifoValue1f("u_kLightColor", m_kLightColor);
        program->passUnifoValue1f("u_kGlowColor", m_kGlowColor);
        program->passUnifoValue1f("u_noiseScale", m_noiseScale);
        //attach texture to texture attach point
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_maskTex->getName());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_noiseTex->getName());
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_maskTex2->getName());
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, m_noiseTex2->getName());
        glActiveTexture(GL_TEXTURE0);//back to GL_TEXTURE0
        //----call CCSprite::draw
        CCSprite::draw();
    }
    

    

    
}


bool ClaserLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();

 
    /////////////////////////////
    // 3. add your codes below...
    //enable touch
	setTouchEnabled( true );
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize

    
    //start update
    this->scheduleUpdate();
    
    CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);
  

    
    m_laser=new Claser();
    m_laser->autorelease();
    m_laser->init();
    this->addChild(m_laser);
    m_laser->setStart(ccp(visibleOrigin.x+visibleSize.width/2,visibleOrigin.y));
    m_laser->setEnd(ccp(visibleOrigin.x+visibleSize.width/2,visibleOrigin.y+visibleSize.height));
    
    //back button
    {
        CCScale9Sprite* btnUp=CCScale9Sprite::create("uiRes/button.png");
        CCScale9Sprite* btnDn=CCScale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("back", "Helvetica", 30);
        CCControlButton* controlButton=CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,CCControlStateHighlighted);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(visibleOrigin.x+visibleSize.width-controlButton->getContentSize().width/2,visibleOrigin.y+controlButton->getContentSize().height/2));
        controlButton->addTargetWithActionForControlEvents(this, (SEL_CCControlHandler)(&ClaserLayer::controlButtonEvent_back), CCControlEventTouchUpInside);
        this->addChild(controlButton);
        m_controlButton_back=controlButton;
    }

    //title
    {
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("laser", "Arial", 45);
            
            // position the label on the center of the screen
            pLabel->setPosition(ccp(visibleOrigin.x + visibleSize.width/2,
                                    visibleOrigin.y + visibleSize.height - pLabel->getContentSize().height));
            
            // add the label as a child to this layer
            this->addChild(pLabel, 1);
        }
        {
            CCLabelTTF* pLabel = CCLabelTTF::create("2014-9-23", "Arial", 30);
            pLabel->setPosition(ccp(visibleOrigin.x + visibleSize.width/2,
                                    visibleOrigin.y + visibleSize.height - pLabel->getContentSize().height-60));
            this->addChild(pLabel, 1);
        }
        
        
    }

    
    return true;
}

void ClaserLayer::controlButtonEvent_back(CCObject *senderz, CCControlEvent controlEvent){
    CchooseScene*scene=new CchooseScene();
    scene->autorelease();
    scene->init();
    CCDirector::sharedDirector()->replaceScene(scene);
}
void ClaserLayer::update(float dt){

    

    
}

void ClaserLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
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

void ClaserLayer::ccTouchesMoved(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
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

void ClaserLayer::ccTouchesBegan(CCSet* touches, CCEvent* event)
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

    
      
    }
}

