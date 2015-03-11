

#ifndef __HelloCpp__spotLightLayer__
#define __HelloCpp__spotLightLayer__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "ensCommon.h"
class __Cspot{
public:
    __Cspot(){
        m_strength=1;
        m_r=0;
        m_damping=1.0;
        m_A=0;
    }
    __Cspot(CCPoint c,float r,ccColor4F color){
        init(c, r,color);
    }
    void init(CCPoint c,float r,ccColor4F color){
        m_r=r;
        m_c=c;
        m_color=color;
    }
    float m_r;
    CCPoint m_c;
    float m_strength;
    ccColor4F m_color;//m_color.a can greater than 1.0 for achieve highlight
    CCPoint m_velocity;
    float m_A;
    float m_damping;
    string m_texFileName;
};

class __ChighlightSprite:public CCSprite
{
public:
    __ChighlightSprite(){
        m_highlight=1.0;
        m_program=NULL;
    }
    virtual~__ChighlightSprite(){
        if(m_program)m_program->release();
    }
    bool init(CCTexture2D*texture){
        this->CCSprite::initWithTexture(texture);
        //mask shader
        {
            GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/splash/highlight.fsh").c_str())->getCString();
            ens::CGLProgramWithUnifos* program = new ens::CGLProgramWithUnifos();
            program->autorelease();
            program->initWithVertexShaderByteArray(ccPositionTextureColor_noMVP_vert, fragSource);
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
            program->attachUniform("u_highlight");
            //make program
            m_program=program;
            m_program->retain();
            //check gl error
            CHECK_GL_ERROR_DEBUG();
        }
        //check gl error
        CHECK_GL_ERROR_DEBUG();
        return true;
    }
    bool init(const string&texFileName){
        CCTexture2D*texture=CCTextureCache::sharedTextureCache()->addImage(texFileName.c_str());
        return this->init(texture);
    }

    void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags){
        //----change shader
        this->setShaderProgram(m_program);
        ens::CGLProgramWithUnifos*program=(ens::CGLProgramWithUnifos*)this->getShaderProgram();
        /////ccGLEnable(m_eGLServerState);
        //pass values for cocos2d-x build-in uniforms
        program->use();
        program->setUniformsForBuiltins();
        //pass values for my own uniforms
        program->passUnifoValue1f("u_highlight", m_highlight);
        //draw quad
        {
            _quadCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, &_quad, 1, transform);
            renderer->addCommand(&_quadCommand);
        }

    
    }
    
    float m_highlight;//use highlight instead of opacity
    ens::CGLProgramWithUnifos*m_program;

};



class __CmetaBallSprite:public CCSprite
{
public:
    __CmetaBallSprite(){
        m_highlight=1.0;
        m_program=NULL;
    }
    virtual~__CmetaBallSprite(){
        if(m_program)m_program->release();
    }
    bool init(CCTexture2D*texture){
        this->CCSprite::initWithTexture(texture);
        //mask shader
        {
            GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/splash/metaBall.fsh").c_str())->getCString();
            ens::CGLProgramWithUnifos* program = new ens::CGLProgramWithUnifos();
            program->autorelease();
            program->initWithVertexShaderByteArray(ccPositionTextureColor_noMVP_vert, fragSource);
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
            program->attachUniform("u_highlight");
            //make program
            m_program=program;
            m_program->retain();
            //check gl error
            CHECK_GL_ERROR_DEBUG();
        }
        //check gl error
        CHECK_GL_ERROR_DEBUG();
        
        return true;
    }
    bool init(const string&texFileName){
        CCTexture2D*texture=CCTextureCache::sharedTextureCache()->addImage(texFileName.c_str());
        return this->init(texture);
    }
    void onPassUnifoAndBindTex(const Mat4 &transform, uint32_t flags){
        assert(getGLProgram()==m_program);
        ens::CGLProgramWithUnifos*program=(ens::CGLProgramWithUnifos*)this->getShaderProgram();
        /////ccGLEnable(m_eGLServerState);
        //pass values for cocos2d-x build-in uniforms
        program->use();
        //when program is have MV, we should use program->setUniformsForBuiltins(transform) instead of program->setUniformsForBuiltins()
        ////program->setUniformsForBuiltins();
        program->setUniformsForBuiltins(transform);
        //pass values for my own uniforms
        program->passUnifoValue1f("u_highlight", m_highlight);
    
    }
    void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags){
        
        //----change shader
        this->setShaderProgram(m_program);
        //change shader and passUnifo and bindTex
        _passUnifoAndBindTexCommand.init(_globalZOrder);
        _passUnifoAndBindTexCommand.func = CC_CALLBACK_0(__CmetaBallSprite::onPassUnifoAndBindTex, this,transform,flags);
        Director::getInstance()->getRenderer()->addCommand(&_passUnifoAndBindTexCommand);
        //draw quad
        _quadCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, &_quad, 1, transform);
        renderer->addCommand(&_quadCommand);
        
        
        
    }
    
    float m_highlight;//use highlight instead of opacity
    ens::CGLProgramWithUnifos*m_program;
    
    CustomCommand _passUnifoAndBindTexCommand;
    
    
    
};


class __CmultiSpotLightLayer:public CCLayer
{
public:
    __CmultiSpotLightLayer(){

        m_maskRT=NULL;
        m_renderMaskSprite=NULL;
        m_spotSpriteRoot=NULL;
        m_maskColor=ccc4f(0, 0, 0, 1);

    }
    virtual ~__CmultiSpotLightLayer(){
 
        if(m_maskRT){m_maskRT->release();m_maskRT=NULL;}
        if(m_spotSpriteRoot){m_spotSpriteRoot->release();m_spotSpriteRoot=NULL;}
        for(int i=0;i<(int)m_spotSpritePool.size();i++){
            m_spotSpritePool[i]->release();
            m_spotSpritePool[i]=NULL;
        }m_spotSpriteList.clear();
    
    }
    void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
    bool init();
    __CmetaBallSprite*getRenderMaskSprite(){return m_renderMaskSprite;}
public:
    vector<__Cspot> m_spotList;
    ccColor4F m_maskColor;
protected:
    CCRenderTexture*m_maskRT;
    __CmetaBallSprite*m_renderMaskSprite;
    vector<__ChighlightSprite*> m_spotSpritePool;
    vector<__ChighlightSprite*> m_spotSpriteList;
    CCNode* m_spotSpriteRoot;


    

    
};

#endif /* defined(__HelloCpp__spotLightLayer__) */
