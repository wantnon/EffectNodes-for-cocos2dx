//
//  ensDefine.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-14.
//
//

#ifndef __HelloCpp__ensDefine__
#define __HelloCpp__ensDefine__

#include <iostream>
using namespace std;
#define namespace_ens_begin namespace ens{
#define namespace_ens_end }

namespace_ens_begin
#ifndef	INFINITY
#define INFINITY (0.5*(numeric_limits<float>::max)())
#endif
namespace_ens_end
#endif /* defined(__HelloCpp__ensDefine__) */
