#include "GameLayer.h"
#include "HelloWorldLayer.h"

#include "constant.h"

Level GameLayer::level = EASY;
GameLayer::GameLayer() {
	m_pAllCoin = __Array::create();
	m_pAllCoin->retain();
	background1 = nullptr;
	background2 = nullptr;
	bulletLayer = nullptr;
	mutiBulletsLayer = nullptr;
	threeBulletsLayer = nullptr;
	controlLayer = nullptr;
	enemyLayer = nullptr;
	score = 0;
	coinSC = 0;
	god = 0;
	coinPR = UserDefault::getInstance()->getBoolForKey(SHIELD_KEY, 0);
	bigBoomCount=UserDefault::getInstance()->getBoolForKey(BOMB_KEY,0);
	PRcoin = nullptr;
}
GameLayer::~GameLayer() {
	m_pAllCoin->release();
	m_pAllCoin = nullptr;
}

Scene* GameLayer::createScene(){
	auto scene = Scene::create();
	auto layer = GameLayer::create();
	scene->addChild(layer);
	return scene;
}
bool GameLayer::init() {
	if (!Layer::init()) return false;

	if (HelloWorldLayer::B == 1) {
		GameLayer::level = MIDDLE;
	}
	else if (HelloWorldLayer::B == 2) {
		GameLayer::level = HARD;
	}


	if (!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("game_music.mp3", true);
	}

	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	god = HelloWorldLayer::B == 3 ? 1 : 0;


	background1 = Sprite::createWithSpriteFrameName("background.png");
	background1->setAnchorPoint(Point::ZERO);
	background1->setPosition(Point::ZERO);
	background1->getTexture()->setAliasTexParameters();
	this->addChild(background1,-1,2);

	background2 = Sprite::createWithSpriteFrameName("background.png");
	background2->setAnchorPoint(Point::ZERO);
	background2->setPosition(Point(0, background2->getContentSize().height - 2));
	background2->getTexture()->setAliasTexParameters();
	this->addChild(background2,-1, GAME_LAYER_BGtwo);

	auto planelayer = PlaneLayer::createLayer();
	planeLayer = planelayer;
	this->addChild(planelayer);

	bulletLayer = BulletLayer::create();
	this->addChild(bulletLayer);
	bulletLayer->StartShoot();

	ebulletLayer = EBulletLayer::create();
	this->addChild(ebulletLayer);
	ebulletLayer->StartShoot();

	enemyLayer = EnemyLayer::create();
	this->addChild(enemyLayer);
	enemyLayer->StartBorn();

	this->controlLayer = ControlLayer::create();
	this->addChild(controlLayer);

	this->mutiBulletsLayer = MutiBulletsLayer::create();
	this->addChild(mutiBulletsLayer);
	this->threeBulletsLayer = ThreeBulletsLayer::create();
	this->addChild(threeBulletsLayer);
	this->ufoLayer = UFOLayer::create();
	this->addChild(ufoLayer);

	bool isPRcoin = UserDefault::getInstance()->getBoolForKey(SHIELD_KEY, 0);
	PRcoin = Sprite::createWithSpriteFrameName("shield.png");
	PRcoin->setPosition(this->planeLayer->getChildByTag(747)->getPosition());
	this->addChild(PRcoin, 99, 751);
	PRcoin->setVisible(isPRcoin);

	//UserDefault::getInstance()->setIntegerForKey(BOMB_KEY, bigBoomCount);//
	//log("%d", coinPR);
	
	bigBoomCount = UserDefault::getInstance()->getIntegerForKey(BOMB_KEY, 0);
	//log("%d", bigBoomCount);
	updateBigBoomItem(bigBoomCount);

	auto hero = planelayer->getChildByTag(747);
	auto listener = EventListenerTouchOneByOne::create();
	//2.分解事件,处理逻辑
	//a.触摸开始
	listener->onTouchBegan = [=](Touch* t, Event* e) {
		if (!planelayer->isAlive) return false;
		Vec2 toughPos = t->getLocation();
		bool iscontains = hero->getBoundingBox().containsPoint(toughPos);
		this->m_offSet = toughPos - hero->getPosition();
		
		return iscontains;
	};
	//b.持续触摸并移动
	listener->onTouchMoved = [=](Touch* t, Event* e) {
		if (!planelayer->isAlive) return false;
		Vec2 toMovePos = t->getLocation() - m_offSet;
		Vec2 heroPos = hero->getPosition();
		auto sizeX = hero->getContentSize().width;
		auto sizeY = hero->getContentSize().height;
		if (toMovePos.y-sizeY/2 <= 0) {
			toMovePos.y = sizeY / 2;
		}
		else if (toMovePos.y + sizeY /2>= visibleSize.height) {
			toMovePos.y = visibleSize.height - sizeY/2;
		}
		if (toMovePos.x - sizeX / 2 <= 0) {
			toMovePos.x = sizeX / 2;
		}
		else if (toMovePos.x + sizeX / 2 >= visibleSize.width) {
			toMovePos.x = visibleSize.width - sizeX / 2;
		}
		hero->setPosition(toMovePos);

	};
	//c.触摸结束
	//log("%d", HelloWorldLayer::B);
	listener->onTouchEnded = [](Touch*, Event*) {
	};
	//3.注册监听到分发器上
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, hero);

	
	this->schedule(schedule_selector(GameLayer::randomBomb), 5, CC_REPEAT_FOREVER, 0);
	this->schedule(schedule_selector(GameLayer::backgroundMove), 0.01f);
	this->scheduleUpdate();
	return true;

}

void GameLayer::backgroundMove(float dt)
{

	
	background1->setPositionY(int(background1->getPositionY()) - 2);
	background2->setPositionY(int(background1->getPositionY()) + int(background1->getContentSize().height) - 2);
	if (background2->getPositionY() <= 0)
	{
		background1->setPositionY(0);
	}
}
void GameLayer::update(float dt) {
	if (level == EASY && score >= 1000000|| HelloWorldLayer::B==1)
	{
		level = MIDDLE;
		HelloWorldLayer::B = 0;
	}
	else if (level == MIDDLE && score >= 2000000|| HelloWorldLayer::B==2)
	{
		level = HARD;
		HelloWorldLayer::B = 0;
	}


	//PRcoin->setPosition(this->planeLayer->getChildByTag(747)->getPosition());
	if (coinPR) {
		PRcoin->setPosition(this->planeLayer->getChildByTag(747)->getPosition());
		PRcoin->setVisible(1);
		UserDefault::getInstance()->setBoolForKey(SHIELD_KEY, true);
	}
	auto bulletsToDelete = __Array::create();
	bulletsToDelete->retain();
	Ref* bt, *et, *ut;

	//enemy1 & bullet CheckCollosion
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet, bt)
	{
		auto bullet = (Sprite*)bt;

		auto enemy1sToDelete = CCArray::create();
		enemy1sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1, et)
		{
			auto enemy1 = (Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy1->getBoundingBox()))
			{
				if (enemy1->getLife() == 1)
				{
					enemy1->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy1sToDelete->addObject(enemy1);
					score += ENEMY1_SCORE;
					this->controlLayer->updateScore(score);
				}
				else;
			}
		}
		CCARRAY_FOREACH(enemy1sToDelete, et)
		{
			auto enemy1 = (Enemy*)et;
			this->enemyLayer->enemy1Blowup(enemy1);
		}
		enemy1sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete, bt)
	{
		auto bullet = (Sprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();

	//enemy2 & bullet CheckCollosion
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet, bt)
	{
		auto bullet = (Sprite*)bt;

		auto enemy2sToDelete = __Array::create();
		enemy2sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2, et)
		{
			auto enemy2 = (Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy2->getBoundingBox()))
			{
				
				if (enemy2->getLife() > 1)
				{
					auto ani = AnimationCache::getInstance()->getAnimation("MIDDLEHIT");
					auto animate = Animate::create(ani);
					enemy2->getSprite()->runAction(animate);
					enemy2->loseLife();
					bulletsToDelete->addObject(bullet);
				}
				else if (enemy2->getLife() == 1)
				{
					enemy2->loseLife();
					bulletsToDelete->addObject(bullet);
					enemy2sToDelete->addObject(enemy2);
					auto coin = Sprite::createWithSpriteFrameName("ufo_diamond.png");
					coin->setPosition(enemy2->getPosition());
					this->addChild(coin);
					m_pAllCoin->addObject(coin);
					float mt = (coin->getContentSize().height / 2 + enemy2->getPositionY()) / 200;
					auto move1 = MoveBy::create(mt, Point(0, -(coin->getContentSize().height / 2 + enemy2->getPositionY())));
					auto moveDone = CallFuncN::create([=] (Ref*){
						this->removeChild(coin, true);
					});
					auto sequence = Sequence::create(move1, moveDone, nullptr);
					coin->runAction(sequence);
					score += ENEMY2_SCORE;
					this->controlLayer->updateScore(score);
				}
				else;
			}
		}
		CCARRAY_FOREACH(enemy2sToDelete, et)
		{
			auto enemy2 = (Enemy*)et;
			this->enemyLayer->enemy2Blowup(enemy2);
		}
		enemy2sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete, bt)
	{
		auto bullet = (Sprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();

	//enemy3 & bullet CheckCollosion
	CCARRAY_FOREACH(this->bulletLayer->m_pAllBullet, bt)
	{
		auto bullet = (Sprite*)bt;

		auto enemy3sToDelete = __Array::create();
		enemy3sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3, et)
		{
			auto enemy3 = (Enemy*)et;
			if (bullet->boundingBox().intersectsRect(enemy3->getBoundingBox()))
			{
				
				if (enemy3->getLife()>1)
				{
					auto ani = AnimationCache::getInstance()->getAnimation("BIGHIT");
					auto animate = Animate::create(ani);
					enemy3->getSprite()->runAction(animate);
					enemy3->loseLife();
					this->enemyLayer->setLife(enemy3);
					bulletsToDelete->addObject(bullet);
				}
				else if (enemy3->getLife() == 1)
				{
					enemy3->loseLife();
					this->enemyLayer->setLife(enemy3);
					bulletsToDelete->addObject(bullet);
					enemy3sToDelete->addObject(enemy3);
					auto coin = Sprite::createWithSpriteFrameName("ufo_diamond.png");
					coin->setPosition(enemy3->getPosition());
					this->addChild(coin);
					m_pAllCoin->addObject(coin);
					float mt = (coin->getContentSize().height / 2 + enemy3->getPositionY()) / 200;
					auto move1 = MoveBy::create(mt, Point(0, -(coin->getContentSize().height / 2 + enemy3->getPositionY())));
					auto moveDone = CallFuncN::create([=](Ref*) {
						this->removeChild(coin, true);
					});
					auto sequence = Sequence::create(move1, moveDone, nullptr);
					coin->runAction(sequence);
					score += ENEMY3_SCORE;
					this->controlLayer->updateScore(score);
				}
				else;
			}
		}
		CCARRAY_FOREACH(enemy3sToDelete, et)
		{
			auto enemy3 = (Enemy*)et;
			this->enemyLayer->enemy3Blowup(enemy3);
		}
		enemy3sToDelete->release();
	}
	CCARRAY_FOREACH(bulletsToDelete, bt)
	{
		auto bullet = (Sprite*)bt;
		this->bulletLayer->RemoveBullet(bullet);
	}
	bulletsToDelete->removeAllObjects();
	bulletsToDelete->release();

	auto mutiBulletsToDelete = __Array::create();
	mutiBulletsToDelete->retain();
	Ref* mbt;

	////enemy1 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets, mbt)
	{
		auto mutiBullets = (Sprite*)mbt;

		auto enemy1sToDelete = __Array::create();
		enemy1sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1, et)
		{
			auto enemy1 = (Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy1->getBoundingBox()))
			{
				if (enemy1->getLife() == 1)
				{
					enemy1->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy1sToDelete->addObject(enemy1);
					score += ENEMY1_SCORE;
					this->controlLayer->updateScore(score);
				}
				else;
			}
		}
		CCARRAY_FOREACH(enemy1sToDelete, et)
		{
			auto enemy1 = (Enemy*)et;
			this->enemyLayer->enemy1Blowup(enemy1);
		}
		enemy1sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete, mbt)
	{
		auto mutiBullets = (Sprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();

	//enemy2 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets, mbt)
	{
		auto mutiBullets = (Sprite*)mbt;

		auto enemy2sToDelete = __Array::create();
		enemy2sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2, et)
		{
			auto enemy2 = (Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy2->getBoundingBox()))
			{
				if (enemy2->getLife() > 1)
				{
					auto ani = AnimationCache::getInstance()->getAnimation("MIDDLEHIT");
					auto animate = Animate::create(ani);
					enemy2->getSprite()->runAction(animate);
					enemy2->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
				}
				else if (enemy2->getLife() == 1)
				{
					enemy2->loseLife();
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy2sToDelete->addObject(enemy2);
					auto coin = Sprite::createWithSpriteFrameName("ufo_diamond.png");
					coin->setPosition(enemy2->getPosition());
					this->addChild(coin);
					m_pAllCoin->addObject(coin);
					float mt = (coin->getContentSize().height / 2 + enemy2->getPositionY()) / 200;
					auto move1 = MoveBy::create(mt, Point(0, -(coin->getContentSize().height / 2 + enemy2->getPositionY())));
					auto moveDone = CallFuncN::create([=](Ref*) {
						this->removeChild(coin, true);
					});
					auto sequence = Sequence::create(move1, moveDone, nullptr);
					coin->runAction(sequence);
					score += ENEMY2_SCORE;
					this->controlLayer->updateScore(score);
				}
				else;
			}
		}
		CCARRAY_FOREACH(enemy2sToDelete, et)
		{
			auto enemy2 = (Enemy*)et;
			this->enemyLayer->enemy2Blowup(enemy2);
		}
		enemy2sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete, mbt)
	{
		auto mutiBullets = (Sprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();

	//enemy3 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->mutiBulletsLayer->m_pAllMutiBullets, mbt)
	{
		auto mutiBullets = (Sprite*)mbt;

		auto enemy3sToDelete = __Array::create();
		enemy3sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3, et)
		{
			auto enemy3 = (Enemy*)et;
			if (mutiBullets->boundingBox().intersectsRect(enemy3->getBoundingBox()))
			{
				if (enemy3->getLife() > 1)
				{
					auto ani = AnimationCache::getInstance()->getAnimation("BIGHIT");
					auto animate = Animate::create(ani);
					enemy3->getSprite()->runAction(animate);
					enemy3->loseLife();
					this->enemyLayer->setLife(enemy3);
					mutiBulletsToDelete->addObject(mutiBullets);
				}
				else if (enemy3->getLife() == 1)
				{
					enemy3->loseLife();
					this->enemyLayer->setLife(enemy3);
					mutiBulletsToDelete->addObject(mutiBullets);
					enemy3sToDelete->addObject(enemy3);
					auto coin = Sprite::createWithSpriteFrameName("ufo_diamond.png");
					coin->setPosition(enemy3->getPosition());
					this->addChild(coin);
					m_pAllCoin->addObject(coin);
					float mt = (coin->getContentSize().height / 2 + enemy3->getPositionY()) / 200;
					auto move1 = MoveBy::create(mt, Point(0, -(coin->getContentSize().height / 2 + enemy3->getPositionY())));
					auto moveDone = CallFuncN::create([=](Ref*) {
						this->removeChild(coin, true);
					});
					auto sequence = Sequence::create(move1, moveDone, nullptr);
					coin->runAction(sequence);
					score += ENEMY3_SCORE;
					this->controlLayer->updateScore(score);
				}
				else;
			}
		}
		CCARRAY_FOREACH(enemy3sToDelete, et)
		{
			auto enemy3 = (Enemy*)et;
			this->enemyLayer->enemy3Blowup(enemy3);
		}
		enemy3sToDelete->release();
	}
	CCARRAY_FOREACH(mutiBulletsToDelete, mbt)
	{
		auto mutiBullets = (Sprite*)mbt;
		this->mutiBulletsLayer->RemoveMutiBullets(mutiBullets);
	}
	mutiBulletsToDelete->removeAllObjects();
	mutiBulletsToDelete->release();


	//ThreeBullets
	auto threeBulletsToDelete = __Array::create();
	threeBulletsToDelete->retain();

	////enemy1 & threeBullets CheckCollosion
	CCARRAY_FOREACH(this->threeBulletsLayer->m_pAllThreeBullets, mbt)
	{
		auto threeBullets = (Sprite*)mbt;

		auto enemy1sToDelete = __Array::create();
		enemy1sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1, et)
		{
			auto enemy1 = (Enemy*)et;
			if (threeBullets->boundingBox().intersectsRect(enemy1->getBoundingBox()))
			{
				if (enemy1->getLife() == 1)
				{
					enemy1->loseLife();
					threeBulletsToDelete->addObject(threeBullets);
					enemy1sToDelete->addObject(enemy1);
					score += ENEMY1_SCORE;
					this->controlLayer->updateScore(score);
				}
				else;
			}
		}
		CCARRAY_FOREACH(enemy1sToDelete, et)
		{
			auto enemy1 = (Enemy*)et;
			this->enemyLayer->enemy1Blowup(enemy1);
		}
		enemy1sToDelete->release();
	}
	CCARRAY_FOREACH(threeBulletsToDelete, mbt)
	{
		auto threeBullets = (Sprite*)mbt;
		this->threeBulletsLayer->RemoveThreeBullets(threeBullets);
	}
	threeBulletsToDelete->removeAllObjects();

	//enemy2 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->threeBulletsLayer->m_pAllThreeBullets, mbt)
	{
		auto threeBullets = (Sprite*)mbt;

		auto enemy2sToDelete = __Array::create();
		enemy2sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2, et)
		{
			auto enemy2 = (Enemy*)et;
			if (threeBullets->boundingBox().intersectsRect(enemy2->getBoundingBox()))
			{
				if (enemy2->getLife() > 1)
				{
					auto ani = AnimationCache::getInstance()->getAnimation("MIDDLEHIT");
					auto animate = Animate::create(ani);
					enemy2->getSprite()->runAction(animate);
					enemy2->loseLife();
					threeBulletsToDelete->addObject(threeBullets);
				}
				else if (enemy2->getLife() == 1)
				{
					enemy2->loseLife();
					threeBulletsToDelete->addObject(threeBullets);
					enemy2sToDelete->addObject(enemy2);
					auto coin = Sprite::createWithSpriteFrameName("ufo_diamond.png");
					coin->setPosition(enemy2->getPosition());
					this->addChild(coin);
					m_pAllCoin->addObject(coin);
					float mt = (coin->getContentSize().height / 2 + enemy2->getPositionY()) / 200;
					auto move1 = MoveBy::create(mt, Point(0, -(coin->getContentSize().height / 2 + enemy2->getPositionY())));
					auto moveDone = CallFuncN::create([=](Ref*) {
						this->removeChild(coin, true);
					});
					auto sequence = Sequence::create(move1, moveDone, nullptr);
					coin->runAction(sequence);
					score += ENEMY2_SCORE;
					this->controlLayer->updateScore(score);
				}
				else;
			}
		}
		CCARRAY_FOREACH(enemy2sToDelete, et)
		{
			auto enemy2 = (Enemy*)et;
			this->enemyLayer->enemy2Blowup(enemy2);
		}
		enemy2sToDelete->release();
	}
	CCARRAY_FOREACH(threeBulletsToDelete, mbt)
	{
		auto threeBullets = (Sprite*)mbt;
		this->threeBulletsLayer->RemoveThreeBullets(threeBullets);
	}
	threeBulletsToDelete->removeAllObjects();

	//enemy3 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->threeBulletsLayer->m_pAllThreeBullets, mbt)
	{
		auto threeBullets = (Sprite*)mbt;

		auto enemy3sToDelete = __Array::create();
		enemy3sToDelete->retain();
		CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3, et)
		{
			auto enemy3 = (Enemy*)et;
			if (threeBullets->boundingBox().intersectsRect(enemy3->getBoundingBox()))
			{
				if (enemy3->getLife() > 1)
				{
					auto ani = AnimationCache::getInstance()->getAnimation("BIGHIT");
					auto animate = Animate::create(ani);
					enemy3->getSprite()->runAction(animate);
					enemy3->loseLife();
					this->enemyLayer->setLife(enemy3);
					threeBulletsToDelete->addObject(threeBullets);
				}
				else if (enemy3->getLife() == 1)
				{
					enemy3->loseLife();
					this->enemyLayer->setLife(enemy3);
					threeBulletsToDelete->addObject(threeBullets);
					enemy3sToDelete->addObject(enemy3);
					auto coin = Sprite::createWithSpriteFrameName("ufo_diamond.png");
					coin->setPosition(enemy3->getPosition());
					this->addChild(coin);
					m_pAllCoin->addObject(coin);
					float mt = (coin->getContentSize().height / 2 + enemy3->getPositionY()) / 200;
					auto move1 = MoveBy::create(mt, Point(0, -(coin->getContentSize().height / 2 + enemy3->getPositionY())));
					auto moveDone = CallFuncN::create([=](Ref*) {
						this->removeChild(coin, true);
					});
					auto sequence = Sequence::create(move1, moveDone, nullptr);
					coin->runAction(sequence);
					score += ENEMY3_SCORE;
					this->controlLayer->updateScore(score);
				}
				else;
			}
		}
		CCARRAY_FOREACH(enemy3sToDelete, et)
		{
			auto enemy3 = (Enemy*)et;
			this->enemyLayer->enemy3Blowup(enemy3);
		}
		enemy3sToDelete->release();
	}
	CCARRAY_FOREACH(threeBulletsToDelete, mbt)
	{
		auto threeBullets = (Sprite*)mbt;
		this->threeBulletsLayer->RemoveThreeBullets(threeBullets);
	}
	threeBulletsToDelete->removeAllObjects();
	threeBulletsToDelete->release();
	







	auto airplaneRect = this->planeLayer->getChildByTag(747)->boundingBox();
	auto life = this->planeLayer->getLife();
	airplaneRect.origin.x += 30;
	airplaneRect.size.width -= 60;
	//enemy1 & airplane CheckCollosion
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1, et)
	{
		

		auto enemy1sToDelete = __Array::create();
		enemy1sToDelete->retain();
		auto enemy1 = (Enemy*)et;
		if (enemy1->getLife() > 0)
		{
			if (airplaneRect.intersectsRect(enemy1->getBoundingBox())&&!god)
			{
				if (coinPR) {
					coinPR = 0;
					enemy1->setLife(0);
					enemy1sToDelete->addObject(enemy1);
					PRcoin->setVisible(0);
					UserDefault::getInstance()->setBoolForKey(SHIELD_KEY, false);
				}
				else if (life == 4) {
					enemy1->setLife(0);
					enemy1sToDelete->addObject(enemy1);
					this->planeLayer->setLife(3);
					this->planeLayer->getChildByTag(750)->setVisible(0);
				}
				else if(life==3){
					enemy1->setLife(0);
					enemy1sToDelete->addObject(enemy1);
					this->planeLayer->setLife(2);
					this->planeLayer->getChildByTag(749)->setVisible(0);
				}
				else if (life == 2) {
					enemy1->setLife(0);
					enemy1sToDelete->addObject(enemy1);
					this->planeLayer->setLife(1);
					this->planeLayer->getChildByTag(748)->setVisible(0);
				}else {
					this->planeLayer->setLife(0);
					for (auto node : this->getChildren())
						node->stopAllActions();
					this->enemyLayer->stop();
					//this->unscheduleAllSelectors();
					this->bulletLayer->StopShoot();
					this->mutiBulletsLayer->StopShoot();
					this->threeBulletsLayer->StopShoot();
					this->ebulletLayer->StopShoot();
					
					this->planeLayer->Blowup(score);
					this->unscheduleAllSelectors();
					return;
				}
				CCARRAY_FOREACH(enemy1sToDelete, et)
				{
					auto enemy1 = (Enemy*)et;
					this->enemyLayer->enemy1Blowup(enemy1);
				}
				enemy1sToDelete->release();
			}
		}
	}

	//enemy2 & airplane CheckCollosion
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2, et)
	{
		auto enemy2sToDelete = __Array::create();
		enemy2sToDelete->retain();
		auto enemy2 = (Enemy*)et;
		if (enemy2->getLife() > 0)
		{
			if (airplaneRect.intersectsRect(enemy2->getBoundingBox()) && !god)
			{
				if (coinPR) {
					coinPR = 0;
					enemy2->setLife(0);
					enemy2sToDelete->addObject(enemy2);
					PRcoin->setVisible(0);
					UserDefault::getInstance()->setBoolForKey(SHIELD_KEY, false);
				}
				else if (life == 4) {
					enemy2->setLife(0);
					enemy2sToDelete->addObject(enemy2);
					this->planeLayer->setLife(3);
					this->planeLayer->getChildByTag(750)->setVisible(0);
				}
				else if (life == 3) {
					enemy2->setLife(0);
					enemy2sToDelete->addObject(enemy2);
					this->planeLayer->setLife(2);
					this->planeLayer->getChildByTag(749)->setVisible(0);
				}
				else if (life == 2) {
					enemy2->setLife(0);
					enemy2sToDelete->addObject(enemy2);
					this->planeLayer->setLife(1);
					this->planeLayer->getChildByTag(748)->setVisible(0);
				}
				else {
					this->planeLayer->setLife(0);
					for (auto node : this->getChildren())
						node->stopAllActions();
					this->enemyLayer->stop();
					//this->unscheduleAllSelectors();
					this->bulletLayer->StopShoot();
					this->mutiBulletsLayer->StopShoot();
					this->threeBulletsLayer->StopShoot();
					
					ebulletLayer->StopShoot();
					this->planeLayer->Blowup(score);
					this->unscheduleAllSelectors();
					return;
				}
				CCARRAY_FOREACH(enemy2sToDelete, et)
				{
					auto enemy2 = (Enemy*)et;
					this->enemyLayer->enemy2Blowup(enemy2);
				}
				enemy2sToDelete->release();
			}
		}
	}

	//enemy3 & airplane CheckCollosion
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3, et)
	{
		auto enemy3sToDelete = __Array::create();
		enemy3sToDelete->retain();
		auto enemy3 = (Enemy*)et;
		if (enemy3->getLife() > 0)
		{
			if (airplaneRect.intersectsRect(enemy3->getBoundingBox()) && !god)
			{
				if (coinPR) {
					coinPR = 0;
					enemy3->setLife(0);
					enemy3sToDelete->addObject(enemy3);
					PRcoin->setVisible(0);
					UserDefault::getInstance()->setBoolForKey(SHIELD_KEY, false);
				}
				else if (life == 4) {
					enemy3->setLife(0);
					enemy3sToDelete->addObject(enemy3);
					this->planeLayer->setLife(3);
					this->planeLayer->getChildByTag(750)->setVisible(0);
				}
				else if (life == 3) {
					enemy3->setLife(0);
					enemy3sToDelete->addObject(enemy3);
					this->planeLayer->setLife(2);
					this->planeLayer->getChildByTag(749)->setVisible(0);
				}
				else if (life == 2) {
					enemy3->setLife(0);
					enemy3sToDelete->addObject(enemy3);
					this->planeLayer->setLife(1);
					this->planeLayer->getChildByTag(748)->setVisible(0);
				}
				else {
					this->planeLayer->setLife(0);
					for (auto node : this->getChildren())
						node->stopAllActions();
					this->enemyLayer->stop();
					//this->unscheduleAllSelectors();
					this->bulletLayer->StopShoot();
					this->mutiBulletsLayer->StopShoot();
					this->threeBulletsLayer->StopShoot();
					this->ebulletLayer->StopShoot();
					
					this->planeLayer->Blowup(score);
					this->unscheduleAllSelectors();
					return;
				}
				CCARRAY_FOREACH(enemy3sToDelete, et)
				{
					auto enemy3 = (Enemy*)et;
					this->enemyLayer->enemy3Blowup(enemy3);
				}
				enemy3sToDelete->release();
			}
		}
	}

	CCARRAY_FOREACH(this->ufoLayer->m_pAllMutiBullets, ut)
	{
		auto mutiBullets = (Sprite*)ut;
		if (this->planeLayer->getChildByTag(747)->boundingBox().intersectsRect(mutiBullets->boundingBox()))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("get_double_laser.mp3");
			this->ufoLayer->RemoveMutiBullets(mutiBullets);
			this->bulletLayer->StopShoot();
			this->mutiBulletsLayer->StartShoot();
			this->bulletLayer->StartShoot(6.2f);
		}
	}
	auto coinToDel = __Array::create();
	coinToDel->retain();
	CCARRAY_FOREACH(m_pAllCoin, ut)
	{
		auto coin = (Sprite*)ut;
		if (this->planeLayer->getChildByTag(747)->boundingBox().intersectsRect(coin->boundingBox()))
		{
			//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("get_double_laser.mp3");
			coinToDel->addObject(coin);
			coinSC++;
			if (coinSC == 10) {
				coinSC = 0;
				coinPR = 1;
				UserDefault::getInstance()->setBoolForKey(SHIELD_KEY, true);

			}
			this->planeLayer->m_pProgressView1->setCurrentProgress(coinSC);
		}
	}
	CCARRAY_FOREACH(coinToDel, ut)
	{
		auto coin = (Sprite*)ut;
		if (coin != nullptr)
		{
			m_pAllCoin->removeObject(coin);
			this->removeChild(coin, true);
		}
	}
	coinToDel->removeAllObjects();

	CCARRAY_FOREACH(this->ufoLayer->m_pAllThreeBullets, ut)
	{
		auto threeBullets = (Sprite*)ut;
		if (this->planeLayer->getChildByTag(747)->boundingBox().intersectsRect(threeBullets->boundingBox()))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("get_double_laser.mp3");
			this->ufoLayer->RemoveThreeBullets(threeBullets);
			this->bulletLayer->StopShoot();
			this->threeBulletsLayer->StartShoot();
			this->bulletLayer->StartShoot(0.2f);
		}
	}

	//bigBoom & airplane CheckCollision
	CCARRAY_FOREACH(this->ufoLayer->m_pAllBigBoom, ut)
	{
		auto bigBoom = (Sprite*)ut;
		if (this->planeLayer->getChildByTag(747)->boundingBox().intersectsRect(bigBoom->boundingBox()))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("get_bomb.mp3");
			this->ufoLayer->RemoveBigBoom(bigBoom);
			bigBoomCount++;
			//UserDefault::getInstance()->setIntegerForKey(BOMB_KEY, bigBoomCount);//

			updateBigBoomItem(bigBoomCount);
		}
	}

	CCARRAY_FOREACH(this->ufoLayer->m_pAllLife, ut)
	{
		auto Life = (Sprite*)ut;
		if (this->planeLayer->getChildByTag(747)->boundingBox().intersectsRect(Life->boundingBox()))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("get_bomb.mp3");
			this->ufoLayer->RemoveLife(Life);
			life++;
			updateLife(life);
		}
	}

	Ref* hu;
	CCARRAY_FOREACH(this->ufoLayer->m_pAllHouse, hu)
	{
		auto Life = (Sprite*)hu;
		if (this->planeLayer->getChildByTag(747)->boundingBox().intersectsRect(Life->boundingBox()))
		{
			this->planeLayer->setLife(0);
			for (auto node : this->getChildren())
				node->stopAllActions();
			this->enemyLayer->stop();
			//this->unscheduleAllSelectors();
			this->bulletLayer->StopShoot();
			this->mutiBulletsLayer->StopShoot();
			this->threeBulletsLayer->StopShoot();
			this->ebulletLayer->StopShoot();
			for (auto node : this->ufoLayer->getChildren())
				node->stopAllActions(); 
			this->planeLayer->Blowup(score);
			this->unscheduleAllSelectors();
			return;
		}
	}

	Ref* ebt;
	auto eBulletsToDelete = __Array::create();
	eBulletsToDelete->retain();
	//enemy2 & mutiBullets CheckCollosion
	CCARRAY_FOREACH(this->ebulletLayer->m_pAllEBullets, ebt)
	{
		auto eBullets = (Sprite*)ebt;
		auto plane = this->planeLayer;
		if (eBullets->boundingBox().intersectsRect(this->planeLayer->getChildByTag(747)->boundingBox()) && !god)
			{

				int now = plane->getBlood();
				if (coinPR) {
					coinPR = 0;
					PRcoin->setVisible(0);
					UserDefault::getInstance()->setBoolForKey(SHIELD_KEY, false);
				}
				else if (plane->getBlood() > 0)
				{
					now -= 1;
					plane->m_pProgressView->setCurrentProgress(now);
					plane->setBlood(now);
				}
				else if (plane->getBlood() ==0)
				{
					auto life = plane->getLife();
					if (life == 4) {
						now = 10;
						plane->setBlood(now);
						plane->m_pProgressView->setCurrentProgress(now);
						this->planeLayer->setLife(3);
						this->planeLayer->getChildByTag(750)->setVisible(0);
					}
					else if (life == 3) {
						now = 10;
						plane->setBlood(now);
						plane->m_pProgressView->setCurrentProgress(now);
						this->planeLayer->setLife(2);
						this->planeLayer->getChildByTag(749)->setVisible(0);
					}
					else if (life == 2) {
						now = 10;
						plane->setBlood(now);
						plane->m_pProgressView->setCurrentProgress(now);
						this->planeLayer->setLife(1);
						this->planeLayer->getChildByTag(748)->setVisible(0);
					}
					else {
						this->planeLayer->setLife(0);
						plane->m_pProgressView->setCurrentProgress(0);
						for (auto node : this->getChildren())
							node->stopAllActions();
						this->enemyLayer->stop();
						this->bulletLayer->StopShoot();
						this->mutiBulletsLayer->StopShoot();
						this->threeBulletsLayer->StopShoot();
						this->ebulletLayer->StopShoot();
						this->enemyLayer->stop();
						this->unscheduleAllSelectors();
						this->planeLayer->Blowup(score);
						
						return;
					}
				}
				eBulletsToDelete->addObject(eBullets);
			
		
		}
	}
	CCARRAY_FOREACH(eBulletsToDelete, ebt)
	{
		auto eBullets = (Sprite*)ebt;
		this->ebulletLayer->RemoveEBullets(eBullets);
	}
	eBulletsToDelete->removeAllObjects();
}
void GameLayer::updateLife(int lifeCount) {
	if (lifeCount >= 3) {
		this->planeLayer->setLife(3);
		this->planeLayer->getChildByTag(750)->setVisible(true);
		this->planeLayer->getChildByTag(749)->setVisible(1);
		this->planeLayer->getChildByTag(748)->setVisible(1);
	}
	else if (lifeCount == 2) {
		this->planeLayer->getChildByTag(750)->setVisible(0);
		this->planeLayer->getChildByTag(749)->setVisible(1);
		this->planeLayer->setLife(2);
		this->planeLayer->getChildByTag(748)->setVisible(1);
	}
	else if (lifeCount == 1) {
		this->planeLayer->getChildByTag(750)->setVisible(0);
		this->planeLayer->getChildByTag(749)->setVisible(0);
		this->planeLayer->setLife(1);
		this->planeLayer->getChildByTag(748)->setVisible(1);
	}
}

void GameLayer::updateBigBoomItem(int bigBoomCount)
{
	auto normalBomb = Sprite::createWithSpriteFrameName("bomb.png");
	auto pressedBomb = Sprite::createWithSpriteFrameName("bomb.png");
	if (bigBoomCount < 0)
	{
		return;
	}
	else if (bigBoomCount == 0)
	{
		if (this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			this->removeChildByTag(TAG_BIGBOOM_MENUITEM, true);
		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL, true);
		}
	}
	else if (bigBoomCount == 1)
	{
		if (!this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			auto pBigBoomItem = MenuItemSprite::create(normalBomb, pressedBomb, nullptr, CC_CALLBACK_1(GameLayer::menuBigBoomCallback, this));
			pBigBoomItem->setPosition(Point(normalBomb->getContentSize().width / 2 + 10, normalBomb->getContentSize().height / 2 + 10));
			menuBigBoom = Menu::create(pBigBoomItem, nullptr);
			menuBigBoom->setPosition(Point::ZERO);
			this->addChild(menuBigBoom, 0, TAG_BIGBOOM_MENUITEM);
		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL, true);
		}
	}
	else
	{
		if (!this->getChildByTag(TAG_BIGBOOM_MENUITEM))
		{
			auto pBigBoomItem = MenuItemSprite::create(normalBomb, pressedBomb, nullptr, CC_CALLBACK_1(GameLayer::menuBigBoomCallback, this));
			pBigBoomItem->setPosition(Point(normalBomb->getContentSize().width / 2 + 10, normalBomb->getContentSize().height / 2 + 10));
			menuBigBoom = Menu::create(pBigBoomItem, nullptr);
			menuBigBoom->setPosition(Point::ZERO);
			this->addChild(menuBigBoom, 0, TAG_BIGBOOM_MENUITEM);
		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT_LABEL))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT_LABEL, true);
		}
		if (bigBoomCount >= 0 && bigBoomCount <= MAX_BIGBOOM_COUNT)
		{
			auto strScore = __String::createWithFormat("X%d", bigBoomCount);
			bigBoomCountItem = Label::createWithBMFont("font.fnt", strScore->getCString());
			bigBoomCountItem->setColor(Color3B(143, 146, 147));
			bigBoomCountItem->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
			bigBoomCountItem->setPosition(Point(normalBomb->getContentSize().width + 15, normalBomb->getContentSize().height / 2 + 5));
			this->addChild(bigBoomCountItem, 0, TAG_BIGBOOMCOUNT_LABEL);
		}
	}
}

void GameLayer::menuBigBoomCallback(Ref* pSender)
{
	if (bigBoomCount > 0 && !Director::getInstance()->isPaused())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("use_bomb.mp3");
		bigBoomCount--;
		UserDefault::getInstance()->setIntegerForKey(BOMB_KEY, bigBoomCount);//

		score += this->enemyLayer->m_pAllEnemy1->count() * ENEMY1_SCORE;
		score += this->enemyLayer->m_pAllEnemy2->count() * ENEMY2_SCORE;
		score += this->enemyLayer->m_pAllEnemy3->count() * ENEMY3_SCORE;
		this->enemyLayer->removeAllEnemy();
		updateBigBoomItem(bigBoomCount);
		this->controlLayer->updateScore(score);
	}
}

Level GameLayer::getCurLevel()
{
	return level;
}
void GameLayer::randomBomb(float) {
	auto hero = this->planeLayer->getChildByTag(747);
	auto randomBomb = Sprite::createWithSpriteFrameName("aim_n1.png");
	auto aniRandomPrepare = Animation::create();
	aniRandomPrepare->addSpriteFrameWithFileName("aim_n2.png");
	aniRandomPrepare->addSpriteFrameWithFileName("aim_n1.png");
	aniRandomPrepare->setDelayPerUnit(0.1f);
	aniRandomPrepare->setLoops(10);

	auto aniRandomBomb = Animation::create();
	for (int i = 1; i <= 9; i++) {
		auto frameName = StringUtils::format("bomb_down_n%d.png", i);
		aniRandomBomb->addSpriteFrameWithFileName(frameName);
	}
	aniRandomBomb->setDelayPerUnit(0.1f);
	aniRandomBomb->setLoops(1);


	auto animate1 = Animate::create(aniRandomPrepare);
	auto animate2 = Animate::create(aniRandomBomb);
	int life = this->planeLayer->getLife();
	auto seq = Sequence::create(animate1, animate2,
		CallFuncN::create([=](Ref*) {
		if (randomBomb->getBoundingBox().intersectsRect(hero->getBoundingBox()) && !god) {
			this->removeChild(randomBomb);
			if (life==4) {
				this->planeLayer->setLife(3);
				this->planeLayer->getChildByTag(750)->setVisible(0);
			}
			else if (life == 3) {
				this->planeLayer->setLife(2);
				this->planeLayer->getChildByTag(749)->setVisible(0);
			}
			else if (life == 2) {
				this->planeLayer->setLife(1);
				this->planeLayer->getChildByTag(748)->setVisible(0);
			}
			else {
				this->planeLayer->setLife(0);
				for (auto node : this->getChildren())
					node->stopAllActions();
				this->enemyLayer->stop();
				this->bulletLayer->StopShoot();
				this->mutiBulletsLayer->StopShoot();
				this->threeBulletsLayer->StopShoot();
				this->ebulletLayer->StopShoot();
				this->enemyLayer->stop();
				this->planeLayer->Blowup(score);
				this->unscheduleAllSelectors();
				return;
			}
		}
		}
	), nullptr);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	randomBomb->runAction(seq);
	float minX = randomBomb->getContentSize().width / 2;
	float maxX = visibleSize.width - minX;
	float minY = randomBomb->getContentSize().height / 2;
	float maxY = visibleSize.width - minY;
	float randomx = rand() % (int)(maxX - minX + 1) + minX;
	float randomy = rand() % (int)(maxY - minY + 1) + minY;
	randomBomb->setPosition(randomx, randomy);
	this->addChild(randomBomb,99);
}

void GameLayer::aimedMove(Sprite * bullet) {
	int nullenemy = 0;
	//if (enemies.size() == 0)	bullet->setPositionY(bullet->getPositionY() + 10);
	Ref *et;
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy3, et)
	{
		if (this->enemyLayer->m_pAllEnemy3 == nullptr)	nullenemy++;
		auto enemy3 = (Enemy*)et;
		float speed = 2 * 10;									//提升追踪子弹速度
		auto hero = this->planeLayer->getChildByTag(747);
		float deltax = enemy3->getPositionX() - hero->getPositionX();
		float deltay = enemy3->getPositionY() - hero->getPositionY();
		float angleRadians = atan(deltay / deltax);
		float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);
		bullet->setRotation(90 + angleDegrees);
		auto moveAction = MoveTo::create(sqrt(pow(hero->getPositionX() - enemy3->getPositionX(), 2) +
			pow(hero->getPositionY() - enemy3->getPositionY(), 2)) / speed, enemy3->getPosition());
		Sequence * seq = Sequence::create(moveAction, nullptr);
		bullet->runAction(seq);
	}
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy2, et)
	{
		if (this->enemyLayer->m_pAllEnemy2 == nullptr)	nullenemy++;
		auto enemy2 = (Enemy*)et;
		float speed = 2 * 10;									//提升追踪子弹速度
		auto hero = this->planeLayer->getChildByTag(747);
		float deltax = enemy2->getPositionX() - hero->getPositionX();
		float deltay = enemy2->getPositionY() - hero->getPositionY();
		float angleRadians = atan(deltay / deltax);
		float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);
		bullet->setRotation(90 + angleDegrees);
		auto moveAction = MoveTo::create(sqrt(pow(hero->getPositionX() - enemy2->getPositionX(), 2) +
			pow(hero->getPositionY() - enemy2->getPositionY(), 2)) / speed, enemy2->getPosition());
		Sequence * seq = Sequence::create(moveAction, nullptr);
		bullet->runAction(seq);
	}
	CCARRAY_FOREACH(this->enemyLayer->m_pAllEnemy1, et)
	{
		if (this->enemyLayer->m_pAllEnemy1 == nullptr)	nullenemy++;
		auto enemy1 = (Enemy*)et;
		float speed = 2 * 10;									//提升追踪子弹速度
		auto hero = this->planeLayer->getChildByTag(747);
		float deltax = enemy1->getPositionX() - hero->getPositionX();
		float deltay = enemy1->getPositionY() - hero->getPositionY();
		float angleRadians = atan(deltay / deltax);
		float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);
		bullet->setRotation(90 + angleDegrees);
		auto moveAction = MoveTo::create(sqrt(pow(hero->getPositionX() - enemy1->getPositionX(), 2) +
			pow(hero->getPositionY() - enemy1->getPositionY(), 2)) / speed, enemy1->getPosition());
		Sequence * seq = Sequence::create(moveAction, nullptr);
		bullet->runAction(seq);
	}
	if (nullenemy == 3)	bullet->setPositionY(bullet->getPositionY() + 10);
}
