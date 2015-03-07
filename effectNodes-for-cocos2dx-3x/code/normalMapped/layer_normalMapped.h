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
#include "ensNormalMappedNode.h"
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
	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
protected:
    //slider
    ControlSlider* m_pSliderCtl;
    void sliderAction(Ref *senderz, Control::EventType controlEvent);

	//slider2
	ControlSlider* m_pSliderCtl2;
    void sliderAction2(Ref *senderz, Control::EventType controlEvent);
 
    //slider3
	ControlSlider* m_pSliderCtl3;
    void sliderAction3(Ref *senderz, Control::EventType controlEvent);
    
    //slider4
	ControlSlider* m_pSliderCtl4;
    void sliderAction4(Ref *senderz, Control::EventType controlEvent);

protected:
    ControlButton* m_controlButton_back;
    void controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent);
protected:
    CnormalMappedSprite* m_normalMappedSprite;
    normalMapped::ClightSprite* m_lightSprite;
};

#endif /* defined(__HelloCpp__layer_normalMapped__) */
