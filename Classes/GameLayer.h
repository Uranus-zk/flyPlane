#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__



#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "PlaneLayer.h"
#include "BulletLayer.h"
#include "EnemyLayer.h"
#include "ControlLayer.h"
#include "MutiBulletsLayer.h"
#include "ThreeBulletsLayer.h"
#include "UFOLayer.h"
#include "EBulletLayer.h"
USING_NS_CC;

class GameLayer:public Layer {
public:
	GameLayer();
	~GameLayer();
	static Scene* createScene();
	CREATE_FUNC(GameLayer);
	bool init();
	void backgroundMove(float dt);
	void update(float);
	static Level getCurLevel();
	void updateBigBoomItem(int bigBoomCount);
	void menuBigBoomCallback(Ref* pSender);
	void updateLife(int lifeCount);
	void randomBomb(float);
	void aimedMove(Sprite * bullet);
private:
	Sprite * background1;
	Sprite* background2;
	PlaneLayer* planeLayer;
	BulletLayer* bulletLayer;
	EBulletLayer* ebulletLayer;
	MutiBulletsLayer* mutiBulletsLayer;
	ThreeBulletsLayer* threeBulletsLayer;
	ControlLayer* controlLayer;
	UFOLayer* ufoLayer;
	EnemyLayer* enemyLayer;
	Sprite* PRcoin;
	__Array* m_pAllCoin;
	Menu* menuBigBoom;
	Label* bigBoomCountItem;
	Vec2 m_offSet;
	static Level level;
	int score;
	int coinSC;
	bool coinPR;
	bool god;
	int bigBoomCount;

};
#endif // !__GAME_LAYER_H__