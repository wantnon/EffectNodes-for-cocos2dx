

#include "ensLightningBoltNode.h"

namespace_ens_begin
using namespace __lb;
void ClineSeg::init(CCPoint start,CCPoint end){
    startPoint=start;
    endPoint=end;
}
bool ClineSeg::addChildRef(ClineSeg*child){
    //if already have this child, do not add, return false
    int nChild=(int)childList.size();
    for(int i=0;i<nChild;i++){
        if(childList[i]==child){
            return false;
        }
    }
    childList.push_back(child);
    return true;
}
bool ClineSeg::removeChildRef(ClineSeg*child){
    //only remove the first matched child
    int nChild=(int)childList.size();
    for(int i=0;i<nChild;i++){
        if(childList[i]==child){
            childList.erase(childList.begin()+i);
            nChild--;
            i--;
            return true;
        }
    }
    return false;
}
float ClineSeg::getLen()const {
    return ccpDistance(startPoint, endPoint);
}
CCPoint ClineSeg::getDir()const {
    return ccpNormalize(endPoint-startPoint);
}
CCPoint ClineSeg::getVec()const {
    return endPoint-startPoint;
}
CCPoint ClineSeg::getRightNormal()const {//right side normal or outer normal
    CCPoint dir=getDir();
    CCPoint normal(dir.y,-dir.x);
    return normal;
}
CCPoint ClineSeg::getMidPoint()const {
    return ccpMidpoint(startPoint, endPoint);
}


void ClightningBoltSprite::initProgram(){
    //init shader program
    {
        GLchar * vertSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/lightningBolt.vsh").c_str())->getCString();
        GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/lightningBolt.fsh").c_str())->getCString();
        CGLProgramWithUnifos* pProgram = new CGLProgramWithUnifos();
        pProgram->autorelease();
        pProgram->initWithVertexShaderByteArray(vertSource, fragSource);
        //            --below code is no longer needed, because bindPredefinedVertexAttribs() is called in link() in 3.x
        //            --bind attribute
        //            --program->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        //            --program->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        //            --program->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        //link  (must after bindAttribute)
        pProgram->link();
        //get cocos2d-x build-in uniforms
        pProgram->updateUniforms();
        //get my own uniforms
        pProgram->attachUniform("u_opacity");
        //set program
        m_program=pProgram;
        m_program->retain();
        //check gl error
        CHECK_GL_ERROR_DEBUG();
        
    }
}
bool ClightningBoltSprite::init(){
 
    //
    initProgram();
    //
    this->CCSprite::initWithFile("demoRes/lightingSeg.png");
    this->setAnchorPoint(ccp(0,0));
    ccBlendFunc blendFuncSeg={GL_SRC_ALPHA,GL_ONE};
    this->setBlendFunc(blendFuncSeg);
	//antiAlias
	this->getTexture()->setAntiAliasTexParameters();
    //create mesh
    m_mesh=new Cmesh();
    m_mesh->autorelease();
    m_mesh->retain();
    //create indexVBO
    m_indexVBO=new CindexVBO();
    m_indexVBO->autorelease();
    m_indexVBO->retain();
    CHECK_GL_ERROR_DEBUG();

 

    return true;
}
void ClightningBoltSprite::updateNGeneration(){
    float lightingLen=2*ccpDistance(m_startPoint, m_endPoint)*sqrtf(0.25+m_k_offset*m_k_offset);
    int count=0;
    while(lightingLen>m_mainSegLen){
        lightingLen/=2;
        count++;
    }//count is the smallest number let mainSegLen*2^count not less than lightingLen
    m_nGeneration=count;
    //  CCLOG("nGeneration:%i",nGeneration);
}

vector<ClineSeg*> ClightningBoltSprite::genSegesFromBranchSeg(ClineSeg*branchSeg,int nGeneration)
//input branchSeg and get subSeges, branchSeg itself will be deleted
{
    if(nGeneration<=0){
        return vector<ClineSeg*>();
        CCLOG("warning:nGeneration<=0!");
    }
    vector<ClineSeg*> segList;
    //use branchSeg as inital seg
    segList.push_back(branchSeg);//branchSeg will be deleted in the loop below
    //begin generations
    //becuase in the loop we will change (delete) branchSeg, so here we must save branchSeg's branchStartGenerationID
    //in order to the loop length not changed during the loop.
    int branchStartGenerationID=branchSeg->branchStartGenerationID;
    for(int i=branchStartGenerationID+1;i<branchStartGenerationID+1+nGeneration;i++){
        vector<ClineSeg*> tSegList;
        //split each seg in segList
        int nseg=(int)segList.size();
        for(int j=0;j<nseg;j++){
            ClineSeg*seg=segList[j];//cur seg
            vector<ClineSeg*> subseges=splitSeg(seg,i);//split seg, get sub seges
            //add sub seges to segList
            int nSubSeg=(int)subseges.size();
            for(int k=0;k<nSubSeg;k++){
                tSegList.push_back(subseges[k]);
            }
        }
        //clear segList
        for(int j=0;j<nseg;j++){
            delete segList[j];
            segList[j]=NULL;
        }
        //
        segList=tSegList;
    }
    return segList;
}
void ClightningBoltSprite::genLighting(){
    genSeges();
    genMesh();
}
void ClightningBoltSprite::genSeges(){
    //clear segList and secondaryBranchRootList
    int nSeg=(int)m_segList.size();
    for(int i=0;i<nSeg;i++){
        delete m_segList[i];
        m_segList[i]=NULL;
    }
    m_segList.clear();
    //if nGeneration==0, namely lighting is too short
    if(m_nGeneration==0){
        ClineSeg* branchSeg=new ClineSeg(this->m_startPoint,this->m_endPoint);
        branchSeg->branchID=0;
        branchSeg->branchStartGenerationID=0;
        branchSeg->generationID=0;
        branchSeg->branchEndPoint=this->m_endPoint;
        m_segList.push_back(branchSeg);
        m_headSeg=branchSeg;
        m_tailSeg=branchSeg;
        return;
    }
    //create inital branchSeg
    ClineSeg* branchSeg=new ClineSeg(this->m_startPoint,this->m_endPoint);
    branchSeg->branchID=0;
    branchSeg->branchStartGenerationID=0;
    branchSeg->generationID=0;
    branchSeg->branchEndPoint=this->m_endPoint;
    //use inital branchSeg to create segList
    m_segList=genSegesFromBranchSeg(branchSeg, m_nGeneration);
    //find head and tail seg
    m_headSeg=m_segList[0];
    m_tailSeg=m_segList[(int)m_segList.size()-1];
    assert(m_headSeg->branchID==0&&m_headSeg->father==NULL);
    assert(m_tailSeg->branchID==0&&(int)m_tailSeg->childList.size()==0);
    
}

vector<ClineSeg*> ClightningBoltSprite::splitSeg(ClineSeg*seg,int generationID)
{
   // CCLOG("seg->father branchID:%i",seg->branchID);
    
    //  *-------seg-------*
    //  *--seg0--*--seg1--*
    //
    vector<ClineSeg*> seges;
    {
        float maxOffset=seg->getLen()*m_k_offset;
        float offset=randomInRange(-maxOffset,maxOffset);
        CCPoint midPoint=seg->getMidPoint()+ccpMult(seg->getRightNormal(),offset);
        ClineSeg* seg0=new ClineSeg(seg->startPoint,midPoint);
        ClineSeg* seg1=new ClineSeg(midPoint,seg->endPoint);
        seg0->branchID=seg->branchID;//sub seg's branchID inherited from seg
        seg0->branchStartGenerationID=seg->branchStartGenerationID;
        seg0->branchEndPoint=seg->branchEndPoint;
        seg0->generationID=seg->generationID+1;
        seg1->branchID=seg->branchID;
        seg1->branchStartGenerationID=seg->branchStartGenerationID;
        seg1->branchEndPoint=seg->branchEndPoint;
        seg1->generationID=seg->generationID+1;
        //update links of seg0, seg1
        seg0->father=seg->father;
        seg0->addChildRef(seg1);
        seg1->father=seg0;
        seg1->childList=seg->childList;
        //update links of seg's neighbour
        if(seg->father){
            seg->father->addChildRef(seg0);
            seg->father->removeChildRef(seg);
        }
        for(int i=0;i<(int)seg->childList.size();i++){
            ClineSeg*child=seg->childList[i];
            child->father=seg1;
        }
        //add seg0,seg2(if has),seg1 to seges, notice the order, if we keep this order, we can guarantee segList[0] is headSeg and segList[(int)segList.size()-1] is tailSeg.
        //note, jitter will break this law
        //add seg0 to seges
        seges.push_back(seg0);
        //calculate branch
        float p=nPow(0.6,generationID-1);//0.55//0.6
        if(catchProbability(p))
        {
            float randomSmallAngle=randomInRange(-3,3);//-3,3
            float lengthScale=0.6*nPow(1.2, seg->branchID);//0.7//0.6
            CCPoint splitEnd=ccpMult(rotateVector2(seg0->getVec(),randomSmallAngle), lengthScale)+midPoint;
            ClineSeg*seg2=new ClineSeg(midPoint,splitEnd);
            seg2->branchID=seg->branchID+1;//branch's branchID is seg's branchID+1
            seg2->branchStartGenerationID=generationID;
            seg2->branchEndPoint=seg2->endPoint;
            seg2->generationID=seg->generationID+1;
            //update links of seg2
            seg2->father=seg0;
            seg0->addChildRef(seg2);
            //add seg2 to seges
            seges.push_back(seg2);
        }
        //add seg1 to seges
        seges.push_back(seg1);
    }
    return seges;
}
void ClightningBoltSprite::doJitter(){
    
    //generate branchList
    vector<ClineSeg*> branchList;
    {
        ClineSeg*seg=m_headSeg;
        while(1){
            //seg must be mainBranch seg
            //see if seg's children have secondary branch
            ClineSeg*nextSeg=NULL;
            int nChild=(int)seg->childList.size();
            if(nChild>1){//have secondary branch
                for(int i=0;i<nChild;i++){
                    ClineSeg*_seg=seg->childList[i];
                    if(_seg->branchID==1){//_seg is secondary branch root seg
                        //make branchSeg based on _seg
                        branchList.push_back(_seg);
                    }else if(_seg->branchID==0){
                        nextSeg=_seg;
                    }
                }
            }else if(nChild==1){
                nextSeg=seg->childList[0];
            }
            //if have next seg then go on, else jump out
            if(nextSeg){
                seg=nextSeg;
            }else{
                break;
            }
        }//got secondaryBranchList
 
    }
    
    //mark branch to be regenerated
    {
        int nbranch=(int)branchList.size();
        for(int i=0;i<nbranch;i++){
            ClineSeg*branchSeg=branchList[i];
            if(catchProbability(m_percentageOfJitterBranch)){
                branchSeg->needReGenerate=true;
            }else{
                branchSeg->needReGenerate=false;
            }
        }

     }

    //generate seeds
    //if a branch need not regenerate, it's seed is NULL
    vector<ClineSeg*> seedList((int)branchList.size());
    {
        int nSeed=(int)seedList.size();
        for(int i=0;i<nSeed;i++){
            if(branchList[i]->needReGenerate){
                ClineSeg*seed=new ClineSeg();
                *seed=*branchList[i];
                seed->endPoint=seed->branchEndPoint;
                seed->generationID=branchList[i]->branchStartGenerationID;
                seed->childList.clear();
                seedList[i]=seed;
            }else{
                seedList[i]=NULL;
            }
            
        }
 
    }
    
    //clear branches need to regenerate
    releaseNeedReGenBranch(branchList,m_segList);
    
    
    //generate seges for each seg in secondaryBranchList
    int nSeed=(int)seedList.size();
    for(int i=0;i<nSeed;i++){
        ClineSeg *seed=seedList[i];
        if(seed==NULL)continue;
        seed->endPoint=seed->startPoint+ccpMult(rotateVector2(seed->getVec(),randomInRange(-3, 3)),0.7);
        //generate seges for branchSeg
        int nGeneration_seed=m_nGeneration-seed->branchStartGenerationID;//how many generations this seed will grow
        if(nGeneration_seed<=0)continue;//because nGeneration may changed, so nGeneration_seed may <= 0
        vector<ClineSeg*> tSegList=genSegesFromBranchSeg(seed, nGeneration_seed);
        assert(tSegList[0]->father);
        assert((int)tSegList[(int)tSegList.size()-1]->childList.size()==0);
        //add tSegList to segList
        int ntSeg=(int)tSegList.size();
        for(int j=0;j<ntSeg;j++){
            m_segList.push_back(tSegList[j]);
        }
        
    }
    genMesh();
}
void ClightningBoltSprite::releaseNeedReGenBranch(vector<ClineSeg*>&branchList,vector<ClineSeg*>&segList){
    int nbranch=(int)branchList.size();
    for(int i=0;i<nbranch;i++){
        ClineSeg*branchSeg=branchList[i];
        if(branchSeg->needReGenerate){
            //remove its reference from its father's childList
            branchSeg->father->removeChildRef(branchSeg);
            //release this whole branch
            releaseNeedReGenBranch_inn(branchSeg,segList);
        }
    }
    //remove all NULL elements in segList
    int nseg=(int)segList.size();
    for(int i=0;i<nseg;i++){
        if(segList[i]->needReGenerate){
            delete segList[i];
            segList[i]=NULL;
        }
    }
    
    vector<ClineSeg*> tSegList;
    for(int i=0;i<nseg;i++){
        if(segList[i]!=NULL){
            tSegList.push_back(segList[i]);
        }
    }
    segList=tSegList;
}
void ClightningBoltSprite::releaseNeedReGenBranch_inn(ClineSeg*seg,vector<ClineSeg*>&segList){
    if(seg==NULL)return;
    seg->needReGenerate=true;
    //same process for children
    int nChild=(int)seg->childList.size();
    for(int i=0;i<nChild;i++){
        ClineSeg*_seg=seg->childList[i];
        releaseNeedReGenBranch_inn(_seg,segList);
    }
}

void ClightningBoltSprite::genMesh(){

    //make mesh
    {
        m_mesh->vlist.clear();
        m_mesh->IDtriList.clear();
        m_mesh->texCoordList.clear();
        m_mesh->colorList.clear();
        int nSeg=(int)m_segList.size();
        for(int i=0;i<nSeg;i++){
            ClineSeg*seg=m_segList[i];
            //calculate quad for seg
            //      v0        v3
            //       * -------*
            //       |segW/2  |
            //   end <--------- start
            //       |        |
            //       *--------*
            //      v1        v2
            const CCPoint&start=seg->startPoint;
            const CCPoint&end=seg->endPoint;
            CCPoint rightNorm=seg->getRightNormal();
            float segW=12*nPow(0.3,seg->branchID);
            float opacity=1.0*nPow(0.4, seg->branchID);
            CCPoint p0=end+ccpMult(rightNorm, segW/2);
            CCPoint p1=end+ccpMult(rightNorm, -segW/2);
            CCPoint p2=start+ccpMult(rightNorm, -segW/2);
            CCPoint p3=start+ccpMult(rightNorm, segW/2);
            {
                CCPoint mid=seg->getMidPoint();
                p0=mid+ccpMult(p0-mid, 8);
                p1=mid+ccpMult(p1-mid, 8);
                p2=mid+ccpMult(p2-mid, 8);
                p3=mid+ccpMult(p3-mid, 8);
                
            }
            CCPoint texCoord0=CCPoint(0,0);
            CCPoint texCoord1=CCPoint(0,1);
            CCPoint texCoord2=CCPoint(1,1);
            CCPoint texCoord3=CCPoint(1,0);
            m_mesh->vlist.push_back(Cv2(p0.x,p0.y));int vID0=(int)m_mesh->vlist.size()-1;
            m_mesh->vlist.push_back(Cv2(p1.x,p1.y));int vID1=(int)m_mesh->vlist.size()-1;
            m_mesh->vlist.push_back(Cv2(p2.x,p2.y));int vID2=(int)m_mesh->vlist.size()-1;
            m_mesh->vlist.push_back(Cv2(p3.x,p3.y));int vID3=(int)m_mesh->vlist.size()-1;
            m_mesh->texCoordList.push_back(Cv2(texCoord0.x,texCoord0.y));
            m_mesh->texCoordList.push_back(Cv2(texCoord1.x,texCoord1.y));
            m_mesh->texCoordList.push_back(Cv2(texCoord2.x,texCoord2.y));
            m_mesh->texCoordList.push_back(Cv2(texCoord3.x,texCoord3.y));
            
            m_mesh->colorList.push_back(Cv4(1,1,1,opacity));
            m_mesh->colorList.push_back(Cv4(1,1,1,opacity));
            m_mesh->colorList.push_back(Cv4(1,1,1,opacity));
            m_mesh->colorList.push_back(Cv4(1,1,1,opacity));
            
            CIDTriangle IDtri0(vID0,vID1,vID2);
            CIDTriangle IDtri1(vID0,vID2,vID3);
            m_mesh->IDtriList.push_back(IDtri0);
            m_mesh->IDtriList.push_back(IDtri1);

        }
        
    }
    //submit mesh to indexVBO
    {
        //submit data to VBO
        m_indexVBO->submitPos(m_mesh->vlist,GL_STREAM_DRAW);//GL_DYNAMIC_DRAW
        m_indexVBO->submitTexCoord(m_mesh->texCoordList,GL_STREAM_DRAW);
        m_indexVBO->submitColor(m_mesh->colorList, GL_STATIC_DRAW);
        m_indexVBO->submitIndex(m_mesh->IDtriList,GL_STREAM_DRAW);
        //check gl error
        CHECK_GL_ERROR_DEBUG();
    }

    
  

    
    
}

void ClightningBoltSprite::setStartAndEnd(CCPoint start,CCPoint end){
    m_startPoint_parentSpace=start;
    m_endPoint_parentSpace=end;
    this->getNodeToParentTransform();//in order this->parentToNodeTransform() got right result, we must call this->getNodeToParentTransform() first, this is a bug of cocos2dx 3.3,see:http://www.cnblogs.com/wantnon/p/4330226.html
    m_startPoint= CCPointApplyAffineTransform(m_startPoint_parentSpace,this->parentToNodeTransform());
    m_endPoint=CCPointApplyAffineTransform(m_endPoint_parentSpace,this->parentToNodeTransform());
    //startPoint and endPoint is changed
    
    //relocate all the seges in segList
    if(m_headSeg&&m_tailSeg)
    {
        //move all seges let headStart equals to startPoint
        CCPoint vec_headStartToStart=m_startPoint-m_headSeg->startPoint;
        int nseg=(int)m_segList.size();
        for(int i=0;i<nseg;i++){
            ClineSeg*seg=m_segList[i];
            seg->startPoint=seg->startPoint+vec_headStartToStart;
            seg->endPoint=seg->endPoint+vec_headStartToStart;
            if(seg->branchID==1&&seg->father->branchID==0){//secondary branch root seg
                seg->branchEndPoint=seg->branchEndPoint+vec_headStartToStart;
            }
        }
        
        
        //rotate all seges in segList
        CCPoint vec_headStartToTailEnd=m_tailSeg->endPoint-m_headSeg->startPoint;
        CCPoint vec_headStartToEnd=this->m_endPoint-m_headSeg->startPoint;
        CCPoint cosA_sinA=calculateCosASinAOfVec1ToVec2(vec_headStartToTailEnd, vec_headStartToEnd);
        float cosA=cosA_sinA.x;
        float sinA=cosA_sinA.y;
        for(int i=0;i<nseg;i++){
            ClineSeg*seg=m_segList[i];
            seg->startPoint=m_headSeg->startPoint+rotateVector2(seg->startPoint-m_headSeg->startPoint, cosA,sinA);
            seg->endPoint=m_headSeg->startPoint+rotateVector2(seg->endPoint-m_headSeg->startPoint, cosA,sinA);
            if(seg->branchID==1&&seg->father->branchID==0){//secondary branch root seg
                seg->branchEndPoint=m_headSeg->startPoint+rotateVector2(seg->branchEndPoint-m_headSeg->startPoint, cosA, sinA);
            }
        }
        //scale segList, use headSeg->startPoint as the center
        float dis_headStartToEnd=ccpDistance(m_headSeg->startPoint, this->m_endPoint);
        float dis_headStartToTailEnd=ccpDistance(m_headSeg->startPoint, m_tailSeg->endPoint);
        float scaleFactor=dis_headStartToTailEnd==0?1:dis_headStartToEnd/dis_headStartToTailEnd;
        for(int i=0;i<nseg;i++){
            ClineSeg*seg=m_segList[i];
            seg->startPoint=m_headSeg->startPoint+ccpMult(seg->startPoint-m_headSeg->startPoint, scaleFactor);
            seg->endPoint=m_headSeg->startPoint+ccpMult(seg->endPoint-m_headSeg->startPoint, scaleFactor);
            if(seg->branchID==1&&seg->father->branchID==0){//secondary branch root seg
                seg->branchEndPoint=m_headSeg->startPoint+ccpMult(seg->branchEndPoint-m_headSeg->startPoint, scaleFactor);
            }
        }
        //because segList changed, so must re-generate mesh and glow sprites
        genMesh();
    }
    //update nGeneration
    updateNGeneration();
    
}

void ClightningBoltSprite::debugDraw_segWireFrame(float lineWidth){
    glLineWidth(lineWidth);
    //draw mesh wireframe
    int nIDtri=(int)m_mesh->IDtriList.size();
    for(int i=0;i<nIDtri;i++){
        CIDTriangle&IDtri=m_mesh->IDtriList[i];
        Cv2&p0=m_mesh->vlist[IDtri.getvIDByIndex(0)];
        Cv2&p1=m_mesh->vlist[IDtri.getvIDByIndex(1)];
        Cv2&p2=m_mesh->vlist[IDtri.getvIDByIndex(2)];
        CCPoint vertices[3]={ccp(p0.x(),p0.y()),ccp(p1.x(),p1.y()),ccp(p2.x(),p2.y())};
        ccDrawPoly(vertices, 3, true);
    }

}
void ClightningBoltSprite::debugDraw_segLineAndDot(float lineWidth,float pointSize){
     //draw segList
     glLineWidth(lineWidth);
     ccPointSize(pointSize);
     int nSeg=(int)m_segList.size();
     for(int i=0;i<nSeg;i++){
         ClineSeg*seg=m_segList[i];
         ccDrawLine(seg->startPoint, seg->endPoint);
         ccDrawPoint(seg->startPoint);
         ccDrawPoint(seg->endPoint);
     }
}
void ClightningBoltSprite::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags){
    
    
   
        _customCommand.init(_globalZOrder);
        _customCommand.func = CC_CALLBACK_0(ClightningBoltSprite::onDraw,this,transform, flags);
        renderer->addCommand(&_customCommand);
    
    if(m_isDrawDebug){
        
        _customCommand_debug.init(_globalZOrder);
        _customCommand_debug.func = CC_CALLBACK_0(ClightningBoltSprite::onDrawDebug,this,transform, flags);
        renderer->addCommand(&_customCommand_debug);
    }
    
}
void ClightningBoltSprite::onDraw(const Mat4 &transform, uint32_t flags){
 
    //draw mesh
    ccGLBlendFunc( _blendFunc.src, _blendFunc.dst );
    //----change shader
    assert(m_program);
    this->setShaderProgram(m_program);
    CGLProgramWithUnifos*program=(CGLProgramWithUnifos*)this->getShaderProgram();
    //pass values for cocos2d-x build-in uniforms
    program->use();
    program->setUniformsForBuiltins();
    //pass values for my own uniforms
    float alpha=(float)this->getOpacity()/255.0*m_opacityZoom;
    program->passUnifoValue1f("u_opacity", alpha);
    //----enable attributes
    bool isAttribPositionOn=CindexVBO::isEnabledAttribArray_position();
    bool isAttribColorOn=CindexVBO::isEnabledAttribArray_color();
    bool isAttribTexCoordOn=CindexVBO::isEnabledAttribArray_texCoord();
    CindexVBO::enableAttribArray_position(true);
    CindexVBO::enableAttribArray_texCoord(true);
    CindexVBO::enableAttribArray_color(true);
    //----bindTexture
    ccGLBindTexture2D( this->getTexture()->getName());
    //----draw
    m_indexVBO->setPointer_position();
    m_indexVBO->setPointer_texCoord();
    m_indexVBO->setPointer_color();
    m_indexVBO->draw(GL_TRIANGLES);
    //----disable attributes
    CindexVBO::enableAttribArray_position(isAttribPositionOn);
    CindexVBO::enableAttribArray_color(isAttribColorOn);
    CindexVBO::enableAttribArray_texCoord(isAttribTexCoordOn);
    //----unbindTexture
    ccGLBindTexture2D(0);
   

    CHECK_GL_ERROR_DEBUG();
}
void ClightningBoltSprite::onDrawDebug(const Mat4 &transform, uint32_t flags){
    debugDraw_segLineAndDot(0.5,3);
    //    debugDraw_segWireFrame(1);
}
//---------------------------------------------
bool ClightningBoltNode::init(const CCPoint&start,const CCPoint&end){
    m_start=start;
    m_end=end;
    //lightingSprit
    {
        m_lbSprite=new ClightningBoltSprite();
        m_lbSprite->autorelease();
        m_lbSprite->init();
        m_lbSprite->setStartAndEnd(m_start, m_end);
        m_lbSprite->genLighting();
        this->addChild(m_lbSprite);
    }
    //lightingSprit2
    {
        m_lbSprite2=new ClightningBoltSprite();
        m_lbSprite2->autorelease();
        m_lbSprite2->init();
        m_lbSprite2->setStartAndEnd(m_start, m_end);
        m_lbSprite2->genLighting();
        this->addChild(m_lbSprite2);
        
    }
    return true;
}
void ClightningBoltNode::runFlashAction(){
    
    const GLubyte startOpacity=255;
    const GLubyte endOpacity=80;
    this->getLBSprite1()->setOpacity(startOpacity);
    this->getLBSprite2()->setOpacity(endOpacity);
    //run action
    const float oneFlashTime=0.7;
    CCDelayTime*delay=CCDelayTime::create(oneFlashTime/2);
    CallFunc*flashForever = CallFuncN::create([=](Node*target){
        const float jitterTimeOfOneFlash=oneFlashTime/4;
        const float jitterInterval=oneFlashTime/6;
        const bool isDoJitter=true;
        //----fadeTo action
        CCFadeTo*fadeTo=CCFadeTo::create(oneFlashTime, endOpacity) ;
        
        CallFunc* genLightning = CallFuncN::create([=](Node*target){
            ((ens::ClightningBoltSprite*)target)->genLighting();
        });
        CallFunc* jitter = CallFuncN::create([=](Node*target){
            ((ens::ClightningBoltSprite*)target)->doJitter();
        });
        CallFunc* resetOpacity = CallFuncN::create([=](Node*target){
            ((ens::ClightningBoltSprite*)target)->setOpacity(startOpacity);
        });
        
        //----jitter action sequence
        CCDelayTime*delay_jitterInterval=CCDelayTime::create(jitterInterval);
        Vector<cocos2d::FiniteTimeAction *> vector;
        float totalTime=0;
        while (true) {
            if(totalTime>=jitterTimeOfOneFlash)break;
            vector.pushBack(delay_jitterInterval);
            if(isDoJitter)vector.pushBack(jitter);
            totalTime+=jitterInterval;
        }
        CCSequence*seq=CCSequence::create(vector);
        //----make whole action
        CCAction*action=CCRepeatForever::create(CCSpawn::create(seq,CCSequence::create(resetOpacity,fadeTo,genLightning,NULL),NULL));
        //----runAction
        ((ens::ClightningBoltSprite*)target)->runAction(action);
    });
    
    CCAction*action1=CCSequence::create(dynamic_cast<CallFunc*>(flashForever->clone()),NULL);
    CCAction*action2=CCSequence::create(delay,dynamic_cast<CallFunc*>(flashForever->clone()),NULL);
    this->getLBSprite1()->runAction(action1);
    this->getLBSprite2()->runAction(action2);
}

//-------------------------------------------------------



namespace_ens_end