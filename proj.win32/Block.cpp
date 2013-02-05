#include "Block.h"


Block* Block::spriteWithFile(const char *pszFileName, CCSpriteFrameCache *cacher, int columns, int rows)
{
	CCSpriteFrame *frame = cacher->spriteFrameByName("wood.png");
	CCArray* animFrames = CCArray::createWithCapacity(columns * rows);
	float textureWidth =  frame->getRect().getMaxX() - frame->getRect().getMinX();
	float textureHeight = frame->getRect().getMaxY() - frame->getRect().getMinY();
	float frameWidth = textureWidth / columns;
	float frameHeight = textureHeight / rows;

	for (int c = 1; c < columns; c++)
	{
		for (int r = 1; r < rows; r++)
		{
			CCSpriteFrame *frame0 = CCSpriteFrame::createWithTexture(frame->getTexture(), CCRectMake(frame->getRect().getMinX()+(columns*), frame->getRect().getMinY(), 80, 80));
		}

	}
	CCSpriteFrame *frame0 = CCSpriteFrame::createWithTexture(frame->getTexture(), CCRectMake(frame->getRect().getMinX(), frame->getRect().getMinY(), 80, 80));
	CCSpriteFrame *frame1 = CCSpriteFrame::createWithTexture(frame->getTexture(), CCRectMake(frame->getRect().getMinX()+80, frame->getRect().getMinY(), 80, 80));

		CCArray* animFrames = CCArray::createWithCapacity(2);
		animFrames->addObject(frame0);
		animFrames->addObject(frame1);
		CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.2f);
		CCAnimate *animate = CCAnimate::create(animation);
		CCActionInterval* seq = (CCActionInterval*)(CCSequence::create( animate,   animate->copy()->autorelease(),   CCFlipX::create(false),   NULL) );
    

	CCSprite* someSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)animFrames->objectAtIndex(0));
}
