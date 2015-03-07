//
//  layer_tinyWingsTerrain.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__layer_tinyWingsTerrain__
#define __HelloCpp__layer_tinyWingsTerrain__

#include <iostream>
#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include "ensTinyWingsTerrainNode.h"
#include "CCControlButton.h"
using namespace ens;
class Clayer_tinyWingsTerrain : public CCLayer
{
public:
   
    Clayer_tinyWingsTerrain(){
        m_tinyWingsTerrainSprite=NULL;
        m_isDebugOn=false;
    }
    virtual ~Clayer_tinyWingsTerrain(){
    }
    
    bool init();
protected:
    //slider
    ControlSlider* m_pSliderCtl;
    void sliderAction(Ref *senderz, Control::EventType controlEvent);
    CCLabelTTF* m_label;
    //ribbonRotation
    ControlSlider* m_pSliderCtl_ribbonRotation;
    void sliderAction_ribbonRotation(Ref *senderz, Control::EventType controlEvent);
    CCLabelTTF* m_label_ribbonRotation;
    //ribbonRepeat
    ControlSlider* m_pSliderCtl_ribbonRepeat;
    void sliderAction_ribbonRepeat(Ref *senderz, Control::EventType controlEvent);;
    CCLabelTTF* m_label_ribbonRepeat;
protected:
    //debug
    ControlButton* m_controlButton_debug;
    void controlButtonEvent_debug(Ref *senderz, Control::EventType controlEvent);
    CCLabelTTF* m_label_debugState;
    bool m_isDebugOn;
    //reGenerate
    ControlButton* m_controlButton_reGenerate;
    void controlButtonEvent_reGenerate(Ref *senderz, Control::EventType controlEvent);;

    //back
    ControlButton* m_controlButton_back;
    void controlButtonEvent_back(Ref *senderz, Control::EventType controlEvent);;
protected:
    CtinyWingsTerrainSprite*m_tinyWingsTerrainSprite;
};

#endif
