
#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class UFOLayer : public Layer
{
public:

	UFOLayer(void);
	
	~UFOLayer(void);

	virtual bool init();

	CREATE_FUNC(UFOLayer);

	void AddMutiBullets(float dt);

	void mutiBulletsMoveFinished(Node* pSender);
	void AddThreeBullets(float dt);

	void threeBulletsMoveFinished(Node* pSender);
	void AddBigBoom(float dt);
	void AddLife(float dt);
	void lifeMoveFinished(Node* pSender);
	void bigBoomMoveFinished(Node* pSender);
	void RemoveLife(Sprite* life);
	void RemoveMutiBullets(Sprite* mutiBullets);
	void RemoveThreeBullets(Sprite* mutiBullets);
	void RemoveBigBoom(Sprite* bigBoom);
	void AddHouse1(float dt);
	void houseMoveFinished(Node* pSender);
	void AddCloud(float dt);
	void cloudMoveFinished(Node* pSender);

	void updatenow(float dt);

public:
	__Array * m_pAllThreeBullets;
	__Array * m_pAllLife;
	__Array* m_pAllCloud;

	__Array* m_pAllMutiBullets;
	__Array* m_pAllHouse;

	__Array* m_pAllBigBoom;

};


