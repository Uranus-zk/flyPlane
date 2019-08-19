#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class MyDropdownButton : public cocos2d::CCSprite, public TableViewDataSource, public TableViewDelegate
{
public:
	MyDropdownButton(void);
	~MyDropdownButton(void);


	bool init(const char* _filePath);
	static MyDropdownButton* create(const char* _filePath);

	virtual void onExit();
	virtual void onEnterTransitionDidFinish();

private:
	bool					m_bBoxVisible;//��������ʾ
	bool					m_bEnable;//�Ƿ��ܹ����
	bool					m_bBoxClicked;//box�Ƿ񱻵��
	std::string				m_filePath;//��ť���ļ�·��
	vector<std::string>		m_vec_listName;//�������е���ʾ������
	vector<std::string>		m_vec_listValue;//����������ʾ���������Ӧ��ֵ��
	CCTableView*			m_tableView;//tableview

public:
	void setBox(const char* _filePath);//����������
	void setBoxListName(vector<std::string> _vec_listName);//�����������е���ʾ������
	void setBoxListValue(vector<std::string> _vec_listValue);//��������������ʾ���������Ӧ��ֵ��
	void setBoxVisible(bool _bVisible);
	void setEnabled(bool _bEnabled);
	bool getBoxVisible() { return m_bBoxVisible; };
	bool isEnabled() { return m_bEnable; };
	bool isBoxClicked() { return m_bBoxClicked; };
	void setBoxClicked(bool _bBoxClicked) { m_bBoxClicked = _bBoxClicked; };
	CCRect getBoxRect();

private:
	//tableview Ҫʵ�ֵĺ���
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

};
