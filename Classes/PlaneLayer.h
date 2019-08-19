#pragma once
#ifndef __PLANE_LAYER_H__
#define __PLANE_LAYER_H__
#include "cocos2d.h"
#include "progress.h"
#include "GameOverScene.h"
USING_NS_CC;

class PlaneLayer :public Layer {
public:
	PlaneLayer();
	~PlaneLayer();
	static PlaneLayer* createLayer();
	virtual bool init();
	void Blowup(int);
	void RemovePlane();
	int getLife();
	void setLife(int now);
	int getBlood();
	void setBlood(int now);
	//CREATE_FUNC(PlaneLayer);
public:
	ProgressView* m_pProgressView;
	ProgressView* m_pProgressView1;
	static PlaneLayer* sharedPlane;
	int isAlive;
	int score;
	int blood;
};
#endif // !__PLANE_LAYER_H__
