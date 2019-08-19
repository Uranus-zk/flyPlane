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
	bool					m_bBoxVisible;//下拉框显示
	bool					m_bEnable;//是否能够点击
	bool					m_bBoxClicked;//box是否被点击
	std::string				m_filePath;//按钮的文件路径
	vector<std::string>		m_vec_listName;//下拉框中的显示的名字
	vector<std::string>		m_vec_listValue;//下拉框中显示的名字相对应的值。
	CCTableView*			m_tableView;//tableview

public:
	void setBox(const char* _filePath);//设置下拉框
	void setBoxListName(vector<std::string> _vec_listName);//设置下拉框中的显示的名字
	void setBoxListValue(vector<std::string> _vec_listValue);//设置下拉框中显示的名字相对应的值。
	void setBoxVisible(bool _bVisible);
	void setEnabled(bool _bEnabled);
	bool getBoxVisible() { return m_bBoxVisible; };
	bool isEnabled() { return m_bEnable; };
	bool isBoxClicked() { return m_bBoxClicked; };
	void setBoxClicked(bool _bBoxClicked) { m_bBoxClicked = _bBoxClicked; };
	CCRect getBoxRect();

private:
	//tableview 要实现的函数
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

};
