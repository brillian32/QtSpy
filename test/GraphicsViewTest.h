//
// Created by 17305 on 2024/11/28.
//

#ifndef GRAPHICSVIEWTEST_H
#define GRAPHICSVIEWTEST_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>


class MovableRect: public QGraphicsRectItem
{
public:
	MovableRect(qreal x, qreal y, qreal w, qreal h) : QGraphicsRectItem(x, y, w, h)
	{
		setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		// 设置接受悬停事件
		setAcceptHoverEvents(true);
	}

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override
	{
		// 设置当前项为选中状态
		this->setSelected(true);
		QGraphicsRectItem::mousePressEvent(event);
	}

	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override
	{
		// 取消选中状态
		this->setSelected(false);
		QGraphicsRectItem::mouseReleaseEvent(event);
	}
};

class GraphicsViewTest: public QGraphicsView
{
	Q_OBJECT
public:
	explicit GraphicsViewTest(QWidget *parent = nullptr)
	{
		// 创建场景
		auto *scene = new QGraphicsScene(this);
		setScene(scene);

		// 添加可移动矩形
		auto *rect1 = new MovableRect(50, 50, 100, 50);
		auto *rect2 = new MovableRect(200, 50, 100, 50);
		auto *rect3 = new MovableRect(350, 50, 100, 50);
		auto *rect4 = new MovableRect(350, 50, 50, 25);

		rect4->setParentItem(rect3);
		scene->addItem(rect1);
		scene->addItem(rect2);
		scene->addItem(rect3);
	}
	~GraphicsViewTest() override
	= default;
};

#endif//GRAPHICSVIEWTEST_H
