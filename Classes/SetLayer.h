#pragma once


#include "cocos2d.h"

USING_NS_CC;

class SetLayer :public Layer {
public:
	SetLayer();
	~SetLayer();
	static Scene* createScene();
	CREATE_FUNC(SetLayer);
	bool init();
	static int A;
	static int B;

};