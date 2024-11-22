//
// Created by 17305 on 2024/11/15.
//

#ifndef DRAWINFOWIDGET_H
#define DRAWINFOWIDGET_H
#include <QWidget>
#include "QMap"


class DrawInfoWidget: public QWidget
{

	Q_OBJECT
public:
	explicit DrawInfoWidget(QWidget *parent = nullptr);
	~DrawInfoWidget() override;

	void setCurWidget(QWidget*);

	signals:
		void sigSendInfo(QList<QPair<QString, QString>>&);

public slots:
	void updateRect();
	void updateInfo();

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	QList<QPair<QString, QString>> m_info;
	QRect m_rect;
	QWidget* m_curWidget = nullptr;
};


#endif//DRAWINFOWIDGET_H
