#include "UFOLayer.h"
#include "time.h"

UFOLayer::UFOLayer(void)
{
	m_pAllMutiBullets = __Array::create();
	m_pAllMutiBullets->retain();
	m_pAllThreeBullets = __Array::create();
	m_pAllThreeBullets->retain();
	m_pAllBigBoom = __Array::create();
	m_pAllBigBoom->retain();
	m_pAllLife = __Array::create();
	m_pAllLife->retain();
	m_pAllCloud = __Array::create();
	m_pAllCloud->retain();
	m_pAllHouse = __Array::create();
	m_pAllHouse->retain();
}

UFOLayer::~UFOLayer(void)
{
	m_pAllMutiBullets->release();
	m_pAllMutiBullets = nullptr;
	m_pAllThreeBullets->release();
	m_pAllThreeBullets = nullptr;
	m_pAllBigBoom->release();
	m_pAllBigBoom = nullptr;
	m_pAllLife->release();
	m_pAllLife = nullptr;
	m_pAllCloud->release();
	m_pAllCloud = nullptr;
	m_pAllHouse->release();
	m_pAllHouse = nullptr;
}

bool UFOLayer::init()
{
	bool bRet = false;
	do 
	{
		srand((unsigned)time(NULL));
		CC_BREAK_IF(!Layer::init());

		this->schedule(schedule_selector(UFOLayer::AddHouse1), 10.0, -1, 0.0f);
		this->schedule(schedule_selector(UFOLayer::updatenow), 30.0,-1,0.0f);
		this->schedule(schedule_selector(UFOLayer::AddCloud),20);
        bRet = true;
	} while (0);

	return bRet;
}


void UFOLayer::updatenow(float dt) {

	int a = (int)rand() % 4;
	//int a = 1;
	switch (a)
	{
	case 0:
		this->scheduleOnce(schedule_selector(UFOLayer::AddMutiBullets), 0.0f); break;
	case 1:
		this->scheduleOnce(schedule_selector(UFOLayer::AddThreeBullets), 0.0f); break;
	case 2:
		this->scheduleOnce(schedule_selector(UFOLayer::AddLife), 0.0f);	break;
	default:
		this->scheduleOnce(schedule_selector(UFOLayer::AddBigBoom),0.0f);
		break;
	}
}

void UFOLayer::AddMutiBullets(float dt)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("out_porp.mp3");
	auto mutiBullets = Sprite::createWithSpriteFrameName("ufo_bullet2.png");

	auto mutiBlletsSize = mutiBullets->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();
	int minX = mutiBlletsSize.width/2;
	int maxX = winSize.width-mutiBlletsSize.width/2;
	int rangeX = maxX-minX;
	int actualX = (rand()%rangeX)+minX;

	mutiBullets->setPosition(Point(actualX, winSize.height+mutiBlletsSize.height/2));
	this->addChild(mutiBullets);
	this->m_pAllMutiBullets->addObject(mutiBullets);

	auto move1 = MoveBy::create(0.5f, Point(0, -150));
	auto move2 = MoveBy::create(0.3f, Point(0, 100));
	auto move3 = MoveBy::create(1.0f, Point(0, 0-winSize.height-mutiBlletsSize.height/2));
	auto moveDone = CallFuncN::create(CC_CALLBACK_1(UFOLayer::mutiBulletsMoveFinished, this));

	auto sequence = Sequence::create(move1, move2, move3, moveDone, nullptr);
	mutiBullets->runAction(sequence);
}

void UFOLayer::mutiBulletsMoveFinished(Node* pSender)
{
	auto mutiBullets = (Sprite*)pSender;
	this->removeChild(mutiBullets, true);
	this->m_pAllMutiBullets->removeObject(mutiBullets);
}
void UFOLayer::AddThreeBullets(float dt)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("out_porp.mp3");
	auto threeBullets = Sprite::createWithSpriteFrameName("ufo3.png");

	auto threeBlletsSize = threeBullets->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();
	int minX = threeBlletsSize.width / 2;
	int maxX = winSize.width - threeBlletsSize.width / 2;
	int rangeX = maxX - minX;
	int actualX = (rand() % rangeX) + minX;

	threeBullets->setPosition(Point(actualX, winSize.height + threeBlletsSize.height / 2));
	this->addChild(threeBullets);
	this->m_pAllThreeBullets->addObject(threeBullets);

	auto move1 = MoveBy::create(0.5f, Point(0, -150));
	auto move2 = MoveBy::create(0.3f, Point(0, 100));
	auto move3 = MoveBy::create(1.0f, Point(0, 0 - winSize.height - threeBlletsSize.height / 2));
	auto moveDone = CallFuncN::create(CC_CALLBACK_1(UFOLayer::threeBulletsMoveFinished, this));

	auto sequence = Sequence::create(move1, move2, move3, moveDone, nullptr);
	threeBullets->runAction(sequence);
}

void UFOLayer::threeBulletsMoveFinished(Node* pSender)
{
	auto threeBullets = (Sprite*)pSender;
	this->removeChild(threeBullets, true);
	this->m_pAllThreeBullets->removeObject(threeBullets);
}
void UFOLayer::AddHouse1(float dt) {
	auto cloud = Sprite::create("blockhouse1.png");
	auto cloudSize = cloud->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();
	int minX = cloudSize.width / 2;
	int maxX = winSize.width - cloudSize.width / 2;
	int rangeX = maxX - minX;
	int actualX = (rand() % rangeX) + minX;
	cloud->setPosition(Point(actualX , winSize.height + cloudSize.height / 2));
	this->addChild(cloud);
	this->m_pAllHouse->addObject(cloud);
	int speed = 2;
	int v = (cloudSize.height+ cloudSize.height) / 60;
	auto move1 = MoveTo::create(v, Point(actualX,  - cloudSize.height / 2));
	auto moveDone = CallFuncN::create(CC_CALLBACK_1(UFOLayer::houseMoveFinished, this));

	auto sequence = Sequence::create(move1, moveDone, nullptr);
	cloud->runAction(sequence);
}

void UFOLayer::houseMoveFinished(Node* pSender)
{
	auto life = (Sprite*)pSender;
	this->removeChild(life, true);
	this->m_pAllHouse->removeObject(life);
}
void UFOLayer::AddLife(float dt) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("out_porp.mp3");
	auto life = Sprite::createWithSpriteFrameName("ufo_life.png");
	auto lifeSize = life->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();
	int minX = lifeSize.width / 2;
	int maxX = winSize.width - lifeSize.width / 2;
	int rangeX = maxX - minX;
	int actualX = (rand() % rangeX) + minX;

	life->setPosition(Point(actualX, winSize.height + lifeSize.height / 2));
	this->addChild(life);
	this->m_pAllLife->addObject(life);

	auto move1 = MoveBy::create(0.5f, Point(0, -150));
	auto move2 = MoveBy::create(0.3f, Point(0, 100));
	auto move3 = MoveBy::create(1.0f, Point(0, 0 - winSize.height - lifeSize.height / 2));
	auto moveDone = CallFuncN::create(CC_CALLBACK_1(UFOLayer::lifeMoveFinished, this));

	auto sequence = Sequence::create(move1, move2, move3, moveDone, nullptr);
	life->runAction(sequence);
}
void UFOLayer::AddCloud(float dt) {
	auto cloud = Sprite::create("cloud.png");
	cloud->setScale(0.75);
	auto cloudSize = cloud->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();

	cloud->setPosition(Point(winSize.width/2, winSize.height + cloudSize.height / 2));
	this->addChild(cloud,128);
	this->m_pAllCloud->addObject(cloud);

	auto move1 = MoveBy::create(5.0f, Point(0, 0 - winSize.height - cloudSize.height / 2));
	auto moveDone = CallFuncN::create(CC_CALLBACK_1(UFOLayer::cloudMoveFinished, this));

	auto sequence = Sequence::create(move1,moveDone, nullptr);
	cloud->runAction(sequence);
}
void UFOLayer::lifeMoveFinished(Node* pSender)
{
	auto life = (Sprite*)pSender;
	this->removeChild(life, true);
	this->m_pAllLife->removeObject(life);
}

void UFOLayer::cloudMoveFinished(Node* pSender)
{
	auto cloud = (Sprite*)pSender;
	this->removeChild(cloud, true);
	this->m_pAllCloud->removeObject(cloud);
}

void UFOLayer::AddBigBoom(float dt)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("out_porp.mp3");
	auto bigBoom = Sprite::createWithSpriteFrameName("ufo_nuclear.png");

	auto bigBoomSize = bigBoom->getContentSize();
	auto winSize = Director::getInstance()->getWinSize();
	int minX = bigBoomSize.width/2;
	int maxX = winSize.width-bigBoomSize.width/2;
	int rangeX = maxX-minX;
	int actualX = (rand()%rangeX)+minX;

	bigBoom->setPosition(Point(actualX, winSize.height+bigBoomSize.height/2));
	this->addChild(bigBoom);
	this->m_pAllBigBoom->addObject(bigBoom);

	auto move1 = MoveBy::create(0.5, Point(0, -150));
	auto move2 = MoveBy::create(0.3, Point(0, 100));
	auto move3 = MoveBy::create(1.0, Point(0, 0-winSize.height-bigBoomSize.height/2));
	auto moveDone = CallFuncN::create(CC_CALLBACK_1(UFOLayer::bigBoomMoveFinished, this));

	auto sequence = Sequence::create(move1, move2, move3, moveDone, nullptr);
	bigBoom->runAction(sequence);
}

void UFOLayer::bigBoomMoveFinished(Node* pSender)
{
	auto bigBoom = (Sprite*)pSender;
	this->removeChild(bigBoom, true);
	this->m_pAllBigBoom->removeObject(bigBoom);
}

void UFOLayer::RemoveMutiBullets(Sprite* mutiBullets)
{
	this->removeChild(mutiBullets, true);
	this->m_pAllMutiBullets->removeObject(mutiBullets);
}
void UFOLayer::RemoveThreeBullets(Sprite* mutiBullets)
{
	this->removeChild(mutiBullets, true);
	this->m_pAllThreeBullets->removeObject(mutiBullets);
}

void UFOLayer::RemoveBigBoom(Sprite* bigBoom)
{
	this->removeChild(bigBoom, true);
	this->m_pAllBigBoom->removeObject(bigBoom);
}

void UFOLayer::RemoveLife(Sprite* life)
{
	this->removeChild(life, true);
	this->m_pAllBigBoom->removeObject(life);
}

