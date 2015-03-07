

#include "ensShatterNode.h"
namespace_ens_begin
	using namespace shatter;

bool CshatterSprite::init(const string&texFileName){
		this->CCSprite::initWithFile(texFileName.c_str());
		createShatter();
		return true;
	}
	void CshatterSprite::createShatter(){
		//----calculate grid size and fragCount
		CCSize contentSize=this->getContentSize();
		const int nRow=(int)floorf(contentSize.height/m_gridSideLen);//grid row count
		const int nCol=(int)floorf(contentSize.width/m_gridSideLen);//grid col count
		const int fragCount=nRow*nCol;
		//----create fragBatchNode
		m_fragBatchNode=CCSpriteBatchNode::createWithTexture(this->getTexture(),fragCount);
		this->addChild(m_fragBatchNode);
		m_fragBatchNode->setVisible(false);
		//----create frags and add them to fragBatchNode and grid
		//make m_grid
		m_grid.resize(nRow);
		for(int i=0;i<nRow;i++){m_grid[i].resize(nCol);}
		const float halfGridSideLen=0.5*m_gridSideLen;
		for(int i=0;i<nRow;i++){
			for(int j=0;j<nCol;j++){
				Cfrag*frag=new Cfrag();
				frag->autorelease();
				frag->initWithTexture(this->getTexture());
				//set to grid
				m_grid[i][j]=frag;
				//add to batchNode
				m_fragBatchNode->addChild(frag);
				//random 
				frag->m_randomNumber=rand();
			}
		}
	}
	void CshatterSprite::resetShatter(){
		CCSize contentSize=this->getContentSize();
		int nRow=(int)m_grid.size();
		int nCol=(nRow==0?0:(int)m_grid[0].size());
		const float halfGridSideLen=0.5*m_gridSideLen;
		for(int i=0;i<nRow;i++){
			for(int j=0;j<nCol;j++){
				Cfrag*frag=m_grid[i][j];
				//position
				float x=j*m_gridSideLen+halfGridSideLen;
				float y=contentSize.height-(i*m_gridSideLen+halfGridSideLen);
				//texture and textureRect
				frag->setTextureRect(CCRect(x-halfGridSideLen,(contentSize.height-y)-halfGridSideLen,m_gridSideLen,m_gridSideLen));
				//set position
				frag->setPosition(ccp(x,y));
				//scale
				frag->setScale(m_initalFrageScale);
				//opacity
				frag->setOpacity(255);
				//visible
				frag->setVisible(true);
			}
		}
	
	}
	void CshatterSprite::updateShatterAction(float time,float dt,float growSpeedOfTargetR){
		//update frags
		CCSize contentSize=this->getContentSize();
		CCPoint center=CCPoint(contentSize.width/2,contentSize.height/2);
		float initalTargetR=ccpLength(ccp(contentSize.width,contentSize.height))/2;//radius of srounding circle
		int nRow=(int)m_grid.size();
		int nCol=nRow?(int)m_grid[0].size():0;
		for(int i=0;i<nRow;i++){
			for(int j=0;j<nCol;j++){
				Cfrag*frag=m_grid[i][j];
				if(frag->getOpacity()==0||frag->getScale()==0){
					frag->setVisible(false);
					continue;
				}
				//current target R
				float targetR=initalTargetR+time*growSpeedOfTargetR;
				//update postion
				CCPoint fragPos=frag->getPosition();
				float disToCenter=ccpLength(fragPos-center);
				CCPoint dir;
				if(disToCenter==0){
					dir=CCPoint(0,0);
				}else{
					dir=fragPos-center;
					dir.x/=disToCenter;
					dir.y/=disToCenter;
				}
				float disToEdge=targetR-disToCenter;
				float disToEdgeWithRandom=disToEdge+frag->m_randomNumber%(int)initalTargetR-initalTargetR/2;//add random to disToEdge
				float movLen=disToEdgeWithRandom*0.0333;//we only move some percent of disToEdgeWithRandom
				CCPoint movVec=ccpMult(dir,movLen);
				frag->setPosition(fragPos+movVec);
				//update opacity
				float opacity=MAX(0,255-255*disToCenter/initalTargetR);
				frag->setOpacity(opacity);
				//update scale
				frag->setScale(MAX(0,m_initalFrageScale-m_initalFrageScale*disToCenter/initalTargetR));
			}
		}
	}
//-----------------------------------------
//creates the action
CshatterAction* CshatterAction::create(float fDuration){
    
    CshatterAction *p = new CshatterAction();
    p->initWithDuration(fDuration);
    p->autorelease();
    
    return p;
}
//initializes the action
bool CshatterAction::initWithDuration(float fDuration){
    if (CCActionInterval::initWithDuration(fDuration))
    {
        //my init code
        initMembers();

        return true;
    }
    
    return false;
}
bool CshatterAction::isDone(){
    return CCActionInterval::isDone();
    
}


void CshatterAction::stop(){

    //clean up and reset
	((CshatterSprite*)_target)->m_fragBatchNode->setVisible(true);
    //call father stop
    CCActionInterval::stop();
}

void CshatterAction::startWithTarget(CCNode *pTarget){
    //reset
	((CshatterSprite*)pTarget)->resetShatter();
	((CshatterSprite*)pTarget)->m_fragBatchNode->setVisible(true);
    //call father startWithTarget
    CCActionInterval::startWithTarget(pTarget);
    
}
void CshatterAction::update(float time){//the time actually is percentage, namely eclipsedTime/totalDuration
    //the t in action's update(t) and sprite's update(t) is different meaning
    //t in action's update(t) means the progress percentage, see CCActionInterval::step()
    //t in sprite's update(t) means the deltaTime between current frame and previous frame, see CCDirector::drawScene()
    //cout<<"time:"<<time<<endl;
    m_timeFoe=m_timeCur;
    float progressPercentage=time;
    m_timeCur=progressPercentage*getDuration();
    ((CshatterSprite*)_target)->updateShatterAction(m_timeCur,m_timeCur-m_timeFoe,m_growSpeedOfTargetR);
    
}




namespace_ens_end