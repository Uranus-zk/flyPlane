#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorldScene::createScene()
{
    return HelloWorldScene::create();
}

// on "init" you need to initialize your instance
bool HelloWorldScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
	auto pHelloWorldLayer = HelloWorldLayer::create();
	if (!pHelloWorldLayer)	return false;
	this->addChild(pHelloWorldLayer, -128, "HL");
	PreloadMusic();
    

    
    return true;
}



void HelloWorldScene::PreloadMusic()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("game_music.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("bullet.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("enemy1_down.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("enemy2_down.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("enemy3_down.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("game_over.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("get_bomb.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("get_double_laser.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("use_bomb.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("big_spaceship_flying.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("achievement.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("out_porp.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("button.mp3");

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("game_music.mp3", true);
}