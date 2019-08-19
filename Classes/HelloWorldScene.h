#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "HelloWorldLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class HelloWorldScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	void PreloadMusic();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorldScene);
};

#endif // __HELLOWORLD_SCENE_H__
