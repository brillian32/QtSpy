//
// Created by 17305 on 2024/11/17.
//

#ifndef OBJTREEWIDGET_H
#define OBJTREEWIDGET_H
#include "QTreeWidget.h"

class ObjTreeWidget: public QTreeWidget
{
	Q_OBJECT
public:
	explicit ObjTreeWidget(QWidget *parent = nullptr);
	~ObjTreeWidget() override;

	void updateTree(QWidget *widget);
signals:
	void sigUpdateCurWidget(QWidget *wid);

private:
	template<typename T>
	void updateObjectTree(QWidget *widget, T parentItem);
	static QWidget *getTopLevelWidget(QWidget *widget);

	QWidget* m_curWidget{};
};


#endif//OBJTREEWIDGET_H
