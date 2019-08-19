#pragma once
#ifndef __PROGRESSVIEW_H__
#define __PROGRESSVIEW_H__

#include "cocos2d.h"
using namespace cocos2d;

class ProgressView : public Node
{
public:
	ProgressView();

public:
	void setBackgroundTexture(const char *pName);
	void setForegroundTexture(const char *pName);
	void setTotalProgress(float total);
	void setCurrentProgress(float progress);
	float getCurrentProgress() const;
	float getTotalProgress() const;

private:
	void setForegroundTextureRect(const Rect &rect);

private:
	Sprite * m_progressBackground;
	Sprite *m_progressForeground;
	float m_totalProgress;
	float m_currentProgress;
	float m_scale;
};

#endif