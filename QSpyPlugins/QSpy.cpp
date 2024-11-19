//
// Created by 17305 on 2024/11/15.
//

#include "QSpy.h"
#include "QMouseEvent"
#include "qdebug.h"
#include "qmap.h"
#include "qwidget.h"
#include <QCoreApplication>
#include <QMetaEnum>


bool QSpy::eventFilter(QObject *obj, QEvent *event)
{
	static bool noShow = false;
	static QWidget *curWid = nullptr;
	if (obj->objectName() == "infoTreeWidget" && event->type() == QEvent::Enter)
	{
		noShow = true;
		return QObject::eventFilter(obj, event);
	}

	if (obj->objectName() == "infoTreeWidget" && event->type() == QEvent::Leave)
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

		if (obj->objectName() == "DrawInfoObj")
		{
			return QObject::eventFilter(obj, event);
		}

		if (auto *widget = qobject_cast<QWidget *>(obj))
		{
			curWid = widget;
			m_drawInfoWidget->setCurWidget(widget);
			m_drawInfoWidget->updateRect();
			m_drawInfoWidget->updateInfo();
		}
		else
		{
			qWarning() << "Object is not a QWidget : " << obj->objectName() << "  " << obj->metaObject()->className() << " " << obj;
		}
	}
	//	else if (event->type() == QEvent::Leave)
	//	{
	//	}

	if (event->type() == QEvent::MouseButtonPress)
	{
		auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
		if (mouseEvent->button() == Qt::MiddleButton)
		{
			// 处理鼠标中键按下事件
			qDebug("Middle mouse button pressed");
			if (m_treeWidget)
			{
				m_treeWidget->updateTree(curWid);
			}
			m_treeWidget->show();
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
		m_drawInfoWidget->updateRect();
		m_drawInfoWidget->updateInfo();
	});
	m_drawInfoWidget->showFullScreen();
	qApp->installEventFilter(this);
}

