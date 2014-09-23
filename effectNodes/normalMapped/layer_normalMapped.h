//
//  layer_normalMapped.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__layer_normalMapped__
#define __HelloCpp__layer_normalMapped__
#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include "normalMapped/ensNormalMappedNode.h"
using namespace ens;
class Clayer_normalMapped : public CCLayer
{
public:
    Clayer_normalMapped(){
        m_normalMappedSprite=NULL;
        m_lightSprite=NULL;
    };
    virtual~Clayer_normalMapped(){}
    
    bool init();
protected:
    //touch
	virtual void ccTouchesBegan(cocos2d::CCSet* touches , cocos2d::CCEvent* event);
	virtual void ccTouchesMoved(cocos2d::CCSet* touches , cocos2d::CCEvent* event);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches , cocos2d::CCEvent* event);
protected:
    //slider
    CCControlSlider* m_pSliderCtl;
    void sliderAction(CCObject* sender, CCControlEvent controlEvent);

	//slider2
	CCControlSlider* m_pSliderCtl2;
    void sliderAction2(CCObject* sender, CCControlEvent controlEvent);
 
    //slider3
	CCControlSlider* m_pSliderCtl3;
    void sliderAction3(CCObject* sender, CCControlEvent controlEvent);
    
    //slider4
	CCControlSlider* m_pSliderCtl4;
    void sliderAction4(CCObject* sender, CCControlEvent controlEvent);

protected:
    CCControlButton* m_controlButton_back;
    void controlButtonEvent_back(CCObject *senderz, CCControlEvent controlEvent);
protected:
    CnormalMappedSprite* m_normalMappedSprite;
    normalMapped::ClightSprite* m_lightSprite;
};

#endif /* defined(__HelloCpp__layer_normalMapped__) */
