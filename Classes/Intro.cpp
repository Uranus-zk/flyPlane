#include "Intro.h"
#include "constant.h"
#include "HelloWorldLayer.h"
#include "HelloWorldScene.h"
#include "GameLayer.h"

Intro* Intro::createScene()
{
	/*Intro *intro = new(std::nothrow) Intro();
	if (intro && intro->init())
	{
		intro->autorelease();
		return intro;
	}
	else
	{
		delete intro;
		intro = nullptr;
		return nullptr;
	}*/
	return Intro::create();
}


bool Intro::init()
{
	auto VisibleSize = Director::getInstance()->getVisibleSize();
	do {
		CC_BREAK_IF(!Scene::init());

		//auto VisibleSize = Director::getInstance()->getVisibleSize();
		auto bg = Sprite::createWithSpriteFrameName("background.png");
		bg->setPosition(VisibleSize / 2);
		this->addChild(bg);

		//小人
		auto man = Sprite::create("intro.png");
		man->setPosition(man->getContentSize().width / 2 , VisibleSize.height - man->getContentSize().height / 2);
		this->addChild(man, 99);

		/*//hero
		auto hero = Sprite::create("life.png");
		hero->setPosition(-Vec2(hero->getContentSize().width + 40, hero->getContentSize().height + 20) + VisibleSize);
		this->addChild(hero);*/

		//图片切换
		MenuItemImage * R1 = MenuItemImage::create("dialog1.png", "dialog1.png");
		MenuItemImage * R2 = MenuItemImage::create("dialog2.png", "dialog2.png");
		MenuItemImage * R3 = MenuItemImage::create("return_to_menu.png", "return_to_menu.png");
		//MenuItemImage * R4 = MenuItemImage::create("gameover.png", "gameover.png");
		//MenuItemImage * R5 = MenuItemImage::create("return_to_menu.png", "return_to_menu.png");
		auto toggle = MenuItemToggle::createWithCallback([&](Ref* sender) {

			int index = dynamic_cast<MenuItemToggle*>(sender)->getSelectedIndex();
			if (index == 0)
			{
				//回到主菜单
				Director::getInstance()->popScene();
			}

		}, R1, R2, R3, nullptr);
		

		//返回按钮
		auto bk = Sprite::create("bk.png");
		auto bkMenu = MenuItemSprite::create(
			bk, bk,
			[](Ref*) {
			Director::getInstance()->popScene(); }
		);
		bkMenu->setAnchorPoint(Point(0, 1));
		bkMenu->setScale(0.25);
		bkMenu->setPosition(Vec2(-280, -210));


		auto menu = Menu::create(toggle, bkMenu, nullptr);
		menu->setPosition(Vec2(45, 0) + VisibleSize / 2);
		R3->setPosition(Vec2(-200, 0) + VisibleSize / 2);
		this->addChild(menu, 5);

		//文字部分
		auto word = Label::create("Click anywhere to continue", "Aharoni", 22.0f);
		word->setPosition(VisibleSize.width - 160, 15);
		word->setColor(Color3B(138, 54, 15));
		this->addChild(word, 99);

		//返回按钮
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [=](Touch* t, Event* e) {
			//回到主菜单
			Director::getInstance()->popScene();


			return false;

		};



		return true;


	} while (0);
	return false;
}
