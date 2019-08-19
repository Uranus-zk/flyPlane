#include "ShopScene.h"
#include "HelloWorldScene.h"
#include "constant.h"
Scene* ShopScene::createScene()
{
	auto scene = new ShopScene();
	if (scene && scene->init())
	{
		scene->autorelease();
		return scene;
	}
	delete scene;
	scene = nullptr;
	return nullptr;
}

bool ShopScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto winSize = Director::getInstance()->getWinSize();

	//将通过加载精灵集文件,将精灵都缓存在 精灵集缓存中

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist");

	//背景 创建
	auto bg = Sprite::createWithSpriteFrameName("background.png");
	bg->setPosition(Point(winSize.width / 2, winSize.height / 2));
	this->addChild(bg, -1);

	//商店标题 创建
	auto title = Sprite::create("shop_1.png");
	title->setPosition(Point(winSize.width / 2, winSize.height / 2 + 200));
	this->addChild(title,99);

	auto introduce1 = Sprite::create("introduce1.png");
	introduce1->setPosition(Point(winSize.width / 2 -100, winSize.height / 2 - 50));
	this->addChild(introduce1,99);
	
	
	auto introduce2 = Sprite::create("introduce2.png");
	introduce2->setPosition(Point(winSize.width / 2 - 100, winSize.height / 3 - 50));
	this->addChild(introduce2,99);
	



	//金钱数int
	money = UserDefault::getInstance()->getIntegerForKey(MONEY_KEY, 0);//
	shield = UserDefault::getInstance()->getBoolForKey(SHIELD_KEY, 0);

	//coin 图标
	auto coin = Sprite::create("coin.png");
	coin->setPosition(Point(winSize.width / 2 + 100, winSize.height - 40));
	this->addChild(coin);


	//金钱数字 创建
	auto labelMoney = Label::createWithBMFont("font.fnt", StringUtils::format("%d", money));
	labelMoney->setPosition(winSize.width - 50, winSize.height - 50);
	this->addChild(labelMoney, 99, MONEY_TAG);
	labelMoney->setColor(Color3B::BLACK);
	labelMoney->setBMFontSize(32);




	//str护盾 创建
	auto lblShield = Sprite::create("buy_shield.png");
	lblShield->setPosition(winSize.width / 2 - 100, winSize.height / 2);
	this->addChild(lblShield, 99, ITEM_SHIELD_TAG);

	//str购买炸弹 创建
	auto lblBomb = Sprite::create("buy_bomb.png");
	lblBomb->setPosition(winSize.width / 2 - 100, winSize.height / 3);
	this->addChild(lblBomb, 99, ITEM_BOMB_TAG);

	//label护盾数值
	auto labelShield = Label::createWithBMFont("font.fnt", StringUtils::format("%d", shield));
	labelShield->setPosition(lblShield->getPosition() + Vec2(200, 0));
	this->addChild(labelShield, 99, SHIELD_TAG);
	labelShield->setColor(Color3B::BLACK);
	labelShield->setBMFontSize(30);

	//label炸弹数
	bomb = UserDefault::getInstance()->getIntegerForKey(BOMB_KEY, 0);//
	auto labelBomb = Label::createWithBMFont("font.fnt", StringUtils::format("%d", bomb));
	labelBomb->setPosition(Vec2(200, 0) + lblBomb->getPosition());
	this->addChild(labelBomb, 99, BOMB_TAG);
	labelBomb->setColor(Color3B::BLACK);
	labelBomb->setBMFontSize(30);


	//护盾按钮
	auto spShield = Sprite::create("add_coin.png");
	auto menuShield = MenuItemSprite::create(spShield, spShield, [this](Ref*)//控件本身
	{
		if (money - 1000 > 0 && !shield)
		{
			shield = true;
			UserDefault::getInstance()->setBoolForKey(SHIELD_KEY, shield);//

			money -= 1000;
			UserDefault::getInstance()->setIntegerForKey(MONEY_KEY, money);//
		}
		log("bullettttt");
	});

	//购买炸弹按钮
	auto spBomb = Sprite::create("add_coin.png");
	auto menuBomb = MenuItemSprite::create(spBomb, spBomb, [this](Ref*)//控件本身
	{
		if (money - 100 > 0 && bomb<99999)
		{
			bomb++;
			UserDefault::getInstance()->setIntegerForKey(BOMB_KEY, bomb);//

			money-=100;
			UserDefault::getInstance()->setIntegerForKey(MONEY_KEY, money);//
		}
		log("bombbbbb");

	});

	//返回按钮
	auto spBack = Sprite::create("back.png");
	auto menuBack = MenuItemSprite::create(spBack, spBack, [this](Ref*)//控件本身
	{
		auto scene = HelloWorldScene::createScene();
		Director::getInstance()->replaceScene(scene);

	});



	//菜单添加按钮
	auto menu = Menu::create();
	menu->addChild(menuShield);
	menu->addChild(menuBomb);
	menu->addChild(menuBack);
	menu->setPosition(50, 50);
	menuShield->setPosition(labelShield->getPosition() + Vec2(20, -40));
	menuBomb->setPosition(labelBomb->getPosition() + Vec2(20, -40));
	menuBack->setPosition(0, winSize.height - menuBack->getContentSize().height*1.5f);
	this->addChild(menu, 99, MENU_TAG);

	scheduleUpdate();

	return true;
}

void ShopScene::update(float delta)
{
	auto nodeShield = this->getChildByTag(SHIELD_TAG);
	auto nodeBomb = this->getChildByTag(BOMB_TAG);
	auto nodeMoney = this->getChildByTag(MONEY_TAG);

	auto labelShield = dynamic_cast<Label*>(nodeShield);
	auto labelBomb = dynamic_cast<Label*>(nodeBomb);
	auto labelMoney = dynamic_cast<Label*>(nodeMoney);


	Vector<Label*> removableLabel;

	std::string strShield = StringUtils::format("%d", shield);
	labelShield->setString(strShield);
	labelShield->setColor(Color3B(0, 255, 0));
	removableLabel.pushBack(labelShield);


	std::string strBomb = StringUtils::format("%d", bomb);
	labelBomb->setString(strBomb);
	labelBomb->setColor(Color3B(0, 255, 0));
	removableLabel.pushBack(labelBomb);

	std::string strMoney = StringUtils::format("%d", money);
	labelMoney->setString(strMoney);
	labelMoney->setColor(Color3B(0, 255, 0));
	removableLabel.pushBack(labelMoney);

	removableLabel.clear();

}

