#include "EnemyLayer.h"
#include "constant.h"
#include "GameLayer.h"
#include "HelloWorldLayer.h"
#include "PlaneLayer.h"
EnemyLayer* EnemyLayer::sharedEnemy = nullptr;
EnemyLayer::EnemyLayer(void)
{
	enemy1SpriteFrame = nullptr;
	enemy2SpriteFrame = nullptr;
	enemy3SpriteFrame_1 = nullptr;
	enemy3SpriteFrame_2 = nullptr;
	m_pProgressView = nullptr;

	m_pAllEnemy1 = __Array::create();
	m_pAllEnemy1->retain();
	m_pAllEnemy2 = __Array::create();
	m_pAllEnemy2->retain();
	m_pAllEnemy3 = __Array::create();
	m_pAllEnemy3->retain();
	m_pAllBlod = __Array::create();
	m_pAllBlod->retain();
}
EnemyLayer* EnemyLayer::create()
{
	EnemyLayer *pRet = new(std::nothrow) EnemyLayer();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		sharedEnemy = pRet;
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}
EnemyLayer::~EnemyLayer(void)
{
	m_pAllEnemy1->release();
	m_pAllEnemy1 = nullptr;
	m_pAllEnemy2->release();
	m_pAllEnemy2 = nullptr;
	m_pAllEnemy3->release();
	m_pAllEnemy3 = nullptr;
	m_pAllBlod->release();
	m_pAllBlod = nullptr;
}

bool EnemyLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());

		enemy1SpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1.png");
		enemy2SpriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2.png");
		enemy3SpriteFrame_1 = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png");
		enemy3SpriteFrame_2 = SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n2.png");

		auto animation1 = Animation::create();
		animation1->setDelayPerUnit(0.1f);
		animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down1.png"));
		animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down2.png"));
		animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down3.png"));
		animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down4.png"));

		auto animation2 = Animation::create();
		animation2->setDelayPerUnit(0.1f);
		animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down1.png"));
		animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down2.png"));
		animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down3.png"));
		animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down4.png"));

		auto animation3 = Animation::create();
		animation3->setDelayPerUnit(0.1f);
		animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down1.png"));
		animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down2.png"));
		animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down3.png"));
		animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down4.png"));
		animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down5.png"));
		animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down6.png"));

		AnimationCache::getInstance()->addAnimation(animation1, "Enemy1Blowup");
		AnimationCache::getInstance()->addAnimation(animation2, "Enemy2Blowup");
		AnimationCache::getInstance()->addAnimation(animation3, "Enemy3Blowup");

		auto aniBigFly = Animation::create();
		aniBigFly->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png"));
		aniBigFly->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n2.png"));
		aniBigFly->setDelayPerUnit(0.1f);
		aniBigFly->setLoops(CC_REPEAT_FOREVER);
		AnimationCache::getInstance()->addAnimation(aniBigFly, "BIGFLY");
		if ((HelloWorldLayer::A) == 0) {
			this->schedule(schedule_selector(EnemyLayer::addEnemy1),1.f,200,0.0);
			this->schedule(schedule_selector(EnemyLayer::addEnemy2),2.0f,100,0.0);
			this->schedule(schedule_selector(EnemyLayer::addEnemy3),12.0f,5,0);
			this->schedule(schedule_selector(EnemyLayer::addEnemy4), 0.5f, 200, 250);
			this->schedule(schedule_selector(EnemyLayer::addEnemy5), 3.f, 30., 250);
			this->schedule(schedule_selector(EnemyLayer::addEnemy6), 1.5f, 30, 350);
			this->schedule(schedule_selector(EnemyLayer::addEnemy7), .5f, 30, 350);
		}
		else if ((HelloWorldLayer::A) == 1) {
			this->schedule(schedule_selector(EnemyLayer::addEnemy4), 0.5f, 200, 0);
			this->schedule(schedule_selector(EnemyLayer::addEnemy5), 3.f, 30., 0);
			this->schedule(schedule_selector(EnemyLayer::addEnemy6), 1.5f, 30, 100);
			this->schedule(schedule_selector(EnemyLayer::addEnemy7), .5f, 30, 100);
		}
		else {
			this->schedule(schedule_selector(EnemyLayer::addEnemy6), 1.5f, 30, 0);
			this->schedule(schedule_selector(EnemyLayer::addEnemy7), .5f, 30, 0);
		}
		

		bRet = true;
	} while (0);
	return bRet;
}


void EnemyLayer::addEnemy1(float dt)
{
	auto enemy1 = Enemy::create();
	enemy1->bindSprite(Sprite::createWithSpriteFrame(enemy1SpriteFrame), ENEMY1_MAXLIFE);

	auto enemy1Size = enemy1->getSprite()->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();
	int minX = enemy1Size.width/2;
	int maxX = winSize.width-enemy1Size.width/2;
	int rangeX = maxX-minX;
	int actualX = (rand()%rangeX)+minX;

	enemy1->setPosition(Point(actualX, winSize.height+enemy1Size.height/2));
	this->addChild(enemy1);
	this->m_pAllEnemy1->addObject(enemy1);

	float minDuration, maxDuration;
	switch(GameLayer::getCurLevel())
	{
	case EASY:
		minDuration = 2.0f;
		maxDuration = 6.0f;
		break;
	case MIDDLE:
		minDuration = 1.8f;
		maxDuration = 5.6f;
		break;
	case HARD:
		minDuration = 1.6f;
		maxDuration = 5.2f;
		break;
	default:
		minDuration = 2.0f;
		maxDuration = 6.0f;
		break;
	}

	int rangeDuration = maxDuration-minDuration;
	int actualDuration = (rand()%rangeDuration)+minDuration;

	auto actionMove = MoveTo::create(actualDuration, Point(actualX, 0-enemy1->getSprite()->getContentSize().height/2));
	auto actionDone = CallFuncN::create(CC_CALLBACK_1(EnemyLayer::enemy1MoveFinished, this));

	auto sequence = Sequence::create(actionMove, actionDone, nullptr);
	enemy1->runAction(sequence);

}

void EnemyLayer::enemy1MoveFinished(Node* pSender)
{
	auto enmey1 = (Enemy*)pSender;
	this->removeChild(enmey1, true);
	this->m_pAllEnemy1->removeObject(enmey1);
}

void EnemyLayer::enemy1Blowup(Enemy* enemy1)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("enemy1_down.mp3");
	auto animation = AnimationCache::getInstance()->getAnimation("Enemy1Blowup");
	auto animate = Animate::create(animation);
	auto removeEnemy1 = CallFunc::create(CC_CALLBACK_0(EnemyLayer::removeEnemy1, this, enemy1));
	auto sequence = Sequence::create(animate, removeEnemy1, nullptr);
	enemy1->getSprite()->runAction(sequence);
}

void EnemyLayer::removeEnemy1(Node* pTarget)
{
	auto enemy1 = (Enemy*)pTarget;
	if (enemy1 != nullptr)
	{
		m_pAllEnemy1->removeObject(enemy1);
		this->removeChild(enemy1, true);
	}
}

void EnemyLayer::removeAllEnemy1()
{
	Ref* obj;
	CCARRAY_FOREACH(m_pAllEnemy1, obj)
	{
		auto enemy1 = (Enemy*)obj;
		if (enemy1->getLife() > 0)
		{
			enemy1Blowup(enemy1);
		}
	}
}


void EnemyLayer::addEnemy2(float dt)
{
	auto enemy2 = Enemy::create();
	enemy2->bindSprite(Sprite::createWithSpriteFrame(enemy2SpriteFrame), ENEMY2_MAXLIFE);

	auto enemy2Size = enemy2->getSprite()->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();
	int minX = enemy2Size.width/2;
	int maxX = winSize.width-enemy2Size.width/2;
	int rangeX = maxX-minX;
	int actualX = (rand()%rangeX)+minX;

	enemy2->setPosition(Point(actualX,winSize.height+enemy2Size.height/2));
	this->addChild(enemy2);
	this->m_pAllEnemy2->addObject(enemy2);

	float minDuration, maxDuration;
	switch(GameLayer::getCurLevel())
	{
	case EASY:
		minDuration = 4.0;
		maxDuration = 10.0;
		break;
	case MIDDLE:
		minDuration = 3.7;
		maxDuration = 9.4;
		break;
	case HARD:
		minDuration = 3.5;
		maxDuration = 9.0;
		break;
	default:
		minDuration = 3.0;
		maxDuration = 6.0;
		break;
	}
	int rangeDuration = maxDuration-minDuration;
	int actualDuration = (rand()%rangeDuration)+minDuration;

	auto actionMove = MoveTo::create(actualDuration,Point(actualX, 0-enemy2->getSprite()->getContentSize().height/2));
	auto actionDone = CallFuncN::create(CC_CALLBACK_1(EnemyLayer::enemy2MoveFinished, this));

	auto sequence = Sequence::create(actionMove, actionDone, nullptr);
	enemy2->runAction(sequence);
}

void EnemyLayer::enemy2MoveFinished(Node* pSender)
{
	auto enmey2 = (Enemy*)pSender;
	this->removeChild(enmey2, true);
	this->m_pAllEnemy2->removeObject(enmey2);
}

void EnemyLayer::enemy2Blowup(Enemy* enemy2)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("enemy2_down.mp3");
	auto animation = AnimationCache::getInstance()->getAnimation("Enemy2Blowup");
	auto animate = Animate::create(animation);

	auto removeEnemy2 = CallFunc::create(CC_CALLBACK_0(EnemyLayer::removeEnemy2, this, enemy2));
	auto sequence = Sequence::create(animate, removeEnemy2, nullptr);
	enemy2->getSprite()->runAction(sequence);
}

void EnemyLayer::removeEnemy2(Node* pTarget)
{
	auto enemy2 = (Enemy*)pTarget;
	if (enemy2 != nullptr)
	{
		m_pAllEnemy2->removeObject(enemy2);
		this->removeChild(enemy2, true);
	}
}

void EnemyLayer::removeAllEnemy2()
{
	Ref* obj;
	CCARRAY_FOREACH(m_pAllEnemy2,obj)
	{
		auto enemy2 = (Enemy*)obj;
		if (enemy2->getLife()>0)
		{
			enemy2Blowup(enemy2);
		}
	}
}


void EnemyLayer::addEnemy3(float dt)
{
	auto enemy3 = Enemy::create();
	enemy3->bindSprite(Sprite::createWithSpriteFrame(enemy3SpriteFrame_1), ENEMY3_MAXLIFE);
	auto enemy3Size = enemy3->getSprite()->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();
	int minX = enemy3Size.width / 2;
	int maxX = winSize.width - enemy3Size.width / 2;
	int rangeX = maxX - minX;
	int actualX = (rand() % rangeX) + minX;

	enemy3->setPosition(Point(actualX, winSize.height + enemy3Size.height / 2));
	m_pProgressView = new ProgressView;
	m_pProgressView->setPosition(Point(actualX, winSize.height + enemy3Size.height));
	m_pProgressView->setScale(2.2f);
	m_pProgressView->setBackgroundTexture("gary.png");
	m_pProgressView->setForegroundTexture("red.png");
	m_pProgressView->setTotalProgress(20.0f);
	m_pProgressView->setCurrentProgress(20.f);
	this->addChild(m_pProgressView, 2);
	this->addChild(enemy3);
	this->m_pAllEnemy3->addObject(enemy3);
	this->m_pAllBlod->addObject(m_pProgressView);

	float minDuration, maxDuration;
	switch (GameLayer::getCurLevel())
	{
	case EASY:
		minDuration = 5.0;
		maxDuration = 14.0;
		break;
	case MIDDLE:
		minDuration = 4.6;
		maxDuration = 13.2;
		break;
	case HARD:
		minDuration = 4.2;
		maxDuration = 12.4;
		break;
	default:
		minDuration = 4.0;
		maxDuration = 8.0;
		break;
	}

	int rangeDuration = maxDuration - minDuration;
	int actualDuration = (rand() % rangeDuration) + minDuration;

	auto actionMove1 = MoveTo::create(actualDuration, Point(actualX, 0-6));
	auto actionDone1 = CallFuncN::create([=](Ref *) {

		this->removeChild(enemy3, true);
		this->m_pAllEnemy3->removeObject(enemy3);
	}
	);
	auto actionMove = MoveTo::create(actualDuration, Point(actualX, 0 - enemy3->getSprite()->getContentSize().height / 2-6));
	auto actionDone = CallFuncN::create([=](Ref *) {
		
		this->removeChild(enemy3, true);
		this->m_pAllEnemy3->removeObject(enemy3);
}
);

	auto sequence = Sequence::create(actionMove, actionDone, nullptr);
	auto sequence1 = Sequence::create(actionMove1, actionDone1, nullptr);
	enemy3->runAction(sequence);
	m_pProgressView->runAction(sequence1);
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.2f);
	animation->addSpriteFrame(enemy3SpriteFrame_1);
	animation->addSpriteFrame(enemy3SpriteFrame_2);
	auto animate = Animate::create(animation);
	enemy3->getSprite()->runAction(RepeatForever::create(animate));
}

void EnemyLayer::enemy3MoveFinished(Node* pSender)
{
	auto enmey3 = (Enemy*)pSender;
	this->removeChild(enmey3, true);
	this->m_pAllEnemy3->removeObject(enmey3);
}

void EnemyLayer::enemy3Blowup(Enemy* enemy3)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("enemy3_down.mp3");
    auto animation = AnimationCache::getInstance()->getAnimation("Enemy3Blowup");

	auto animate = Animate::create(animation);
	auto removeEnemy3 = CallFunc::create(CC_CALLBACK_0(EnemyLayer::removeEnemy3, this, enemy3));
	auto sequence = Sequence::create(animate, removeEnemy3, nullptr);
	enemy3->getSprite()->runAction(sequence);
}
void EnemyLayer::setLife(Enemy* enemy3) {
	this->m_pProgressView->setTotalProgress(20.0f);
	m_pProgressView->setCurrentProgress(enemy3->getLife());
}

void EnemyLayer::removeEnemy3(Node* pTarget)
{
	auto enemy3 = (Enemy*)pTarget;
	if (enemy3 != nullptr)
	{
		enemy3->stopAllActions();
		m_pAllEnemy3->removeObject(enemy3);
		this->removeChild(enemy3, true);
	}
	auto mm = m_pProgressView;
	if (mm != nullptr)
	{
		mm->stopAllActions();
		m_pAllBlod->removeObject(mm);
		this->removeChild(mm, true);
	}
}

void EnemyLayer::removeAllEnemy3()
{
	Ref* obj;
	CCARRAY_FOREACH(m_pAllEnemy3, obj)
	{
		auto enemy3 = (Enemy*)obj;
		if (enemy3->getLife()>0)
		{
			enemy3Blowup(enemy3);
		}
	}
}

void EnemyLayer::removeAllEnemy()
{
	removeAllEnemy1();
	removeAllEnemy2();
	removeAllEnemy3();
	/*removeAllEnemy5();
	removeAllEnemy6();
	removeAllEnemy7();*/
}


void EnemyLayer::addEnemy4(float dt)
{
	auto enemy2 = Enemy::create();
	enemy2->bindSprite(Sprite::createWithSpriteFrame(enemy2SpriteFrame), ENEMY2_MAXLIFE/2+2);
	enemy2->setRotation(-90);
	auto enemy2Size = enemy2->getSprite()->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();

	enemy2->setPosition(Point(0, 550));
	enemy2->setScale(0.75);
	this->addChild(enemy2);
	this->m_pAllEnemy2->addObject(enemy2);

	float minDuration, maxDuration;
	switch (GameLayer::getCurLevel())
	{
	case EASY:
		minDuration = 4.0;
		maxDuration = 10.0;
		break;
	case MIDDLE:
		minDuration = 3.7;
		maxDuration = 9.4;
		break;
	case HARD:
		minDuration = 3.5;
		maxDuration = 9.0;
		break;
	default:
		minDuration = 3.0;
		maxDuration = 6.0;
		break;
	}
	int rangeDuration = maxDuration - minDuration;
	int actualDuration = (rand() % rangeDuration) + minDuration;

	ccBezierConfig tr0;
	tr0.endPosition = Vec2(0, 100);//终点
	tr0.controlPoint_1 = Vec2(500, 400);//控制点1
	tr0.controlPoint_2 = Vec2(500, 250);//控制点2

	ActionInterval* bezierForward = BezierTo::create(4.f, tr0);//创建运行的贝塞尔曲线
	ActionInterval *forwardBy = RotateBy::create(3.f, 180);     // 第二个参数：如果是正数则是顺时针，否则逆时针 
	Spawn* spawn = Spawn::create(bezierForward, forwardBy, NULL);//创建合成动作

																 //飞机执行完动作后进行函数回调，调用移除飞机函数
	auto actionDone = CallFuncN::create(CC_CALLBACK_1(EnemyLayer::enemy2MoveFinished, this));

	//连续动作
	Sequence* sequence = Sequence::create(spawn, actionDone, NULL);
	enemy2->runAction(sequence);
}

void EnemyLayer::addEnemy5(float dt) {
	auto winSize = Director::getInstance()->getWinSize();
	float minDuration, maxDuration;
	switch (GameLayer::getCurLevel())
	{
	case EASY:
		minDuration = 4.0;
		maxDuration = 10.0;
		break;
	case MIDDLE:
		minDuration = 3.7;
		maxDuration = 9.4;
		break;
	case HARD:
		minDuration = 3.5;
		maxDuration = 9.0;
		break;
	default:
		minDuration = 3.0;
		maxDuration = 6.0;
		break;
	}
	int rangeDuration = maxDuration - minDuration;
	int actualDuration = (rand() % rangeDuration) + minDuration;

	auto spritePlane1 = Enemy::create();
	spritePlane1->bindSprite(Sprite::createWithSpriteFrame(enemy2SpriteFrame), ENEMY2_MAXLIFE );
	auto spritePlane2 = Enemy::create();
	spritePlane2->bindSprite(Sprite::createWithSpriteFrame(enemy2SpriteFrame), ENEMY2_MAXLIFE );
	auto spritePlane3 = Enemy::create();
	spritePlane3->bindSprite(Sprite::createWithSpriteFrame(enemy2SpriteFrame), ENEMY2_MAXLIFE);

	//得到精灵宽和高
	//旋转的角度
	//spritePlane1->setRotation(40);
	//spritePlane2->setRotation(180);
	//spritePlane3->setRotation(180);

	//设置缩放
	//spritePlane1->setScale(0.3);
	//spritePlane2->setScale(0.3);
	//spritePlane3->setScale(0.3);

	//设置位置
	spritePlane1->setPosition(Vec2(100, winSize.height +200));
	spritePlane2->setPosition(Vec2(winSize.width / 2-10, winSize.height +100));
	spritePlane3->setPosition(Vec2(winSize.width - 100, winSize.height + 200));


	//层中加入精灵
	this->addChild(spritePlane1);
	this->addChild(spritePlane2);
	this->addChild(spritePlane3);
	this->m_pAllEnemy2->addObject(spritePlane1);
	this->m_pAllEnemy2->addObject(spritePlane2);
	this->m_pAllEnemy2->addObject(spritePlane3);

	//计算飞行时间
	float flyVelocity = 200;//运行速度，可以自己控制，每秒所走的像素
	float flyLen = winSize.height;
	float realFlyDuration = flyLen / flyVelocity;//实际飞行的时间

												 //子弹运行的距离和时间，从飞机处开始运行到屏幕底部
	auto actionMove1 = MoveBy::create(realFlyDuration, Point(0, -winSize.height - 200));
	auto actionMove2 = MoveBy::create(realFlyDuration, Point(0, -winSize.height - 200));
	auto actionMove3 = MoveBy::create(realFlyDuration, Point(0, -winSize.height - 200));

	//子弹执行完动作后进行函数回调，调用移除子弹函数
	auto actionDone = CallFuncN::create(
		CC_CALLBACK_1(EnemyLayer::enemy2MoveFinished, this));

	//连续动作
	Sequence* sequence1 = Sequence::create(actionMove1, actionDone, NULL);
	Sequence* sequence2 = Sequence::create(actionMove2, actionDone, NULL);
	Sequence* sequence3 = Sequence::create(actionMove3, actionDone, NULL);

	//飞机开始跑动
	spritePlane1->runAction(sequence1);
	spritePlane2->runAction(sequence2);
	spritePlane3->runAction(sequence3);
}

void EnemyLayer::addEnemy6(float dt) {
	auto winSize = Director::getInstance()->getWinSize();

	auto spritePlane= Enemy::create();
	spritePlane->bindSprite(Sprite::createWithSpriteFrame(enemy2SpriteFrame), ENEMY2_MAXLIFE/2);
	//得到精灵宽和高
	float height = spritePlane->getContentSize().height;
	float width = spritePlane->getContentSize().width;
	//设置敌机位于右上角

	spritePlane->setPosition(Vec2(winSize.width + width / 2, winSize.height + height / 2));
	this->addChild(spritePlane);
	this->m_pAllEnemy2->addObject(spritePlane);
	//计算英雄飞机和对角点连起的线与边界的角度
	float x = PlaneLayer::sharedPlane->getChildByTag(747)->getPosition().x;
	float y = PlaneLayer::sharedPlane->getChildByTag(747)->getPosition().y;
	float a = winSize.width - x;
	float b = winSize.height - y;
	// 弧度转角度
	float radians = atanf(a / b);
	float mDegree = CC_RADIANS_TO_DEGREES(radians);
	spritePlane->setRotation(180 + mDegree);
	//计算敌机的最终位置
	float endX = winSize.width - (a / b)*winSize.height;
	float endY = 0;
	//计算飞行时间
	float flyVelocity = 200;//运行速度，可以自己控制，每秒所走的像素
	float flyLen = sqrt((winSize.width - endX)*(winSize.width - endX) + (winSize.height - endY)*(winSize.height - endY));
	float realFlyDuration = flyLen / flyVelocity;//实际飞行的时间
												 //子弹运行的距离和时间，从飞机处开始运行到屏幕底部
	auto actionMove = MoveTo::create(realFlyDuration, Point(endX, endY));
	//子弹执行完动作后进行函数回调，调用移除子弹函数
	auto actionDone = CallFuncN::create(
		CC_CALLBACK_1(EnemyLayer::enemy2MoveFinished, this));
	//连续动作
	Sequence* sequence = Sequence::create(actionMove, actionDone, NULL);
	//飞机开始跑动
	spritePlane->runAction(sequence);

}
void EnemyLayer::stopss() {
	this->unschedule(schedule_selector(EnemyLayer::addEnemy6));
}

void EnemyLayer::addEnemy7(float dt) {
	Size winSize = Director::getInstance()->getWinSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	//生成左边敌机
	auto spritePlane1 = Enemy::create();
	spritePlane1->bindSprite(Sprite::createWithSpriteFrame(enemy2SpriteFrame), ENEMY2_MAXLIFE);
	auto spritePlane2 = Enemy::create();
	spritePlane2->bindSprite(Sprite::createWithSpriteFrame(enemy2SpriteFrame), ENEMY2_MAXLIFE);
	
	//生成边敌机
	auto spritePlane4 = Enemy::create();
	spritePlane4->bindSprite(Sprite::createWithSpriteFrame(enemy2SpriteFrame), ENEMY2_MAXLIFE);
	

	//旋转的角度
	spritePlane1->setRotation(90);
	spritePlane2->setRotation(90);
	

	spritePlane4->setRotation(-90);
	

	//设置缩放
	//spritePlane1->setScale(0.3);
	//spritePlane2->setScale(0.3);
	//   spritePlane3->setScale(0.3);


	//得到精灵宽和高
	auto height = spritePlane1->getSprite()->getContentSize().height;
	//log("%d", spritePlane1->getContentSize().height);
	float width = spritePlane1->getSprite()->getContentSize().width;

	//放置敌机位置
	spritePlane1->setPosition(Vec2(-width / 2, winSize.height - height / 2 - 10));
	spritePlane2->setPosition(Vec2(-width / 2, spritePlane1->getPosition().y - 2 * height - 10));
	

	spritePlane4->setPosition(Vec2(winSize.width + width / 2, spritePlane1->getPosition().y - height - 10));
	


	//层中加入精灵
	this->addChild(spritePlane1);
	this->addChild(spritePlane2);
	
	this->addChild(spritePlane4);
	
	this->m_pAllEnemy2->addObject(spritePlane1);
	this->m_pAllEnemy2->addObject(spritePlane2);
	
	this->m_pAllEnemy2->addObject(spritePlane4);
	


	//计算飞行时间
	float flyVelocity = 200;//运行速度，可以自己控制，每秒所走的像素
	float flyLen = winSize.width + width;
	float realFlyDuration = flyLen / flyVelocity;//实际飞行的时间


												 //子弹运行的距离和时间，从飞机处开始运行到屏幕底部
	auto actionMove1 = MoveBy::create(realFlyDuration, Point(flyLen, 0));
	auto actionMove2 = MoveBy::create(realFlyDuration, Point(-flyLen, 0));

	//子弹执行完动作后进行函数回调，调用移除子弹函数
	auto actionDone = CallFuncN::create(
		CC_CALLBACK_1(EnemyLayer::enemy2MoveFinished, this));
	//连续动作
	Sequence* sequence1 = Sequence::create(actionMove1, actionDone, NULL);
	Sequence* sequence2 = Sequence::create(actionMove1->clone(), actionDone, NULL);
	
	Sequence* sequence4 = Sequence::create(actionMove2, actionDone, NULL);
	
	//飞机开始跑动
	spritePlane1->runAction(sequence1);
	spritePlane2->runAction(sequence2);
	
	spritePlane4->runAction(sequence4);
	


}

void EnemyLayer::stop() {
	for (auto node : this->getChildren())
		node->stopAllActions();
	this->unschedule(schedule_selector(EnemyLayer::addEnemy1));
	this->unschedule(schedule_selector(EnemyLayer::addEnemy2));
	this->unschedule(schedule_selector(EnemyLayer::addEnemy3));
	this->unschedule(schedule_selector(EnemyLayer::addEnemy4));
	this->unschedule(schedule_selector(EnemyLayer::addEnemy5));
	this->unschedule(schedule_selector(EnemyLayer::addEnemy6));
	this->unschedule(schedule_selector(EnemyLayer::addEnemy7));
	this->unschedule(schedule_selector(EnemyLayer::addBronedEnemy1));
}


void EnemyLayer::addBronedEnemy1(float)
{
	auto winSize = Director::getInstance()->getWinSize();

	Ref* em;
	CCARRAY_FOREACH(EnemyLayer::sharedEnemy->m_pAllEnemy3, em) {
		auto bigEnemy = (Enemy*)em;
		auto smallEnemy1 = Enemy::create();
		smallEnemy1->bindSprite(Sprite::createWithSpriteFrame(enemy1SpriteFrame), ENEMY1_MAXLIFE);
		auto smallEnemy2 = Enemy::create();
		smallEnemy2->bindSprite(Sprite::createWithSpriteFrame(enemy1SpriteFrame), ENEMY1_MAXLIFE);
		/*auto smallEnemy3 = Enemy::create();
		smallEnemy3->bindSprite(Sprite::createWithSpriteFrame(enemy1SpriteFrame), ENEMY1_MAXLIFE);
		auto smallEnemy4 = Enemy::create();
		smallEnemy4->bindSprite(Sprite::createWithSpriteFrame(enemy1SpriteFrame), ENEMY1_MAXLIFE);*/
		this->m_pAllEnemy1->addObject(smallEnemy1);
		this->m_pAllEnemy1->addObject(smallEnemy2);
		//this->m_pAllEnemy1->addObject(smallEnemy3);
		//this->m_pAllEnemy1->addObject(smallEnemy4);
		this->addChild(smallEnemy1, 13);
		//this->addChild(smallEnemy3, 13);
		//this->addChild(smallEnemy4, 13);
		float ex = bigEnemy->getPositionX();
		float ey = bigEnemy->getPositionY();
		smallEnemy1->setPosition(bigEnemy->getPosition().x - 50, bigEnemy->getPosition().y - 50);
		smallEnemy2->setPosition(bigEnemy->getPosition().x + 50, bigEnemy->getPosition().y - 50);
		//smallEnemy3->setPosition(bigEnemy->getPosition().x - 50, bigEnemy->getPosition().y + 50);
		//smallEnemy4->setPosition(bigEnemy->getPosition().x + 50, bigEnemy->getPosition().y + 50);
		this->addChild(smallEnemy2, 13);
		float endX1 = bigEnemy->getPositionX() - 50;
		float endX2 = bigEnemy->getPositionX() + 50;
		float endY = 0;
		//计算飞行时间
		float flyVelocity = 300;//运行速度，可以自己控制，每秒所走的像素
		float flyLen = sqrt((ex - endX2)*(ex - endX2) + (ey - endY)*(ey - endY));
		float realFlyDuration = flyLen / flyVelocity;//实际飞行的时间
													 //子弹运行的距离和时间，从飞机处开始运行到屏幕底部
		auto actionMove1 = MoveTo::create(realFlyDuration, Point(endX1, endY));
		auto actionMove2 = MoveTo::create(realFlyDuration, Point(endX2, endY));
		//子弹执行完动作后进行函数回调，调用移除子弹函数
		auto actionDone = CallFuncN::create(
			CC_CALLBACK_1(EnemyLayer::enemy1MoveFinished, this));
		//连续动作
		Sequence* sequence1 = Sequence::create(actionMove1, actionDone, NULL);
		Sequence* sequence3 = Sequence::create(actionMove1, actionDone, NULL);
		Sequence* sequence2 = Sequence::create(actionMove2, actionDone, NULL);
		Sequence* sequence4 = Sequence::create(actionMove2, actionDone, NULL);
		//smallEnemy2->setColor(Color3B::BLUE);
		smallEnemy2->runAction(sequence2);
		smallEnemy1->runAction(sequence1);
		//smallEnemy3->runAction(sequence3);
		//smallEnemy4->runAction(sequence4);
	}
}

void EnemyLayer::StartBorn()
{
	this->schedule(schedule_selector(EnemyLayer::addBronedEnemy1), 1.2f, -1, 0.0f);
}

void EnemyLayer::StopBorn()
{
	this->unschedule(schedule_selector(EnemyLayer::addBronedEnemy1));
}
