#pragma once
#include "cocos2d.h"
#include "HelloWorldLayer.h"
#ifndef DropDownListBox_DROPDOWNLIST_h
#define DropDownListBox_DROPDOWNLIST_h

USING_NS_CC;

namespace CustomDropDownListBox
{
	//设置背景颜色
#define DROPDOWNLIST_NORMAL_COLOR       ccc4(128, 128, 128, 255)
#define DROPDOWNLIST_SELECTED_COLOR     ccc4(200, 200, 200, 255)
#define DROPDOWNLIST_HIGHLIGHT_COLOR    ccc4(0, 0, 255, 255)

#define DROPDOWNLIST_NORMAL_COLOR3       ccc3(128, 128, 128)
#define DROPDOWNLIST_SELECTED_COLOR3     ccc3(200, 200, 200)
#define DROPDOWNLIST_HIGHLIGHT_COLOR3    ccc3(0, 0, 0)

	class DropDownList : public CCLayer
	{
	public:
		//构造方法
		DropDownList(CCLabelTTF* label, CCSize size)
			: showLabel(label)
			, isShowMenu(false)
			, lastSelectedIndex(0)
		{
			//创建好一个menu，但是现在还不添加到视图中
			mainMenu = CCMenu::create();
			mainMenu->setPosition(CCPoint(size.width / 2, size.height / 2));
			mainMenu->retain();

			showLabel->setPosition(CCPoint(size.width / 2, size.height / 2));
			addChild(showLabel);

			setContentSize(size);
		}

		//析构函数
		~DropDownList()
		{
			mainMenu->release();
		}

		//创建实例对象方法
		static DropDownList* create(CCLabelTTF* label, CCSize size)
		{
			DropDownList* list = new DropDownList(label, size);
			list->autorelease();
			return list;
		}
		//获取当前选中label的string
		std::string getString()
		{
			return showLabel->getString();
		}

		//获取当前选中的index
		int getSelectedIndex()
		{
			return lastSelectedIndex;
		}

		//设置选中index
		void setSelectedIndex(int index)
		{
			lastSelectedIndex = index;

			for (int i = 0, j = (int)selectLabels.size(); i < j; ++i)
			{
				if (i == lastSelectedIndex)
				{
					bgLayers[i]->setColor(DROPDOWNLIST_HIGHLIGHT_COLOR3);
					showLabel->setString(selectLabels[i]->getString());
				}
				else
				{
					bgLayers[i]->setColor(DROPDOWNLIST_NORMAL_COLOR3);
				}
			}
		}

		void onEnter()
		{
			setTouchEnabled(true);
			EventListenerTouchOneByOne* listener =  EventListenerTouchOneByOne::create();
			listener->onTouchBegan = CC_CALLBACK_2(DropDownList::onTouchBegan, this);
			//listener->onTouchEnded = CC_CALLBACK_2(PlayLayer::onTouchEnded, this);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

			CCLayer::onEnter();
		}

		

		virtual bool onTouchBegan(CCTouch *touch, CCEvent *event)
		{
			if (!isShowMenu)
			{
				CCRect rect;
				//rect.origin = CCPointZero;
				rect.size = getContentSize();
				CCPoint position = convertTouchToNodeSpace(touch);

				if (rect.containsPoint(position))
				{
					isShowMenu = true;

					//点击，显示下拉列表框，也就是mainMenu
					addChild(mainMenu);

					for (int i = 0, j = (int)selectLabels.size(); i < j; ++i)
					{
						if (i == lastSelectedIndex)
						{
							bgLayers[i]->setColor(DROPDOWNLIST_HIGHLIGHT_COLOR3);
						}
						else
						{
							bgLayers[i]->setColor(DROPDOWNLIST_NORMAL_COLOR3);
						}
					}



					return true;
				}
			}

			return false;
		}

		//创建以menu item 并添加一个label覆盖到上面
		void addLabel(CCLabelTTF* label)
		{
			CCSize size = getContentSize();

			CCLayerColor* normal = CCLayerColor::create(DROPDOWNLIST_NORMAL_COLOR, size.width, size.height);
			CCLayerColor* selected = CCLayerColor::create(DROPDOWNLIST_SELECTED_COLOR, size.width, size.height);

			bgLayers.push_back(normal);

			selectLabels.push_back(label);

			CCMenuItem* item = CCMenuItemSprite::create(
				normal,
				selected,
				NULL,
				this,
				SEL_MenuHandler(&DropDownList::onSelected)
			);

			label->setPosition(CCPoint(size.width / 2, size.height / 2));
			item->addChild(label);
			item->setTag((int)selectLabels.size() - 1);
			item->setPosition(0, -(int)selectLabels.size() * size.height);

			mainMenu->addChild(item);

		}

		//选中下拉列表后
		void onSelected(CCObject* sender)
		{
			CCMenuItem* item = dynamic_cast<CCMenuItem*>(sender);
			if (item)
			{
				lastSelectedIndex = item->getTag();
				showLabel->setString(selectLabels[item->getTag()]->getString());
				if (selectLabels[item->getTag()]->getString() == "1" || selectLabels[item->getTag()]->getString() == "2" || selectLabels[item->getTag()]->getString() == "3") {
					HelloWorldLayer::A = lastSelectedIndex;
				}
				else {
					HelloWorldLayer::B = lastSelectedIndex;
				}
				//log("%d", HelloWorldLayer::B);
			}
			onClose(); //关闭下拉列表框
		}

		//关闭下拉列表框
		void onClose()
		{
			removeChild(mainMenu, true);  //通过删除mainMenu,关闭下拉列表框
			isShowMenu = false;
		}
	private:
		CCMenu * mainMenu;  //下拉列表选项的集合

		CCLabelTTF* showLabel;  //显示选中的结果

		std::vector<CCLabelTTF*> selectLabels;  //下拉列表label

		std::vector<CCLayerColor*> bgLayers; //用于设置 背景

		bool isShowMenu;  //是否显示了下拉列表

		int lastSelectedIndex;  //选中下拉列表的index

	};

}

#endif