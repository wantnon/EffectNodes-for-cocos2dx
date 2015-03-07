#include "ensLaserLayer.h"
#include "chooseScene.h"

bool CensHalfLaser::init(){
    
    
    //lightTex
    this->CCSprite::initWithFile("demoRes/laserRes/light2.png");
    {
        Texture2D::TexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
        this->getTexture()->setTexParameters(&texParams);
    }
    //maskTex
    m_maskTex=CCTextureCache::sharedTextureCache()->addImage("demoRes/laserRes/mask.png");
    m_maskTex->retain();
    {
        Texture2D::TexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
        m_maskTex->setTexParameters(&texParams);
    }
    //maskTex2
    m_maskTex2=CCTextureCache::sharedTextureCache()->addImage("demoRes/laserRes/mask2.png");
    m_maskTex2->retain();
    {
        Texture2D::TexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
        m_maskTex2->setTexParameters(&texParams);
    }

    //noiseTex
    m_noiseTex=CCTextureCache::sharedTextureCache()->addImage("demoRes/laserRes/noise.png");
     m_noiseTex->retain();
    {
        Texture2D::TexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
        m_noiseTex->setTexParameters(&texParams);
    }
    //noiseTex2
    m_noiseTex2=CCTextureCache::sharedTextureCache()->addImage("demoRes/laserRes/noise2.png");
    m_noiseTex2->retain();
    {
        Texture2D::TexParams texParams= {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
        m_noiseTex2->setTexParameters(&texParams);
    }

    //create and set shader program
    {
        GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/laser.fsh").c_str())->getCString();
        ens::CGLProgramWithUnifos* program = new ens::CGLProgramWithUnifos();
        program->autorelease();
        program->initWithVertexShaderByteArray(ccPositionTextureColor_noMVP_vert, fragSource);//use ccPositionTextureColor_noMVP_vert instead of ccPositionTextureColor_vert, see: http://www.cnblogs.com/wantnon/p/4190341.html
        //            --below code is no longer needed, because bindPredefinedVertexAttribs() is called in link() in 3.x
        //            --bind attribute
        //            --program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        //            --program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        //            --program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
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
void CensHalfLaser::setBlendFunc(GLenum src,GLenum dst){
    //
    ccBlendFunc blendFunc={src,dst};
    this->CCSprite::setBlendFunc(blendFunc);
    
}
void CensHalfLaser::update(float dt){
    m_time+=dt;
    if(m_time>=10000)m_time=0;
    
}

void CensHalfLaser::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags){
    //change shader
    this->setShaderProgram(m_program);
    
    
    //passUnifo and bindTex
    _passUnifoAndBindTexCommand.init(_globalZOrder);
    _passUnifoAndBindTexCommand.func = CC_CALLBACK_0(CensHalfLaser::onPassUnifoAndBindTex, this,transform,flags);
    Director::getInstance()->getRenderer()->addCommand(&_passUnifoAndBindTexCommand);
    //draw quad
    {
        assert(getGLProgramState()->getGLProgram()==m_program);
        //if we use _quadCommand to draw we must use noMV shader.
        //because (if i am not seeing wrong) in _quadCommand modelview is applied to vertex via cpu and at the same time modelview is passed in to shader.
        //shit!
        _quadCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, &_quad, 1, transform);
        renderer->addCommand(&_quadCommand);
    }
    

    
}
void CensHalfLaser::onPassUnifoAndBindTex(const Mat4 &transform, uint32_t flags){
    assert(getGLProgram()==m_program);
    float spriteSize_tmp[2]={this->getContentSize().width*this->getScaleX(),
        this->getContentSize().height*this->getScaleY()};//because this sprite may rotate, so we use contentSize*scale instead of boundingBox().size
    float LUPos_tmp[2]={0,0};//if we use {boundingBox().getMinX(),boundingBox().getMaxY()} instead of {0,0}, when this sprite scale, the noiseTex will move, cause uncontinuous effect. to avoid this, so we should use {0,0}
    float noiseSize_tmp[2]={m_noiseTex->getContentSize().width,m_noiseTex->getContentSize().height};
    float scale_tmp[2]={this->getScaleX(),this->getScaleY()};
    //pass values for cocos2d-x build-in uniforms
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
    
    //note: i am not investigated in detail yet,but it seems that if we do not use any of CC_Texture0,CC_Texture1,..., we can use our own sampler names, otherwise, we should only use CC_Texture0, CC_Texture1,.... .
}



bool CensLaserLayer::init()
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
  

    
    m_laser=new CensLaser();
    m_laser->autorelease();
    m_laser->init();
    this->addChild(m_laser);
    m_laser->setStart(ccp(visibleOrigin.x+visibleSize.width/2,visibleOrigin.y));
    m_laser->setEnd(ccp(visibleOrigin.x+visibleSize.width/2,visibleOrigin.y+visibleSize.height));
    
    //back button
    {
        Scale9Sprite* btnUp=Scale9Sprite::create("uiRes/button.png");
        Scale9Sprite* btnDn=Scale9Sprite::create("uiRes/button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("back", "Helvetica", 30);
        ControlButton* controlButton=ControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,Control::State::HIGH_LIGHTED);
        controlButton->setPreferredSize(CCSize(100,50));
        controlButton->setPosition(ccp(visibleOrigin.x+visibleSize.width-controlButton->getContentSize().width/2,visibleOrigin.y+controlButton->getContentSize().height/2));
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CensLaserLayer::controlButtonEvent_back), Control::EventType::TOUCH_UP_INSIDE);
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

void CensLaserLayer::controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent){
    CchooseScene*scene=new CchooseScene();
    scene->autorelease();
    scene->init();
    CCDirector::sharedDirector()->replaceScene(scene);
}
void CensLaserLayer::update(float dt){

    

    
}

void CensLaserLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
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
        

       
    }
    
    

}

void CensLaserLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
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
 


    }
    
}

void CensLaserLayer:: onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
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

    
        break;
      
    }
}

