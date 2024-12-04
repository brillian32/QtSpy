//
// Created by 17305 on 2024/11/15.
//

#include "QSpy.h"
#include "QMouseEvent"
#include "qdebug.h"
#include "qgraphicsitem.h"
#include "qgraphicsscene.h"
#include "qgraphicsview.h"
#include "qmap.h"
#include "qwidget.h"
#include <QCoreApplication>
#include <QMetaEnum>


bool QSpy::eventFilter(QObject *obj, QEvent *event)
{
	if(!m_switchState)
	{
		return QObject::eventFilter(obj, event);
	}
	static bool noShow = false;
	static QWidget *curWid = nullptr;
	static QGraphicsScene *curScene = nullptr;
	static QGraphicsItem *curItem = nullptr;

	if (!isListen(obj) && event->type() == QEvent::Enter)
	{
		noShow = true;
		return QObject::eventFilter(obj, event);
	}

	if (!isListen(obj) && event->type() == QEvent::Leave)
	{
		noShow = false;
		return QObject::eventFilter(obj, event);
	}

	if (event->type() == QEvent::Enter)
	{
		if (noShow)
		{
			return QObject::eventFilter(obj, event);
		}

		if (auto *widget = qobject_cast<QWidget *>(obj))
		{
			curWid = widget;
			m_drawInfoWidget->setCurWidget(widget);
			m_drawInfoWidget->updateRectAndInfo();
		}
		else if (auto *Scene = qobject_cast<QGraphicsScene *>(obj))
		{
			curScene = Scene;
			m_drawInfoWidget->setCurScene(Scene);
			m_drawInfoWidget->updateRectAndInfo();
		}
		else
		{
			//			qWarning() << "Object do not get information : " << obj->objectName() << "  " << obj->metaObject()->className() << " " << obj;
		}
	}

	if (event->type() == QEvent::GraphicsSceneMouseMove)
	{
		if (auto *scene = qobject_cast<QGraphicsScene *>(obj))
		{
			if (!scene->views().isEmpty())
			{
				auto view = scene->views().first();
				// 获取鼠标指针的全局桌面坐标
				QPoint globalPos = QCursor::pos();

				// 将全局桌面坐标转换为视图坐标
				QPoint viewPos = view->mapFromGlobal(globalPos);

				// 将视图坐标转换为场景坐标
				QPointF scenePos = view->mapToScene(viewPos);

				// 使用 itemAt 方法查找指定坐标的 QGraphicsItem
				QGraphicsItem *item = scene->itemAt(scenePos, QTransform());
				if (item)
				{
					m_drawInfoWidget->setCurItem(item);
					m_drawInfoWidget->updateRectAndInfo();
				}
			}
		}
	}

	if (event->type() == QEvent::MouseButtonPress)
	{
		auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
		if (mouseEvent->button() == Qt::MiddleButton)
		{
			// 处理鼠标中键按下事件
			//			qDebug() << "Middle mouse button pressed " << obj;
			if (m_treeWidget)
			{
				if (auto *view = qobject_cast<QGraphicsView *>(obj))
				{
					QPoint globalPos = QCursor::pos();
					QPoint viewPos = view->mapFromGlobal(globalPos);
					auto rect = view->viewport()->rect();
					if (rect.contains(viewPos))
					{
						qDebug() << "In viewport";
						m_treeWidget->updateTree(view);
					}
				}
				else
				{
					m_treeWidget->updateTree(curWid);
				}
			}
			m_treeWidget->show();
			m_treeWidget->raise();
			m_treeWidget->activateWindow();
			return QObject::eventFilter(obj, event);
		}
	}
	return QObject::eventFilter(obj, event);
}

void QSpy::start()
{
#ifdef QT_DEBUG
	qDebug() << "Current build is Debug";
#elif QT_NO_DEBUG
	qDebug() << "Current build is Release";
#else
	qDebug() << "Unknown build type";
#endif
	connect(m_treeWidget.data(), &ObjTreeWidget::sigUpdateCurWidget, this, [&](QWidget *cur) {
		m_drawInfoWidget->setCurWidget(cur);
		m_drawInfoWidget->updateRectAndInfo();
	});
	connect(m_drawInfoWidget.data(), &DrawInfoWidget::sigSendInfo, m_treeWidget.data(), &ObjTreeWidget::onGetInfo);

	connect(m_switchToolWidget.data(), &SwitchToolWidget::sigsStateChange, [&](bool state)
			{
				m_switchState = state;
				m_drawInfoWidget->setVisible(state);
				m_treeWidget->setVisible(state);
			});
	m_drawInfoWidget->showFullScreen();
	qApp->installEventFilter(this);

	m_switchToolWidget->show();
}
bool QSpy::isListen(QObject *obj)
{
	for (auto &item : gNoListenObjList)
	{
		if (item == obj->objectName())
		{
			return false;
		}
	}
	return true;
}
QSpy::QSpy()
{
}
QSpy::~QSpy()
{
}

IQSpyInterface *createQSpy()
{
	return new QSpy;
}
