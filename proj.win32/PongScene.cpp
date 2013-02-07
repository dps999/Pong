#include "PongScene.h"
#include "Ball.h"
#include "Paddle.h"
#include "VisibleRect.h"
#include "Block.h"
#include <vector>

typedef std::pair< unsigned int, char* >		td_Pair;	// key / value
typedef std::vector< td_Pair  >		td_Vector;
//typedef td_Vector::iterator ItemIterator;
td_Vector tilesetVector;

enum tagPlayer 
{
    kHighPlayer,
    kLowPlayer
} PlayerTouches;    

#define kStatusBarHeight 0.0f //20.0f
//#define k1UpperLimit (480.0f - kStatusBarHeight)

enum 
{
    kSpriteTag
};


//------------------------------------------------------------------
//
// PongScene
//
//------------------------------------------------------------------
PongScene::PongScene()
{
    PongLayer *pongLayer = new PongLayer();//PongLayer::create();
    addChild(pongLayer);
    pongLayer->release();
}

void PongScene::MainMenuCallback(CCObject* pSender)
{
  //  TestScene::MainMenuCallback(pSender);
}

//------------------------------------------------------------------
//
// PongLayer
//
//------------------------------------------------------------------
PongLayer::PongLayer()
{


	//		CCLayerColor *blueSky = CCLayerColor::create( ccc4(100, 100, 250, 255));
	//	this->addChild(blueSky);

	// set the appropriate resource directory for this device
	CCFileUtils::sharedFileUtils()->setResourceDirectory("gfx");

	
	loadLevel();
	

	
	//Block *someSprite = Block::spriteWithFile("wood.png", cacher, 2, 1);

//	someSprite->setPosition(ccp( 100, 100));
//	this->addChild(someSprite);


    m_ballStartingVelocity = ccp(20.0f, -100.0f);
    
    m_ball = Ball::ballWithTexture( CCTextureCache::sharedTextureCache()->addImage("Ball.png") );
    m_ball->setPosition( VisibleRect::center() );
    m_ball->setVelocity( m_ballStartingVelocity );
    addChild( m_ball );
    m_ball->retain();
    
    CCTexture2D* paddleTexture = CCTextureCache::sharedTextureCache()->addImage("Paddle.png");
    
    CCArray *paddlesM = CCArray::createWithCapacity(4);
    
    Paddle* paddle = Paddle::paddleWithTexture(paddleTexture);
    paddle->setPosition( ccp(VisibleRect::center().x, VisibleRect::bottom().y + 15) );
    paddlesM->addObject( paddle );
    
  /*  paddle = Paddle::paddleWithTexture( paddleTexture );
    paddle->setPosition( ccp(VisibleRect::center().x, VisibleRect::top().y - kStatusBarHeight - 15) );
    paddlesM->addObject( paddle );
    
    paddle = Paddle::paddleWithTexture( paddleTexture );
    paddle->setPosition( ccp(VisibleRect::center().x, VisibleRect::bottom().y + 100) );
    paddlesM->addObject( paddle );
    
    paddle = Paddle::paddleWithTexture( paddleTexture );
    paddle->setPosition( ccp(VisibleRect::center().x, VisibleRect::top().y - kStatusBarHeight - 100) );
    paddlesM->addObject( paddle );*/
    
    m_paddles = (CCArray*)paddlesM->copy();
    
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(m_paddles, pObj)
    {
        paddle = (Paddle*)(pObj);

        if(!paddle)
            break;

        addChild(paddle);
    }

    schedule( schedule_selector(PongLayer::doStep) );
}

PongLayer::~PongLayer()
{
    m_ball->release();
    m_paddles->release();
}

void PongLayer::resetAndScoreBallForPlayer(int player)
{
    m_ballStartingVelocity = ccpMult(m_ballStartingVelocity, -1.1f);
    m_ball->setVelocity( m_ballStartingVelocity );
    m_ball->setPosition( VisibleRect::center() );
    
    // TODO -- scoring
}

void PongLayer::doStep(float delta)
{
    m_ball->move(delta*2);

    Paddle* paddle = NULL;
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(m_paddles, pObj)
    {
        paddle = (Paddle*)(pObj);

        if(!paddle)
            break;

        m_ball->collideWithPaddle( paddle );
    }

    //if (m_ball->getPosition().y > VisibleRect::top().y - kStatusBarHeight + m_ball->radius())
    //   // resetAndScoreBallForPlayer( kLowPlayer );
    //else if (m_ball->getPosition().y < VisibleRect::bottom().y-m_ball->radius())
    //   // resetAndScoreBallForPlayer( kHighPlayer );
    m_ball->draw();
} 

void PongScene::runThisTest()
{
    CCDirector::sharedDirector()->replaceScene(this);
}

void PongLayer::loadLevel()
{
	
	CCArray* a = new CCArray();

	tilesetVector.clear();
	CCTMXMapInfo *mapInfo = new CCTMXMapInfo();
	mapInfo->initWithTMXFile("level1.tmx");
	CCArray *gids = mapInfo->getTilesets();
	CCTMXTilesetInfo* dictInfo = NULL;
	CCObject* m_ObjInfo = NULL;

	CCARRAY_FOREACH(gids, m_ObjInfo)
	{
		dictInfo = (CCTMXTilesetInfo*)m_ObjInfo;

		if(!dictInfo)
			break;
		char *file = strcat((char*)dictInfo->m_sName.c_str(), ".png");
		tilesetVector.push_back( td_Pair(dictInfo->m_uFirstGid, file));
	//	a->insertObject(CCString::create( dictInfo->m_sName), dictInfo->m_uFirstGid);

	}

	// load and cache the texture and sprite frames
	auto cacher = CCSpriteFrameCache::sharedSpriteFrameCache();
	cacher->addSpriteFramesWithFile("gfx.plist");

	// load and cache the texture and sprite frames
	auto cacher2 = CCSpriteFrameCache::sharedSpriteFrameCache();
	cacher2->addSpriteFramesWithFile("gfx2.plist");

	CCTMXTiledMap *map = CCTMXTiledMap::create("level1.tmx");

	CCTMXObjectGroup* group = map->objectGroupNamed("objects");

	CCAssert(group != NULL, "'Objects' object group not found");
	CCArray* objects = group->getObjects();

	CCDictionary* dict = NULL;
	CCObject* m_Obj = NULL;

	CCARRAY_FOREACH(objects, m_Obj)
	{
		dict = (CCDictionary*)m_Obj;

		if(!dict)
			break;

		int gid = ((CCString*)dict->objectForKey("gid"))->intValue();
		int x = ((CCString*)dict->objectForKey("x"))->intValue();
		int y = ((CCString*)dict->objectForKey("y"))->intValue();
		//td_Vector::iterator pIter = std::find(tilesetVector.begin(), tilesetVector.end(), gid);
		td_Vector::iterator pIter = tilesetVector.begin();
		while (pIter != tilesetVector.end()) {
			if(gid == pIter->first)
			{
				char *cc = pIter->second;
				if(strcmp(cc, "star_gb")>0) 
				{
					Block *someSprite = Block::spriteWithFile( "star.png", cacher, cacher2, 1, 1);
					someSprite->setPosition(ccp( x, y));
					this->addChild(someSprite);
					break;
				}
				//else if(strcmp(cc, "double_gb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "explosive_gb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "fire_ball_gb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "magnet_gb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "guard_gb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "gun_gb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "increase_platform_gb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "next_gb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "slow_ball_gb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "stone_ball_gb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "black_hole_bb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "broken_heart_bb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "down_bb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "mini_ball_bb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "poison_bb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "randome_bb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "reduction_bb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "speed_ball_bb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "boom_gb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "heart_gb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}
				//else if(strcmp(cc, "star_gb")>-1) 
				//{
				//	Block *someSprite = Block::spriteWithFile( cc, cacher, cacher2, 1, 1);
				//	someSprite->setPosition(ccp( x, y));
				//	this->addChild(someSprite);
				//	break;
				//}

			}
			++pIter;
		}
	}

}