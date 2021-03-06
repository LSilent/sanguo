﻿#ifndef __GAMESCENE_BASEOBJ_H__
#define __GAMESCENE_BASEOBJ_H__

#include "cocos2d.h"

USING_NS_CC;

class BaseObj
{
public:
    virtual void setPosition(float xx, float yy) = 0;
	virtual float getPositionX() = 0;
	virtual float getPositionY() = 0;
    
    virtual void onIdle(int ot) = 0;
    
    virtual void release() = 0;
protected:
	BaseObj();
    virtual ~BaseObj();
protected:
};

#endif // __GAMESCENE_BASEOBJ_H__