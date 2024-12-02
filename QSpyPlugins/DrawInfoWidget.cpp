//
// Created by 17305 on 2024/11/15.
//

#include "DrawInfoWidget.h"

#include "QFontMetrics"
#include "QMetaEnum"
#include "QPainter"
// #include "QtWin"
#include "QTimer"
#include "qdebug.h"
#include "qgraphicsview.h"
#include "qgraphicsitem.h"
#include "qstring.h"
#include <utility>

#include <typeinfo>

#ifdef Q_OS_LINUX
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#endif

DrawInfoWidget::~DrawInfoWidget() = default;

DrawInfoWidget::DrawInfoWidget(QWidget *parent) : QWidget(parent)
{
	setObjectName("DrawInfoObj");
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_NoSystemBackground);
	//	QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);// 使窗口完全透明
	setWindowState(Qt::WindowFullScreen);
	////	设置窗口对鼠标事件透明
	setAttribute(Qt::WA_TransparentForMouseEvents, true);
#ifdef Q_OS_LINUX
	QTimer::singleShot(0, [this]() {
		// 获取窗口 ID
		WId winId = this->winId();
		Display *display = XOpenDisplay(nullptr);
		if (!display)
		{
			qDebug() << "Cannot open X display!";
			return;
		}

		// 设置鼠标事件穿透
		Window x11Window = static_cast<Window>(winId);
		XRectangle rect;
		rect.x = 0;
		rect.y = 0;
		rect.width = 1;
		rect.height = 1;

		// 设置空的形状区域，鼠标事件可穿透
		XShapeCombineRectangles(display, x11Window, ShapeInput, 0, 0, &rect, 0, ShapeSet, 0);
		XFlush(display);
		XCloseDisplay(display);
	});
#endif
}

void DrawInfoWidget::paintEvent(QPaintEvent *event)
{
	// 绘制窗体轮廓
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen pen;
//	pen.setColor(QColor(128, 203, 196,128));
	pen.setColor(Qt::red);
	pen.setWidth(2);
	painter.setPen(pen);
	painter.drawRect(m_rect);

	// 绘制窗体信息
	QPen textPen;
	textPen.setColor(Qt::red);
	painter.setPen(textPen);
	QFont font("Arial", 12);// 设置字体和大小
	font.setBold(true);
	painter.setFont(font);
	QFontMetrics fm(font);
	int textHeight = fm.height();
	int x = 50;// 初始左边距
	int y = 50;// 初始上边距
	for (auto &it : m_info)
	{
		const QString &key = it.first;
		QString value = it.second;
		QString text = key + ": " + value;
		painter.drawText(x, y, text);
		y += textHeight + 5;
	}
	QWidget::paintEvent(event);
}
void DrawInfoWidget::updateRect()
{
	if (m_infoType == InfoType::Scene)
	{
		// 将场景中的矩形映射到视图坐标
		QGraphicsView *view = m_curScene->views().first();
		// 将场景中的矩形转换为视图中的矩形
		QRectF viewRect = view->mapFromScene(m_curScene->sceneRect()).boundingRect();

		// 将视图中的矩形转换为全局桌面坐标
		auto topLeft = view->mapToGlobal(viewRect.topLeft().toPoint());
		auto bottomRight = view->mapToGlobal(viewRect.bottomRight().toPoint());
		m_rect = QRect(topLeft, bottomRight);
		m_rect.adjust(1, 1, 1, 1);
	}
	else if (m_infoType == InfoType::Item)
	{
		QGraphicsView *view = m_curScene->views().first();
		// 获取 item 在场景中的矩形
		QRectF sceneRect = m_curItem->sceneBoundingRect();

		// 将场景中的矩形转换为视图中的矩形
		QRectF viewRect = view->mapFromScene(sceneRect).boundingRect();

		// 将视图中的矩形转换为全局桌面坐标
		QPoint topLeft = view->mapToGlobal(viewRect.topLeft().toPoint());
		QPoint bottomRight = view->mapToGlobal(viewRect.bottomRight().toPoint());

		m_rect =  QRect(topLeft, bottomRight);
		m_rect.adjust(1, 1, 1, 1);
	}
	else if (m_infoType == InfoType::Widget)
	{
		QPoint globalPos = m_curWidget->mapToGlobal(QPoint(0, 0));
		m_rect = QRect(globalPos, m_curWidget->size());
		update();
	}
	else
	{
	}
}
void DrawInfoWidget::updateInfo()
{
	if (m_infoType == InfoType::Widget)
	{
		updateWidgetInfo();
	}
	else if (m_infoType == InfoType::Scene)
	{
		updateSceneInfo();
	}
	else if (m_infoType == InfoType::Item)
	{
		updateItemInfo();
	}

	update();
	sigSendInfo(m_info);
}

void DrawInfoWidget::setCurWidget(QWidget *cur)
{
	m_infoType = InfoType::Widget;
	m_curWidget = cur;
}

void DrawInfoWidget::setCurScene(QGraphicsScene *Scene)
{
	m_infoType = InfoType::Scene;
	m_curScene = Scene;
}
void DrawInfoWidget::updateWidgetInfo()
{
	QPoint globalPos = m_curWidget->mapToGlobal(QPoint(0, 0));
	QList<QPair<QString, QString>> infoList;
	infoList.append({"Class Name", QString(m_curWidget->metaObject()->className())});
	infoList.append({"Object Name", m_curWidget->objectName()});
	infoList.append({"StyleSheet", m_curWidget->styleSheet()});
	auto geom = m_curWidget->geometry();
	infoList.append({"Geometry", QString::number(geom.x()) + "," + QString::number(geom.y()) + "," +
									 QString::number(geom.width()) + "," +
									 QString::number(geom.height())});
	auto frameGeom = m_curWidget->frameGeometry();
	infoList.append({"Size", QString::number(m_curWidget->size().width()) + "," +
								 QString::number(m_curWidget->size().height())});
	infoList.append({"Frame Geometry", QString::number(frameGeom.x()) + "," +
										   QString::number(frameGeom.y()) + "," +
										   QString::number(frameGeom.width()) + "," +
										   QString::number(frameGeom.height())});
	infoList.append({"Pos", QString::number(m_curWidget->pos().x()) + "," +
								QString::number(m_curWidget->pos().y())});
	infoList.append(
		{"Global Position", QString::number(globalPos.x()) + "," + QString::number(globalPos.y())});
	QMetaEnum metaEnum = QMetaEnum::fromType<Qt::WindowType>();
	auto value = metaEnum.valueToKeys(m_curWidget->windowType());
	infoList.append({"Window Type", QString::fromUtf8(value)});

	metaEnum = QMetaEnum::fromType<Qt::WindowFlags>();
	infoList.append(
		{"Window Flags", QString::fromUtf8(metaEnum.valueToKeys(m_curWidget->windowFlags()))});
	infoList.append({"Window Title", m_curWidget->windowTitle()});
	infoList.append({"is Visible", QString(m_curWidget->isVisible() ? "true" : "false")});
	infoList.append(
		{"is Active Window", QString(m_curWidget->isActiveWindow() ? "true" : "false")});
	infoList.append({"is Window Modal", QString(m_curWidget->isModal() ? "true" : "false")});
	infoList.append({"is Enable", QString(m_curWidget->isEnabled() ? "true" : "false")});
	metaEnum = QMetaEnum::fromType<Qt::FocusPolicy>();
	infoList.append(
		{"focusPolicy", QString::fromUtf8(metaEnum.valueToKeys(m_curWidget->focusPolicy()))});
	m_info = std::move(infoList);
}

void DrawInfoWidget::updateSceneInfo()
{
	QList<QPair<QString, QString>> infoList;
	auto objName = m_curScene->objectName();
	infoList.append({"Object Name", objName});
	infoList.append({"Class Name", QString(m_curScene->metaObject()->className())});
	infoList.append({"Scene Rect", QString::number(m_curScene->sceneRect().x()) + "," +
										 QString::number(m_curScene->sceneRect().y()) + "," +
										 QString::number(m_curScene->sceneRect().width()) + "," +
										 QString::number(m_curScene->sceneRect().height())});


	infoList.append({ "Global Rect", QString::number(m_rect.x()) + "," + QString::number(m_rect.y()) + "," +
									 QString::number(m_rect.width()) + "," +
									 QString::number(m_rect.height()) });

	m_info = std::move(infoList);
}

void DrawInfoWidget::updateItemInfo()
{
	auto itemName = QString(typeid(*m_curItem).name());
	itemName.remove("class ");
	QList<QPair<QString, QString>> infoList;
	infoList.append({"Item Class Name", itemName});

	QRectF sceneRect = m_curItem->sceneBoundingRect();
	infoList.append({"In Scene Rect", QString::number(sceneRect.x()) + "," +
										 QString::number(sceneRect.y()) + "," +
										 QString::number(sceneRect.width()) + "," +
										 QString::number(sceneRect.height())});
	QGraphicsView *view = m_curScene->views().first();
	QRectF viewRect = view->mapFromScene(sceneRect).boundingRect();
	infoList.append({"In View Rect", QString::number(viewRect.x()) + "," +
										 QString::number(viewRect.y()) + "," +
										 QString::number(viewRect.width()) + "," +
										 QString::number(viewRect.height())});

	QPoint topLeft = view->mapToGlobal(viewRect.topLeft().toPoint());
	QPoint bottomRight = view->mapToGlobal(viewRect.bottomRight().toPoint());
	QRect globalRect = QRect(topLeft, bottomRight);
	infoList.append({"In Global Rect", QString::number(globalRect.x()) + "," +
									 QString::number(globalRect.y()) + "," +
									 QString::number(globalRect.width()) + "," +
									 QString::number(globalRect.height())});

	m_info = std::move(infoList);
}

void DrawInfoWidget::updateRectAndInfo()
{
	updateRect();
	updateInfo();
}

void DrawInfoWidget::setCurItem(QGraphicsItem * item)
{
	m_infoType = InfoType::Item;
	m_curItem = item;
}
