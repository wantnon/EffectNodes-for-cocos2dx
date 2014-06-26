//
//  layer_colorAdjust.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__layer_colorAdjust__
#define __HelloCpp__layer_colorAdjust__
#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include "colorAdjust/ensColorAdjustNode.h"
using namespace ens;
class Clayer_colorAdjust : public CCLayer
{
public:
    Clayer_colorAdjust(){
        m_colorAdjustSprite=NULL;
    };
    virtual~Clayer_colorAdjust(){}
    
    bool init();
    
protected:
    //slider
    CCControlSlider* m_pSliderCtl;
    void sliderAction(CCObject* sender, CCControlEvent controlEvent);
    CCLabelTTF* m_label;
	//slider2
	CCControlSlider* m_pSliderCtl2;
    void sliderAction2(CCObject* sender, CCControlEvent controlEvent);
    CCLabelTTF* m_label2;
    //slider3
	CCControlSlider* m_pSliderCtl3;
    void sliderAction3(CCObject* sender, CCControlEvent controlEvent);
    CCLabelTTF* m_label3;
protected:
    CCControlButton* m_controlButton_back;
    void controlButtonEvent_back(CCObject *senderz, CCControlEvent controlEvent);
protected:
    CcolorAdjustSprite* m_colorAdjustSprite;
};

#endif /* defined(__HelloCpp__layer_colorAdjust__) */
