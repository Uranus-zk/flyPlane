#include "PlaneLayer.h"
#include "constant.h"

PlaneLayer* PlaneLayer::sharedPlane = nullptr;
PlaneLayer::PlaneLayer() {
	isAlive = 4;
	int score = 0;
	blood = 10;
}
PlaneLayer::~PlaneLayer(){}

PlaneLayer* PlaneLayer::createLayer() {
	PlaneLayer *pRet = new PlaneLayer(); 
	if (pRet && pRet->init()) 
	{ 
		pRet->autorelease();
		sharedPlane = pRet;
		return pRet; 
	} 
	else 
	{ 
		delete pRet; 
		pRet = nullptr; 
		return nullptr; 
	} 
	
}

bool PlaneLayer::init() {
	if (!Layer::init()) return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_pProgressView = new ProgressView;
	m_pProgressView->setPosition(visibleSize.width-100, visibleSize.height-50);
	m_pProgressView->setScale(4.2f);
	m_pProgressView->setBackgroundTexture("gary.png");
	m_pProgressView->setForegroundTexture("red.png");
	m_pProgressView->setTotalProgress(10.0f);
	m_pProgressView->setCurrentProgress(blood);
	this->addChild(m_pProgressView, 2);


	m_pProgressView1= new ProgressView;
	m_pProgressView1->setPosition(visibleSize.width - 100, visibleSize.height - 100);
	m_pProgressView1->setScale(4.2f);
	m_pProgressView1->setBackgroundTexture("gary.png");
	m_pProgressView1->setForegroundTexture("green.png");
	m_pProgressView1->setTotalProgress(10.0f);
	m_pProgressView1->setCurrentProgress(0);
	this->addChild(m_pProgressView1, 2);

	auto plane = Sprite::createWithSpriteFrameName("hero1.png");
	plane->setPosition(Vec2(visibleSize.width / 2, plane->getContentSize().height / 2));
	auto life1 = Sprite::create("life.png");
	life1->setPosition(Vec2(visibleSize.width - life1->getContentSize().width / 2, life1->getContentSize().height/ 2));
	auto life2 = Sprite::create("life.png");
	life2->setPosition(Vec2(visibleSize.width - life1->getContentSize().width*3 / 2, life1->getContentSize().height/ 2));
	auto life3 = Sprite::create("life.png");

	life3->setPosition(Vec2(visibleSize.width - life1->getContentSize().width *5/ 2, life1->getContentSize().height / 2));
	this->addChild(plane, 1, PLANE_LAYER);
	this->addChild(life1, 1, LIFE1);
	this->addChild(life2, 1, LIFE2);
	this->addChild(life3, 1, LIFE3);

	auto blink = Blink::create(1, 3);

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.1f);
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero1.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
	auto animate = Animate::create(animation);

	plane->runAction(blink);
	plane->runAction(RepeatForever::create(animate));
	return true;
}

void PlaneLayer::Blowup(int passScore)
{
	
		
		score = passScore;
		auto animation = Animation::create();
		animation->setDelayPerUnit(0.2f);
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n1.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n2.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n3.png"));
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n4.png"));

		auto animate = Animate::create(animation);
		auto removePlane = CallFunc::create(CC_CALLBACK_0(PlaneLayer::RemovePlane, this));
		auto sequence = Sequence::create(animate, removePlane, nullptr);
		this->getChildByTag(747)->stopAllActions();
		this->getChildByTag(747)->runAction(sequence);
	
}

void PlaneLayer::RemovePlane()
{	

	this->removeChildByTag(747, true);
	auto pScene = GameOverScene::create(score);
	auto animateScene = TransitionFadeBL::create(0.8f, pScene);
	Director::getInstance()->replaceScene(animateScene);
}
int PlaneLayer::getLife() {
	return isAlive;
}
void PlaneLayer::setLife(int now) {
	isAlive = now;
}

int PlaneLayer::getBlood() {
	return blood;
}
void PlaneLayer::setBlood(int now) {
	blood = now;
}