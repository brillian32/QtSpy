//
// Created by 17305 on 24-12-4.
//

#include "SwitchToolWidget.h"
#include "QPainterPath"
#include "qevent.h"
#include "qpainter.h"
#include "qtimer.h"
#include <QImageReader>
#include <QLabel>
#include "qapplication.h"

//根据实际dpi 获取实际倍图
QString getMultiplePixmap(QString path)
{
	auto dpr = qApp->devicePixelRatio();
	if (dpr > 1)
	{
		auto pathRet = path.remove(".png") + "@2x";
		return pathRet;
	}
	return path;
}
SwitchToolWidget::SwitchToolWidget(QWidget *parent) : QWidget(parent)
{
	setObjectName("SwitchToolWidgetObj");
	setToolTip("QtSpy Switch");
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setGeometry(0, 0, 70, 70);
	QTimer::singleShot(0, [this]() {
		m_openState = false;
		emit sigsStateChange(m_openState);
	});
//	setStyleSheet("border-image:url(:/img/switch_open.png);");
//	auto label = new QLabel(this);
//	label->setGeometry(0, 0, 80, 80);
//	label->setStyleSheet("border-image:url(:/img/switch_open.png);");
}
void SwitchToolWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.setBrush(QBrush(QColor(0,0,0,1)));
	painter.setPen(Qt::NoPen);
	painter.drawEllipse(rect());
	QString pixmapPath;
	if (m_openState)
	{
		pixmapPath = getMultiplePixmap(":/img/switch_open.png");
	}
	else
	{
		pixmapPath =  getMultiplePixmap(":/img/switch_close.png");
	}

	QPixmap pixmap = QPixmap(pixmapPath);
	if (!pixmap.isNull())
	{
		pixmap = pixmap.scaled(rect().size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		painter.drawPixmap(rect(), pixmap);
	}
	QWidget::paintEvent(event);
}
void SwitchToolWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_dragging = true;
		m_dragPosition = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}
void SwitchToolWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton && m_dragging)
	{
		move(event->globalPos() - m_dragPosition);
	}
	QWidget::mouseMoveEvent(event);
}
void SwitchToolWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_dragging = false;
	}

	QWidget::mouseReleaseEvent(event);
}
void SwitchToolWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	m_openState = !m_openState;
	update();
	emit sigsStateChange(m_openState);
	QWidget::mouseDoubleClickEvent(event);
}
SwitchToolWidget::~SwitchToolWidget() = default;
