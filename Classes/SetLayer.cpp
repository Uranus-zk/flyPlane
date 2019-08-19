#include "SetLayer.h"
#include "HelloWorldLayer.h"
#include "Drop.h"

SetLayer::SetLayer() {
	
}
SetLayer::~SetLayer() {

}

Scene* SetLayer::createScene() {
	auto scene = Scene::create();
	auto layer = SetLayer::create();
	scene->addChild(layer);
	return scene;
}

bool SetLayer::init() {
	if (!Layer::init()) return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background1 = Sprite::createWithSpriteFrameName("background.png");
	background1->setAnchorPoint(Point::ZERO);
	background1->setPosition(Point::ZERO);
	background1->getTexture()->setAliasTexParameters();
	this->addChild(background1, -1, 2);

	auto bk = Sprite::create("bk.png");
	auto bkMenu = MenuItemSprite::create(
		bk, bk,
		[](Ref*) {
	Director::getInstance()->popScene(); }
		//CC_CALLBACK_1(HelloWorldLayer::setCallback, this)
	);
	bkMenu->setAnchorPoint(Point(0,1));
	bkMenu->setScale(0.25);
	bkMenu->setPosition(Vec2(0, visibleSize.height ));
	Menu* mn = Menu::create(bkMenu, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn, 0);

	auto copyright = Sprite::createWithSpriteFrameName("shoot_copyright.png");
	copyright->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	copyright->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 100));
	this->addChild(copyright, -1);

	__Dictionary* string = __Dictionary::createWithContentsOfFile("helloworld.xml");
	__String* a = (__String*)string->objectForKey("Layer");
	const char* b = a->getCString();
	Label* label1 = Label::createWithSystemFont(b, "宋体", 30);
	label1->setPosition(Point(100, 340));
	label1->setColor(Color3B(255, 255, 255));
	this->addChild(label1, -1);
	__String* c = (__String*)string->objectForKey("hard");
	const char* d = c->getCString();
	Label* label = Label::createWithSystemFont(d, "宋体", 30);
	label->setPosition(Point(100, 180));
	label->setColor(Color3B(255, 255, 255));
	this->addChild(label, -1);


	CCLabelTTF* initLabel = CCLabelTTF::create("1", "Arial", 30);
	//设置显示栏目的size
	CCSize size = CCSizeMake(80, 30);
	//创建下拉列表框对象
	CustomDropDownListBox::DropDownList* listBox = CustomDropDownListBox::DropDownList::create(initLabel, size);

	//向下拉列表添加三个选项
	CCLabelTTF* label2 = CCLabelTTF::create("1", "Arial", 30);
	listBox->addLabel(label2);


	CCLabelTTF* label3 = CCLabelTTF::create("2", "Arial", 30);
	listBox->addLabel(label3);

	CCLabelTTF* label4 = CCLabelTTF::create("3", "Arial", 30);
	listBox->addLabel(label4);
	
	//设置下拉列表框默认选项，默认是第0项
	listBox->setSelectedIndex(HelloWorldLayer::A);

	listBox->setPosition(300, 326);
	this->addChild(listBox, 2);

	CCLabelTTF* initLabel2 = CCLabelTTF::create("EASY", "Arial", 30);
	//设置显示栏目的size
	CCSize size1 = CCSizeMake(120, 30);
	//创建下拉列表框对象
	CustomDropDownListBox::DropDownList* listBox2 = CustomDropDownListBox::DropDownList::create(initLabel2, size1);

	//向下拉列表添加三个选项
	CCLabelTTF* label22 = CCLabelTTF::create("EASY", "Arial", 30);
	listBox2->addLabel(label22);


	CCLabelTTF* label32 = CCLabelTTF::create("MIDDLE", "Arial", 30);
	listBox2->addLabel(label32);

	CCLabelTTF* label42 = CCLabelTTF::create("HARD", "Arial", 30);
	listBox2->addLabel(label42);

	CCLabelTTF* label52 = CCLabelTTF::create("GOD", "Arial", 30);
	listBox2->addLabel(label52);
	//设置下拉列表框默认选项，默认是第0项
	    listBox2->setSelectedIndex(HelloWorldLayer::B);

	listBox2->setPosition(300, 160);
	this->addChild(listBox2, 2);
	return true;
}