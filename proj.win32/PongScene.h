#pragma once
#ifndef _TOUCHES_TEST__H_
#define _TOUCHES_TEST__H_

#include "cocos2d.h"
#include "Ball.h"

USING_NS_CC;

class PongScene : public cocos2d::CCScene
{
public:
    PongScene();

    virtual void runThisTest();

    virtual void MainMenuCallback(cocos2d::CCObject* pSender);
};


class PongLayer : public cocos2d::CCLayer
{
private:
    Ball*       m_ball;
    cocos2d::CCArray*    m_paddles;
    cocos2d::CCPoint     m_ballStartingVelocity; 
public:
    PongLayer();
    ~PongLayer();
	CCArray* iniGIDS();
    void resetAndScoreBallForPlayer(int player);
    void doStep(float delta);
};

#endif
