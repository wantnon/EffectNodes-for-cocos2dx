
#include "ensNormalMappedNode.h"

namespace_ens_begin
bool CnormalMappedSprite::init(const string&texFileName,const string&normalMapFileName)
{

    //init this CCSprite
    this->CCSprite::initWithFile(texFileName.c_str());
    //create m_normalMap
    m_normalMap=CCTextureCache::sharedTextureCache()->addImage(normalMapFileName.c_str());
    m_normalMap->retain();
    //create lightSprite
    {
        normalMapped::ClightSprite*lightSprite_dft=new normalMapped::ClightSprite();
        lightSprite_dft->autorelease();
        lightSprite_dft->init("demoRes/light.png");
        this->setLightSprite(lightSprite_dft);
        lightSprite_dft->setPosition(ccp(0,0));//default lightSprite is located at parent-space's origin point
        lightSprite_dft->setZ(0);
    }
    //create and set shader program
	{
		GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/normalMapped.fsh").c_str())->getCString();
		CGLProgramWithUnifos* program = new CGLProgramWithUnifos();
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
        program->attachUniform("u_diffuseM");
        program->attachUniform("u_ambientM");
        program->attachUniform("u_diffuseL");
        program->attachUniform("u_ambientL");
        program->attachUniform("u_contentSize");
        program->attachUniform("u_lightPosInLocalSpace");
        program->attachUniform("u_kBump");
        //set program
        m_program=program;
        m_program->retain();
        //check gl error
		CHECK_GL_ERROR_DEBUG();
	}
	//check gl error
	CHECK_GL_ERROR_DEBUG();
   
    
    
	return true ;
}
void CnormalMappedSprite::onPassUnifoAndBindTex(const Mat4 &transform, uint32_t flags){

    assert(getGLProgram()==m_program);
    //CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    //CCLOG("winSize:%f,%f",winSize.width,winSize.height);
    //----prepare uniform values
    float diffuseM_tmp[4]={m_material.m_diffuse.r,m_material.m_diffuse.g,
        m_material.m_diffuse.b,m_material.m_diffuse.a};
    float ambientM_tmp[4]={m_material.m_ambient.r,m_material.m_ambient.g,
        m_material.m_ambient.b,m_material.m_ambient.a};
    float diffuseL_tmp[4]={m_lightSprite->getDiffuse().r,m_lightSprite->getDiffuse().g,
        m_lightSprite->getDiffuse().b,m_lightSprite->getDiffuse().a};
    float ambientL_tmp[4]={m_lightSprite->getAmbient().r,m_lightSprite->getAmbient().g,
        m_lightSprite->getAmbient().b,m_lightSprite->getAmbient().a};
    float contentSize_tmp[2]={this->getContentSize().width,this->getContentSize().height};
    CCSize contentSize=this->getContentSize();
    this->getNodeToParentTransform();//in order this->parentToNodeTransform() got right result, we must call this->getNodeToParentTransform() first, this is a bug of cocos2dx 3.3,see:http://www.cnblogs.com/wantnon/p/4330226.html
    CCPoint lightPosInLocalSpace=CCPointApplyAffineTransform(m_lightSprite->getPosition(),this->parentToNodeTransform());
    float lightPosInLocalSpace_tmp[4]={lightPosInLocalSpace.x,lightPosInLocalSpace.y,m_lightSprite->getZ(),1};
    CGLProgramWithUnifos*program=(CGLProgramWithUnifos*)this->getShaderProgram();
    program->use();
    program->setUniformsForBuiltins();
    
    //pass values for my own uniforms
    program->passUnifoValueNfv("u_diffuseM", diffuseM_tmp,4);
    program->passUnifoValueNfv("u_ambientM",ambientM_tmp,4);
    program->passUnifoValueNfv("u_diffuseL",diffuseL_tmp,4);
    program->passUnifoValueNfv("u_ambientL",ambientL_tmp,4);
    program->passUnifoValueNfv("u_contentSize", contentSize_tmp, 2);
    program->passUnifoValueNfv("u_lightPosInLocalSpace",lightPosInLocalSpace_tmp,4);
    program->passUnifoValue1f("u_kBump", m_kBump);
    // CCLOG("contentSize:%f,%f",contentSize_tmp[0],contentSize_tmp[1]);
    // CCLOG("lightPosInLocalSpace:%f,%f",lightPosInLocalSpace_tmp[0],lightPosInLocalSpace_tmp[1]);
    //attach texture to texture attach point
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalMap->getName());
    glActiveTexture(GL_TEXTURE0);//back to GL_TEXTURE0
}
void CnormalMappedSprite::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags){
    //change shader
    this->setShaderProgram(m_program);
    
    //passUnifo and bindTex
    _passUnifoAndBindTexCommand.init(_globalZOrder);
    _passUnifoAndBindTexCommand.func = CC_CALLBACK_0(CnormalMappedSprite::onPassUnifoAndBindTex, this,transform,flags);
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

namespace_ens_end