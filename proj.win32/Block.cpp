#include "Block.h"


Block* Block::spriteWithFile(const char *pszFileName, CCSpriteFrameCache *cacher, CCSpriteFrameCache *cacher2, int columns, int rows)
{
	
		CCSpriteFrame *frame = cacher->spriteFrameByName( pszFileName);
		if(frame == NULL) frame = cacher2->spriteFrameByName( pszFileName);
		CCArray* aFrames = CCArray::createWithCapacity(columns * rows);


		float textureWidth =  frame->getRect().getMaxX() - frame->getRect().getMinX();
		float textureHeight = frame->getRect().getMaxY() - frame->getRect().getMinY();
		float frameWidth = textureWidth / columns;
		float frameHeight = textureHeight / rows;

		for (int c = 0; c < columns; c++)
		{
			for (int r = 0; r < rows; r++)
			{
				CCSpriteFrame *frame0 = CCSpriteFrame::createWithTexture(frame->getTexture(), CCRectMake(frame->getRect().getMinX()+(c*frameWidth), frame->getRect().getMinY()+(r*frameHeight), 80, 80));
				aFrames->addObject(frame0);
			}

		}

		CCAnimation *animation = CCAnimation::createWithSpriteFrames(aFrames, 0.2f);
		CCAnimate *animate = CCAnimate::create(animation);
		CCActionInterval* seq = (CCActionInterval*)(CCSequence::create( animate,   animate->copy()->autorelease(),   CCFlipX::create(false),   NULL) );
    
		Block *sprite =(Block*) CCSprite::createWithSpriteFrame((CCSpriteFrame*) aFrames->objectAtIndex(0));
	//	sprite->runAction(CCRepeatForever::create( seq ) );
		sprite->setAnimFrames( aFrames);
		sprite->setCurrentFrame(0);

	return sprite;
}

void Block::setAnimFrames(CCArray* a)
{
	animFrames = a;
}

void Block::setCurrentFrame(int i)
{
	this->setDisplayFrame( (CCSpriteFrame*) animFrames->objectAtIndex(i));
}