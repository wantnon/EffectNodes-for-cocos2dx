//
//  ensSimpleClasses.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 13-12-4.
//
//

#ifndef __HelloCpp__ensSimpleClasses__
#define __HelloCpp__ensSimpleClasses__

#include <iostream>
#include <vector>
#include <string>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include <assert.h>
#include "ensDefine.h"
namespace_ens_begin
class CIDTriangle /* final*/
{
public:
    CIDTriangle(){}
    /**virtual*/~CIDTriangle(){}//not use virtual destructor !
    CIDTriangle(short ID0,short ID1,short ID2){
        initMembers();
        init(ID0, ID1, ID2);
    }
    void init(short ID0,short ID1,short ID2){
        m_vID[0]=ID0;
        m_vID[1]=ID1;
        m_vID[2]=ID2;
    }
    short getvIDByIndex(int index)const {
        assert(index>=0&&index<3);
        return m_vID[index];
    }
    void setvIDByIndex(int index,short ID){
        assert(index>=0&&index<3);
        m_vID[index]=ID;
    }
protected:
    void initMembers(){
        m_vID[0]=m_vID[1]=m_vID[2]=0;
    }
protected:
    short m_vID[3];
    
};

class CIDLine/* final*/
{
public:
    CIDLine(){}
    /**virtual*/~CIDLine(){}//not use virtual destructor !
    CIDLine(short ID0,short ID1){
        initMembers();
        init(ID0, ID1);
    }
    void init(short ID0,short ID1){
        m_vID[0]=ID0;
        m_vID[1]=ID1;
    }
    short getvIDByIndex(int index){
        assert(index>=0&&index<2);
        return m_vID[index];
    }
    void setvIDByIndex(int index,short ID){
        assert(index>=0&&index<2);
        m_vID[index]=ID;
    }
protected:
    void initMembers(){
        m_vID[0]=m_vID[1]=0;
    }
protected:
    short m_vID[2];
};


class Cv2/* final*/
{
public:
    Cv2(){}
    Cv2(float _x,float _y){
        initMembers();
        init(_x, _y);
    }
    /**virtual*/~Cv2(){};//not use virtual destructor !
    void init(float x,float y){
        m_v[0]=x;
        m_v[1]=y;
    }
    void setx(float x){m_v[0]=x;}
    void sety(float y){m_v[1]=y;}
    float x()const{return m_v[0];}
    float y()const{return m_v[1];}
    float*getArray(){return m_v;}
protected:
    void initMembers(){
        m_v[0]=m_v[1]=0;
    }
protected:
    float m_v[2];
    
};

class Cv4 /*final*/
{
public:
    Cv4(){}
    Cv4(float x,float y,float z,float w){
        initMembers();
        init(x, y, z, w);
    }
    /**virtual*/~Cv4(){};//not use virtual destructor !
    void init(float x,float y,float z,float w){
        m_v[0]=x;
        m_v[1]=y;
        m_v[2]=z;
        m_v[3]=w;
    }
    float x()const{return m_v[0];}
    float y()const{return m_v[1];}
    float z()const{return m_v[2];}
    float w()const{return m_v[3];}
    void setx(float x){m_v[0]=x;}
    void sety(float y){m_v[1]=y;}
    void setz(float z){m_v[2]=z;}
    void setw(float w){m_v[3]=w;}
protected:
    void initMembers(){
        m_v[0]=0;
        m_v[1]=0;
        m_v[2]=0;
        m_v[3]=0;
    }
protected:
    float m_v[4];
    
};
class Cij{
public:
    int m_i;
    int m_j;
    Cij(){
        initMembers();
    }
    Cij(int i,int j){
        initMembers();
        init(i,j);
    }
    void init(int i,int j){
        m_i=i;
        m_j=j;
    }
protected:
    void initMembers(){
        m_i=m_j=0;
    }
};

class Cpolygon
{
public:
    vector<CCPoint> m_pointList;
    
};
Cpolygon transPolygonByM4(const Cpolygon&polygon,const Mat4&mat);
Cpolygon makeRegularPolygon(int r,int n);
Cpolygon makeRectPolygon(float rx,float ry);
namespace_ens_end
#endif /* defined(__HelloCpp__ensSimpleClasses__) */
