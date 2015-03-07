//
//  ensTinyWingsTerrainNode.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 13-12-16.
//
//

#include "ensTinyWingsTerrainNode.h"
namespace_ens_begin
void CtinyWingsTerrainSprite::init(float width,float height,int insertControlPointCount){
    this->CCSprite::init();
    m_width=width;
    m_height=height;
    m_pointMat.resize(m_nRow);
    vector<CCPoint>&pointList=m_pointMat[0];
    int nseg=ceilf(m_width/m_dx);
    m_dx=m_width/nseg;//revise dx
    int nPoint=nseg+1;
    for(int i=0;i<nPoint;i++){
        float x=i*m_dx;
        CCPoint point(x,m_height);
        pointList.push_back(point);
        int nRow=(int)m_pointMat.size();
        for(int j=1;j<nRow;j++){
            CCPoint _point;
            _point.x=point.x;
            _point.y=point.y*(1-(float)j/(nRow-1));//must convert j to float !!!
            m_pointMat[j].push_back(_point);
        }
    }//got m_pointMat;
    //----create mesh
    m_mesh=new Cmesh();
    m_mesh->autorelease();
    m_mesh->retain();
    //----create indexVBO
    m_indexVBO=new CindexVBO();
    m_indexVBO->autorelease();
    m_indexVBO->retain();
    //----colorList
    m_colorList.push_back(ccc4f(random01(), random01(), random01(), 1));
    m_colorList.push_back(ccc4f(random01(), random01(), random01(), 1));
    m_colorList.push_back(ccc4f(random01(), random01(), random01(), 1));
    //----reShape
    reGenerateShape(insertControlPointCount);
    //----update mesh
    updateMesh();
    //----submitMesh
    submit();
    //----create and set shader program
	{
		GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/tinyWingsTerrain_nonlinearTexCoord.fsh").c_str())->getCString();
		CGLProgramWithUnifos* program = new CGLProgramWithUnifos();
        program->autorelease();
		program->initWithVertexShaderByteArray(ccPositionTextureColor_vert, fragSource);
        //            --below code is no longer needed, because bindPredefinedVertexAttribs() is called in link() in 3.x
        //            --bind attribute
        //            --program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        //            --program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        //            --program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        //link  (must after bindAttribute)
		program->link();
        //get cocos2d-x build-in uniforms
		program->updateUniforms();
        //get my own uniforms
        program->attachUniform("u_texNonlinearFactor");
        program->attachUniform("u_colors");
        program->attachUniform("u_cosA");
        program->attachUniform("u_sinA");
        program->attachUniform("u_ribbonRepeat");
        //set program
        m_program=program;
        m_program->retain();
        //check gl error
		CHECK_GL_ERROR_DEBUG();
	}



    
    
}
void CtinyWingsTerrainSprite::reGenerateColors(){
    m_colorList.clear();
    m_colorList.push_back(ccc4f(random01(), random01(), random01(), 1));
    m_colorList.push_back(ccc4f(random01(), random01(), random01(), 1));
    m_colorList.push_back(ccc4f(random01(), random01(), random01(), 1));
}
void CtinyWingsTerrainSprite::submit(){
    m_indexVBO->submitPos(m_mesh->vlist, GL_STATIC_DRAW);
    m_indexVBO->submitTexCoord(m_mesh->texCoordList, GL_STATIC_DRAW);
    m_indexVBO->submitColor(m_mesh->colorList, GL_STATIC_DRAW);
    m_indexVBO->submitIndex(m_mesh->IDtriList, GL_STATIC_DRAW);
}

void CtinyWingsTerrainSprite::reGenerateShape(int insertControlPointCount){
    m_insertControlPointCount=insertControlPointCount;
    vector<CCPoint>&pointList=m_pointMat[0];
    //generate controlPoints
    CCPoint startPoint=pointList[0];
    CCPoint endPoint=pointList[(int)pointList.size()-1];
    float len=endPoint.x-startPoint.x;
    float stepLen=len/(insertControlPointCount+1);
    vector<CCPoint> controlPointList;
    controlPointList.push_back(CCPoint(startPoint.x,m_height));//start control point
    for(int i=0;i<insertControlPointCount;i++){
        float x=(i+1)*stepLen;
        float y=m_height;
        controlPointList.push_back(CCPoint(x,y));
    }
    controlPointList.push_back(CCPoint(endPoint.x,m_height));//end control point. now got controlPointList
    //random y for each control point
    int nControlPoint=(int)controlPointList.size();
    for(int i=0;i<nControlPoint;i++){
        CCPoint&controlPoint=controlPointList[i];
        float dy=(random01()*2-1)*MIN(m_height*0.8,stepLen*0.7);
        if(i>0&&dy*(controlPointList[i-1].y-m_height)>0){
            dy=-dy;
        }
        controlPoint.y=m_height+dy;
    }
    //reshape pointList
    int controlPointIndex=0;
    int nPoint=(int)pointList.size();
    for(int i=0;i<nPoint;i++){
        CCPoint&point=pointList[i];
        //find the first control point whose x is bigger than point.x
        while(controlPointIndex<(int)controlPointList.size()&&controlPointList[controlPointIndex].x<=point.x){
            controlPointIndex++;
        }
        if(controlPointIndex>=(int)controlPointList.size()){//point is endPoint
            point=controlPointList[(int)controlPointList.size()-1];
            
        }else{
            assert(controlPointIndex>=1);
            CCPoint controlPoint=controlPointList[controlPointIndex-1];
            CCPoint controlPointn=controlPointList[controlPointIndex];
            
            //get point.y by sin interpolation
            const float dx=controlPointn.x-controlPoint.x;
            const float dy=controlPointn.y-controlPoint.y;
            point.y=controlPoint.y+dy/2*(1-cosf((M_PI/dx)*(point.x-controlPoint.x)));
        }
        int nRow=(int)m_pointMat.size();
        for(int j=1;j<nRow;j++){
            CCPoint&_point=m_pointMat[j][i];
            _point.y=point.y*(1-pow((float)j/(nRow-1),1));//must convert j to float !!!
        }
    }
    //update mesh and submit
    updateMesh();
    submit();
}

void CtinyWingsTerrainSprite::updateMesh(){
    
    m_mesh->clear();
    CCPoint startPoint=m_pointMat[0][0];
    CCPoint endPoint=m_pointMat[0][(int)m_pointMat[0].size()-1];
    const float len=endPoint.x-startPoint.x;
    int nRow=(int)m_pointMat.size();
    for(int i=0;i<nRow-1;i++){
        int nPoint=(int)m_pointMat[i].size();
        for(int j=0;j<nPoint-1;j++){
            const CCPoint&pLU=m_pointMat[i][j];
            const CCPoint&pLD=m_pointMat[i+1][j];
            const CCPoint&pRU=m_pointMat[i][j+1];
            const CCPoint&pRD=m_pointMat[i+1][j+1];
            
            //vLU
            Cv2 posLU=Cv2(pLU.x,pLU.y);
            Cv2 texCoordLU=Cv2((posLU.x()-startPoint.x)/len, (float)i/(nRow-1));
            Cv4 colorLU=Cv4(1,1,1,1);
            //vLD
            Cv2 posLD=Cv2(pLD.x,pLD.y);
            Cv2 texCoordLD=Cv2((posLD.x()-startPoint.x)/len, (float)(i+1)/(nRow-1));
            Cv4 colorLD=Cv4(1,1,1,1);
            //vRU
            Cv2 posRU=Cv2(pRU.x,pRU.y);
            Cv2 texCoordRU=Cv2((posRU.x()-startPoint.x)/len, (float)i/(nRow-1));
            Cv4 colorRU=Cv4(1,1,1,1);
            //vRD
            Cv2 posRD=Cv2(pRD.x,pRD.y);
            Cv2 texCoordRD=Cv2((posRD.x()-startPoint.x)/len, (float)(i+1)/(nRow-1));
            Cv4 colorRD=Cv4(1,1,1,1);
            
            //add vertex
            m_mesh->vlist.push_back(posLU);
            m_mesh->texCoordList.push_back(texCoordLU);
            m_mesh->colorList.push_back(colorLU);
            int IDLU=(int)m_mesh->vlist.size()-1;
            //
            m_mesh->vlist.push_back(posLD);
            m_mesh->texCoordList.push_back(texCoordLD);
            m_mesh->colorList.push_back(colorLD);
            int IDLD=(int)m_mesh->vlist.size()-1;
            //
            m_mesh->vlist.push_back(posRU);
            m_mesh->texCoordList.push_back(texCoordRU);
            m_mesh->colorList.push_back(colorRU);
            int IDRU=(int)m_mesh->vlist.size()-1;
            //
            m_mesh->vlist.push_back(posRD);
            m_mesh->texCoordList.push_back(texCoordRD);
            m_mesh->colorList.push_back(colorRD);
            int IDRD=(int)m_mesh->vlist.size()-1;
            //
            CIDTriangle IDtri0,IDtri1;
            IDtri0.init(IDLU,IDLD,IDRD);
            IDtri1.init(IDLU, IDRD, IDRU);
            //
            m_mesh->IDtriList.push_back(IDtri0);
            m_mesh->IDtriList.push_back(IDtri1);
            
            
            
            
            
        }
    }

}

void CtinyWingsTerrainSprite::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags){
    
        _customCommand.init(_globalZOrder);
        _customCommand.func = CC_CALLBACK_0(CtinyWingsTerrainSprite::onDraw,this,transform, flags);
        renderer->addCommand(&_customCommand);
    if(m_isDrawDebug){
        
        _customCommand_debug.init(_globalZOrder);
        _customCommand_debug.func = CC_CALLBACK_0(CtinyWingsTerrainSprite::onDrawDebug,this,transform, flags);
        renderer->addCommand(&_customCommand_debug);
    }
    
    
}
void CtinyWingsTerrainSprite::onDraw(const Mat4 &transform, uint32_t flags){

    
    //----change shader
    ccGLBlendFunc( _blendFunc.src, _blendFunc.dst );
    //pass values for cocos2d-x build-in uniforms
    this->setShaderProgram(m_program);
    CGLProgramWithUnifos*program=(CGLProgramWithUnifos*)this->getShaderProgram();
    program->use();
    //when program is have MV, we should use program->setUniformsForBuiltins(transform) instead of program->setUniformsForBuiltins()
    ////program->setUniformsForBuiltins();
    program->setUniformsForBuiltins(transform);
    //pass values for my own uniforms
    float colorList_tmp[4*3]={m_colorList[0].r,m_colorList[0].g,m_colorList[0].b,m_colorList[0].a,
        m_colorList[1].r,m_colorList[1].g,m_colorList[1].b,m_colorList[1].a,
        m_colorList[2].r,m_colorList[2].g,m_colorList[2].b,m_colorList[2].a};
    program->passUnifoValue1f("u_texNonlinearFactor", m_texNonlinearFactor);
    program->passUnifoValueNfvN("u_colors", colorList_tmp, 4, 3);
    program->passUnifoValue1f("u_cosA", cosf(m_ribbonRotation*M_PI/180));
    program->passUnifoValue1f("u_sinA", sinf(m_ribbonRotation*M_PI/180));
    program->passUnifoValue1f("u_ribbonRepeat", m_ribbonRepeat);
    //enable attributes
    bool isAttribPositionOn=CindexVBO::isEnabledAttribArray_position();
    bool isAttribColorOn=CindexVBO::isEnabledAttribArray_color();
    bool isAttribTexCoordOn=CindexVBO::isEnabledAttribArray_texCoord();
    CindexVBO::enableAttribArray_position(true);
    CindexVBO::enableAttribArray_color(true);
    CindexVBO::enableAttribArray_texCoord(true);
    //bind texture
    ccGLBindTexture2D( this->getTexture()->getName());
    //draw m_indexVBO
    m_indexVBO->setPointer_position();
    m_indexVBO->setPointer_texCoord();
    m_indexVBO->setPointer_color();
    m_indexVBO->draw(GL_TRIANGLES);
    //unbind texture
    ccGLBindTexture2D(0);
    //disable attributes
    CindexVBO::enableAttribArray_position(isAttribPositionOn);
    CindexVBO::enableAttribArray_color(isAttribColorOn);
    CindexVBO::enableAttribArray_texCoord(isAttribTexCoordOn);

    
}

void CtinyWingsTerrainSprite::onDrawDebug(const Mat4 &transform, uint32_t flags){
    const Mat4&m=transform;
    vector<CCPoint>&pointList=m_pointMat[0];
    //draw segList
    {
        glLineWidth(2);
        ccPointSize(4);
        int nPoint=(int)pointList.size();
        int nSeg=nPoint-1;
        for(int i=0;i<nSeg;i++){
            CCPoint point=pointList[i];
            CCPoint pointn=pointList[i+1];
            ccDrawLine(M4TransV2(m,point), M4TransV2(m,pointn));
            ccDrawPoint(M4TransV2(m,point));
            ccDrawPoint(M4TransV2(m,pointn));
        }
    }
    
    //draw mesh
    {
        glLineWidth(2);
        int nRow=(int)m_pointMat.size();
        for(int i=0;i<nRow-1;i++){
            int nPoint=(int)m_pointMat[i].size();
            for(int j=0;j<nPoint-1;j++){
                const CCPoint&pLU=m_pointMat[i][j];
                const CCPoint&pLD=m_pointMat[i+1][j];
                const CCPoint&pRU=m_pointMat[i][j+1];
                const CCPoint&pRD=m_pointMat[i+1][j+1];
            
                ccDrawLine(M4TransV2(m,pLU), M4TransV2(m,pLD));
                ccDrawLine(M4TransV2(m,pLD), M4TransV2(m,pRD));
                ccDrawLine(M4TransV2(m,pRD), M4TransV2(m,pRU));
                ccDrawLine(M4TransV2(m,pRU), M4TransV2(m,pLU));
            }
        }
        
    }
    

    
    
}

namespace_ens_end