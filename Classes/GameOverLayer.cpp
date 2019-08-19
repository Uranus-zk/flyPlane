#include "GameOverLayer.h"
#include "GameLayer.h"
#include "constant.h"
#include "ShopScene.h"
int GameOverLayer::highestHistoryScore = 0;

GameOverLayer::GameOverLayer(void)
{
	score = 0;
	highestScore = nullptr;
}

GameOverLayer::~GameOverLayer(void)
{
}

GameOverLayer* GameOverLayer::create(int passScore)
{
	GameOverLayer *pRet = new GameOverLayer();
	pRet->score = passScore;
	if (pRet && pRet->init())
	{ 
		pRet->autorelease();
	} 
	else 
	{ 
		delete pRet; 
		pRet = nullptr;
	}
    return pRet;
}

bool GameOverLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		}
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("game_over.mp3");

		auto winSize = Director::getInstance()->getWinSize();

	    Sprite* background = Sprite::createWithSpriteFrameName("gameover.png");
		background->setPosition(Point(winSize.width/2, winSize.height/2));
		this->addChild(background);

		auto label4 = Sprite::create("shop.png");//////**********////////////
		auto shopMenu = MenuItemSprite::create(
			label4, label4,
			[](Ref*) {
			auto scene = ShopScene::createScene();
			Director::getInstance()->replaceScene(scene);
		}
			//CC_CALLBACK_1(HelloWorldLayer::setCallback, this)
		);
		shopMenu->setPosition(Point(50, winSize.height - 50));


		auto normalBackToGame = Sprite::createWithSpriteFrameName("btn_finish.png");
		auto pressedBackToGame = Sprite::createWithSpriteFrameName("btn_finish.png");
		auto pBackItem = MenuItemSprite::create(normalBackToGame,
                                                pressedBackToGame,
                                                nullptr,
                                                CC_CALLBACK_1(GameOverLayer::menuBackCallback, this));
		pBackItem->setPosition(Point(winSize.width-normalBackToGame->getContentSize().width/2-10, normalBackToGame->getContentSize().height/2+10));
		auto menuBack = Menu::create(pBackItem, shopMenu, nullptr);
		menuBack->setPosition(Point::ZERO);
		this->addChild(menuBack);

		int money = UserDefault::getInstance()->getIntegerForKey(MONEY_KEY, 0);//
		auto coin = Sprite::create("coin.png");
		coin->setPosition(Point(winSize.width / 3, winSize.height / 3));
		money += score / 1000;
		UserDefault::getInstance()->setIntegerForKey(MONEY_KEY, money);
		auto lblMoney = Label::createWithBMFont("font.fnt", StringUtils::format("%d", score / 1000));
		lblMoney->setPosition(coin->getPosition() + Vec2(100, 0));
		this->addChild(coin, 5, COIN_TAG);
		this->addChild(lblMoney, 5, LBLMONEY_TAG);


        Value strScore(score);
		auto finalScore = Label::createWithBMFont("font.fnt", strScore.asString());
		finalScore->setColor(Color3B(143,146,147));
		finalScore->setPosition(Point(winSize.width/2, winSize.height/2));
		this->addChild(finalScore);

		auto delay = DelayTime::create(1.0f);
		auto scalebig = ScaleTo::create(1.0f,3.0f);
		auto scalelittle = ScaleTo::create(0.3f,2.0f);
		auto showAD = CallFunc::create(CC_CALLBACK_0(GameOverLayer::showAD, this));
		auto sequence = Sequence::create(delay, scalebig, scalelittle, showAD, nullptr);
		finalScore->runAction(sequence);

        Value strHighestScore(highestHistoryScore);
		highestScore = Label::createWithBMFont("font.fnt", strHighestScore.asString());
		highestScore->setColor(Color3B(143,146,147));
		highestScore->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
		highestScore->setPosition(Point(140,winSize.height-30));
		this->addChild(highestScore);

		if (score>highestHistoryScore)
		{
			UserDefault::getInstance()->setIntegerForKey("HighestScore", score);
			highestHistoryScore = score;
			auto delayChange = DelayTime::create(1.3f);
			auto moveOut = MoveBy::create(0.1f, Point(0,100));
			auto beginChange = CallFuncN::create(CC_CALLBACK_1(GameOverLayer::beginChangeHighestScore, this));
			auto moveIn = MoveBy::create(0.1f,Point(0,-100));
			auto sequence = Sequence::create(delayChange, moveOut, beginChange, moveIn, nullptr);
			highestScore->runAction(sequence);
		}

		bRet = true;
	} while (0);
	return bRet;
}

void GameOverLayer::menuBackCallback(Ref* pSender)
{
	auto pScene = GameLayer::createScene();
	auto animateScene = TransitionSlideInL::create(1.0f, pScene);
	Director::getInstance()->replaceScene(animateScene);
}

void GameOverLayer::beginChangeHighestScore(Node* pNode)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("achievement.mp3");
	Value changeScore(score);
	highestScore->setString(changeScore.asString());
}

void GameOverLayer::showAD()
{
	
}
