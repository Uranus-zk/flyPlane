#pragma once
#include "cocos2d.h"
#include "HelloWorldLayer.h"
#ifndef DropDownListBox_DROPDOWNLIST_h
#define DropDownListBox_DROPDOWNLIST_h

USING_NS_CC;

namespace CustomDropDownListBox
{
	//���ñ�����ɫ
#define DROPDOWNLIST_NORMAL_COLOR       ccc4(128, 128, 128, 255)
#define DROPDOWNLIST_SELECTED_COLOR     ccc4(200, 200, 200, 255)
#define DROPDOWNLIST_HIGHLIGHT_COLOR    ccc4(0, 0, 255, 255)

#define DROPDOWNLIST_NORMAL_COLOR3       ccc3(128, 128, 128)
#define DROPDOWNLIST_SELECTED_COLOR3     ccc3(200, 200, 200)
#define DROPDOWNLIST_HIGHLIGHT_COLOR3    ccc3(0, 0, 0)

	class DropDownList : public CCLayer
	{
	public:
		//���췽��
		DropDownList(CCLabelTTF* label, CCSize size)
			: showLabel(label)
			, isShowMenu(false)
			, lastSelectedIndex(0)
		{
			//������һ��menu���������ڻ�����ӵ���ͼ��
			mainMenu = CCMenu::create();
			mainMenu->setPosition(CCPoint(size.width / 2, size.height / 2));
			mainMenu->retain();

			showLabel->setPosition(CCPoint(size.width / 2, size.height / 2));
			addChild(showLabel);

			setContentSize(size);
		}

		//��������
		~DropDownList()
		{
			mainMenu->release();
		}

		//����ʵ�����󷽷�
		static DropDownList* create(CCLabelTTF* label, CCSize size)
		{
			DropDownList* list = new DropDownList(label, size);
			list->autorelease();
			return list;
		}
		//��ȡ��ǰѡ��label��string
		std::string getString()
		{
			return showLabel->getString();
		}

		//��ȡ��ǰѡ�е�index
		int getSelectedIndex()
		{
			return lastSelectedIndex;
		}

		//����ѡ��index
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

					//�������ʾ�����б��Ҳ����mainMenu
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

		//������menu item �����һ��label���ǵ�����
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

		//ѡ�������б��
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
			onClose(); //�ر������б��
		}

		//�ر������б��
		void onClose()
		{
			removeChild(mainMenu, true);  //ͨ��ɾ��mainMenu,�ر������б��
			isShowMenu = false;
		}
	private:
		CCMenu * mainMenu;  //�����б�ѡ��ļ���

		CCLabelTTF* showLabel;  //��ʾѡ�еĽ��

		std::vector<CCLabelTTF*> selectLabels;  //�����б�label

		std::vector<CCLayerColor*> bgLayers; //�������� ����

		bool isShowMenu;  //�Ƿ���ʾ�������б�

		int lastSelectedIndex;  //ѡ�������б��index

	};

}

#endif