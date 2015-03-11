//
//  ensRipple_horizontalNode.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 13-12-16.
//
//

#include "ensRipple_horizontalNode.h"
namespace_ens_begin
void Cripple_horizontalNode::init(const string&texFileName){
    this->CCSprite::initWithFile(texFileName.c_str());
    CCSize contentSize=this->getContentSize();
    int nseg=ceilf(contentSize.width/m_dx);
    m_dx=contentSize.width/nseg;//revise dx
    int nPoint=nseg+1;
    for(int i=0;i<nPoint;i++){
        float x=i*m_dx;
        CCPoint point(x,contentSize.height);
        m_surfacePointList.push_back(point);
    }//got surfacePointList;
    //surfacePointList_back copy surfacePointList
    m_surfacePointList_back=m_surfacePointList;
    //----create mesh
    m_mesh=new Cmesh();
    m_mesh->autorelease();
    m_mesh->retain();
    //----create indexVBO
    m_indexVBO=new CindexVBO();
    m_indexVBO->autorelease();
    m_indexVBO->retain();
    //----init mesh
    initMesh();
    //----submitMesh
    m_indexVBO->submitPos(m_mesh->vlist, GL_STATIC_DRAW);
    m_indexVBO->submitTexCoord(m_mesh->texCoordList, GL_STATIC_DRAW);
    m_indexVBO->submitColor(m_mesh->colorList, GL_STATIC_DRAW);
    m_indexVBO->submitIndex(m_mesh->IDtriList, GL_STATIC_DRAW);
    
    
}
void Cripple_horizontalNode::pressAtX(float x,float h_press,float r_press)
//if h_press>0 is press
//if h_press<0 is pull up
//x is in parentSpace
{
    //get surfaceline's startPoint and endPoint
    if((int)m_surfacePointList.size()==0)return;
    if((int)m_surfacePointList.size()==1)return;
    CCPoint startPoint_local=m_surfacePointList[0];
    CCPoint endPoint_local=m_surfacePointList[(int)m_surfacePointList.size()-1];
    //convert startPoint and endPoint to parent space
    CCPoint startPoint=CCPointApplyAffineTransform(startPoint_local,nodeToParentTransform());
    CCPoint endPoint=CCPointApplyAffineTransform(endPoint_local,nodeToParentTransform());
    //calculate the intersect point of line(X=x) and line(startPoint-endPoint)
    CCPoint leftPoint=(startPoint.x<endPoint.x)?startPoint:endPoint;
    CCPoint rightPoint=(startPoint.x<endPoint.x)?endPoint:startPoint;
    if(x>leftPoint.x&&x<rightPoint.x){//have intersect`
        assert(leftPoint.x!=rightPoint.x);
        float y=(leftPoint.y-rightPoint.y)/(leftPoint.x-rightPoint.x)*(x-leftPoint.x)+leftPoint.y;
        CCPoint touchPoint(x,y);//parent space touchPoint
        this->getNodeToParentTransform();//in order this->parentToNodeTransform() got right result, we must call this->getNodeToParentTransform() first, this is a bug of cocos2dx 3.3,see:http://www.cnblogs.com/wantnon/p/4330226.html
        CCPoint touchPoint_local=CCPointApplyAffineTransform(touchPoint,parentToNodeTransform());
        //use touchPoint_local.x to press the surfacePointList
        float touchx=touchPoint_local.x;
        //press
        int nPoint=(int)m_surfacePointList.size();
        for(int i=0;i<nPoint;i++){
            if(i==0||i==nPoint-1)continue;//do not press the start and end point
            CCPoint&point=m_surfacePointList[i];
            float r=fabsf(point.x-touchx);
            if(r>r_press)continue;
            point.y-=(cosf(r/r_press*M_PI)+1)/2*h_press;
        }
    }else{//do not have intersect
        return;
    }
}
void Cripple_horizontalNode::initMesh(){
    m_mesh->clear();
    m_surfacePointList_reduced.clear();
    for(int i=0;i<(int)m_surfacePointList.size();i+=m_reduceStep){
        CCPoint pos=m_surfacePointList[i];
        m_surfacePointList_reduced.push_back(pos);
    }
    m_surfacePointList_reduced.push_back(m_surfacePointList[(int)m_surfacePointList.size()-1]);
    //got m_surfacePointList_reduced
    CCPoint startPoint=m_surfacePointList_reduced[0];
    CCPoint endPoint=m_surfacePointList_reduced[(int)m_surfacePointList_reduced.size()-1];
    const float len=endPoint.x-startPoint.x;
    int nPoint=(int)m_surfacePointList_reduced.size();
    for(int i=0;i<nPoint-1;i++){
        const CCPoint&p=m_surfacePointList_reduced[i];
        const CCPoint&pn=m_surfacePointList_reduced[i+1];
        //LU
        Cv2 posLU=Cv2(p.x,p.y);
        Cv2 texCoordLU=Cv2((posLU.x()-startPoint.x)/len, 0);
        Cv4 colorLU=Cv4(1,1,1,1);
        
        //LD
        Cv2 posLD=Cv2(p.x,0);
        Cv2 texCoordLD=Cv2((posLD.x()-startPoint.x)/len, 1);
        Cv4 colorLD=Cv4(1,1,1,1);
        
        //RD
        Cv2 posRD=Cv2(pn.x,0);
        Cv2 texCoordRD=Cv2((posRD.x()-startPoint.x)/len, 1);
        Cv4 colorRD=Cv4(1,1,1,1);
        
        //RU
        Cv2 posRU=Cv2(pn.x,pn.y);
        Cv2 texCoordRU=Cv2((posRU.x()-startPoint.x)/len, 0);
        Cv4 colorRU=Cv4(1,1,1,1);
        
        //add vertex
        m_mesh->vlist.push_back(posLU);
        m_mesh->texCoordList.push_back(texCoordLU);
        m_mesh->colorList.push_back(colorLU);
        int IDLU=(int)m_mesh->vlist.size()-1;
        
        m_mesh->vlist.push_back(posLD);
        m_mesh->texCoordList.push_back(texCoordLD);
        m_mesh->colorList.push_back(colorLD);
        int IDLD=(int)m_mesh->vlist.size()-1;
        
        m_mesh->vlist.push_back(posRD);
        m_mesh->texCoordList.push_back(texCoordRD);
        m_mesh->colorList.push_back(colorRD);
        int IDRD=(int)m_mesh->vlist.size()-1;
        
        m_mesh->vlist.push_back(posRU);
        m_mesh->texCoordList.push_back(texCoordRU);
        m_mesh->colorList.push_back(colorRU);
        int IDRU=(int)m_mesh->vlist.size()-1;
        
        //IDtri
        CIDTriangle IDtri0,IDtri1;
        IDtri0.init(IDLU, IDLD, IDRD);
        IDtri1.init(IDLU, IDRD, IDRU);
        m_mesh->IDtriList.push_back(IDtri0);
        m_mesh->IDtriList.push_back(IDtri1);
    }
    
}
void Cripple_horizontalNode::updateMesh(){
   
    m_surfacePointList_reduced.clear();
    for(int i=0;i<(int)m_surfacePointList.size();i+=m_reduceStep){
        CCPoint pos=m_surfacePointList[i];
        m_surfacePointList_reduced.push_back(pos);
    }
    m_surfacePointList_reduced.push_back(m_surfacePointList[(int)m_surfacePointList.size()-1]);
    //got m_surfacePointList_reduced
    
    int nV=(int)m_mesh->vlist.size();
    int nPoint=(int)m_surfacePointList_reduced.size();
    for(int i=0;i<nPoint;i++){
        const CCPoint&p=m_surfacePointList_reduced[i];
        //  0           1            2
        //  *-----*-----*
        //    0-3   4-7
        //    |    |    |    |
        //    1-2   5-6
        int imul4=i*4;
        if(imul4<nV){
            m_mesh->vlist[imul4].sety(p.y);
        }
        
        if(imul4-1>0){
            m_mesh->vlist[imul4-1].sety(p.y);
        }
        
    }
}

void Cripple_horizontalNode::updateRipple(){
    
    CCSize contentSize=this->getContentSize();
    //update surfacePointList's point height
    {
        int nPoint=(int)m_surfacePointList.size();
        for(int i=0;i<nPoint;i++){
            //start and end point not update
        //    if(i==0)continue;
        //    if(i==nPoint-1)continue;
            CCPoint&point=m_surfacePointList[i];
            CCPoint&pointf=m_surfacePointList[i==0?nPoint-1:i-1];
            CCPoint&pointn=m_surfacePointList[i==nPoint-1?0:i+1];
            CCPoint&point_back=m_surfacePointList_back[i];
            point_back.y=contentSize.height+((pointf.y-contentSize.height)
                                             +(pointn.y-contentSize.height)
                                             -(point_back.y-contentSize.height))*(1.0-1.0/60);//150);
        }
        //move to right  [hack]
        {
            int nPoint=(int)m_surfacePointList.size();
            CCPoint lastPoint=m_surfacePointList[nPoint-1];
            CCPoint lastPoint_back=m_surfacePointList_back[nPoint-1];
            for(int i=nPoint-1;i>=1;i--){
                m_surfacePointList[i].y=m_surfacePointList[i-1].y;
                m_surfacePointList_back[i].y=m_surfacePointList_back[i-1].y;
            }
            m_surfacePointList[0].y=lastPoint.y;
            m_surfacePointList_back[0].y=lastPoint_back.y;
        }
        //---
        //switch surfacePointList and surfacePointList_back
        vector<CCPoint> temp=m_surfacePointList;
        m_surfacePointList=m_surfacePointList_back;
        m_surfacePointList_back=temp;
        
    }
   

    updateMesh();
    m_indexVBO->submitPos(m_mesh->vlist, GL_DYNAMIC_DRAW);
}
void Cripple_horizontalNode::update(float dt){
 
    updateRipple();

}

void Cripple_horizontalNode::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags){
    
    //change shader to have_mv
    setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
    
        _customCommand.init(_globalZOrder);
        _customCommand.func = CC_CALLBACK_0(Cripple_horizontalNode::onDraw,this,transform, flags);
        renderer->addCommand(&_customCommand);
    
    if(m_isDrawDebug){
        
        _customCommand_debug.init(_globalZOrder);
        _customCommand_debug.func = CC_CALLBACK_0(Cripple_horizontalNode::onDrawDebug,this,transform, flags);
        renderer->addCommand(&_customCommand_debug);
    }
    
}


void Cripple_horizontalNode::onDraw(const Mat4 &transform, uint32_t flags){

    //----change shader
    ccGLBlendFunc( _blendFunc.src, _blendFunc.dst );
    //pass values for cocos2d-x build-in uniforms
    CGLProgramWithUnifos*program=(CGLProgramWithUnifos*)this->getShaderProgram();
    program->use();
    //when program is have MV, we should use program->setUniformsForBuiltins(transform) instead of program->setUniformsForBuiltins()
    ////program->setUniformsForBuiltins();
    program->setUniformsForBuiltins(transform);
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

void Cripple_horizontalNode::onDrawDebug(const Mat4 &transform, uint32_t flags){
    const Mat4&m=transform;
    //draw segList
    glLineWidth(2);
    ccPointSize(4);
    int nPoint=(int)m_surfacePointList_reduced.size();
    int nSeg=nPoint-1;
    for(int i=0;i<nSeg;i++){
        CCPoint point=m_surfacePointList_reduced[i];
        CCPoint pointn=m_surfacePointList_reduced[i+1];
        ccDrawLine(M4TransV2(m,point), M4TransV2(m,pointn));
        ccDrawPoint(M4TransV2(m,point));
        ccDrawPoint(M4TransV2(m,pointn));
    }
}
namespace_ens_end