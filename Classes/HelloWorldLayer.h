#pragma once
#ifndef __HELLO_WORLD_H__
#define	__HELLO_WORLD_H__


#include "cocos2d.h"
#include "SetLayer.h"
USING_NS_CC;

class HelloWorldLayer :public Layer
{
public:
	static int A;
	static int B;
	static int C;
	virtual bool init();
	CREATE_FUNC(HelloWorldLayer);
	void loadingDone(Node* pNode);
	bool isHaveSaveFile();
	void getHighestHistorySorce();
};
#endif // !__HELLO_WORLD_H__