//
//  ensIndexVBO.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-2-18.
//
//

#ifndef __HelloCpp__ensIndexVBO__
#define __HelloCpp__ensIndexVBO__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "ensDefine.h"
#include "ensSimpleClasses.h"
namespace_ens_begin

class CindexVBO :public CCObject
{

public:
	CindexVBO(){
		posBuffer=0 ;
		texCoordBuffer=0 ;
		indexBuffer=0 ;
        colorBuffer=0;
		m_indexArrayLen=0;
	    m_posArrayLen=0;
	    m_texCoordArrayLen=0;
        m_colorArrayLen=0;
        init_dft();
	}
    
	virtual ~CindexVBO(){
		if(posBuffer!=0)glDeleteBuffers(1, &posBuffer) ;
		if(texCoordBuffer!=0)glDeleteBuffers(1, &texCoordBuffer) ;
		if(indexBuffer!=0)glDeleteBuffers(1, &indexBuffer) ;
        if(colorBuffer!=0)glDeleteBuffers(1, &colorBuffer) ;
        
	}
	
    static void enableAttribArray_position(bool isEnable){
        if(isEnable)glEnableVertexAttribArray(kCCVertexAttrib_Position);
        else glDisableVertexAttribArray(kCCVertexAttrib_Position);
    }
    static void enableAttribArray_texCoord(bool isEnable){
        if(isEnable)glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);
        else glDisableVertexAttribArray(kCCVertexAttrib_TexCoords);
    }
   
    static void enableAttribArray_color(bool isEnable){
        if(isEnable)glEnableVertexAttribArray(kCCVertexAttrib_Color);
        else glDisableVertexAttribArray(kCCVertexAttrib_Color);
    }
    
    
    static bool isEnabledAttribArray_position(){
        GLint isEnabled;
        glGetVertexAttribiv(kCCVertexAttrib_Position,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&isEnabled);
        return (bool)isEnabled;
    }
    static bool isEnabledAttribArray_texCoord(){
        GLint isEnabled;
        glGetVertexAttribiv(kCCVertexAttrib_TexCoords,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&isEnabled);
        return (bool)isEnabled;
    }
    static bool isEnabledAttribArray_color(){
        GLint isEnabled;
        glGetVertexAttribiv(kCCVertexAttrib_Color,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&isEnabled);
        return (bool)isEnabled;
    }
	
    void submitPos(const vector<Cv2> &pointList,GLenum usage){
		m_posArrayLen=((int)pointList.size())*2;
		if(m_posArrayLen==0)return;
		glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_posArrayLen, &pointList.front(), usage);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void submitTexCoord(const vector<Cv2> &texCoordList,GLenum usage){
		m_texCoordArrayLen=((int)texCoordList.size())*2;
		if(m_texCoordArrayLen==0)return;
	    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_texCoordArrayLen, &texCoordList.front(), usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
    void submitColor(const vector<Cv4> &colorList,GLenum usage){
		m_colorArrayLen=((int)colorList.size())*4;
		if(m_colorArrayLen==0)return;
	    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_colorArrayLen, &colorList.front(), usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void submitIndex(const vector<CIDTriangle> &IDtriList,GLenum usage){//CIDTriangle's element must use short type
		m_indexArrayLen=((int)IDtriList.size())*3;
        if(m_indexArrayLen==0)return;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short)*m_indexArrayLen, &IDtriList.front(), usage);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
    void submitIndex(const vector<CIDLine> &IDlineList,GLenum usage){//CIDLine's element must use short type
        m_indexArrayLen=((int)IDlineList.size())*2;
        if(m_indexArrayLen==0)return;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short)*m_indexArrayLen, &IDlineList.front(), usage);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
     }
    void submitIndex(const vector<short> &indexList,GLenum usage){//element must use short type
		m_indexArrayLen=((int)indexList.size())*1;
        if(m_indexArrayLen==0)return;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short)*m_indexArrayLen, &indexList.front(), usage);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
    void setPointer_position(){
		//set position format
		glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
		glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void setPointer_texCoord(){
		//set texCoord format
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
		glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void setPointer_color(){
        ///set color format
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void draw(GLenum mode){
        if(m_indexArrayLen==0)return;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        //the third parameter of glDrawElements must be GL_UNSIGNED_SHORT or GL_UNSIGNED_BYTE
        //or there will be OpenGL error 0x0500
        //http://blog.csdn.net/zhucde/article/details/3547442
        //http://www.khronos.org/opengles/sdk/docs/man/xhtml/glDrawElements.xml
        //http://www.opengl.org/wiki/OpenGL_Error
        //here we use GL_UNSIGNED_SHORT
        glDrawElements(mode, m_indexArrayLen, GL_UNSIGNED_SHORT, 0) ;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
protected:
    void init_dft(){
        genBuffers();
    }
    void genBuffers(){
		if(posBuffer==0)glGenBuffers(1, &posBuffer);
		if(texCoordBuffer==0)glGenBuffers(1, &texCoordBuffer);
		if(indexBuffer==0)glGenBuffers(1, &indexBuffer);
        if(colorBuffer==0)glGenBuffers(1, &colorBuffer);
	}
protected:
	GLuint posBuffer ;
    GLuint texCoordBuffer ;
    GLuint indexBuffer ;
    GLuint colorBuffer ;
	int m_indexArrayLen;
	int m_posArrayLen;
	int m_texCoordArrayLen;
    int m_colorArrayLen;
};




namespace_ens_end
#endif /* defined(__HelloCpp__ensIndexVBO__) */