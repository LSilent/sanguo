#ifndef __GAMESCENE_EFFECTMGR_H__
#define __GAMESCENE_EFFECTMGR_H__

#include "cocos2d.h"
#include "BaseDef.h"

USING_NS_CC;

class EffectMgr
{
public:
    static EffectMgr& getSingleton();

	void init(Node* personode);
	void release();

	//! ��ʱ
	void onIdle(int ot);

	//! ���һ����
	void addArrow(float bx, float by, float ex, float ey, int movetime, int delay);
	//! �ͷ����еļ�
	void releaseAllArrow();

protected:
	struct _ArrowData{
		Sprite*			sprite;
		int				delay;		//! �ӳ�ʱ��

		float			bx;			//! ���λ��
		float			by;
		float			ex;			//! �յ�λ��
		float			ey;
		float			px1;		//! ���Ƶ�1
		float			py1;
		float			px2;		//! ���Ƶ�2
		float			py2;

		int				curtime;	//! ������ʱ��
		int				alltime;	//! ��ʱ��

		//! ������ʱ���������������ɷ���true
		bool onIdle(int ot);
		//! �ͷ�
		void release();

		//! ����Ƕ�
		float getRotate(float px, float py, float x, float y);
	};

	EffectMgr();
    virtual ~EffectMgr();

	//! ��ʱ���
	//! ������ʱ
	void onIlde_Arrow(int ot);

protected:
	Node*			m_pPersonNode;		//! ����ڵ�


	std::list<_ArrowData>		m_lstArrow;			//! ����Ч��
};

#endif	// __GAMESCENE_EFFECTMGR_H__