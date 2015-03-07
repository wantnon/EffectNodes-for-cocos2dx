
#include "ensTailNode.h"

namespace_ens_begin
using namespace tail;
bool CtailSprite::init(const string&texFileName)
{

    //init this CCSprite
    this->CCSprite::initWithFile(texFileName.c_str());
    //indexVBO
    m_indexVBO=new CindexVBO();
    m_indexVBO->autorelease();
    m_indexVBO->retain();
    //create and set shader program
	{
        GLchar * vertSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/tailEffect.vsh").c_str())->getCString();
		GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/tailEffect.fsh").c_str())->getCString();
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
        //attach my own uniforms
        //...
        //set program
        m_program=program;
        m_program->retain();
        //check gl error
		CHECK_GL_ERROR_DEBUG();
	}
	//check gl error
	CHECK_GL_ERROR_DEBUG();
    ccBlendFunc blendFunc={GL_SRC_ALPHA,GL_ONE/*GL_ONE_MINUS_SRC_ALPHA*/};
    this->setBlendFunc(blendFunc);
	return true ;
}
void CtailSprite::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags){
    
    
  
        _customCommand.init(_globalZOrder);
        _customCommand.func = CC_CALLBACK_0(CtailSprite::onDraw,this,transform, flags);
        renderer->addCommand(&_customCommand);
    if(m_isDrawDebug){
        
        _customCommand_debug.init(_globalZOrder);
        _customCommand_debug.func = CC_CALLBACK_0(CtailSprite::onDrawDebug,this,transform, flags);
        renderer->addCommand(&_customCommand_debug);
    }
  
    
}

void CtailSprite::onDrawDebug(const Mat4 &transform, uint32_t flags){
    const Mat4&m=transform;
    vector<vector<tail::Cquad> >&allQuadMat=m_allQuadMat;
    //----draw line
    glLineWidth(1);
    //    ccDrawColor4B(255, 255, 255, 255);
    int nRow=(int)allQuadMat.size();
    for(int i=0;i<nRow;i++){
        const vector<Cquad>&quadList=allQuadMat[i];
        int nQuad=(int)quadList.size();
        for(int j=0;j<nQuad;j++){
            const Cquad&quad=quadList[j];
            ccDrawLine(M4TransV2(m,quad.m_posLU),M4TransV2(m,quad.m_posLD));
            ccDrawLine(M4TransV2(m,quad.m_posLD),M4TransV2(m,quad.m_posRD));
            ccDrawLine(M4TransV2(m,quad.m_posRD),M4TransV2(m,quad.m_posRU));
            ccDrawLine(M4TransV2(m,quad.m_posRU),M4TransV2(m,quad.m_posLU));
        }
    }


}
void CtailSprite::onDraw(const Mat4 &transform, uint32_t flags)
{
    
    //----generate mesh
    if(m_footPrintList.empty()){
        //because m_allQuadMat is update in generateMesh(),
        //if we quick return here, we can not call generateMesh(),
        //m_allQuadMat will not be updated.
        //so we should update m_allQuadMat manually
        m_allQuadMat.clear();//manually update m_allQuadMat
        return;
    }
    vector<CfootPrint> footPrintList=m_footPrintList;
    if(isPointEqual(this->getPosition(), footPrintList[(int)footPrintList.size()-1].m_pos)==false){
        footPrintList.push_back(CfootPrint(this->getPosition(),m_initialOpacity));
    }
    //convert m_vlist to local space
    int nFootPrint=(int)footPrintList.size();
    for(int i=0;i<nFootPrint;i++)
    {
        CfootPrint&footPrint=footPrintList[i];
        footPrint.m_pos=convertToNodeSpace(footPrint.m_pos);
    }
    Cmesh*mesh=generateMesh(footPrintList);
    //submit mesh
    m_indexVBO->submitPos(mesh->vlist, GL_STREAM_DRAW);
    m_indexVBO->submitIndex(mesh->IDtriList, GL_STREAM_DRAW);
    m_indexVBO->submitTexCoord(mesh->texCoordList, GL_STREAM_DRAW);
    m_indexVBO->submitColor(mesh->colorList, GL_STREAM_DRAW);
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
    //...
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
  
}
void CtailSprite::update(float dt){
    //add a pos
    CCPoint posCur=this->getPosition();
    if(m_footPrintList.empty()){
        m_footPrintList.push_back(CfootPrint(posCur,m_initialOpacity));
    }else{
        CCPoint posFoe=m_footPrintList[(int)m_footPrintList.size()-1].m_pos;
        if(ccpDistance(posCur, posFoe)>m_minDis)m_footPrintList.push_back(CfootPrint(posCur,m_initialOpacity));
    }
    //disapear
    int nFootPrint=(int)m_footPrintList.size();
    for(int i=0;i<nFootPrint;i++){
        CfootPrint&footPrint=m_footPrintList[i];
        footPrint.m_opacity*=m_opacityDamping;
        if(footPrint.m_opacity<0.1){
            m_footPrintList.erase(m_footPrintList.begin()+i);
            i--;
            nFootPrint--;
        }
    }
    
}

Cmesh* CtailSprite::generateMesh(const vector<CfootPrint>&footPrintList){
    Cmesh*mesh=new Cmesh();
    mesh->autorelease();
    int nFootPrint=(int)footPrintList.size();
    if(nFootPrint<2)return mesh;
    //caluculate length of each seg in footPrintList
    vector<float> lengthList;
    float totalLength=0;
    for(int i=0;i<nFootPrint-1;i++){
        const CCPoint&start=footPrintList[i].m_pos;
        const CCPoint&end=footPrintList[i+1].m_pos;
        float length=ccpDistance(start, end);
        lengthList.push_back(length);
        totalLength+=length;
    }//got lengthList and totalLength
    //calculate length from start for each footPrint
    vector<float> lengthFromStartList;
    float lengthFromStart=0;
    int nLen=(int)lengthList.size();
    for(int i=0;i<nLen;i++){
        lengthFromStartList.push_back(lengthFromStart);
        lengthFromStart+=lengthList[i];
    }
    lengthFromStartList.push_back(lengthFromStart);//got lengthFromStartList

    //generate turnTypeList
    vector<eTurnType> turnTypeList;
    for(int i=0;i<nFootPrint;i++){
        if(i==0){
            turnTypeList.push_back(eTurnType_unKnown);
        }else if(i==nFootPrint-1){
            turnTypeList.push_back(eTurnType_unKnown);
        }else{
            CCPoint posf=footPrintList[i-1].m_pos;
            CCPoint pos=footPrintList[i].m_pos;
            CCPoint posn=footPrintList[i+1].m_pos;
            CCPoint vec=pos-posf;
            CCPoint vecn=posn-pos;
            float crossProduct=ccpCross(vec, vecn);
            if(crossProduct==0){
                float dotProduct=ccpDot(vec, vecn);
                if(dotProduct>0){//parallel and same direction
                    turnTypeList.push_back(eTurnType_straightForward);
                }else if(dotProduct<0){//parallel and contrary direction
                    turnTypeList.push_back(eTurnType_straightBack);
                }else{
                    assert(false);
                }
            }else if(crossProduct>0){
                turnTypeList.push_back(eTurnType_left);
            }else{
                turnTypeList.push_back(eTurnType_right);
            }
        }
    }
    //generate quadList
    vector<Cquad> upQuadList;
    vector<Cquad> dnQuadList;
    float R=35;
    float curLength=0;
    for(int i=0;i<nFootPrint-1;i++){
        float curLengthFoe=curLength;
        curLength+=lengthList[i];
        //
        CCPoint start=footPrintList[i].m_pos;
        CCPoint end=footPrintList[i+1].m_pos;
        CCPoint dir=ccpNormalize(end-start);
        //
        CCPoint normLeft=CCPoint(-dir.y,dir.x);
        CCPoint normRight=CCPoint(0,0)-normLeft;
        CCPoint LU=start+ccpMult(normLeft, R);
        CCPoint LD=start+ccpMult(normRight, R);
        CCPoint RU=end+ccpMult(normLeft, R);
        CCPoint RD=end+ccpMult(normRight, R);
        //  LU----RU
        //   |               |
        // start--end
        //   |               |
        //  LD----RD
        Cquad upQuad;
        upQuad.m_posLU=LU;
        upQuad.m_posLD=start;
        upQuad.m_posRU=RU;
        upQuad.m_posRD=end;
        upQuad.m_texCoordLU=CCPoint(curLengthFoe/totalLength,0);
        upQuad.m_texCoordLD=CCPoint(curLengthFoe/totalLength,0.5);
        upQuad.m_texCoordRU=CCPoint(curLength/totalLength,0);
        upQuad.m_texCoordRD=CCPoint(curLength/totalLength,0.5);
        upQuad.m_opacity=footPrintList[i].m_opacity;
        Cquad dnQuad;
        dnQuad.m_posLU=start;
        dnQuad.m_posLD=LD;
        dnQuad.m_posRU=end;
        dnQuad.m_posRD=RD;
        dnQuad.m_texCoordLU=CCPoint(curLengthFoe/totalLength,0.5);
        dnQuad.m_texCoordLD=CCPoint(curLengthFoe/totalLength,1);
        dnQuad.m_texCoordRU=CCPoint(curLength/totalLength,0.5);
        dnQuad.m_texCoordRD=CCPoint(curLength/totalLength,1);
        dnQuad.m_opacity=footPrintList[i].m_opacity;
        upQuadList.push_back(upQuad);
        dnQuadList.push_back(dnQuad);
    }//got upQuadList and dnQuadList
    //modify quads in upQuadList and dnQuadList
    vector<Cquad> upQuadList_makeup;
    vector<int> insertIndexList_up;//insert after upQuadList[index]
    vector<Cquad> dnQuadList_makeup;
    vector<int> insertIndexList_dn;//insert after dnQuadList[index]
    int nUpQuad=(int)upQuadList.size();
    int nDnQuad=(int)dnQuadList.size();
    assert(nUpQuad==nDnQuad);
    int nQuad=nUpQuad;
    for(int i=0;i<nQuad-1;i++){
        Cquad&upQuad=upQuadList[i];
        Cquad&upQuadn=upQuadList[i+1];
        Cquad&dnQuad=dnQuadList[i];
        Cquad&dnQuadn=dnQuadList[i+1];
        eTurnType turnType=turnTypeList[i+1];
        CCPoint leftVec=ccpNormalize(upQuad.m_posLD-upQuad.m_posRD);
        CCPoint rightVec=ccpNormalize(upQuadn.m_posRD-upQuadn.m_posLD);
        bool isSharp=(ccpDot(leftVec, rightVec)>cosf(135*M_PI/180));//120,145
        if(turnType==eTurnType_left||turnType==eTurnType_straightBack){
            //need interpolate between dnQuad and dnQuadn
            vector<Cquad> quadList_makeup;
            CCPoint L1=dnQuad.m_posRD-dnQuad.m_posRU;
            CCPoint L2=dnQuadn.m_posLD-dnQuadn.m_posLU;
            vector<CCPoint> LList;//L1,...,L2
            LList.push_back(L1);
            const float dA=30;
            float A=0;
            while (1) {
                A+=dA;
                CCPoint L=CCPoint(L1.x*cosf(A*M_PI/180)-L1.y*sinf(A*M_PI/180),
                                  L1.x*sinf(A*M_PI/180)+L1.y*cosf(A*M_PI/180));
                if(ccpCross(L, L2)>0){//L not over L2
                    LList.push_back(L);
                }else{//L is over L2, stop
                    break;
                }
            }
            
            LList.push_back(L2);
            //generate triangle quads basing on LList
            int nL=(int)LList.size();
            for(int j=0;j<nL-1;j++){
                CCPoint L=LList[j];
                CCPoint Ln=LList[j+1];
                CCPoint P=dnQuad.m_posRU;
                Cquad quad;
                quad.m_posLU=P;
                quad.m_posLD=P+L;
                quad.m_posRU=P;
                quad.m_posRD=P+Ln;
                quad.m_opacity=dnQuad.m_opacity+(dnQuadn.m_opacity-dnQuad.m_opacity)*((j+1)/nL);
                quad.m_isTriangle=true;
                quad.m_opacityLU=(dnQuad.m_opacity+dnQuadn.m_opacity)/2;
                quad.m_opacityRU=(dnQuad.m_opacity+dnQuadn.m_opacity)/2;
                quadList_makeup.push_back(quad);
            }//got quadList_makeup
            //calculate down segLen of each quad in quadList
            vector<float> lenList;
            int nQuadMakeUp=(int)quadList_makeup.size();
            for(int j=0;j<nQuadMakeUp;j++){
                const Cquad&quad=quadList_makeup[j];
                float len=ccpLength(quad.m_posLD-quad.m_posRD);
                lenList.push_back(len);
            }//got lenList
            //calculate sum of lens in lenList
            float d=0;
            int nLen=lenList.size();
            for(int j=0;j<nLen;j++){
                d+=lenList[j];
            }//got L
            float d1=ccpLength(dnQuad.m_posLU-dnQuad.m_posRU)/2;
            float d2=ccpLength(dnQuadn.m_posLU-dnQuadn.m_posRD)/2;
            //       /-------/\-------\
            //      /           d1/d\d2           \
            //     /---*---/--\---*---\
            //            sa  _sa  _sb   sb
            float sa=(dnQuad.m_texCoordLU.x+dnQuad.m_texCoordRU.x)/2;
            float sb=(dnQuadn.m_texCoordLU.x+dnQuadn.m_texCoordRU.x)/2;
            float _sa=sa+(sb-sa)/(d1+d+d2)*d1;
            float _sb=sa+(sb-sa)/(d1+d+d2)*(d1+d);
            //the texCoord s of up edges of quads in qudList_makeup is range from _sa to _sb
            //the texCoord s of down edges of quads in quadList_makeup is all equals to dnQuad.m_texCoordRU.x
            for(int j=0;j<nQuadMakeUp;j++){
                Cquad&quad=quadList_makeup[j];
                quad.m_texCoordLU=CCPoint(dnQuad.m_texCoordRU.x,0.5);
                quad.m_texCoordRU=CCPoint(dnQuad.m_texCoordRU.x,0.5);
                quad.m_texCoordLD=CCPoint(_sa+j*(_sb-_sa)/nQuadMakeUp,1);
                quad.m_texCoordRD=CCPoint(_sa+(j+1)*(_sb-_sa)/nQuadMakeUp,1);
                
            }
            //the opacity of quads in quadList_makeup is range from opacityFirst to opacityLast
            float opacityFirst=dnQuad.m_opacity+(dnQuadn.m_opacity-dnQuad.m_opacity)/(d1+d)*d1;
            float opacityLast=dnQuadn.m_opacity;
            for(int j=0;j<nQuadMakeUp;j++){
                Cquad&quad=quadList_makeup[j];
                quad.m_opacity=opacityFirst+(opacityLast-opacityFirst)/nQuadMakeUp*j;
            }

            //add quads in quadList_makeup to dnQuadList_makeUp
            for(int j=0;j<nQuadMakeUp;j++){
                dnQuadList_makeup.push_back(quadList_makeup[j]);
                insertIndexList_dn.push_back(i);
            }

        }
        if(turnType==eTurnType_right||turnType==eTurnType_straightBack){
            //----need interpolate between upQuad and upQuadn
            vector<Cquad> quadList_makeup;
            CCPoint L1=upQuad.m_posRU-upQuad.m_posRD;
            CCPoint L2=upQuadn.m_posLU-upQuadn.m_posLD;
            vector<CCPoint> LList;//L1,...,L2
            LList.push_back(L1);
            const float dA=-30;
            float A=0;
            while (1) {
                A+=dA;
                CCPoint L=CCPoint(L1.x*cosf(A*M_PI/180)-L1.y*sinf(A*M_PI/180),
                                  L1.x*sinf(A*M_PI/180)+L1.y*cosf(A*M_PI/180));
                if(ccpCross(L, L2)<0){//L not over L2
                    LList.push_back(L);
                }else{//L is over L2, stop
                    break;
                }
            }
            LList.push_back(L2);
            //generate triangle quads basing on LList
            int nL=(int)LList.size();
            for(int j=0;j<nL-1;j++){
                CCPoint L=LList[j];
                CCPoint Ln=LList[j+1];
                CCPoint P=upQuad.m_posRD;
                Cquad quad;
                quad.m_posLU=P+L;
                quad.m_posLD=P;
                quad.m_posRU=P+Ln;
                quad.m_posRD=P;
                quad.m_opacity=upQuad.m_opacity+(upQuadn.m_opacity-upQuad.m_opacity)*((j+1)/nL);
                quad.m_isTriangle=true;
                quad.m_opacityLD=(upQuad.m_opacity+upQuadn.m_opacity)/2;
                quad.m_opacityRD=(upQuad.m_opacity+upQuadn.m_opacity)/2;
                quadList_makeup.push_back(quad);
            }//got quadList_makeup
            //calculate up segLen of each quad in quadList
            vector<float> lenList;
            int nQuadMakeUp=(int)quadList_makeup.size();
            for(int j=0;j<nQuadMakeUp;j++){
                const Cquad&quad=quadList_makeup[j];
                float len=ccpLength(quad.m_posLU-quad.m_posRU);
                lenList.push_back(len);
            }//got lenList
            //calculate sum of lens in lenList
            float d=0;
            int nLen=lenList.size();
            for(int j=0;j<nLen;j++){
                d+=lenList[j];
            }//got L
            float d1=ccpLength(upQuad.m_posLD-upQuad.m_posRD)/2;
            float d2=ccpLength(upQuadn.m_posLD-upQuadn.m_posRD)/2;
            //           sa   _sa  _sb  sb
            //   \---*---\-- /---*---/
            //    \          d1  \  / d2           /
            //     \-------\/-------/
            float sa=(upQuad.m_texCoordLD.x+upQuad.m_texCoordRD.x)/2;
            float sb=(upQuadn.m_texCoordLD.x+upQuadn.m_texCoordRD.x)/2;
            float _sa=sa+(sb-sa)/(d1+d+d2)*d1;
            float _sb=sa+(sb-sa)/(d1+d+d2)*(d1+d);
            //the texCoord s of up edges of quads in qudList_makeup is range from _sa to _sb
            //the texCoord s of down edges of quads in quadList_makeup is all equals to upQuad.m_texCoordRD.x
            for(int j=0;j<nQuadMakeUp;j++){
                Cquad&quad=quadList_makeup[j];
                quad.m_texCoordLU=CCPoint(_sa+j*(_sb-_sa)/nQuadMakeUp,0);
                quad.m_texCoordRU=CCPoint(_sa+(j+1)*(_sb-_sa)/nQuadMakeUp,0);
                quad.m_texCoordLD=CCPoint(upQuad.m_texCoordRD.x,0.5);
                quad.m_texCoordRD=CCPoint(upQuad.m_texCoordRD.x,0.5);
                
            }
            //the opacity of quads in quadList_makeup is range from opacityFirst to opacityLast
            float opacityFirst=upQuad.m_opacity+(upQuadn.m_opacity-upQuad.m_opacity)/(d1+d)*d1;
            float opacityLast=upQuadn.m_opacity;
            for(int j=0;j<nQuadMakeUp;j++){
                Cquad&quad=quadList_makeup[j];
                quad.m_opacity=opacityFirst+(opacityLast-opacityFirst)/nQuadMakeUp*j;
            }

            //add quads in quadList_makeup to upQuadList_makeUp
            for(int j=0;j<nQuadMakeUp;j++){
                upQuadList_makeup.push_back(quadList_makeup[j]);
                insertIndexList_up.push_back(i);
            }

        
        }
        if(turnType==eTurnType_left&&isSharp==false){
            CCPoint upMid=ccpMidpoint(upQuad.m_posRU, upQuadn.m_posLU);
            upQuad.m_posRU=upMid;
            upQuadn.m_posLU=upMid;
        }
        
        if(turnType==eTurnType_right&&isSharp==false){
            CCPoint dnMid=ccpMidpoint(dnQuad.m_posRD, dnQuadn.m_posLD);
            dnQuad.m_posRD=dnMid;
            dnQuadn.m_posLD=dnMid;
        }
        

        //----if corner is sharp
        
        
        if(turnType==eTurnType_right&&isSharp){
            //cout<<"fsdkfjsdkfd"<<endl;
            //need interpolate between dnQuad and dnQuadn
            vector<Cquad> quadList_makeup;
            CCPoint L1=dnQuad.m_posRD-dnQuad.m_posRU;
            CCPoint L2=dnQuadn.m_posLD-dnQuadn.m_posLU;
            vector<CCPoint> LList;//L1,...,L2
            LList.push_back(L1);
            const float dA=30;
            float A=0;
            while (1) {
                A+=dA;
                CCPoint L=CCPoint(L1.x*cosf(A*M_PI/180)-L1.y*sinf(A*M_PI/180),
                                  L1.x*sinf(A*M_PI/180)+L1.y*cosf(A*M_PI/180));
                if(A<=180//if fabs(A) is less than 180, we must continue rotation
                   ||ccpCross(L, L2)>0){//L not over L2
                    LList.push_back(L);
                }else{//L is over L2, stop
                    break;
                }
            }
            
            LList.push_back(L2);
            //generate triangle quads basing on LList
            int nL=(int)LList.size();
            for(int j=0;j<nL-1;j++){
                CCPoint L=LList[j];
                CCPoint Ln=LList[j+1];
                CCPoint P=dnQuad.m_posRU;
                Cquad quad;
                quad.m_posLU=P;
                quad.m_posLD=P+L;
                quad.m_posRU=P;
                quad.m_posRD=P+Ln;
                quad.m_opacity=dnQuad.m_opacity+(dnQuadn.m_opacity-dnQuad.m_opacity)*((j+1)/nL);
                quad.m_isTriangle=true;
                quad.m_opacityLU=(dnQuad.m_opacity+dnQuadn.m_opacity)/2;
                quad.m_opacityRU=(dnQuad.m_opacity+dnQuadn.m_opacity)/2;
                quadList_makeup.push_back(quad);
            }//got quadList_makeup
            //calculate down segLen of each quad in quadList
            vector<float> lenList;
            int nQuadMakeUp=(int)quadList_makeup.size();
            for(int j=0;j<nQuadMakeUp;j++){
                const Cquad&quad=quadList_makeup[j];
                float len=ccpLength(quad.m_posLD-quad.m_posRD);
                lenList.push_back(len);
            }//got lenList
            //calculate sum of lens in lenList
            float d=0;
            int nLen=lenList.size();
            for(int j=0;j<nLen;j++){
                d+=lenList[j];
            }//got L
            float d1=ccpLength(dnQuad.m_posLU-dnQuad.m_posRU)/2;
            float d2=ccpLength(dnQuadn.m_posLU-dnQuadn.m_posRD)/2;
            //       /-------/\-------\
            //      /           d1/d\d2           \
            //     /---*---/--\---*---\
            //            sa  _sa  _sb   sb
            float sa=(dnQuad.m_texCoordLU.x+dnQuad.m_texCoordRU.x)/2;
            float sb=(dnQuadn.m_texCoordLU.x+dnQuadn.m_texCoordRU.x)/2;
            float _sa=sa+(sb-sa)/(d1+d+d2)*d1;
            float _sb=sa+(sb-sa)/(d1+d+d2)*(d1+d);
            //the texCoord s of up edges of quads in qudList_makeup is range from _sa to _sb
            //the texCoord s of down edges of quads in quadList_makeup is all equals to dnQuad.m_texCoordRU.x
            for(int j=0;j<nQuadMakeUp;j++){
                Cquad&quad=quadList_makeup[j];
                quad.m_texCoordLU=CCPoint(dnQuad.m_texCoordRU.x,0.5);
                quad.m_texCoordRU=CCPoint(dnQuad.m_texCoordRU.x,0.5);
                quad.m_texCoordLD=CCPoint(_sa+j*(_sb-_sa)/nQuadMakeUp,1);
                quad.m_texCoordRD=CCPoint(_sa+(j+1)*(_sb-_sa)/nQuadMakeUp,1);
                
            }
            //the opacity of quads in quadList_makeup is range from opacityFirst to opacityLast
            float opacityFirst=dnQuad.m_opacity+(dnQuadn.m_opacity-dnQuad.m_opacity)/(d1+d)*d1;
            float opacityLast=dnQuadn.m_opacity;
            for(int j=0;j<nQuadMakeUp;j++){
                Cquad&quad=quadList_makeup[j];
                quad.m_opacity=opacityFirst+(opacityLast-opacityFirst)/nQuadMakeUp*j;
            }
            //add quads in quadList_makeup to dnQuadList_makeUp
            for(int j=0;j<nQuadMakeUp;j++){
                dnQuadList_makeup.push_back(quadList_makeup[j]);
                insertIndexList_dn.push_back(i);
            }
            

        }
        
        
        if(turnType==eTurnType_left&&isSharp){
            //----need interpolate between upQuad and upQuadn
            vector<Cquad> quadList_makeup;
            CCPoint L1=upQuad.m_posRU-upQuad.m_posRD;
            CCPoint L2=upQuadn.m_posLU-upQuadn.m_posLD;
            vector<CCPoint> LList;//L1,...,L2
            LList.push_back(L1);
            const float dA=-30;
            float A=0;
            while (1) {
                A+=dA;
                CCPoint L=CCPoint(L1.x*cosf(A*M_PI/180)-L1.y*sinf(A*M_PI/180),
                                  L1.x*sinf(A*M_PI/180)+L1.y*cosf(A*M_PI/180));
                if(A>=-180//if fabs(A) is less than 180, we must continue rotation
                   ||ccpCross(L, L2)<0){//L not over L2
                    LList.push_back(L);
                }else{//L is over L2, stop
                    break;
                }
            }
            LList.push_back(L2);
            //generate triangle quads basing on LList
            int nL=(int)LList.size();
            for(int j=0;j<nL-1;j++){
                CCPoint L=LList[j];
                CCPoint Ln=LList[j+1];
                CCPoint P=upQuad.m_posRD;
                Cquad quad;
                quad.m_posLU=P+L;
                quad.m_posLD=P;
                quad.m_posRU=P+Ln;
                quad.m_posRD=P;
                quad.m_opacity=upQuad.m_opacity+(upQuadn.m_opacity-upQuad.m_opacity)*((j+1)/nL);
                quad.m_isTriangle=true;
                quad.m_opacityLD=(upQuad.m_opacity+upQuadn.m_opacity)/2;
                quad.m_opacityRD=(upQuad.m_opacity+upQuadn.m_opacity)/2;
                quadList_makeup.push_back(quad);
            }//got quadList_makeup
            //calculate up segLen of each quad in quadList
            vector<float> lenList;
            int nQuadMakeUp=(int)quadList_makeup.size();
            for(int j=0;j<nQuadMakeUp;j++){
                const Cquad&quad=quadList_makeup[j];
                float len=ccpLength(quad.m_posLU-quad.m_posRU);
                lenList.push_back(len);
            }//got lenList
            //calculate sum of lens in lenList
            float d=0;
            int nLen=lenList.size();
            for(int j=0;j<nLen;j++){
                d+=lenList[j];
            }//got L
            float d1=ccpLength(upQuad.m_posLD-upQuad.m_posRD)/2;
            float d2=ccpLength(upQuadn.m_posLD-upQuadn.m_posRD)/2;
            //           sa   _sa  _sb  sb
            //   \---*---\-- /---*---/
            //    \          d1  \  / d2           /
            //     \-------\/-------/
            float sa=(upQuad.m_texCoordLD.x+upQuad.m_texCoordRD.x)/2;
            float sb=(upQuadn.m_texCoordLD.x+upQuadn.m_texCoordRD.x)/2;
            float _sa=sa+(sb-sa)/(d1+d+d2)*d1;
            float _sb=sa+(sb-sa)/(d1+d+d2)*(d1+d);
            //the texCoord s of up edges of quads in qudList_makeup is range from _sa to _sb
            //the texCoord s of down edges of quads in quadList_makeup is all equals to upQuad.m_texCoordRD.x
            for(int j=0;j<nQuadMakeUp;j++){
                Cquad&quad=quadList_makeup[j];
                quad.m_texCoordLU=CCPoint(_sa+j*(_sb-_sa)/nQuadMakeUp,0);
                quad.m_texCoordRU=CCPoint(_sa+(j+1)*(_sb-_sa)/nQuadMakeUp,0);
                quad.m_texCoordLD=CCPoint(upQuad.m_texCoordRD.x,0.5);
                quad.m_texCoordRD=CCPoint(upQuad.m_texCoordRD.x,0.5);
                
            }
            //the opacity of quads in quadList_makeup is range from opacityFirst to opacityLast
            float opacityFirst=upQuad.m_opacity+(upQuadn.m_opacity-upQuad.m_opacity)/(d1+d)*d1;
            float opacityLast=upQuadn.m_opacity;
            for(int j=0;j<nQuadMakeUp;j++){
                Cquad&quad=quadList_makeup[j];
                quad.m_opacity=opacityFirst+(opacityLast-opacityFirst)/nQuadMakeUp*j;
            }
            //add quads in quadList_makeup to upQuadList_makeUp
            for(int j=0;j<nQuadMakeUp;j++){
                upQuadList_makeup.push_back(quadList_makeup[j]);
                insertIndexList_up.push_back(i);
            }

        
        
        
        }
        
        
    }
    //make allQuadMat
    vector<vector<Cquad> >&allQuadMat=m_allQuadMat;
    allQuadMat.clear();
    {
        allQuadMat.resize(2);
        vector<Cquad>&allUpQuadList=allQuadMat[0];
        vector<Cquad>&allDnQuadList=allQuadMat[1];
        //allUpQuadList=upQuadList+upQuadList_makeup+insertIndexList_up
        //allDnQuadList=dnQuadList+dnQuadList_makeup+insertIndexList_dn
        //----allUpQuadList
        {
            int i_insertIndexList=0;
            int nUpQuad=(int)upQuadList.size();
            for(int i=0;i<nUpQuad;i++){
                const Cquad&quad=upQuadList[i];
                allUpQuadList.push_back(quad);
                while(i_insertIndexList<(int)insertIndexList_up.size()&&insertIndexList_up[i_insertIndexList]<i){
                    i_insertIndexList++;
                    if(i_insertIndexList>=(int)insertIndexList_up.size())break;
                }//now insertIndexList_up[i_insertIndexList]>=i or i_insertIndexList overbound
                if(i_insertIndexList<(int)insertIndexList_up.size()&&insertIndexList_up[i_insertIndexList]==i){
                    while(i_insertIndexList<(int)insertIndexList_up.size()&&insertIndexList_up[i_insertIndexList]==i){
                        int iUpQuad_makeUp=i_insertIndexList;
                        allUpQuadList.push_back(upQuadList_makeup[iUpQuad_makeUp]);
                        i_insertIndexList++;
                        if(i_insertIndexList>=(int)insertIndexList_up.size())break;
                    }//now insertIndexList_up[i_insertIndexList]>i or i_insertIndexList overbound
                }else if(i_insertIndexList<(int)insertIndexList_up.size()&&insertIndexList_up[i_insertIndexList]>i){
                    //do nothing
                }//now insertIndexList_up[i_insertIndexList]>i or i_insertIndexList overbound
            }//got allUpQuadList
        }
        //----allDnQuadList
        {
            int i_insertIndexList=0;
            int nDnQuad=(int)dnQuadList.size();
            for(int i=0;i<nDnQuad;i++){
                const Cquad&quad=dnQuadList[i];
                allDnQuadList.push_back(quad);
                while(i_insertIndexList<(int)insertIndexList_dn.size()&&insertIndexList_dn[i_insertIndexList]<i){
                    i_insertIndexList++;
                    if(i_insertIndexList>=(int)insertIndexList_dn.size())break;
                }//now insertIndexList_dn[i_insertIndexList]>=i or i_insertIndexList overbound
                if(i_insertIndexList<(int)insertIndexList_dn.size()&&insertIndexList_dn[i_insertIndexList]==i){
                    while(i_insertIndexList<(int)insertIndexList_dn.size()&&insertIndexList_dn[i_insertIndexList]==i){
                        int iDnQuad_makeUp=i_insertIndexList;
                        allDnQuadList.push_back(dnQuadList_makeup[iDnQuad_makeUp]);
                        i_insertIndexList++;
                        if(i_insertIndexList>=(int)insertIndexList_dn.size())break;
                    }//now insertIndexList_dn[i_insertIndexList]>i or i_insertIndexList overbound
                }else if(i_insertIndexList<(int)insertIndexList_dn.size()&&insertIndexList_dn[i_insertIndexList]>i){
                    //do nothing
                }//now insertIndexList_dn[i_insertIndexList]>i or i_insertIndexList overbound
            }//got allDnQuadList
        
        }
        
    }
    //calculate opacityLU,opacityLD,opacityRU,opacityRD for each quad in allQuadMat
    {
        int nRow=(int)allQuadMat.size();
        for(int i=0;i<nRow;i++){
            vector<Cquad>&quadList=allQuadMat[i];
            int nQuad=(int)quadList.size();
            if(nQuad==0){
                //do nothing
            }else if(nQuad==1){
                Cquad&quad=quadList[0];
                if(quad.m_opacityLU==INFINITY)
                    quad.m_opacityLU=quad.m_opacity;
                if(quad.m_opacityLD==INFINITY)
                    quad.m_opacityLD=quad.m_opacity;
                if(quad.m_opacityRU==INFINITY)
                    quad.m_opacityRU=quad.m_opacity;
                if(quad.m_opacityRD==INFINITY)
                    quad.m_opacityRD=quad.m_opacity;
            }else{//nQuad>=2
                for(int j=0;j<nQuad;j++){
                    Cquad&quad=quadList[j];
                    if(j==nQuad-1){//last
                        if(quad.m_opacityLU==INFINITY)
                            quad.m_opacityLU=quad.m_opacity;
                        if(quad.m_opacityLD==INFINITY)
                            quad.m_opacityLD=quad.m_opacity;
                        if(quad.m_opacityRU==INFINITY)
                            quad.m_opacityRU=quad.m_opacity;
                        if(quad.m_opacityRD==INFINITY)
                            quad.m_opacityRD=quad.m_opacity;
                    }else{
                        const Cquad&quadn=quadList[j+1];
                        if(quad.m_opacityLU==INFINITY)
                            quad.m_opacityLU=quad.m_opacity;
                        if(quad.m_opacityLD==INFINITY)
                            quad.m_opacityLD=quad.m_opacity;
                        if(quad.m_opacityRU==INFINITY)
                            quad.m_opacityRU=quadn.m_opacity;
                        if(quad.m_opacityRD==INFINITY)
                            quad.m_opacityRD=quadn.m_opacity;
                    }
                    
                }

            }
        }
    }
    //convert allQuadMat to mesh
    vector<Cv2> vlist;
    vector<CIDTriangle> IDtriList;
    vector<Cv2> texCoordList;
    vector<Cv4> colorList;
    int nRow=(int)allQuadMat.size();
    for(int i=0;i<nRow;i++){
        //i==0:upQuad, i==1:dnQuad
        const vector<Cquad>&quadList=allQuadMat[i];
        int nQuad=(int)quadList.size();
        for(int j=0;j<nQuad;j++){
            const Cquad&quad=quadList[j];
            //whether quad is self-intersect
            float PND1=PNDisPointToLine(quad.m_posRU, quad.m_posLU, quad.m_posLD);
            float PND2=PNDisPointToLine(quad.m_posRD, quad.m_posLU, quad.m_posLD);
            if(PND1*PND2<0&&quad.m_isTriangle==false){//self-intersect
                //    LU --- RU
                //       |/
                //       * P
                //      /|
                //  RD --- LD
                //split the quad to triangle (P,RU,LU) and triangle (P,RD,LD)
                //calculate P and texCoordP and opacityP
                CCPoint P,texCoordP;
                float opacityP;
                {
                    P=getIntersectPointOfTwoLine(quad.m_posLU,quad.m_posLD-quad.m_posLU,
                                                 quad.m_posRU,quad.m_posRD-quad.m_posRU);
                    float dis_PtoUp=fabsf(PNDisPointToLine(P, quad.m_posLU, quad.m_posRU));
                    float dis_PtoDn=fabsf(PNDisPointToLine(P, quad.m_posRD, quad.m_posLD));
                    texCoordP=CCPoint((quad.m_texCoordLU.x+quad.m_texCoordRU.x)/2,quad.m_texCoordLU.y+0.5*dis_PtoUp/(dis_PtoUp+dis_PtoDn));
                    opacityP=quad.m_opacity;
                }
                
                //convert to mesh
                vlist.push_back(Cv2(quad.m_posLU.x,quad.m_posLU.y));
                texCoordList.push_back(Cv2(quad.m_texCoordLU.x,quad.m_texCoordLU.y));
                colorList.push_back(Cv4(1,1,1,quad.m_opacityLU));
                int vID_LU=(int)vlist.size()-1;
                vlist.push_back(Cv2(quad.m_posLD.x,quad.m_posLD.y));
                texCoordList.push_back(Cv2(quad.m_texCoordLD.x,quad.m_texCoordLD.y));
                colorList.push_back(Cv4(1,1,1,quad.m_opacityLD));
                int vID_LD=(int)vlist.size()-1;
                vlist.push_back(Cv2(quad.m_posRD.x,quad.m_posRD.y));
                texCoordList.push_back(Cv2(quad.m_texCoordRD.x,quad.m_texCoordRD.y));
                colorList.push_back(Cv4(1,1,1,quad.m_opacityRD));
                int vID_RD=(int)vlist.size()-1;
                vlist.push_back(Cv2(quad.m_posRU.x,quad.m_posRU.y));
                texCoordList.push_back(Cv2(quad.m_texCoordRU.x,quad.m_texCoordRU.y));
                colorList.push_back(Cv4(1,1,1,quad.m_opacityRU));
                int vID_RU=(int)vlist.size()-1;
                vlist.push_back(Cv2(P.x,P.y));
                texCoordList.push_back(Cv2(texCoordP.x,texCoordP.y));
                colorList.push_back(Cv4(1,1,1,opacityP));
                int vID_P=(int)vlist.size()-1;
                //make IDtri
                if(i==1)
                {//the quad is dnQuad
                    //only want the up triangle
                    IDtriList.push_back(CIDTriangle(vID_P,vID_RU,vID_LU));
                }
                if(i==0)
                {//the quad is upQuad
                    //only want the dn triangle
                    IDtriList.push_back(CIDTriangle(vID_P,vID_RD,vID_LD));
                }
                
            }else{//not self-intersect
                //convert to mesh
                vlist.push_back(Cv2(quad.m_posLU.x,quad.m_posLU.y));
                texCoordList.push_back(Cv2(quad.m_texCoordLU.x,quad.m_texCoordLU.y));
                colorList.push_back(Cv4(1,1,1,quad.m_opacityLU));
                int vID_LU=(int)vlist.size()-1;
                vlist.push_back(Cv2(quad.m_posLD.x,quad.m_posLD.y));
                texCoordList.push_back(Cv2(quad.m_texCoordLD.x,quad.m_texCoordLD.y));
                colorList.push_back(Cv4(1,1,1,quad.m_opacityLD));
                int vID_LD=(int)vlist.size()-1;
                vlist.push_back(Cv2(quad.m_posRD.x,quad.m_posRD.y));
                texCoordList.push_back(Cv2(quad.m_texCoordRD.x,quad.m_texCoordRD.y));
                colorList.push_back(Cv4(1,1,1,quad.m_opacityRD));
                int vID_RD=(int)vlist.size()-1;
                vlist.push_back(Cv2(quad.m_posRU.x,quad.m_posRU.y));
                texCoordList.push_back(Cv2(quad.m_texCoordRU.x,quad.m_texCoordRU.y));
                colorList.push_back(Cv4(1,1,1,quad.m_opacityRU));
                int vID_RU=(int)vlist.size()-1;
                //make IDtri
                IDtriList.push_back(CIDTriangle(vID_LU,vID_LD,vID_RD));
                IDtriList.push_back(CIDTriangle(vID_RD,vID_RU,vID_LU));
            }
 
        }
    }
    //fill mesh
    mesh->vlist=vlist;
    mesh->IDtriList=IDtriList;
    mesh->texCoordList=texCoordList;
    mesh->colorList=colorList;
  
    //
    return mesh;
}
namespace_ens_end

