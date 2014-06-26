
#include "colorAdjust/ensColorAdjustNode.h"

namespace_ens_begin
bool CcolorAdjustSprite::init(const string&texFileName)
{

    //init this CCSprite
    this->CCSprite::initWithFile(texFileName.c_str());
    //blendfunc
    ccBlendFunc blendFunc={GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
    this->setBlendFunc(blendFunc);
  
    //create and set shader program
	{
		GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/colorAdjustHSL.fsh").c_str())->getCString();
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
        program->attachUniform("u_dH");
        program->attachUniform("u_dS");
        program->attachUniform("u_dL");
        //set program
        m_program_adjustColorHSL=program;
        m_program_adjustColorHSL->retain();
        //check gl error
		CHECK_GL_ERROR_DEBUG();
	}
	//check gl error
	CHECK_GL_ERROR_DEBUG();
   
    
    
	return true ;
}
void CcolorAdjustSprite::draw()
{
    
    //----change shader
    ccGLEnable(m_eGLServerState);
    //pass values for cocos2d-x build-in uniforms
    this->setShaderProgram(m_program_adjustColorHSL);
    CGLProgramWithUnifos*program=(CGLProgramWithUnifos*)this->getShaderProgram();
    program->use();
    program->setUniformsForBuiltins();
    //pass values for my own uniforms
    program->passUnifoValue1f("u_dH", m_dH);
    program->passUnifoValue1f("u_dS", m_dS);
    program->passUnifoValue1f("u_dL", m_dL);
    
    //----call CCSprite::draw
    CCSprite::draw();
}
namespace_ens_end