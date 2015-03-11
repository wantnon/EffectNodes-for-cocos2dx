

#include "ensRippleNode.h"

namespace_ens_begin
using namespace ripple;
bool CrippleSprite::init(string texFileName,float gridSideLen){
    m_gridSideLen=gridSideLen;
    //----init this sprite
    this->CCSprite::initWithFile(texFileName.c_str());
    //----calculate nRow and nCol
    CCSize contentSize=this->getContentSize();
    m_nRow=floorf(contentSize.height/gridSideLen)+1;//add one more row to ensure buffer bigger than background pic
    m_nCol=floorf(contentSize.width/gridSideLen)+1;//add one more col to ensure buffer bigger than background pic
   // CCLOG("m_nRow,m_nCol:%i,%i",m_nRow,m_nCol);
    //----generate mesh
    //create mesh
    m_mesh=new Cmesh();
    m_mesh->autorelease();
    m_mesh->retain();
    //fill vertex attributes
    //nVertex should be (nRow+1)*(nCol+1)
    m_mesh->vlist.reserve((m_nRow+1)*(m_nCol+1));//in order to faster push_back, do reserve
    m_mesh->texCoordList.reserve((m_nRow+1)*(m_nCol+1));
    m_mesh->colorList.reserve((m_nRow+1)*(m_nCol+1));
    for(int i=0;i<m_nRow+1;i++){
        for(int j=0;j<m_nCol+1;j++){
            float x=m_gridSideLen*j;
            float y=contentSize.height-m_gridSideLen*i;
            float s=x/contentSize.width;
            float t=1-y/contentSize.height;
            Cv2 pos=Cv2(x,y);
            Cv2 texCoord=Cv2(s,t);
            Cv4 color=Cv4(1,1,1,1);
            m_mesh->vlist.push_back(pos);
            m_mesh->texCoordList.push_back(texCoord);
            m_mesh->colorList.push_back(color);
        }
    }
    //fill indexs
    const int nVertexPerRow=m_nCol+1;
    for(int i=0;i<m_nRow;i++){
        for(int j=0;j<m_nCol;j++){
            //current grid is grid(i,j)
            //grid(i,j)'s leftup vertex is vertex(i,j)
            //vertex(i,j) is vertex(i*nVertexPerRow+j)
            int vID_LU=i*nVertexPerRow+j;
            int vID_RU=vID_LU+1;
            int vID_LD=vID_LU+nVertexPerRow;
            int vID_RD=vID_LD+1;
            CIDTriangle IDtri1=CIDTriangle(vID_LU, vID_LD, vID_RD);
            CIDTriangle IDtri2=CIDTriangle(vID_LU, vID_RD, vID_RU);
            //current grid = IDtri1+IDtri2
            m_mesh->IDtriList.push_back(IDtri1);
            m_mesh->IDtriList.push_back(IDtri2);
            
        }
    }
    //----m_texCoordList_store
    m_texCoordList_store=m_mesh->texCoordList;
    //----create indexVBO
    m_indexVBO=new CindexVBO();
    m_indexVBO->autorelease();
    m_indexVBO->retain();
    //----submit mesh
    m_indexVBO->submitPos(m_mesh->vlist, GL_STATIC_DRAW);
    m_indexVBO->submitTexCoord(m_mesh->texCoordList, GL_STATIC_DRAW);
    m_indexVBO->submitColor(m_mesh->colorList, GL_STATIC_DRAW);
    m_indexVBO->submitIndex(m_mesh->IDtriList, GL_STATIC_DRAW);
    
    //----generate srcBuffer and dstBuffer
    {
        //each element of srcBuffer or dstBuffer correspond to a vertex, present the vertex's height
        //so srcBuffer and dstBuffer are all (nRow+1)*(nCol+1) size
        //----srcBuffer
        m_srcBuffer=new CrippleBuffer();
        m_srcBuffer->autorelease();
        m_srcBuffer->retain();
        m_srcBuffer->mat.resize(m_nRow+1);
        for(int i=0;i<(int)m_srcBuffer->mat.size();i++)m_srcBuffer->mat[i].resize(m_nCol+1);
        //----dstBuffer
        m_dstBuffer=new CrippleBuffer();
        m_dstBuffer->autorelease();
        m_dstBuffer->retain();
        *m_dstBuffer=*m_srcBuffer;//m_dstBuffer copy m_srcBuffer
    }
    //change shader to have_mv
    setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
    

    return true;
}
void CrippleSprite::update(float dt){
    m_time+=dt;
    if(m_time>=1.0/45){
        updateOnce();
        m_time=0;
    }
}
void CrippleSprite::updateOnce(){
    //update buffer and mesh
    float k=0.5-0.5/m_rippleStrength;
    float kTexCoord=1.0/1048;
    const int nRow=m_nRow+1;
    const int nCol=m_nCol+1;
    for(int i=1;i<nRow-1;i++){
        for(int j=1;j<nCol-1;j++){
            //update m_dstBuffer
            float Hup_src=m_srcBuffer->mat[i-1][j];
            float Hdn_src=m_srcBuffer->mat[i+1][j];
            float Hleft_src=m_srcBuffer->mat[i][j-1];
            float Hright_src=m_srcBuffer->mat[i][j+1];
            float Hcenter_dst=m_dstBuffer->mat[i][j];
            float H=(Hup_src+Hdn_src+Hleft_src+Hright_src-2*Hcenter_dst)*k;
            m_dstBuffer->mat[i][j]=H;
            //update texCoord
            float s_offset=(Hup_src-Hdn_src)*kTexCoord;
            float t_offset=(Hleft_src-Hright_src)*kTexCoord;
            Cv2&texCoord=m_mesh->texCoordList[i*nCol+j];
            Cv2&texCoord_store=m_texCoordList_store[i*nCol+j];
            texCoord.setx(texCoord_store.x()+s_offset);
            texCoord.sety(texCoord_store.y()+t_offset);
        }
    }
    CrippleBuffer*temp=m_dstBuffer;
    m_dstBuffer=m_srcBuffer;
    m_srcBuffer=temp;
    //resubmit texCoord
    m_indexVBO->submitTexCoord(m_mesh->texCoordList, GL_DYNAMIC_DRAW);

}
void CrippleSprite::doTouch(const CCPoint&touchPoint,float depth,float r){
    //touchPoint is in parent space, convert it to local space
    this->getNodeToParentTransform();//in order this->parentToNodeTransform() got right result, we must call this->getNodeToParentTransform() first, this is a bug of cocos2dx 3.3,see:http://www.cnblogs.com/wantnon/p/4330226.html
    CCAffineTransform parentToNodeTransform=this->parentToNodeTransform();
    CCPoint touchPoint_localSpace=CCPointApplyAffineTransform(touchPoint,parentToNodeTransform);
    //convert touchPoint_localSpace to OLU (origin at left up corner) space
    CCSize contentSize=this->getContentSize();
    float x_OLU=touchPoint_localSpace.x;//origin at left up corner
    float y_OLU=contentSize.height-touchPoint_localSpace.y;//origin at left up corner
 //   CCLOG("x_OLU,y_OLU:%f,%f",x_OLU,y_OLU);
    //position range in OLU space
    float xmin=x_OLU-r;
    float xmax=x_OLU+r;
    float ymin=y_OLU-r;
    float ymax=y_OLU+r;
    //calculate index range from position range
    //note: min is floor(x), max is ceil ceil(x)-1
    int imin,imax,jmin,jmax;
    int nRow=m_nRow+1;
    int nCol=m_nCol+1;
    const int imargin=1;//do not let the vertex on the edge to be pressed
    const int jmargin=1;//do not let the vertex on the edge to be pressed
    imin=MAX(imargin,floorf(ymin/m_gridSideLen));
    imax=MIN(nRow-1-imargin,ceilf(ymax/m_gridSideLen)-1);
    jmin=MAX(jmargin,floorf(xmin/m_gridSideLen));
    jmax=MIN(nCol-1-jmargin,ceilf(xmax/m_gridSideLen)-1);
  //  cout<<"range:"<<imin<<" "<<imax<<" "<<jmin<<" "<<jmax<<endl;
    //iterate all vertex in range [imin,imax]x[jmin,jmax], and press them
    for(int i=imin;i<=imax;i++){
        for(int j=jmin;j<=jmax;j++){
            const Cv2&v=m_mesh->vlist[i*nCol+j];
            const Cv2&v_OLU=Cv2(v.x(),contentSize.height-v.y());
            //press m_srcBuffer at point v
            float dis=ccpDistance(CCPoint(x_OLU,y_OLU), CCPoint(v_OLU.x(),v_OLU.y()));
            if(dis<r){
                float curDepth=depth*(0.5+0.5*cosf(dis*M_PI/r));
                m_srcBuffer->mat[i][j]-=curDepth;
            }else{
                //do nothing;
            }
        }
    }
 

}
void CrippleSprite::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags){
    
    

        _customCommand.init(_globalZOrder);
        _customCommand.func = CC_CALLBACK_0(CrippleSprite::onDraw,this,transform, flags);
        renderer->addCommand(&_customCommand);
    
    if(m_isDrawDebug){
        
        _customCommand_debug.init(_globalZOrder);
        _customCommand_debug.func = CC_CALLBACK_0(CrippleSprite::onDrawDebug,this,transform, flags);
        renderer->addCommand(&_customCommand_debug);
    }
    
}
void CrippleSprite::onDraw(const Mat4 &transform, uint32_t flags){

  //use scissor test is more fast than clippingNode for large rect.
    //enable scissor test
    glEnable(GL_SCISSOR_TEST);
    //scissor test
    //calculate world rect
    CCSize contentSize=this->getContentSize();
    CCRect rect_local=CCRect(0,0,contentSize.width,contentSize.height);
	CCRect rect_world=CCRectApplyAffineTransform(rect_local, this->nodeToWorldTransform());
	//do not use glScissor, instead use cocos2dx api CCEGLView::setScissorInPoints, or the result will be wrong.
	//note: setScissorInPoints receive world space rect as parameter
	Director::getInstance()->getOpenGLView()->setScissorInPoints(rect_world.getMinX(), rect_world.getMinY(), rect_world.size.width, rect_world.size.height);
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
    //bindTexture
    ccGLBindTexture2D( this->getTexture()->getName());
    m_indexVBO->setPointer_position();
    m_indexVBO->setPointer_texCoord();
    m_indexVBO->setPointer_color();
    m_indexVBO->draw(GL_TRIANGLES);
    ccGLBindTexture2D(0);
    //disable attributes
    CindexVBO::enableAttribArray_position(isAttribPositionOn);
    CindexVBO::enableAttribArray_color(isAttribColorOn);
    CindexVBO::enableAttribArray_texCoord(isAttribTexCoordOn);
    //disable scissor test
    glDisable(GL_SCISSOR_TEST);

}
void CrippleSprite::onDrawDebug(const Mat4 &transform, uint32_t flags){
    const Mat4&m=transform;
    glLineWidth(1);
    int nIDtri=(int)m_mesh->IDtriList.size();
    for(int i=0;i<nIDtri;i++){
        const CIDTriangle&IDtri=m_mesh->IDtriList[i];
        Cv2 v0=m_mesh->vlist[IDtri.getvIDByIndex(0)];
        Cv2 v1=m_mesh->vlist[IDtri.getvIDByIndex(1)];
        Cv2 v2=m_mesh->vlist[IDtri.getvIDByIndex(2)];
        CCPoint vertices[]={M4TransV2(m,CCPoint(v0.x(),v0.y())),M4TransV2(m,CCPoint(v1.x(),v1.y())),M4TransV2(m,CCPoint(v2.x(),v2.y()))};
        ccDrawPoly(vertices, 3, true);
        
    }
}

namespace_ens_end
