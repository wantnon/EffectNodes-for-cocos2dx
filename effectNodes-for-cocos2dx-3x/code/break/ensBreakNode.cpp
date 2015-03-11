
#include "ensBreakNode.h"

namespace_ens_begin
using namespace breakEffect;
bool CbreakSprite::init(const string&texFileName)
{

    //init this CCSprite
    this->CCSprite::initWithFile(texFileName.c_str());
    //create mesh
    m_mesh=new Cmesh();
    m_mesh->autorelease();
    m_mesh->retain();
    //create meshWire
    m_meshWire=new CmeshWire();
    m_meshWire->autorelease();
    m_meshWire->retain();
    //create indexVBO
    m_indexVBO=new CindexVBO();
    m_indexVBO->autorelease();
    m_indexVBO->retain();
    //create indexVBO_wire
    m_indexVBO_wire=new CindexVBO();
    m_indexVBO_wire->autorelease();
    m_indexVBO_wire->retain();
    //----m_program
    {
		GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/break_mediumpFloat_outRangeTransp.fsh").c_str())->getCString();
        GLchar * vertSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/break_mediumpFloat_outRangeTransp.vsh").c_str())->getCString();
		CGLProgramWithUnifos* program = new CGLProgramWithUnifos();
        program->autorelease();
		program->initWithVertexShaderByteArray(vertSource, fragSource);
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
        //...
        //set program
        m_program=program;
        m_program->retain();
        //check gl error
		CHECK_GL_ERROR_DEBUG();
	}
	return true ;
}
void CbreakSprite::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags){
    
 
    if(m_state==eState_well){
        //change shader to no_mv
        setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
        this->CCSprite::draw(renderer, transform, flags);
    }else{//m_state!=eState_well
        _customCommand.init(_globalZOrder);
        _customCommand.func = CC_CALLBACK_0(CbreakSprite::onDraw,this,transform, flags);
        renderer->addCommand(&_customCommand);
    }
    if(m_isDrawDebug){
        
        _customCommand_debug.init(_globalZOrder);
        _customCommand_debug.func = CC_CALLBACK_0(CbreakSprite::onDrawDebug,this,transform, flags);
        renderer->addCommand(&_customCommand_debug);
    }
    
}
void CbreakSprite::onDraw(const Mat4 &transform, uint32_t flags)
{
        
    //----change shader
    ccGLBlendFunc( _blendFunc.src, _blendFunc.dst );
    //pass values for cocos2d-x build-in uniforms
    this->setShaderProgram(m_program);
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
    //draw m_indexVBO_wire
    glLineWidth(1);
    m_indexVBO_wire->setPointer_position();
    m_indexVBO_wire->setPointer_texCoord();
    m_indexVBO_wire->setPointer_color();
    m_indexVBO_wire->draw(GL_LINES);
    //unbind texture
    ccGLBindTexture2D(0);
    //disable attributes
    CindexVBO::enableAttribArray_position(isAttribPositionOn);
    CindexVBO::enableAttribArray_color(isAttribColorOn);
    CindexVBO::enableAttribArray_texCoord(isAttribTexCoordOn);
    
}
void CbreakSprite::onDrawDebug(const Mat4 &transform, uint32_t flags){
    const Mat4&m=transform;
    int nTri=(int)m_triList.size();
    for(int i=0;i<nTri;i++){
        const Ctri&tri=m_triList[i];
        CCPoint vertices[3]={M4TransV2(m,CCPoint(tri.m_pos[0].x(),tri.m_pos[0].y())),
            M4TransV2(m,CCPoint(tri.m_pos[1].x(),tri.m_pos[1].y())),
            M4TransV2(m,CCPoint(tri.m_pos[2].x(),tri.m_pos[2].y()))};
        ccDrawPoly(vertices, 3, true);
    }
}
void CbreakSprite::reSet(){
    m_state=breakEffect::eState_well;
    this->stopAllActions();
    this->clear();
}
void CbreakSprite::clear(){
    m_nRow=m_nCol=0;
    m_nRowPoint=m_nColPoint=0;
    m_pointList.clear();
    m_IDquadList.clear();
    m_triList.clear();
    m_mesh->clear();
    m_meshWire->clear();
}
int CbreakSprite::getPointInRangeEqualsToTargetPoint(const CCPoint&targetPoint,const vector<CCPoint>&pointList,int imin,int imax,float eps)
//return the point ID, if not found, return -1
{
    int nPoint=(int)pointList.size();
    if(imin>=nPoint){
        return -1;
    }
    assert(imin>=0&&imin<nPoint);
    assert(imax>=0&&imax<nPoint);
    for(int i=imin;i<=imax;i++){
        const CCPoint&point=pointList[i];
        if(isPointEqual(point, targetPoint,eps)){
            return i;
        }
    }
    return -1;
}
void CbreakSprite::doCrack(const CCPoint&touchPoint){
    m_state=eState_crack;
    //clear
    this->clear();
    //get contentSize
    CCSize contentSize=this->getContentSize();
    //touchPoint is in parent space, convert it to local space
    this->getNodeToParentTransform();//in order this->parentToNodeTransform() got right result, we must call this->getNodeToParentTransform() first, this is a bug of cocos2dx 3.3,see:http://www.cnblogs.com/wantnon/p/4330226.html
    CCAffineTransform parentToNodeTransform=this->parentToNodeTransform();
    CCPoint touchPoint_localSpace=CCPointApplyAffineTransform(touchPoint,parentToNodeTransform);
    //---make m_pointList and m_nRow,m_nCol
    for(float y=contentSize.height+m_gridSideLenMax;y>-m_gridSideLenMax;y-=m_gridSideLenMax){
        m_nRowPoint++;
        m_nColPoint=0;//must have this line, or the m_nColPoint will be add too many times
        for(float x=-m_gridSideLenMax;x<=contentSize.width+m_gridSideLenMax;x+=m_gridSideLenMax){
            m_nColPoint++;
            CCPoint point(x,y);
            m_pointList.push_back(point);
        }
    }//got m_pointList and m_nRowPoint,m_nColPoint
    //----calculate m_nRow and m_nCol
    m_nRow=m_nRowPoint-1;
    m_nCol=m_nColPoint-1;
    //----make m_IDquadList
    m_IDquadList.reserve(m_nRow*m_nCol);//for fast push_back
    for(int i=0;i<m_nRow;i++){
        for(int j=0;j<m_nCol;j++){
            //quad(i,j)
            CIDquad IDquad;
            IDquad.m_vIDLU=i*m_nColPoint+j;//m_pointList(i,j)
            IDquad.m_vIDRU=IDquad.m_vIDLU+1;
            IDquad.m_vIDLD=IDquad.m_vIDLU+m_nColPoint;
            IDquad.m_vIDRD=IDquad.m_vIDLD+1;
            m_IDquadList.push_back(IDquad);
        }
    }//got m_IDquadList
    //----further divide
    const float k=0.5;
    while(1){
        bool isOneOrMoreDivided=false;
        int nIDquad=(int)m_IDquadList.size();
        for(int i=0;i<nIDquad;i++){
            CIDquad&IDquad=m_IDquadList[i];
            if(IDquad.m_isRemoved)continue;
            const int IDLU=IDquad.m_vIDLU;
            const int IDLD=IDquad.m_vIDLD;
            const int IDRU=IDquad.m_vIDRU;
            const int IDRD=IDquad.m_vIDRD;
            CCPoint posLU=m_pointList[IDLU];
            CCPoint posLD=m_pointList[IDLD];
            CCPoint posRU=m_pointList[IDRU];
            CCPoint posRD=m_pointList[IDRD];
            CCPoint posCenter=ccpMidpoint(posLU, posRD);
            //judge whether DIDquad need divide
            float sideLenx=posRU.x-posLU.x;
            float sideLeny=posLU.y-posLD.y;
            assert(sideLenx==sideLeny);
            float sideLen=sideLenx;
            float disQuadToTouch=ccpDistance(touchPoint_localSpace, posCenter);
            if(sideLen/disQuadToTouch>k&&sideLen/2>m_gridSideLenMin){//if sideLen/disQuadToTouch is big and sideLen after divide is bigger than minSideLen
                isOneOrMoreDivided=true;
                //divide
                //mark IDquad isRemoved=true
                IDquad.m_isRemoved=true;
                //add new point
                int startSearchIndex=m_nRowPoint*m_nColPoint;//a optimization, because we know the result must NOT in the first m_nRowPoint*m_nColPoint elements.
                //posL
                int IDL;
                CCPoint posL=ccpMidpoint(posLU, posLD);
                {
                    //find first, if not found, add
                    int ID=getPointInRangeEqualsToTargetPoint(posL,m_pointList,startSearchIndex,(int)m_pointList.size()-1,0.0001);
                    if(ID==-1){
                        m_pointList.push_back(posL);
                        IDL=(int)m_pointList.size()-1;
                    }else{
                        IDL=ID;
                    }
                }
                //posU
                int IDU;
                CCPoint posU=ccpMidpoint(posLU, posRU);
                {
                    //find first, if not found, add
                    int ID=getPointInRangeEqualsToTargetPoint(posU,m_pointList,startSearchIndex,(int)m_pointList.size()-1,0.0001);
                    if(ID==-1){
                        m_pointList.push_back(posU);
                        IDU=(int)m_pointList.size()-1;
                    }else{
                        IDU=ID;
                    }
                }
                //posR
                int IDR;
                CCPoint posR=ccpMidpoint(posRU, posRD);
                {
                    //find first, if not found, add
                    int ID=-1;//because we iterate from left to right, so posR must NOT exist in m_pointList, so the find result must be -1
                    if(ID==-1){
                        m_pointList.push_back(posR);
                        IDR=(int)m_pointList.size()-1;
                    }else{
                        IDR=ID;
                    }
                }
                //posD
                int IDD;
                CCPoint posD=ccpMidpoint(posLD, posRD);
                {
                    //find first, if not found, add
                    int ID=-1;//because we iterate from up to dn, so posR must NOT exist in m_pointList, so the find result must be -1
                    if(ID==-1){
                        m_pointList.push_back(posD);
                        IDD=(int)m_pointList.size()-1;
                    }else{
                        IDD=ID;
                    }
                }
                //posC
                int IDC;
                CCPoint posC=ccpMidpoint(posLU, posRD);//center
                {
                    //posC do not share between IDquads, so it must NOT exist in m_pointList
                    m_pointList.push_back(posC);
                    IDC=(int)m_pointList.size()-1;
                }
                //divide IDquad into 4 smaller IDquad
                CIDquad IDquadLU,IDquadLD,IDquadRU,IDquadRD;
                //IDquadLU
                IDquadLU.m_vIDLU=IDquad.m_vIDLU;
                IDquadLU.m_vIDLD=IDL;
                IDquadLU.m_vIDRU=IDU;
                IDquadLU.m_vIDRD=IDC;
                //IDquadLD
                IDquadLD.m_vIDLU=IDL;
                IDquadLD.m_vIDLD=IDquad.m_vIDLD;
                IDquadLD.m_vIDRU=IDC;
                IDquadLD.m_vIDRD=IDD;
                //IDquadRU
                IDquadRU.m_vIDLU=IDU;
                IDquadRU.m_vIDLD=IDC;
                IDquadRU.m_vIDRU=IDquad.m_vIDRU;
                IDquadRU.m_vIDRD=IDR;
                //IDquadRD
                IDquadRD.m_vIDLU=IDC;
                IDquadRD.m_vIDLD=IDD;
                IDquadRD.m_vIDRU=IDR;
                IDquadRD.m_vIDRD=IDquad.m_vIDRD;
                //add the 4 smaller IDquad to m_IDquadList
                m_IDquadList.push_back(IDquadLU);
                m_IDquadList.push_back(IDquadLD);
                m_IDquadList.push_back(IDquadRU);
                m_IDquadList.push_back(IDquadRD);
                
            
            }
            
        }
        if(isOneOrMoreDivided==false)break;
    }
    //----caculate vIDL,vIDR,vIDU,vIDD for each IDquad
    {
        int nIDquad=(int)m_IDquadList.size();
        for(int i=0;i<nIDquad;i++){
            CIDquad&IDquad=m_IDquadList[i];
            if(IDquad.m_isRemoved)continue;
            //caculate vIDL,vIDR,vIDU,vIDD for IDquad
            //caculate IDquad's center point
            const int IDLU=IDquad.m_vIDLU;
            const int IDLD=IDquad.m_vIDLD;
            const int IDRU=IDquad.m_vIDRU;
            const int IDRD=IDquad.m_vIDRD;
            CCPoint posLU=m_pointList[IDLU];
            CCPoint posLD=m_pointList[IDLD];
            CCPoint posRU=m_pointList[IDRU];
            CCPoint posRD=m_pointList[IDRD];
            //caculate IDquad's posL,posR,posU,posD
            CCPoint posL=ccpMidpoint(posLU, posLD);
            CCPoint posR=ccpMidpoint(posRU, posRD);
            CCPoint posU=ccpMidpoint(posLU, posRU);
            CCPoint posD=ccpMidpoint(posLD, posRD);
            //see whether there is a point in m_pointList equals to posL, if(yes) IDquad.m_vIDL=pointID else IDquad.m_vIDL remains -1
            //same for IDquad.m_vIDR,IDquad.m_vIDU and IDquad.m_vIDD
            int startSearchIndex=m_nRowPoint*m_nColPoint;//a optimization, because we know the result must NOT in the first m_nRowPoint*m_nColPoint elements.
            int IDL=getPointInRangeEqualsToTargetPoint(posL,m_pointList,startSearchIndex,(int)m_pointList.size()-1,0.0001);
            int IDR=getPointInRangeEqualsToTargetPoint(posR,m_pointList,startSearchIndex,(int)m_pointList.size()-1,0.0001);
            int IDU=getPointInRangeEqualsToTargetPoint(posU,m_pointList,startSearchIndex,(int)m_pointList.size()-1,0.0001);
            int IDD=getPointInRangeEqualsToTargetPoint(posD,m_pointList,startSearchIndex,(int)m_pointList.size()-1,0.0001);
            IDquad.m_vIDL=IDL;
            IDquad.m_vIDR=IDR;
            IDquad.m_vIDU=IDU;
            IDquad.m_vIDD=IDD;
        }
    }
    //----random offset each point
    //for a point, we must find all IDquad who use it, in order to determine how big the point offset can be
    {
        vector<vector<int> > findIDquadIDsByPointID;
        findIDquadIDsByPointID.resize((int)m_pointList.size());
        int nIDquad=(int)m_IDquadList.size();
        for(int i=0;i<nIDquad;i++){
            const CIDquad&IDquad=m_IDquadList[i];
            if(IDquad.m_isRemoved)continue;
            const int IDquadID=i;
            const int IDLU=IDquad.m_vIDLU;
            const int IDLD=IDquad.m_vIDLD;
            const int IDRU=IDquad.m_vIDRU;
            const int IDRD=IDquad.m_vIDRD;
            findIDquadIDsByPointID[IDLU].push_back(IDquadID);
            findIDquadIDsByPointID[IDLD].push_back(IDquadID);
            findIDquadIDsByPointID[IDRU].push_back(IDquadID);
            findIDquadIDsByPointID[IDRD].push_back(IDquadID);
        }//got findIDquadIDsByPointID
        vector<CCPoint> pointOffsetList;
        int nPoint=(int)m_pointList.size();
        for(int i=0;i<nPoint;i++){
            //find all IDquads who use point as a vertex
            const vector<int>&IDquadIDList=findIDquadIDsByPointID[i];
            //find the min IDquad in IDquadIDList
            int nIDquad=(int)IDquadIDList.size();
            int minIDquadID=-1;
            float minSideLen=INFINITY;
            for(int j=0;j<nIDquad;j++){
                const int IDquadID=IDquadIDList[j];
                const CIDquad&IDquad=m_IDquadList[IDquadID];
                assert(IDquad.m_isRemoved==false);
                const int IDLU=IDquad.m_vIDLU;
                const int IDLD=IDquad.m_vIDLD;
                const int IDRU=IDquad.m_vIDRU;
                const int IDRD=IDquad.m_vIDRD;
                CCPoint posLU=m_pointList[IDLU];
                CCPoint posLD=m_pointList[IDLD];
                CCPoint posRU=m_pointList[IDRU];
                CCPoint posRD=m_pointList[IDRD];
                float sideLenx=posRU.x-posLU.x;
                float sideLeny=posLU.y-posLD.y;
                assert(sideLenx==sideLeny);
                float sideLen=sideLenx;
                if(sideLen<minSideLen){
                    minSideLen=sideLen;
                    minIDquadID=IDquadID;
                }
            }//got minIDquadID and minSideLen
            float offsetR=minSideLen/4;
            float A=rand()%360;
            float dx=offsetR*cosf(A*M_PI/180);
            float dy=offsetR*sinf(A*M_PI/180);
            pointOffsetList.push_back(ccp(dx,dy));
        }//got pointOffsetList
        //do offset for each point in m_pointList based on pointOffsetList
        for(int i=0;i<nPoint;i++){
            CCPoint&point=m_pointList[i];
            const CCPoint&offset=pointOffsetList[i];
            point=point+offset;
        }
    
    }
    //----make m_triList
    int nIDquad=(int)m_IDquadList.size();
    for(int i=0;i<nIDquad;i++){
        const CIDquad&IDquad=m_IDquadList[i];
        if(IDquad.m_isRemoved)continue;
        const int IDLU=IDquad.m_vIDLU;
        const int IDLD=IDquad.m_vIDLD;
        const int IDRU=IDquad.m_vIDRU;
        const int IDRD=IDquad.m_vIDRD;
        CCPoint posLU=m_pointList[IDLU];
        CCPoint posLD=m_pointList[IDLD];
        CCPoint posRU=m_pointList[IDRU];
        CCPoint posRD=m_pointList[IDRD];
        CCPoint center=ccpMidpoint(posLU, posRD);
        const int IDL=IDquad.m_vIDL;
        const int IDR=IDquad.m_vIDR;
        const int IDU=IDquad.m_vIDU;
        const int IDD=IDquad.m_vIDD;
        if(IDL==-1&&IDR==-1&&IDU==-1&&IDD==-1){//if no neighbors divided
            //make two tris
            Ctri tri0,tri1;
            tri0.setPositions(ccpTov2(m_pointList[IDLU]) , ccpTov2(m_pointList[IDLD]), ccpTov2(m_pointList[IDRD]));
            tri1.setPositions(ccpTov2(m_pointList[IDLU]) , ccpTov2(m_pointList[IDRD]), ccpTov2(m_pointList[IDRU]));
            m_triList.push_back(tri0);
            m_triList.push_back(tri1);
        }else{//if one or more neighbors divided
            //add center to m_pointList
            m_pointList.push_back(center);
            int IDC=(int)m_pointList.size()-1;
            //left quarter
            if(IDL!=-1){
                //left quarter break into two tris
                Ctri tri0,tri1;
                tri0.setPositions(ccpTov2(m_pointList[IDC]), ccpTov2(m_pointList[IDLU]), ccpTov2(m_pointList[IDL]));
                tri1.setPositions(ccpTov2(m_pointList[IDC]), ccpTov2(m_pointList[IDL]), ccpTov2(m_pointList[IDLD]));
                m_triList.push_back(tri0);
                m_triList.push_back(tri1);
            }else{
                //left quarter make one tri
                Ctri tri;
                tri.setPositions(ccpTov2(m_pointList[IDC]), ccpTov2(m_pointList[IDLU]), ccpTov2(m_pointList[IDLD]));
                m_triList.push_back(tri);
            }
            //right quarter
            if(IDR!=-1){
                //right quarter break into two tris
                Ctri tri0,tri1;
                tri0.setPositions(ccpTov2(m_pointList[IDC]), ccpTov2(m_pointList[IDRD]), ccpTov2(m_pointList[IDR]));
                tri1.setPositions(ccpTov2(m_pointList[IDC]), ccpTov2(m_pointList[IDR]), ccpTov2(m_pointList[IDRU]));
                m_triList.push_back(tri0);
                m_triList.push_back(tri1);
            }else{
                //right quarter make one tri
                Ctri tri;
                tri.setPositions(ccpTov2(m_pointList[IDC]), ccpTov2(m_pointList[IDRD]), ccpTov2(m_pointList[IDRU]));
                m_triList.push_back(tri);
            }
            //up quarter
            if(IDU!=-1){
                //up quarter break into two tris
                Ctri tri0,tri1;
                tri0.setPositions(ccpTov2(m_pointList[IDC]), ccpTov2(m_pointList[IDU]), ccpTov2(m_pointList[IDLU]));
                tri1.setPositions(ccpTov2(m_pointList[IDC]), ccpTov2(m_pointList[IDRU]), ccpTov2(m_pointList[IDU]));
                m_triList.push_back(tri0);
                m_triList.push_back(tri1);
                
            }else{
                //up quarter make one tri
                Ctri tri;
                tri.setPositions(ccpTov2(m_pointList[IDC]), ccpTov2(m_pointList[IDRU]), ccpTov2(m_pointList[IDLU]));
                m_triList.push_back(tri);
            }
            //dn quarter
            if(IDD!=-1){
                //dn quarter break into two tris
                Ctri tri0,tri1;
                tri0.setPositions(ccpTov2(m_pointList[IDC]), ccpTov2(m_pointList[IDLD]), ccpTov2(m_pointList[IDD]));
                tri1.setPositions(ccpTov2(m_pointList[IDC]), ccpTov2(m_pointList[IDD]), ccpTov2(m_pointList[IDRD]));
                m_triList.push_back(tri0);
                m_triList.push_back(tri1);
            }else{
                //dn quarter make one tri
                Ctri tri;
                tri.setPositions(ccpTov2(m_pointList[IDC]), ccpTov2(m_pointList[IDLD]), ccpTov2(m_pointList[IDRD]));
                m_triList.push_back(tri);
            }
        }
    }
    //----make each tri smaller
    {
        const float dOffset=1.0;
        int nTri=(int)m_triList.size();
        for(int i=0;i<nTri;i++){
            Ctri&tri=m_triList[i];
            CCPoint p0=v2Toccp(tri.m_pos[0]);
            CCPoint p1=v2Toccp(tri.m_pos[1]);
            CCPoint p2=v2Toccp(tri.m_pos[2]);
            vector<CCPoint> points=makeTriangleSmaller(p0, p1, p2, dOffset);
            if((int)points.size()==3){
                tri.setPositions(ccpTov2(points[0]), ccpTov2(points[1]), ccpTov2(points[2]));
            }else if((int)points.size()<3){
                //do nothing
            }else{//(int)points.size()>3
                assert(false);
            }
        }
    }
    //----calculate texCoord for each vertex of each tri
    {
        CCSize contentSize=this->getContentSize();
        int nTri=(int)m_triList.size();
        for(int i=0;i<nTri;i++){
            Ctri&tri=m_triList[i];
            const Cv2&v0=tri.m_pos[0];
            const Cv2&v1=tri.m_pos[1];
            const Cv2&v2=tri.m_pos[2];
            Cv2&texCoord0=tri.m_texCoord[0];
            Cv2&texCoord1=tri.m_texCoord[1];
            Cv2&texCoord2=tri.m_texCoord[2];
            texCoord0.init(v0.x()/contentSize.width, 1-v0.y()/contentSize.height);
            texCoord1.init(v1.x()/contentSize.width, 1-v1.y()/contentSize.height);
            texCoord2.init(v2.x()/contentSize.width, 1-v2.y()/contentSize.height);
        }
    }

    //----fill mesh based on triList
    int nTri=(int)m_triList.size();
    for(int i=0;i<nTri;i++){
        const Ctri&tri=m_triList[i];
        const Cv2&v0=tri.m_pos[0];
        const Cv2&v1=tri.m_pos[1];
        const Cv2&v2=tri.m_pos[2];
        const Cv2&texCoord0=tri.m_texCoord[0];
        const Cv2&texCoord1=tri.m_texCoord[1];
        const Cv2&texCoord2=tri.m_texCoord[2];
        const Cv4&color0=tri.m_color[0];
        const Cv4&color1=tri.m_color[1];
        const Cv4&color2=tri.m_color[2];
        m_mesh->vlist.push_back(v0);
        m_mesh->texCoordList.push_back(texCoord0);
        m_mesh->colorList.push_back(color0);
        int ID0=(int)m_mesh->vlist.size()-1;
        m_mesh->vlist.push_back(v1);
        m_mesh->texCoordList.push_back(texCoord1);
        m_mesh->colorList.push_back(color1);
        int ID1=(int)m_mesh->vlist.size()-1;
        m_mesh->vlist.push_back(v2);
        m_mesh->texCoordList.push_back(texCoord2);
        m_mesh->colorList.push_back(color2);
        int ID2=(int)m_mesh->vlist.size()-1;
        m_mesh->IDtriList.push_back(CIDTriangle(ID0,ID1,ID2));
    }//got m_mesh
    //----fill m_meshWire based on triList
    //in order to independently control each IDline's color,
    //we need to duplicate vertexs in m_meshWire: 6 vertex per triangle
    for(int i=0;i<nTri;i++){
        const Ctri&tri=m_triList[i];
        const Cv2&v0=tri.m_pos[0];
        const Cv2&v1=tri.m_pos[1];
        const Cv2&v2=tri.m_pos[2];
        const Cv2&texCoord0=tri.m_texCoord[0];
        const Cv2&texCoord1=tri.m_texCoord[1];
        const Cv2&texCoord2=tri.m_texCoord[2];
        const Cv4&color0=tri.m_color[0];
        const Cv4&color1=tri.m_color[1];
        const Cv4&color2=tri.m_color[2];
        //make three IDlines (duplicate vertex)
        //IDline0
        m_meshWire->vlist.push_back(v0);//v0
        m_meshWire->texCoordList.push_back(texCoord0);
        m_meshWire->colorList.push_back(color0);
        int ID0=(int)m_meshWire->vlist.size()-1;
        m_meshWire->vlist.push_back(v1);//v1
        m_meshWire->texCoordList.push_back(texCoord1);
        m_meshWire->colorList.push_back(color1);
        int ID1=(int)m_meshWire->vlist.size()-1;
        //IDline1
        m_meshWire->vlist.push_back(v1);//v2
        m_meshWire->texCoordList.push_back(texCoord1);
        m_meshWire->colorList.push_back(color1);
        int ID2=(int)m_meshWire->vlist.size()-1;
        m_meshWire->vlist.push_back(v2);//v3
        m_meshWire->texCoordList.push_back(texCoord2);
        m_meshWire->colorList.push_back(color2);
        int ID3=(int)m_meshWire->vlist.size()-1;
        //IDline2
        m_meshWire->vlist.push_back(v2);//v4
        m_meshWire->texCoordList.push_back(texCoord2);
        m_meshWire->colorList.push_back(color2);
        int ID4=(int)m_meshWire->vlist.size()-1;
        m_meshWire->vlist.push_back(v0);//v5
        m_meshWire->texCoordList.push_back(texCoord0);
        m_meshWire->colorList.push_back(color0);
        int ID5=(int)m_meshWire->vlist.size()-1;
        //make three IDlines
        m_meshWire->IDlineList.push_back(CIDLine(ID0,ID1));
        m_meshWire->IDlineList.push_back(CIDLine(ID2,ID3));
        m_meshWire->IDlineList.push_back(CIDLine(ID4,ID5));

    }//got m_meshWire

    //caculate color of each IDline
    {
        const float lightStrength0=3.0;
        const float lightStrength1=1.5;
        const CCPoint lightDir0=CCPoint(cosf(-45*M_PI/180),sinf(-45*M_PI/180));
        const CCPoint lightDir1=CCPoint(cosf(-135*M_PI/180),sinf(-135*M_PI/180));
        const float ambientStrength=0.5;
        int nIDline=(int)m_meshWire->IDlineList.size();
        for(int i=0;i<nIDline;i++){
            CIDLine&IDline=m_meshWire->IDlineList[i];
            Cv4&colorA=m_meshWire->colorList[IDline.getvIDByIndex(0)];
            Cv4&colorB=m_meshWire->colorList[IDline.getvIDByIndex(1)];
            Cv2&vA=m_meshWire->vlist[IDline.getvIDByIndex(0)];
            Cv2&vB=m_meshWire->vlist[IDline.getvIDByIndex(1)];
            //calculate rightNormal
            CCPoint dir=ccpNormalize(v2Toccp(vB)-v2Toccp(vA));
            CCPoint rightNormal=getRightNormal(dir);
            //rightNormal and -lightDir
            float colorStrength0=MAX(0,ccpDot(-lightDir0, rightNormal)*lightStrength0)+ambientStrength;
            float colorStrength1=MAX(0,ccpDot(-lightDir1, rightNormal)*lightStrength1)+ambientStrength;
            float colorStrength=MAX(colorStrength0,colorStrength1);
            colorA.init(colorStrength, colorStrength, colorStrength, 1);
            colorB.init(colorStrength, colorStrength, colorStrength, 1);
        }
    }
    //----submit
    //submit m_mesh
    m_indexVBO->submitPos(m_mesh->vlist, GL_STATIC_DRAW);
    m_indexVBO->submitColor(m_mesh->colorList, GL_STATIC_DRAW);
    m_indexVBO->submitTexCoord(m_mesh->texCoordList, GL_STATIC_DRAW);
    m_indexVBO->submitIndex(m_mesh->IDtriList, GL_STATIC_DRAW);
    //submit m_meshWire
    m_indexVBO_wire->submitPos(m_meshWire->vlist, GL_STATIC_DRAW);
    m_indexVBO_wire->submitColor(m_meshWire->colorList, GL_STATIC_DRAW);
    m_indexVBO_wire->submitTexCoord(m_meshWire->texCoordList, GL_STATIC_DRAW);
    m_indexVBO_wire->submitIndex(m_meshWire->IDlineList, GL_STATIC_DRAW);
    

}

void CbreakSprite::generateDelayTimeForTris(float minDelayTime,float maxDelayTime){
    //----we use the formula below:
    // delayTime=(area^k-minArea^k)/(maxArea^k-minArea^k)*(maxDelayTime-minDelayTime)+minDelayTime
    //----
    const float k=1.5;
    //calculate pow(area,k) for each tri
    vector<float> areaPowKList;
    areaPowKList.reserve((int)m_triList.size());//for fast push_back
    int nTri=(int)m_triList.size();
    for(int i=0;i<nTri;i++){
        Ctri&tri=m_triList[i];
        float Area=calculateTriangleArea(v2Toccp(tri.m_pos[0]), v2Toccp(tri.m_pos[1]), v2Toccp(tri.m_pos[2]));
        //add random to Area
        Area+=(random01()-0.5)*Area/3;
        //
        float AreaPowK=powf(Area, k);
        areaPowKList.push_back(AreaPowK);
    }//got areaPowKList
    //find minAreaPowK and maxAreaPowK
    float minAreaPowK=INFINITY;
    float maxAreaPowK=-INFINITY;
    int nAreaPowK=(int)areaPowKList.size();
    for(int i=0;i<nAreaPowK;i++){
        float areaPowK=areaPowKList[i];
        if(areaPowK>maxAreaPowK)maxAreaPowK=areaPowK;
        if(areaPowK<minAreaPowK)minAreaPowK=areaPowK;
    }//got minAreaPowK and maxAreaPowK
    //calculate delayTime for each tri
    for(int i=0;i<nTri;i++){
        Ctri&tri=m_triList[i];
        float areaPowK=areaPowKList[i];
        float delayTime=(areaPowK-minAreaPowK)/(maxAreaPowK-minAreaPowK)*(maxDelayTime-minDelayTime)+minDelayTime;
        tri.m_fallOffDelay=delayTime;
    }
}

void CbreakSprite::updateMeshAndSubmitChange(){
    //----update m_mesh and m_meshWire based on m_triList
    int nTri=(int)m_triList.size();
    for(int i=0;i<nTri;i++){
        const Ctri&tri=m_triList[i];
        const Cv2&v0=tri.m_pos[0];
        const Cv2&v1=tri.m_pos[1];
        const Cv2&v2=tri.m_pos[2];
        //update triangle(i)'s vertex data in m_mesh
        //m_mesh->vlist[3*i]~[3*i+2] is triangle(i)'s vertex data
        m_mesh->vlist[3*i]=v0;
        m_mesh->vlist[3*i+1]=v1;
        m_mesh->vlist[3*i+2]=v2;
        //update triangle(i)'s vertex data in m_meshWire
        //m_mesh->vlist[6*i]~[6*i+5] is triangle(i)'s vertex data
        m_meshWire->vlist[6*i]=v0;
        m_meshWire->vlist[6*i+1]=v1;
        m_meshWire->vlist[6*i+2]=v1;
        m_meshWire->vlist[6*i+3]=v2;
        m_meshWire->vlist[6*i+4]=v2;
        m_meshWire->vlist[6*i+5]=v0;
    }//m_mesh and m_meshWire have been updated
    //----submit change
    //submit m_mesh's change
    m_indexVBO->submitPos(m_mesh->vlist, GL_STREAM_DRAW);
    //submit m_meshWire's change
    m_indexVBO_wire->submitPos(m_meshWire->vlist, GL_STREAM_DRAW);

    
}

void CbreakSprite::updateBreakAction(float time,float dt,float ay){
    //tris fall
    int nTri=(int)m_triList.size();
    for(int i=0;i<nTri;i++){
        Ctri&tri=m_triList[i];
        if(time>tri.m_fallOffDelay){
            tri.m_vy+=ay*dt;
            float dy=tri.m_vy*dt;
            Cv2&v0=tri.m_pos[0];
            Cv2&v1=tri.m_pos[1];
            Cv2&v2=tri.m_pos[2];
            v0.sety(v0.y()+dy);
            v1.sety(v1.y()+dy);
            v2.sety(v2.y()+dy);
        }
    }
    //update mesh and submit change
    updateMeshAndSubmitChange();
}
//-------------------------------------------------------
//creates the action
CfallOffAction* CfallOffAction::create(float fDuration,float ay){
    
    CfallOffAction *p = new CfallOffAction();
    p->initWithDuration(fDuration,ay);
    p->autorelease();
    
    return p;
}
//initializes the action
bool CfallOffAction::initWithDuration(float fDuration,float ay){
    if (CCActionInterval::initWithDuration(fDuration))
    {
        //my init code
        m_timeFoe=0;
        m_timeCur=0;
        m_ay=ay;
    
        return true;
    }
    
    return false;
}


void CfallOffAction::startWithTarget(CCNode *pTarget){
    //set some members of pTarget
    ((CbreakSprite*)pTarget)->m_state=eState_fallOff;
    //call father startWithTarget
    CCActionInterval::startWithTarget(pTarget);
   
}
void CfallOffAction::update(float time){//the time actually is percentage, namely eclipsedTime/totalDuration
    //the t in action's update(t) and sprite's update(t) is different meaning
    //t in action's update(t) means the progress percentage, see CCActionInterval::step()
    //t in sprite's update(t) means the deltaTime between current frame and previous frame, see CCDirector::drawScene()
    //cout<<"time:"<<time<<endl;
    m_timeFoe=m_timeCur;
    float progressPercentage=time;
    m_timeCur=progressPercentage*getDuration();
    ((CbreakSprite*)_target)->updateBreakAction(m_timeCur,m_timeCur-m_timeFoe,m_ay);
    
}


namespace_ens_end