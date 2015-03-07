//
//  ensFunc.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 13-10-26.
//
//

#ifndef __HelloCpp__ensFunc__
#define __HelloCpp__ensFunc__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "ensDefine.h"
#include "ensSimpleClasses.h"
namespace_ens_begin
string numberToStr(int n);
string valueToStr(float value,int nPrecision=2);
Cv2 ccpTov2(const CCPoint&point);
CCPoint v2Toccp(const Cv2&v);

namespace_ens_end
#endif /* defined(__HelloCpp__ensFunc__) */
