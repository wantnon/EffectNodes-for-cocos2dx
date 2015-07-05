#include "ensShatterNode.h"
using namespace shatter;

bool ShatterSprite::init(const string&texFileName){
    this->Sprite::initWithFile(texFileName.c_str());
    createShatter();
    return true;
}

void ShatterSprite::createShatter(){
    //----calculate grid size and fragCount
    Size contentSize = this->getContentSize();
    const int nRow = (int)floorf(contentSize.height/m_gridSideLen);//grid row count
    const int nCol = (int)floorf(contentSize.width/m_gridSideLen);//grid col count
    const int fragCount = nRow*nCol;
    //----create fragBatchNode
    m_fragBatchNode = SpriteBatchNode::createWithTexture(this->getTexture(),fragCount);
    this->addChild(m_fragBatchNode);
    m_fragBatchNode->setVisible(false);
    //----create frags and add them to fragBatchNode and grid
    //make m_grid
    m_grid.resize(nRow);
    for(int i = 0; i < nRow; i++){
        m_grid[i].resize(nCol);
    }

    const float halfGridSideLen = 0.5 * m_gridSideLen;

    for (int i = 0; i < nRow; i++){
        for (int j = 0; j < nCol; j++){
            ShatterFrag* frag = new ShatterFrag();
            frag->autorelease();
            frag->initWithTexture(this->getTexture());
            //set to grid
            m_grid[i][j] = frag;
            //add to batchNode
            m_fragBatchNode->addChild(frag);
            //random 
            frag->m_randomNumber = rand();
        }
    }
}

void ShatterSprite::resetShatter(){
    Size contentSize = this->getContentSize();
    int nRow = (int)m_grid.size();
    int nCol = (nRow == 0?0:(int)m_grid[0].size());
    const float halfGridSideLen = 0.5*m_gridSideLen;
    for (int i = 0; i < nRow; i++){
        for(int j = 0; j < nCol; j++){
            ShatterFrag* frag = m_grid[i][j];
            //position
            float x = j * m_gridSideLen+halfGridSideLen;
            float y = contentSize.height - (i * m_gridSideLen+halfGridSideLen);
            //texture and textureRect
            frag->setTextureRect(Rect(
                x-halfGridSideLen,
                (contentSize.height-y) - halfGridSideLen,
                m_gridSideLen,
                m_gridSideLen
            ));

            //set position
            frag->setPosition(Vec2(x,y));
            //scale
            frag->setScale(m_initalFragScale);
            //opacity
            frag->setOpacity(255);
            //visible
            frag->setVisible(true);
        }
    }

}
void ShatterSprite::updateShatterAction(float time,float dt,float growSpeedOfTargetR){
    //update frags
    Size contentSize = this->getContentSize();
    Vec2 center = Vec2(contentSize.width/2, contentSize.height/2);

    //radius of surrounding circle
    float initalTargetR = Vec2(contentSize.width, contentSize.height).getLength()/2;

    int nRow = (int)m_grid.size();
    int nCol = nRow?(int)m_grid[0].size():0;

    for (int i = 0; i < nRow; i++){
        for (int j = 0; j < nCol;j ++){
            ShatterFrag*frag = m_grid[i][j];

            if (frag->getOpacity() == 0 || frag->getScale() == 0){
                frag->setVisible(false);
                continue;
            }

            //current target R
            float targetR = initalTargetR + time*growSpeedOfTargetR;
            //update postion

            Vec2 fragPos = frag->getPosition();
            float disToCenter = Vec2(fragPos-center).getLength();
            Vec2 dir;
            if (disToCenter == 0)
            {
                dir = Vec2(0,0);
            }
            else
            {
                dir = fragPos-center;
                dir.x /= disToCenter;
                dir.y /= disToCenter;
            }

            float disToEdge = targetR-disToCenter;
            float disToEdgeWithRandom = disToEdge + frag->m_randomNumber % (int)initalTargetR - initalTargetR/2;//add random to disToEdge
            float movLen = disToEdgeWithRandom*0.0333;//we only move some percent of disToEdgeWithRandom

            Vec2 movVec = dir * movLen;
            frag->setPosition(fragPos + movVec);
            //update opacity
            float opacity = MAX(0, 255 - 255 * disToCenter / initalTargetR);
            frag->setOpacity(opacity);
            //update scale
            float scale = MAX(
                0,
                m_initalFragScale - m_initalFragScale * disToCenter / initalTargetR
                );
            frag->setScale(scale);
        }
    }
}
//-----------------------------------------
//creates the action
ShatterAction* ShatterAction::create(float duration){

    ShatterAction *p = new ShatterAction();
    p->initWithDuration(duration);
    p->autorelease();

    return p;
}

//initializes the action
bool ShatterAction::initWithDuration(float duration){
    if (ActionInterval::initWithDuration(duration))
    {
        //my init code
        initMembers();
        return true;
    }

    return false;
}

bool ShatterAction::isDone(){
    return ActionInterval::isDone();
}

void ShatterAction::stop(){

    //clean up and reset
    ((ShatterSprite*)_target)->m_fragBatchNode->setVisible(true);
    //call father stop
    ActionInterval::stop();
}

void ShatterAction::startWithTarget(Node *pTarget){
    //reset
    ((ShatterSprite*)pTarget)->resetShatter();
    ((ShatterSprite*)pTarget)->m_fragBatchNode->setVisible(true);
    //call father startWithTarget
    ActionInterval::startWithTarget(pTarget);

}

void ShatterAction::update(float time){
    //the time actually is percentage, namely eclipsedTime/totalDuration
    //the t in action's update(t) and sprite's update(t) is different meaning
    //t in action's update(t) means the progress percentage, see ActionInterval::step()
    //t in sprite's update(t) means the deltaTime between current frame and previous frame, see Director::drawScene()
    //cout<<"time:"<<time<<endl;
    m_timeFoe = m_timeCur;
    float progressPercentage = time;
    m_timeCur = progressPercentage*getDuration();
    ((ShatterSprite*)_target)->updateShatterAction(m_timeCur,m_timeCur-m_timeFoe,m_growSpeedOfTargetR);

}
