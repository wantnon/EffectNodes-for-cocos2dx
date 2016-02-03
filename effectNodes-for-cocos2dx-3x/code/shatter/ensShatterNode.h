#ifndef HelloWorld_ensShatterNode_h
#define HelloWorld_ensShatterNode_h

#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;

namespace shatter
{
    class ShatterFrag : public Sprite
    {
    public:
        int m_randomNumber;
        ShatterFrag() { m_randomNumber = -1; }
        bool initWithTexture(Texture2D* texture)
        {
            this->CCSprite::initWithTexture(texture);
            return true;
        }
    };
}
namespace shatter{
	class ShatterAction;
}

class ShatterSprite:public Sprite
{
public:
    friend class shatter::ShatterAction;

    ShatterSprite(){
        m_gridSideLen = 2.7f;
        m_initalFragScale = 4.5f;
        m_fragBatchNode = NULL;
    }
    virtual ~ShatterSprite(){}
    bool init(const string&texFileName);

protected:
    void createShatter();
    void resetShatter();
    void updateShatterAction(float time, float dt, float growSpeedOfTargetR);

    float m_gridSideLen;//grid side length, also frag size
    float m_initalFragScale;//we want to make frags a litte bigger at start time.
    vector<vector<shatter::ShatterFrag*> > m_grid;//hold all the frags
    SpriteBatchNode* m_fragBatchNode;//all frags add to this batchNode
};

namespace shatter
{

    class ShatterAction : public ActionInterval
    {
    public:
        //creates the action
        static ShatterAction* create(float duration);
        //initializes the action
        bool initWithDuration(float duration);

        virtual void startWithTarget(Node *pTarget);
        virtual void update(float time);
        virtual void stop();
        virtual bool isDone();

        ShatterAction(){ initMembers(); }
        virtual~ShatterAction(){ }

    protected:
        void initMembers(){
            m_timeFoe = 0;
            m_timeCur = 0;

            m_growSpeedOfTargetR = 50;
        }

        float m_timeFoe;
        float m_timeCur;

        float m_growSpeedOfTargetR;

    };
}
#endif
