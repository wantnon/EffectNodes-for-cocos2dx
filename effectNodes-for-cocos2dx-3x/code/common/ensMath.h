//
//  ensMath.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-6-14.
//
//

#ifndef __HelloCpp__ensMath__
#define __HelloCpp__ensMath__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "ensDefine.h"
#include "ensSimpleClasses.h"
namespace_ens_begin

float random01();
float randomInRange(float min,float max);
bool catchProbability(float p);
bool isPointEqual(const CCPoint&point1,const CCPoint&point2,float eps);
CCPoint rotateVector2(const CCPoint&vec,float A);
CCPoint rotateVector2(const CCPoint&vec,float cosA,float sinA);
float calculateAngleFromCosAndSin(float cosA,float sinA);
CCPoint calculateCosASinAOfVec1ToVec2(const CCPoint&vec1,const CCPoint&vec2);
float nPow(float a,int n);
float pow2(float x);
float PNDisPointToLine(const CCPoint&point,const CCPoint&start,const CCPoint&end);
bool isPointEqual(const CCPoint&p1,const CCPoint&p2);
CCPoint getIntersectPointOfTwoLine(const CCPoint&p1,const CCPoint&dir1, const CCPoint&p2,const CCPoint&dir2);
float calculateIncircleR2(const CCPoint&v0,const CCPoint&v1,const CCPoint&v2);
vector<CCPoint> makeTriangleSmaller(CCPoint v0,CCPoint v1,CCPoint v2,float d);
CCPoint getRightNormal(const CCPoint&dir);
CCPoint getRightVector(const CCPoint&p1,const CCPoint&p2);
float calculateTriangleArea(const CCPoint&v0,const CCPoint&v1,const CCPoint&v2);
vector<CCPoint> calculateTangentPointsOfPointToCircle(const CCPoint &center, float r,const CCPoint &point);
Vec2 M4TransV2(const Mat4&mat,const Vec2&v);
namespace_ens_end

#endif /* defined(__HelloCpp__ensMath__) */
