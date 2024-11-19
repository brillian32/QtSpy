//
// Created by 17305 on 2024/11/15.
//

#include "DrawInfoWidget.h"

#include "QFontMetrics"
#include "QMetaEnum"
#include "QPainter"
//#include "QtWin"
#include "qdebug.h"
#include <utility>
#include "qstring.h"

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
}

void DrawInfoWidget::paintEvent(QPaintEvent *event)
{
	// 绘制窗体轮廓
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen pen(Qt::red, 2);
	painter.setPen(pen);
	painter.drawRect(m_rect);

	// 绘制窗体信息
	QPen penText(QColor(255, 0, 0, 160));
	painter.setPen(penText);
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
	QPoint globalPos = m_curWidget->mapToGlobal(QPoint(0, 0));
	m_rect = QRect(globalPos, m_curWidget->size());
	update();
}
void DrawInfoWidget::updateInfo()
{
	QPoint globalPos = m_curWidget->mapToGlobal(QPoint(0, 0));
	QList<QPair<QString, QString>> infoList;
	infoList.append({"Class Name", QString(m_curWidget->metaObject()->className())});
	infoList.append({"Object Name", m_curWidget->objectName()});
	infoList.append({"StyleSheet", m_curWidget->styleSheet()});
	auto geom = m_curWidget->geometry();
	infoList.append({"Geometry", QString::number(geom.x()) + "," +
									 QString::number(geom.y()) + "," + QString::number(geom.width()) + "," + QString::number(geom.height())});
	auto frameGeom = m_curWidget->frameGeometry();
	infoList.append({"Size", QString::number(m_curWidget->size().width()) + "," + QString::number(m_curWidget->size().height())});
	infoList.append({"Frame Geometry", QString::number(frameGeom.x()) + "," +
										   QString::number(frameGeom.y()) + "," + QString::number(frameGeom.width()) + "," + QString::number(frameGeom.height())});
	infoList.append({"Pos", QString::number(m_curWidget->pos().x()) + "," + QString::number(m_curWidget->pos().y())});
	infoList.append({"Global Position", QString::number(globalPos.x()) + "," + QString::number(globalPos.y())});
	QMetaEnum metaEnum = QMetaEnum::fromType<Qt::WindowType>();
	auto value = metaEnum.valueToKeys(m_curWidget->windowType());
	infoList.append({"Window Type", QString::fromUtf8(value)});

	qDebug() << "Window Type:" << m_curWidget->windowType();
	metaEnum = QMetaEnum::fromType<Qt::WindowFlags>();
	infoList.append({"Window Flags", QString::fromUtf8(metaEnum.valueToKeys(m_curWidget->windowFlags()))});
	qDebug() << "Window Flags:" << m_curWidget->windowFlags();
	infoList.append({"Window Title", m_curWidget->windowTitle()});
	infoList.append({"is Visible", QString(m_curWidget->isVisible() ? "true" : "false")});
	infoList.append({"is Active Window", QString(m_curWidget->isActiveWindow() ? "true" : "false")});
	infoList.append({"is Window Modal", QString(m_curWidget->isModal() ? "true" : "false")});
	infoList.append({"is Enable", QString(m_curWidget->isEnabled() ? "true" : "false")});
	metaEnum = QMetaEnum::fromType<Qt::FocusPolicy>();
	infoList.append({"focusPolicy", QString::fromUtf8(metaEnum.valueToKeys(m_curWidget->focusPolicy()))});
	m_info = std::move(infoList);
	update();
}

void DrawInfoWidget::setCurWidget(QWidget * cur)
{
	m_curWidget = cur;
}
