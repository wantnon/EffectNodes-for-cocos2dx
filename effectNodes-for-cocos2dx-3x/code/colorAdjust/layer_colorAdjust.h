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
#include "ensColorAdjustNode.h"
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
    ControlSlider* m_pSliderCtl;
    void sliderAction(Ref *senderz, cocos2d::extension::Control::EventType controlEvent);
    CCLabelTTF* m_label;
	//slider2
	ControlSlider* m_pSliderCtl2;
    void sliderAction2(Ref *senderz, cocos2d::extension::Control::EventType controlEvent);
    CCLabelTTF* m_label2;
    //slider3
	ControlSlider* m_pSliderCtl3;
    void sliderAction3(Ref *senderz, cocos2d::extension::Control::EventType controlEvent);
    CCLabelTTF* m_label3;
protected:
    ControlButton* m_controlButton_back;
    void controlButtonEvent_back(Ref *senderz, cocos2d::extension::Control::EventType controlEvent);
protected:
    CcolorAdjustSprite* m_colorAdjustSprite;
};

#endif /* defined(__HelloCpp__layer_colorAdjust__) */
