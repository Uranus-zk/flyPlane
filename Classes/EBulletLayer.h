#pragma once
#include "cocos2d.h"
#include "Enemy.h"
#include "EnemyLayer.h"
#include "SimpleAudioEngine.h"
#include "PlaneLayer.h"
USING_NS_CC;
class EBulletLayer : public Layer
{
public:
	EBulletLayer(void);

	~EBulletLayer(void);

	CREATE_FUNC(EBulletLayer);

	virtual bool init();

	void AddEBullets(float dt);

	void eBulletsMoveFinished(Node* pSender);

	void RemoveEBullets(Sprite* mutiBullets);

	void StartShoot();

	void StopShoot();
	bool getisA() {
		return isA;
	};
	void setisA(bool new1) {
		isA = new1;
	}

public:
	bool isA;

	__Array * m_pAllEBullets;
};
