#include "EBulletLayer.h"

EBulletLayer::EBulletLayer(void)
{
	//mutiBulletsSpriteFrame=NULL;
	//mutiBullesBatchNode = nullptr;
	isA = true;
	m_pAllEBullets = __Array::create();
	m_pAllEBullets->retain();
}

EBulletLayer::~EBulletLayer(void)
{
	m_pAllEBullets->release();
	m_pAllEBullets = nullptr;
}

bool EBulletLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());
		isA = 1;
		//mutiBulletsSpriteFrame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet2.png");

		//auto texture = Director::getInstance()->getTextureCache()->getTextureForKey("shoot.png");
		//mutiBullesBatchNode = SpriteBatchNode::createWithTexture(texture);
		//this->addChild(mutiBullesBatchNode);

		bRet = true;
	} while (0);
	return bRet;
}

void EBulletLayer::StartShoot()
{
	this->schedule(schedule_selector(EBulletLayer::AddEBullets), 5.2f, -1, 0.0f);
}

void EBulletLayer::StopShoot()
{
	this->unschedule(schedule_selector(EBulletLayer::AddEBullets));
}

void EBulletLayer::AddEBullets(float dt)
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("bullet.mp3");
	auto winSize = Director::getInstance()->getWinSize();
	Ref* em;
	CCARRAY_FOREACH(EnemyLayer::sharedEnemy->m_pAllEnemy2, em) {
		auto enemy = (Enemy*)em;
		auto bullet = Sprite::create("boss1_bullet3.png");
		this->m_pAllEBullets->addObject(bullet);
		this->addChild(bullet,12);
		float ex = enemy->getPositionX();
		float ey = enemy->getPositionY();
		float x = PlaneLayer::sharedPlane->getChildByTag(747)->getPosition().x;
		float y = PlaneLayer::sharedPlane->getChildByTag(747)->getPosition().y;
		float a = ex - x;
		float b = ey - y;
		bullet->setPosition(enemy->getPosition().x, enemy->getPosition().y);
		float endX = winSize.width - (a / b)*winSize.height;
		float endY = 0;
		//计算飞行时间
		float flyVelocity = 200;//运行速度，可以自己控制，每秒所走的像素
		float flyLen = sqrt((ex - endX)*(ex - endX) + (ey- endY)*(ey - endY));
		float realFlyDuration = flyLen / flyVelocity;//实际飞行的时间
													 //子弹运行的距离和时间，从飞机处开始运行到屏幕底部
		auto actionMove = MoveTo::create(realFlyDuration, Point(endX, endY));
		//子弹执行完动作后进行函数回调，调用移除子弹函数
		auto actionDone = CallFuncN::create(
			CC_CALLBACK_1(EBulletLayer::eBulletsMoveFinished, this));
		//连续动作
		Sequence* sequence = Sequence::create(actionMove, actionDone, NULL);
		//飞机开始跑动
		bullet->runAction(sequence);
	}
	
}

void EBulletLayer::eBulletsMoveFinished(Node* pSender)
{
	auto mutiBullets = (Sprite*)pSender;
	m_pAllEBullets->removeObject(mutiBullets);
	//this->mutiBullesBatchNode->removeChild(mutiBullets, true);
	this->removeChild(mutiBullets, true);
}

void EBulletLayer::RemoveEBullets(Sprite* mutiBullets)
{
	if (mutiBullets != nullptr)
	{
		this->m_pAllEBullets->removeObject(mutiBullets);
		//this->mutiBullesBatchNode->removeChild(mutiBullets, true);
		this->removeChild(mutiBullets, true);
	}
}
