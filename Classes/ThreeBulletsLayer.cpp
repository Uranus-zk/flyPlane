#include "ThreeBulletsLayer.h"
#include "PlaneLayer.h"

ThreeBulletsLayer::ThreeBulletsLayer(void)
{
	//mutiBulletsSpriteFrame=NULL;
	//mutiBullesBatchNode = nullptr;

	m_pAllThreeBullets = __Array::create();
	m_pAllThreeBullets->retain();
}

ThreeBulletsLayer::~ThreeBulletsLayer(void)
{
	m_pAllThreeBullets->release();
	m_pAllThreeBullets = nullptr;
}

bool ThreeBulletsLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CCLayer::init());

		//mutiBulletsSpriteFrame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("bullet2.png");

		//auto texture = Director::getInstance()->getTextureCache()->getTextureForKey("shoot.png");
		//mutiBullesBatchNode = SpriteBatchNode::createWithTexture(texture);
		//this->addChild(mutiBullesBatchNode);

		bRet = true;
	} while (0);
	return bRet;
}

void ThreeBulletsLayer::StartShoot()
{
	this->schedule(schedule_selector(ThreeBulletsLayer::AddThreeBullets), 0.2f, 30, 0.0f);
}

void ThreeBulletsLayer::StopShoot()
{
	this->unschedule(schedule_selector(ThreeBulletsLayer::AddThreeBullets));
}

void ThreeBulletsLayer::AddThreeBullets(float dt)
{
	auto winSize = Director::getInstance()->getWinSize();
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("bullet.mp3");

	auto bulletLeft = Sprite::createWithSpriteFrameName("bullet2.png");
	auto bulletRight = Sprite::createWithSpriteFrameName("bullet2.png");
	auto bullet= Sprite::createWithSpriteFrameName("bullet2.png");
	bulletLeft->setRotation(-45);
	bulletRight->setRotation(45);
	this->addChild(bulletLeft);
	this->addChild(bulletRight);
	this->addChild(bullet);
	this->m_pAllThreeBullets->addObject(bullet);
	this->m_pAllThreeBullets->addObject(bulletLeft);
	this->m_pAllThreeBullets->addObject(bulletRight);

	auto planePosition = PlaneLayer::sharedPlane->getChildByTag(747)->getPosition();
	auto bulletLeftPosition = Point(planePosition.x - PlaneLayer::sharedPlane->getChildByTag(747)->getContentSize().width / 4 - 10, planePosition.y + 35);
	auto bulletRightPosition = Point(planePosition.x +PlaneLayer::sharedPlane->getChildByTag(747)->getContentSize().width / 4 + 10, planePosition.y + 35);
	auto bulletPosition = Point(planePosition.x, planePosition.y + 45);
	bulletLeft->setPosition(bulletLeftPosition);
	bulletRight->setPosition(bulletRightPosition);
	bullet->setPosition(bulletPosition);

	float length = Director::getInstance()->getWinSize().height + bulletLeft->getContentSize().height / 2 - bulletPosition.y;
	float length1 = planePosition.x / cos(45);//°´ÕÕ¶ÈíËã
	float length2 = (winSize.width - planePosition.x) / cos(45);

	float velocity = 420 / 1;//420pixel/sec
	float realMoveDuration = length / velocity;
	float realMoveDuration1 = length1 / velocity;
	float realMoveDuration2 = length2 / velocity;
	auto actionMove = MoveTo::create(realMoveDuration, Point(bulletPosition.x, Director::getInstance()->getWinSize().height + bulletLeft->getContentSize().height / 2));
	auto actionDone = CallFuncN::create(CC_CALLBACK_1(ThreeBulletsLayer::threeBulletsMoveFinished, this));
	auto sequence = Sequence::create(actionMove, actionDone, nullptr);
	auto actionLeftMove = MoveTo::create(realMoveDuration1, Point(0-bulletLeft->getContentSize().height / 2, planePosition.x*tan(45) + planePosition.y));
	auto actionLeftDone = CallFuncN::create(CC_CALLBACK_1(ThreeBulletsLayer::threeBulletsMoveFinished, this));
	auto sequenceLeft = Sequence::create(actionLeftMove, actionLeftDone, nullptr);

	auto actionRightMove = MoveTo::create(realMoveDuration2, Point(winSize.width, (winSize.width - planePosition.x)*tan(45) + planePosition.y));
	auto actionRightDone = CallFuncN::create(CC_CALLBACK_1(ThreeBulletsLayer::threeBulletsMoveFinished, this));
	auto sequenceRight = Sequence::create(actionRightMove, actionRightDone, nullptr);
	bullet->runAction(sequence);
	bulletLeft->runAction(sequenceLeft);
	bulletRight->runAction(sequenceRight);
}

void ThreeBulletsLayer::threeBulletsMoveFinished(Node* pSender)
{
	auto mutiBullets = (Sprite*)pSender;
	m_pAllThreeBullets->removeObject(mutiBullets);
	//this->mutiBullesBatchNode->removeChild(mutiBullets, true);
	this->removeChild(mutiBullets, true);
}

void ThreeBulletsLayer::RemoveThreeBullets(Sprite* mutiBullets)
{
	if (mutiBullets != nullptr)
	{
		this->m_pAllThreeBullets->removeObject(mutiBullets);
		//this->mutiBullesBatchNode->removeChild(mutiBullets, true);
		this->removeChild(mutiBullets, true);
	}
}
