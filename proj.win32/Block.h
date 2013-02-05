#pragma once
#include "cocos2d.h"
USING_NS_CC;

class Block :	public cocos2d::CCSprite
{
public:
	Block* spriteWithFile(const char *pszFileName, CCSpriteFrameCache *cacher, int columns, int rows);
};

