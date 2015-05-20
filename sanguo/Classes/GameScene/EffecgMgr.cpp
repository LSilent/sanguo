#include "EffectMgr.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
//! _ArrowData
static inline float bezierat( float a, float b, float c, float d, float t )
{
    return (powf(1-t,3) * a + 
            3*t*(powf(1-t,2))*b + 
            3*powf(t,2)*(1-t)*c +
            powf(t,3)*d );
}

static inline float sineEaseInOut(float time)
{
	if(time <= 0.5f)
		return (-2 * time * time + 2 * time) * 0.3f + time * 0.7f;
	else
		return (2 * time * time - 2 * time + 1) * 0.3f + time * 0.7f;
}

//! ������ʱ���������������ɷ���true
bool EffectMgr::_ArrowData::onIdle(int ot)
{
	if(delay > 0)
	{
		if(delay >= ot)
		{
			delay -= ot;
			sprite->setVisible(false);
			return false;
		}
		else
		{
			ot -= delay;
			delay = 0;
		}
	}

	sprite->setVisible(true);
	curtime += ot;

	if(curtime >= alltime)
		return true;

	//! ���ߵ���֮���ʱ��
	int chgtime = sineEaseInOut((float)curtime / alltime) * alltime;

	float xa = 0;
    float xb = px1;
    float xc = px2;
    float xd = ex - bx;

    float ya = 0;
    float yb = py1;
    float yc = py2;
    float yd = ey - by;

	float x = bezierat(xa, xb, xc, xd, (float)chgtime / alltime);
	float y = bezierat(ya, yb, yc, yd, (float)chgtime / alltime);

	float px = sprite->getPositionX();
	float py = sprite->getPositionY();

	sprite->setPosition(bx + x, by + y);
	sprite->setLocalZOrder(_GAMESCENE_Z_PERSONEFF - (by + y));

	float rotate = getRotate(px, py, bx + x, by + y);
	sprite->setRotation(rotate);

	return false;
}

//! �ͷ�
void EffectMgr::_ArrowData::release()
{
	if(sprite != NULL)
	{
		if(sprite->getParent() != NULL)
			sprite->getParent()->removeChild(sprite);

		sprite->release();
		sprite = NULL;
	}
}

//! ����Ƕ�
float EffectMgr::_ArrowData::getRotate(float px, float py, float x, float y)
{
	double sx = x - px;
	double sy = y - py;

	if(sx == 0 && sy == 0)
		return true;

	float degree = 0;

	if(sy == 0 && sx > 0)
		degree = 90;
	else if(sy == 0 && sx < 0)
		degree = 270;
	else if(sx == 0 && sy > 0)
		degree = 0;
	else if(sx == 0 && sy < 0)
		degree = 180;
	else
	{
		double radians = atanf(sx / sy);
		degree = CC_RADIANS_TO_DEGREES(radians);

		if(sx < 0 && sy < 0)
			degree += 180;
		else if(sx > 0 && sy < 0)
			degree += 180;
	}

	if(degree < 0)
		degree += 360;

	if(degree >= 360)
		degree -= 360;

	return degree;
}

//////////////////////////////////////////////////////////////////////////
//! EffectMgr
EffectMgr::EffectMgr()
	: m_pPersonNode(NULL)
{	
}

EffectMgr::~EffectMgr()
{
}

EffectMgr& EffectMgr::getSingleton()
{
    static EffectMgr s_mgr;
    
    return s_mgr;
}

void EffectMgr::init(Node* personode)
{
	m_pPersonNode = personode;
}

void EffectMgr::release()
{
	releaseAllArrow();
}

//! ��ʱ
void EffectMgr::onIdle(int ot)
{
	onIlde_Arrow(ot);
}

//! ���һ����
void EffectMgr::addArrow(float bx, float by, float ex, float ey, int movetime, int delay)
{
	if(m_pPersonNode == NULL)
		return ;

	_ArrowData data;

	SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("arrow.png");

	if(frame == NULL)
		return ;

	data.sprite = Sprite::create();
	data.sprite->retain();
	data.sprite->setPosition(bx, by);
	data.sprite->setSpriteFrame(frame);

	m_pPersonNode->addChild(data.sprite, _GAMESCENE_Z_PERSONEFF - by);

	//! �������
	data.bx = bx;
	data.by = by;
	data.ex = ex;
	data.ey = ey;

	data.px1 = (ex - bx) / 2.0f;
	data.py1 = MAX(by, ey) + 150 + (rand() % 70 - 35) - by;
	data.px2 = data.px1;
	data.py2 = data.py1;

	data.alltime = movetime;
	data.curtime = 0;

	data.delay = delay;
	data.onIdle(0);

	m_lstArrow.push_back(data);
}

//! �ͷ����еļ�
void EffectMgr::releaseAllArrow()
{
	for(std::list<_ArrowData>::iterator it = m_lstArrow.begin(); it != m_lstArrow.end(); ++it)
	{
		it->release();
	}

	m_lstArrow.clear();
}

//! ������ʱ
void EffectMgr::onIlde_Arrow(int ot)
{
	for(std::list<_ArrowData>::iterator it = m_lstArrow.begin(); it != m_lstArrow.end(); )
	{
		if(it->onIdle(ot))
		{
			it->release();
			it = m_lstArrow.erase(it);
		}
		else
			++it;
	}
}