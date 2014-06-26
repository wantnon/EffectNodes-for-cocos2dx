
#include "ghost-like/ensGhostNode.h"
namespace_ens_begin
using namespace ghostlike;
bool CghostSprite::init(CCSpriteFrame* spriteFrame,float filterR,float fliterSrcScale){
    //----create sprite
    CCSprite*sprite=CCSprite::create();
    sprite->initWithSpriteFrame(spriteFrame);
    
    //----init with sprite
    this->init(sprite);
    
    return true ;

}
bool CghostSprite::init(const string&texFileName,float filterR,float fliterSrcScale)
{
    //----create sprite
    CCSprite*sprite=CCSprite::create();
    sprite->initWithFile(texFileName.c_str());
    
    //----init with sprite
    this->init(sprite);
    
    return true ;
}
bool CghostSprite::init(CCSprite* sprite,float filterR,float fliterSrcScale){
    //----blendfunc
    ccBlendFunc blendFunc={GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
    this->setBlendFunc(blendFunc);
    m_filterR=filterR;
    //----m_originTexSprite
    m_refSprite=sprite;
    //----calculate bigger size
    CCSize biggerSize;
    {
        CCSize contentSize=m_refSprite->getContentSize();
        float margin=m_filterR*3;//let the margin wider, so we mult 3
        biggerSize=CCSize(contentSize.width+margin*2, contentSize.height+margin*2);
    }
    //----init this sprite
    //call father init
    this->CCSprite::init();
    //must use the bigger size
    this->setTextureRect(CCRect(0,0,biggerSize.width,biggerSize.height));
    //----add m_refSprite to this sprite
    this->addChild(m_refSprite);
    m_refSprite->setVisible(false);
    //----m_biggerRT
    m_biggerRT=CCRenderTexture::create(biggerSize.width,biggerSize.height);
    m_biggerRT->retain();
    //----m_filterXRT
    m_filterXRT=CCRenderTexture::create(biggerSize.width*m_fliterSrcScale,biggerSize.height*m_fliterSrcScale);
    m_filterXRT->retain();
    //----m_filterYRT
    m_filterYRT=CCRenderTexture::create(biggerSize.width,biggerSize.height);
    m_filterYRT->retain();
    //----m_program_filterX
    {
		GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/ghostlike_filterX.fsh").c_str())->getCString();
		CGLProgramWithUnifos* program = new CGLProgramWithUnifos();
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
        program->attachUniform("u_filterR");
        program->attachUniform("u_ds");
        program->attachUniform("u_s_step");
        //set program
        m_program_filterX=program;
        m_program_filterX->retain();
        //check gl error
		CHECK_GL_ERROR_DEBUG();
	}
    //----m_program_filterY
    {
		GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/ghostlike_filterYAndMerge.fsh").c_str())->getCString();
		CGLProgramWithUnifos* program = new CGLProgramWithUnifos();
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
        program->attachUniform("u_filterR");
        program->attachUniform("u_dt");
        program->attachUniform("u_t_step");
        program->attachUniform("u_refTex");
        //set program
        m_program_filterY=program;
        m_program_filterY->retain();
        //check gl error
		CHECK_GL_ERROR_DEBUG();
	}
    //check gl error
	CHECK_GL_ERROR_DEBUG();

	
	//antiAlias
////	this->getTexture()->setAntiAliasTexParameters();
////	sprite->getTexture()->setAntiAliasTexParameters();
////	this->m_biggerRT->getSprite()->getTexture()->setAntiAliasTexParameters();
////	this->m_filterXRT->getSprite()->getTexture()->setAntiAliasTexParameters();
	this->m_filterYRT->getSprite()->getTexture()->setAntiAliasTexParameters();//only this line make difference
	
    return true;

}

void CghostSprite::draw()
{
    //----draw m_originTexSprite to m_biggerRT (use m_program_renderAlpha)
    {
        m_biggerRT->beginWithClear(0.0,0.0,0.0,0.0);
        {
            //because m_refSprite is ref from outside, so we must ensure it will not be changed
            //store
            CCPoint oldAnchorPoint=m_refSprite->getAnchorPoint();
            CCPoint oldPosition=m_refSprite->getPosition();
            float oldScaleX=m_refSprite->getScaleX();
            float oldScaleY=m_refSprite->getScaleY();
            bool oldIsVisible=m_refSprite->isVisible();
            //temp modify
            m_refSprite->setVisible(true);
            m_refSprite->setScaleY(-1);
            m_refSprite->setScaleX(1);
            m_refSprite->setAnchorPoint(ccp(0.5,0.5));
            m_refSprite->setPosition(ccp(m_biggerRT->getSprite()->getContentSize().width/2,m_biggerRT->getSprite()->getContentSize().height/2));
            m_refSprite->visit();
            //restore
            m_refSprite->setAnchorPoint(oldAnchorPoint);
            m_refSprite->setPosition(oldPosition);
            m_refSprite->setScaleX(oldScaleX);
            m_refSprite->setScaleY(oldScaleY);
            m_refSprite->setVisible(oldIsVisible);
        }
        m_biggerRT->end();
    }
   
    //----draw m_biggerRT to m_filterXRT (use m_program_fliterX)
    {
        //----m_biggerRT change shader
        ccGLEnable(m_eGLServerState);
        //pass values for cocos2d-x build-in uniforms
        m_biggerRT->getSprite()->setShaderProgram(m_program_filterX);
        CGLProgramWithUnifos*program=(CGLProgramWithUnifos*)m_biggerRT->getSprite()->getShaderProgram();
        program->use();
        program->setUniformsForBuiltins();
        //prepare uniform values
        float ds_tmp=1.0/m_biggerRT->getSprite()->getContentSize().width;
        //pass values for my own uniforms
        program->passUnifoValue1f("u_filterR", m_filterR*m_fliterSrcScale);
        program->passUnifoValue1f("u_ds", ds_tmp);
        program->passUnifoValue1f("u_s_step", ds_tmp/m_downSample);
        //----draw m_biggerRT to m_filterXRT
        m_filterXRT->beginWithClear(0.0,0.0,0.0,0.0);
        {
            //because m_biggerRT will be used again later, so we must ensure it will not be changed
            //store
            CCPoint oldAnchorPoint=m_biggerRT->getAnchorPoint();
            CCPoint oldPosition=m_biggerRT->getPosition();
            float oldScaleX=m_biggerRT->getScaleX();
            float oldScaleY=m_biggerRT->getScaleY();
            bool oldIsVisible=m_biggerRT->isVisible();
            //temp modify
            m_biggerRT->getSprite()->setScaleX(m_fliterSrcScale);
            m_biggerRT->getSprite()->setScaleY(-m_fliterSrcScale);
            m_biggerRT->getSprite()->setAnchorPoint(ccp(0.5,0.5));
            m_biggerRT->getSprite()->setPosition(ccp(m_filterXRT->getSprite()->getContentSize().width/2,m_filterXRT->getSprite()->getContentSize().height/2));
            m_biggerRT->getSprite()->visit();
            //restore
            m_biggerRT->setAnchorPoint(oldAnchorPoint);
            m_biggerRT->setPosition(oldPosition);
            m_biggerRT->setScaleX(oldScaleX);
            m_biggerRT->setScaleY(oldScaleY);
            m_biggerRT->setVisible(oldIsVisible);
            
        }
        m_filterXRT->end();
    }

      //----draw m_filterXRT to m_filterYRT (use m_program_fliterY)
    {
        //----m_filterXRT change shader
        ccGLEnable(m_eGLServerState);
        //pass values for cocos2d-x build-in uniforms
        m_filterXRT->getSprite()->setShaderProgram(m_program_filterY);
        CGLProgramWithUnifos*program=(CGLProgramWithUnifos*)m_filterXRT->getSprite()->getShaderProgram();
        program->use();
        program->setUniformsForBuiltins();
        //prepare uniform values
        float dt_tmp=1.0/m_filterXRT->getSprite()->getContentSize().height;
        //pass values for my own uniforms
        program->passUnifoValue1f("u_filterR", m_filterR*m_fliterSrcScale);
        program->passUnifoValue1f("u_dt", dt_tmp);
        program->passUnifoValue1f("u_t_step", dt_tmp/m_downSample);
        //pass texture attach point id to sampler uniform
        program->passUnifoValue1i("u_refTex", 1);
        //attach texture to texture attach point
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_biggerRT->getSprite()->getTexture()->getName());
        glActiveTexture(GL_TEXTURE0);//back to GL_TEXTURE0
        //----draw m_filterXRT to m_filterYRT
        m_filterYRT->beginWithClear(0.0,0.0,0.0,0.0);
        {
            m_filterXRT->getSprite()->setScaleX(1.0/m_fliterSrcScale);
            m_filterXRT->getSprite()->setScaleY(-1.0/m_fliterSrcScale);
            m_filterXRT->getSprite()->setAnchorPoint(ccp(0.5,0.5));
            m_filterXRT->getSprite()->setPosition(ccp(m_filterYRT->getSprite()->getContentSize().width/2,m_filterYRT->getSprite()->getContentSize().height/2));
            m_filterXRT->getSprite()->visit();
            
        }
        m_filterYRT->end();
    }
    //set the result tex to this sprite
    this->setTexture(m_filterYRT->getSprite()->getTexture());
    this->setTextureRect(m_filterYRT->getSprite()->getTextureRect());
    //----call father draw
    this->CCSprite::draw();//
}
namespace_ens_end
