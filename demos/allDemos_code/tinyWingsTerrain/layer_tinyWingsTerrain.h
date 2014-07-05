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
#include "tinyWingsTerrain/ensTinyWingsTerrainNode.h"
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
    CCControlSlider* m_pSliderCtl;
    void sliderAction(CCObject* sender, CCControlEvent controlEvent);
    CCLabelTTF* m_label;
    //ribbonRotation
    CCControlSlider* m_pSliderCtl_ribbonRotation;
    void sliderAction_ribbonRotation(CCObject* sender, CCControlEvent controlEvent);
    CCLabelTTF* m_label_ribbonRotation;
    //ribbonRepeat
    CCControlSlider* m_pSliderCtl_ribbonRepeat;
    void sliderAction_ribbonRepeat(CCObject* sender, CCControlEvent controlEvent);
    CCLabelTTF* m_label_ribbonRepeat;
protected:
    //debug
    CCControlButton* m_controlButton_debug;
    void controlButtonEvent_debug(CCObject *senderz, CCControlEvent controlEvent);
    CCLabelTTF* m_label_debugState;
    bool m_isDebugOn;
    //reGenerate
    CCControlButton* m_controlButton_reGenerate;
    void controlButtonEvent_reGenerate(CCObject *senderz, CCControlEvent controlEvent);

    //back
    CCControlButton* m_controlButton_back;
    void controlButtonEvent_back(CCObject *senderz, CCControlEvent controlEvent);
protected:
    CtinyWingsTerrainSprite*m_tinyWingsTerrainSprite;
};

#endif
