//
// Created by 17305 on 24-12-4.
//

#ifndef MAIN_SWITCHTOOLWIDGET_H
#define MAIN_SWITCHTOOLWIDGET_H

#include "qwidget.h"

class SwitchToolWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SwitchToolWidget(QWidget *parent = nullptr);
	~SwitchToolWidget() override;

	signals:
	void sigsStateChange(bool);

protected:
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
	bool m_dragging = false ;
	QPoint m_dragPosition {};
	bool m_openState = false;
};


#endif//MAIN_SWITCHTOOLWIDGET_H
