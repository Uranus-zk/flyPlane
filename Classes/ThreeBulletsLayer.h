#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class ThreeBulletsLayer : public Layer
{
public:
	ThreeBulletsLayer(void);

	~ThreeBulletsLayer(void);

	CREATE_FUNC(ThreeBulletsLayer);

	virtual bool init();

	void AddThreeBullets(float dt);

	void threeBulletsMoveFinished(Node* pSender);

	void RemoveThreeBullets(Sprite* mutiBullets);

	void StartShoot();

	void StopShoot();

public:

	__Array * m_pAllThreeBullets;

	//CCSpriteFrame* mutiBulletsSpriteFrame;

	//	SpriteBatchNode* mutiBullesBatchNode;
};
