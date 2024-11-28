//
// Created by 17305 on 2024/11/15.
//

#ifndef DRAWINFOWIDGET_H
#define DRAWINFOWIDGET_H
#include "QMap"
#include <QGraphicsScene>
#include <QWidget>

enum class InfoType
{
	Widget,
	Scene,
	Item,
	Unknown
};

class DrawInfoWidget: public QWidget
{

	Q_OBJECT
public:
	explicit DrawInfoWidget(QWidget *parent = nullptr);
	~DrawInfoWidget() override;

	void setCurWidget(QWidget *);
	void setCurScene(QGraphicsScene *);
	void setCurItem(QGraphicsItem *);

signals:
	void sigSendInfo(QList<QPair<QString, QString>> &);

public slots:
	void updateRectAndInfo();

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	//更新矩形和显示信息
	void updateRect();
	void updateInfo();

	//更新不同类型元素显示信息
	void updateWidgetInfo();
	void updateSceneInfo();
	void updateItemInfo();

private:
	QList<QPair<QString, QString>> m_info;
	QRect m_rect;
	QWidget *m_curWidget = nullptr;
	QGraphicsScene *m_curScene = nullptr;
	QGraphicsItem *m_curItem = nullptr;
	InfoType m_infoType = InfoType::Unknown;
};


#endif// DRAWINFOWIDGET_H
