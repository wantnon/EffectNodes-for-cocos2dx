//
//  ensGLProgramWithUnifos.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-2-18.
//
//

#ifndef __HelloCpp__ensGLProgramWithUnifos__
#define __HelloCpp__ensGLProgramWithUnifos__

#include <iostream>
#include<map>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "ensDefine.h"
namespace_ens_begin
//if program1 and program2 have a uniform variable with the same name, the two variable's uniform ID (GLint) may be different.
//so safe way is let each shader program hold his own uniform IDs.
class CGLProgramWithUnifos:public CCGLProgram
{
protected:
    map<string,GLint> m_unifoMap;
public:
	
    CGLProgramWithUnifos(){
	}
	virtual ~CGLProgramWithUnifos(){
	}
    void attachUniform(string unifoName){
        m_unifoMap[unifoName] = glGetUniformLocation(this->getProgram(),unifoName.c_str());
    }
    void passUnifoValue1f(string unifoName,GLfloat v);
    void passUnifoValue1i(string unifoName,GLint v);
    void passUnifoValueMatrixNfv(string unifoName,const GLfloat *array,int arrayLen);
    void passUnifoValueNfv(string unifoName,const GLfloat *array,int arrayLen);
    void passUnifoValueNfvN(string unifoName,const GLfloat*array,int arrayLen,int arrayCount);
    void passUnifoValueNiv(string unifoName,const GLint *array,int arrayLen);
    void passUnifoValueMatrixNfv(string unifoName,const vector<GLfloat>&valueList);
    void passUnifoValueNfv(string unifoName,const vector<GLfloat>&valueList);
    void passUnifoValueNiv(string unifoName,const vector<GLint>&valueList);
    
};
namespace_ens_end
#endif /* defined(__HelloCpp__ensGLProgramWithUnifos__) */
