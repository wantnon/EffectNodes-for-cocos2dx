//
//  ensMesh.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-16.
//
//

#ifndef __HelloCpp__ensMesh__
#define __HelloCpp__ensMesh__

#include <iostream>
#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "ensSimpleClasses.h"
namespace_ens_begin
class Cmesh:public CCObject
{
public:
    vector<Cv2> vlist;
    vector<Cv2> texCoordList;
    vector<Cv4> colorList;
    vector<CIDTriangle> IDtriList;
    void clear(){
        vlist.clear();
        texCoordList.clear();
        colorList.clear();
        IDtriList.clear();
    }
};

class CmeshWire:public CCObject
{
public:
    vector<Cv2> vlist;
    vector<Cv2> texCoordList;
    vector<Cv4> colorList;
    vector<CIDLine> IDlineList;
    void clear(){
        vlist.clear();
        texCoordList.clear();
        colorList.clear();
        IDlineList.clear();
    }
    
};
class CmeshGeneral:public CCObject
{
public:
    vector<Cv2> vlist;
    vector<Cv2> texCoordList;
    vector<Cv4> colorList;
    vector<short> indexList;
    void clear(){
        vlist.clear();
        texCoordList.clear();
        colorList.clear();
        indexList.clear();
    }
    
};
namespace_ens_end
#endif /* defined(__HelloCpp__ensMesh__) */
