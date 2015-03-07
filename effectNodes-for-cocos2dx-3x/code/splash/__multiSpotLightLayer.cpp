

#include "__multiSpotLightLayer.h"

bool __CmultiSpotLightLayer::init(){
    this->CCLayer::init();
    //m_maskRT
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    m_maskRT=CCRenderTexture::create(winSize.width, winSize.height);
    m_maskRT->retain();
    
    //m_spotSpriteRoot
    m_spotSpriteRoot=CCNode::create();
    m_spotSpriteRoot->retain();
    
    //m_renderMaskSprite
    m_renderMaskSprite=new __CmetaBallSprite();
    m_renderMaskSprite->autorelease();
    m_renderMaskSprite->init(m_maskRT->getSprite()->getTexture());
    addChild(m_renderMaskSprite);
    m_renderMaskSprite->setPosition(ccp(winSize.width/2,winSize.height/2));
    

    this->setTouchEnabled(true);
    return true;
}
void __CmultiSpotLightLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags){
    //grow m_spotSpritePool if necessary
    {
        int nSpot=(int)m_spotList.size();
        if((int)m_spotSpritePool.size()<nSpot){
            int nGrowLen=nSpot-(int)m_spotSpritePool.size();
            for(int i=0;i<nGrowLen;i++){
                __ChighlightSprite*spotSprite=new __ChighlightSprite();
                spotSprite->autorelease();
                spotSprite->init(m_spotList[i].m_texFileName);
                m_spotSpritePool.push_back(spotSprite);
                spotSprite->retain();
                
            }
        }//got m_spotSpritePool
    }
    //make m_spotSpriteList
    {
        m_spotSpriteRoot->removeAllChildrenWithCleanup(true);
        m_spotSpriteList.clear();
        int nSpot=(int)m_spotList.size();
        for(int i=0;i<nSpot;i++){
            m_spotSpriteList.push_back(m_spotSpritePool[i]);
            __ChighlightSprite*sprite=m_spotSpriteList[i];
            __Cspot spot=m_spotList[i];
            //pos
            sprite->setPosition(spot.m_c);
            //scale
            assert(sprite->getContentSize().width==sprite->getContentSize().height);
            sprite->setScale(spot.m_r*2/sprite->getContentSize().width);
            //rotation
            sprite->setRotation(-spot.m_A-45+180);
            //color and highlight
            ccColor4B color4b=ccc4BFromccc4F(spot.m_color);
            sprite->setColor(ccc3(color4b.r,color4b.g,color4b.b));
            sprite->m_highlight=spot.m_color.a;
            //blendFunc
       ////     ccBlendFunc blendFunc={GL_SRC_ALPHA,GL_ONE};
       ////     sprite->setBlendFunc(blendFunc);
            //add to m_spotSpriteRoot
            m_spotSpriteRoot->addChild(sprite);
        }//got m_spotSpriteList
    }
    //render m_spriteList to texture
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    m_spotSpriteRoot->setScaleY(-1);
    m_spotSpriteRoot->setPositionY(winSize.height);
    m_maskRT->beginWithClear(m_maskColor.r , m_maskColor.g , m_maskColor.b , m_maskColor.a);
    {
        
        m_spotSpriteRoot->visit();
        
        
    }
    m_maskRT->end();
  
    m_renderMaskSprite->setTexture(m_maskRT->getSprite()->getTexture());
    m_renderMaskSprite->setTextureRect(m_maskRT->getSprite()->getTextureRect());
    //must reset blendfunc after setTexture in cocos2dx 2.x, see: http://user.qzone.qq.com/350479720/blog/1411536646
    {
        ccBlendFunc blendFunc={GL_ONE,GL_ONE_MINUS_SRC_ALPHA};//{GL_DST_COLOR,GL_SRC_COLOR};
        m_renderMaskSprite->setBlendFunc(blendFunc);
        
    }
       
}



