#ifndef _SHOP_SCENE_H_
#define _SHOP_SCENE_H_


#include "cocos2d.h"

USING_NS_CC;

class ShopScene :public Scene
{
private:
	int money;

	bool shield;
	int bomb;
public:

	bool init();
	//����һ������GameScene���Layer�ĳ���
	static Scene* createScene();
	void update(float);

};






#endif
