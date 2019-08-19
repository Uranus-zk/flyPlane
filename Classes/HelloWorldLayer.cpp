#include "HelloWorldLayer.h"
#include "GameLayer.h"
#include "Intro.h"
#include "constant.h"
#include "ShopScene.h"


int HelloWorldLayer::A = 0;
int HelloWorldLayer::B = 0;
bool HelloWorldLayer::init(){
	if (!Layer::init()) return false;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist");
	auto winSize = Director::getInstance()->getWinSize();


	auto aniMiddleHit = Animation::create();
	aniMiddleHit->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_hit.png"));
	aniMiddleHit->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2.png"));
	aniMiddleHit->setDelayPerUnit(0.1f);
	AnimationCache::getInstance()->addAnimation(aniMiddleHit, "MIDDLEHIT");

	auto aniBigHit = Animation::create();
	aniBigHit->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_hit.png"));
	aniBigHit->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_n1.png"));
	aniBigHit->setDelayPerUnit(0.1f);
	AnimationCache::getInstance()->addAnimation(aniBigHit, "BIGHIT");

	auto background = Sprite::createWithSpriteFrameName("background.png");
	background->setPosition(Point(winSize.width / 2, winSize.height / 2));
	this->addChild(background,-1);

	auto copyright = Sprite::createWithSpriteFrameName("shoot_copyright.png");
	copyright->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	copyright->setPosition(Point(winSize.width / 2, winSize.height / 2+100));
	this->addChild(copyright,-1);

	auto label1 = Sprite::createWithSpriteFrameName("start_game.png");
	auto label2 = Sprite::createWithSpriteFrameName("setting.png");
	auto label3 = Sprite::createWithSpriteFrameName("end_game.png");
	auto label4 = Sprite::create("guide.png");
  auto label5 = Sprite::create("shop.png");//////**********////////////

	auto startMenu = MenuItemSprite::create(
		label1,label1,
		[=](Ref*) {
			this->removeChildByTag(Hello_World_Menu_Tag);
			auto loading = Sprite::createWithSpriteFrameName("game_loading1.png");
			loading->setPosition(Point(winSize.width / 2, winSize.height / 2 - 40));
			this->addChild(loading);

			auto animation = Animation::create();
			animation->setDelayPerUnit(0.2f);
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("game_loading1.png"));
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("game_loading2.png"));
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("game_loading3.png"));
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("game_loading4.png"));

			auto animate = Animate::create(animation);
			auto repeat = Repeat::create(animate, 2);
			auto repeatdone = CallFuncN::create(CC_CALLBACK_1(HelloWorldLayer::loadingDone, this));
			auto sequence = Sequence::create(repeat, repeatdone, nullptr);
			loading->runAction(sequence);
		}
	//	CC_CALLBACK_1(HelloWorldLayer::startGameCallback, this)
	);
	startMenu->setPosition(Vec2(winSize.width / 2, winSize.height / 2-80));
	
	auto exitMenu = MenuItemSprite::create(
		label3,label3,
		[](Ref *) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
		return;
#endif

		Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
		}
//		CC_CALLBACK_1(HelloWorldLayer::exitGameCallback, this)
	);
	exitMenu->setPosition(Vec2(winSize.width / 2, winSize.height /2- 160));

	auto setMenu = MenuItemSprite::create(
		label2,label2,
		[](Ref*){auto st = SetLayer::createScene();
	auto tst = TransitionFade::create(1.0f, st);
	Director::getInstance()->pushScene(tst); }
		//CC_CALLBACK_1(HelloWorldLayer::setCallback, this)
	);
	auto guideMenu = MenuItemSprite::create(label4, label4, [](Ref*) {
		auto st = Intro::createScene();
		auto tst = TransitionFade::create(1.0f, st);
		Director::getInstance()->pushScene(tst);
	});
	guideMenu->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 240));

auto shopMenu = MenuItemSprite::create(
	label5, label5,
	[](Ref*) {
	auto scene = ShopScene::createScene();
	Director::getInstance()->replaceScene(scene);
}
//CC_CALLBACK_1(HelloWorldLayer::setCallback, this)
);
shopMenu->setPosition(Point(50, winSize.height - 50));


	setMenu->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	Menu* mn = Menu::create(startMenu, exitMenu,setMenu,guideMenu, shopMenu, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn,0, Hello_World_Menu_Tag);
	getHighestHistorySorce();
	return true;
}

void HelloWorldLayer::loadingDone(Node* pNode)
{

	auto sc = GameLayer::createScene();
	auto tsc = TransitionFade::create(1.0f, sc);
	Director::getInstance()->replaceScene(tsc);
}

bool HelloWorldLayer::isHaveSaveFile()
{
	if (!UserDefault::getInstance()->getBoolForKey("isHaveSaveFileXml"))
	{
		UserDefault::getInstance()->setBoolForKey("isHaveSaveFileXml", true);
		UserDefault::getInstance()->setIntegerForKey("HighestScore", 0);
		UserDefault::getInstance()->flush();
		return false;
	}
	return true;
}

void HelloWorldLayer::getHighestHistorySorce()
{
	if (isHaveSaveFile())
	{
		GameOverLayer::highestHistoryScore = UserDefault::getInstance()->getIntegerForKey("HighestScore", 0);
	}
}

