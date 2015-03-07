//
//  ensFunc.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 13-10-26.
//
//

#include "ensFunc.h"
#include "ensMath.h"
namespace_ens_begin
string numberToStr(int n){//int to string
    char numbuf[128];
    sprintf(numbuf, "%i", n);//there is no itoa in xcode
    string numStr=numbuf;
    return numStr;
}
string valueToStr(float value,int nPrecision){//float to string
    if(value==floorf(value)){
        return numberToStr(value);
    }
    if(value==0)return string("0");
    //
    int sig;
    if(value==0){
        sig=0;
    }else if(value>0){
        sig=1;
    }else{
        sig=-1;
    }
    float abs=fabsf(value);
    int A=floor(abs);
    int B=floor((abs-A)*nPow(10,nPrecision)) ;
    string Astr=numberToStr(A);
    string Bstr=numberToStr(B);
    string zerosStr;
    int makeUpZeroCount=nPrecision-(int)Bstr.size();
    assert(makeUpZeroCount>=0);
    for(int i=0;i<makeUpZeroCount;i++)zerosStr+="0";
    string valueStr;
    if(sig<0)valueStr+="-";
    valueStr+=Astr+"."+zerosStr+Bstr;
    return valueStr;
}
Cv2 ccpTov2(const CCPoint&point){
    Cv2 v(point.x,point.y);
    return v;
}
CCPoint v2Toccp(const Cv2&v){
    CCPoint point(v.x(),v.y());
    return point;
}
namespace_ens_end
