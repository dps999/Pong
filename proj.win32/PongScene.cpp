#include "PongScene.h"
#include "Ball.h"
#include "Paddle.h"
#include "VisibleRect.h"
#include "Block.h"



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
	CCArray *gids = iniGIDS();
	
	CCTMXTiledMap *map = CCTMXTiledMap::create("level1.tmx");
///	this->addChild(map);


	CCTMXObjectGroup* group = map->objectGroupNamed("back");

	CCAssert(group != NULL, "'Objects' object group not found");
	CCArray* objects = group->getObjects();

	CCDictionary* dict = NULL;
	CCObject* m_Obj = NULL;

	CCARRAY_FOREACH(objects, m_Obj)
	{
		dict = (CCDictionary*)m_Obj;

		if(!dict)
			break;

		const char* name = ((CCString*)dict->objectForKey("gid"))->getCString();
		const char* key = "x";
		int x = ((CCString*)dict->objectForKey(key))->intValue();
		key = "y";
		int y = ((CCString*)dict->objectForKey(key))->intValue();
		key = "width";
		int x2 = x+((CCString*)dict->objectForKey(key))->intValue();
		key = "height";
		int y2 = y+((CCString*)dict->objectForKey(key))->intValue();         

		printf( "x %i, y %i, x2 %i, y2 %i\n", x, y, x2, y2 );
	}


	//		CCLayerColor *blueSky = CCLayerColor::create( ccc4(100, 100, 250, 255));
	//	this->addChild(blueSky);

	// set the appropriate resource directory for this device
	CCFileUtils::sharedFileUtils()->setResourceDirectory("gfx");

	// load and cache the texture and sprite frames
	auto cacher = CCSpriteFrameCache::sharedSpriteFrameCache();
	cacher->addSpriteFramesWithFile("gfx.plist");
	
	
	Block *someSprite = Block::spriteWithFile("wood.png", cacher, 2, 1);

	someSprite->setPosition(ccp( 100, 100));
	this->addChild(someSprite);


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

CCArray* PongLayer::iniGIDS()
{
	CCArray* a = new CCArray();
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
		a->insertObject(CCString::create( dictInfo->m_sName), dictInfo->m_uFirstGid);

	}
	return a;
}