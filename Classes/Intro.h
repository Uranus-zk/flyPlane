#pragma once
#ifndef __INTRO_H__
#define	__INTRO_H__


#include "cocos2d.h"
USING_NS_CC;

class Intro : public Scene
{
public:
	static Intro* createScene();
	bool init() override;
	CREATE_FUNC(Intro);

};

#endif // !__Intro_H__