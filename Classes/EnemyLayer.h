
#pragma once
#include "cocos2d.h"
#include "Enemy.h"
#include "progress.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

enum Level
{
	EASY,
	MIDDLE,
	HARD,
};

class EnemyLayer : public Layer
{
public:
	EnemyLayer(void);
	~EnemyLayer(void);
	static EnemyLayer* create();
	//CREATE_FUNC(EnemyLayer);
	virtual bool init();

	void addEnemy1(float dt);
	void enemy1MoveFinished(Node* pSender);
	void enemy1Blowup(Enemy* enemy1);
	void removeEnemy1(Node* pTarget);
	void removeAllEnemy1();

	void addEnemy2(float dt);
	void enemy2MoveFinished(Node* pSender);
	void enemy2Blowup(Enemy* enemy2);
	void removeEnemy2(Node* pTarget);
	void removeAllEnemy2();

	void addEnemy3(float dt);
	void enemy3MoveFinished(Node* pSender);
	void enemy3Blowup(Enemy* enemy3);
	void removeEnemy3(Node* pTarget);
	void removeAllEnemy3();
	void removeAllEnemy();
	void setLife(Enemy* enemy3);

	void stopss();
	void stop();
	void addEnemy4(float dt);

	void addEnemy5(float dt);
	
	void addEnemy6(float dt);

	void addEnemy7(float dt);
	void addEnemy8(float dt);

	void addBronedEnemy1(float);
	void StartBorn();
	void StopBorn();

	__Array* m_pAllCoin;
	__Array* m_pAllEnemy1;
	__Array* m_pAllEnemy2;
	__Array* m_pAllEnemy3;
	__Array* m_pAllBlod;
	
	static EnemyLayer* sharedEnemy;

private:
	
	ProgressView * m_pProgressView;
	SpriteFrame* enemy1SpriteFrame;
	SpriteFrame* enemy2SpriteFrame;
	SpriteFrame* enemy3SpriteFrame_1;
	SpriteFrame* enemy3SpriteFrame_2;


};
