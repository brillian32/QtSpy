//
// Created by 17305 on 2024/11/22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TreeObjMenu.h" resolved

#include "TreeObjMenu.h"
#include "ui_TreeObjMenu.h"
#include "QClipboard.h"


TreeObjMenu::TreeObjMenu(QWidget *parent) : QMenu(parent), ui(new Ui::TreeObjMenu)
{
	ui->setupUi(this);
	setObjectName("TreeObjMenu");
	// 添加菜单项
	QAction *cpClassName = addAction("拷贝类名");
	QAction *cpObjName = addAction("拷贝对象名");
	QAction *cpStylesheet = addAction("拷贝stylesheet");
	QAction *cpAllAttr = addAction("拷贝全部属性");

	connect(cpClassName, &QAction::triggered, [=](){
		QString className = m_winInfo[0].second;
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(className);
	});
	connect(cpObjName, &QAction::triggered, [=](){
		QString objName = m_winInfo[1].second;
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(objName);
	});
	connect(cpStylesheet, &QAction::triggered, [=](){
		QString stylesheet = m_winInfo[2].second;
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(stylesheet);
	});
	connect(cpAllAttr, &QAction::triggered, [=](){
		QString allAttr;
		for(auto &item : m_winInfo)
		{
			allAttr += item.first + ": " + item.second + "\n";
		}
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(allAttr);
	});
}

TreeObjMenu::~TreeObjMenu()
{
	delete ui;
}
void TreeObjMenu::setWinInfo(QList<QPair<QString, QString>> & info)
{
	m_winInfo = info;
}
