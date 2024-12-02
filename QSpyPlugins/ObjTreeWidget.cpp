//
// Created by 17305 on 2024/11/17.
//

#include "ObjTreeWidget.h"
#include "QGraphicsItem.h"
#include "QGraphicsScene.h"
#include "qapplication.h"
#include "qdebug.h"
#include "qgraphicsview.h"
#include <QDesktopWidget>

ObjTreeWidget::ObjTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
	// 设置右键菜单策略
	setContextMenuPolicy(Qt::CustomContextMenu);
	m_menu = new TreeObjMenu(this);
	m_menu->hide();
	QDesktopWidget *desktop = QApplication::desktop();
	QRect screenRect = desktop->screenGeometry();
	int x = screenRect.center().x();
	int y = screenRect.center().y();
	setGeometry(x, y, 300, 300);

	setObjectName("infoTreeWidget");
	setHeaderLabel("Widget Tree:");

	connect(this, &ObjTreeWidget::itemClicked, this, [this](QTreeWidgetItem *item, int column) {
		auto data = item->data(column, Qt::UserRole);
		if (data.canConvert<QWidget *>())
		{
			auto wid = data.value<QWidget *>();
			qDebug() << "clicked:" << wid;
			sigUpdateCurWidget(wid);
		}
	});

	connect(this, &ObjTreeWidget::customContextMenuRequested, [this](const QPoint &pos) {
		qDebug() << pos;
		auto item = itemAt(pos);
		auto data = item->data(0, Qt::UserRole);
		if (data.canConvert<QWidget *>())
		{
			auto wid = data.value<QWidget *>();
			sigUpdateCurWidget(wid);
		}
		m_menu->move(mapToGlobal(pos));
		m_menu->show();
	});
}

QWidget *ObjTreeWidget::getTopLevelWidget(QWidget *widget)
{
	if (!widget)
	{
		return nullptr;// 如果传入的控件为空，直接返回
	}

	QWidget *parent = widget;
	while (parent->parentWidget())
	{
		parent = parent->parentWidget();// 一直寻找父窗口
	}
	return parent;// 返回最顶层的父窗口
}


void ObjTreeWidget::updateTree(QWidget *widget)
{
	clear();
	if (auto view = qobject_cast<QGraphicsView *>(widget))
	{
		updateObjectTree(widget, this);
		expandAll();
		return;
	}
	m_curWidget = widget;
	auto topLevelWidget = getTopLevelWidget(widget);
	updateObjectTree(topLevelWidget, this);
	expandAll();
}

void ObjTreeWidget::onGetInfo(QList<QPair<QString, QString>> &info)
{
	m_menu->setWinInfo(info);
}

template<typename T>
void ObjTreeWidget::updateObjectTree(QObject *obj, T parentItem)
{
	//QGraphicsScene
	if (auto view = qobject_cast<QGraphicsView *>(obj))
	{
		if (auto scene = view->scene())
		{
			addItemsFromScene(scene, (QTreeWidget *) parentItem);
			return;
		}
	}

	//qwidget
	if (auto widget = qobject_cast<QWidget *>(obj))
	{
		auto *item = new QTreeWidgetItem(parentItem);
		item->setText(0, widget->metaObject()->className() + QString(" | ") + widget->objectName());
		item->setData(0, Qt::UserRole, QVariant::fromValue(widget));
		item->setBackground(0, QBrush(Qt::white));
		if (widget == m_curWidget)
		{
			item->setSelected(true);
			scrollToItem(item);
		}
		for (QObject *child : widget->children())
		{
			if (QWidget *childWidget = qobject_cast<QWidget *>(child))
			{
				updateObjectTree(childWidget, item);
			}
		}
	}
}


template<typename T>
void ObjTreeWidget::addItemsToTree(QGraphicsItem *item, T treeWidget)
{
	auto *newItem = new QTreeWidgetItem(treeWidget);
	auto itemName = QString(typeid(*item).name());
	itemName.remove("class ");
	newItem->setText(0, itemName);
	newItem->setBackground(0, QBrush(Qt::white));

	foreach (QGraphicsItem *child, item->childItems())
	{
		addItemsToTree(child, newItem);
	}
}

void ObjTreeWidget::addItemsFromScene(QGraphicsScene *scene, QTreeWidget *treeWidget)
{
	treeWidget->clear();
	QList<QGraphicsItem *> items = scene->items();

	foreach (QGraphicsItem *item, items)
	{
		if (!item->parentItem())
		{// Only add top-level items
			addItemsToTree(item, treeWidget);
		}
	}
}

ObjTreeWidget::~ObjTreeWidget() = default;
