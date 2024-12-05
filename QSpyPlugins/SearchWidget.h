//
// Created by 17305 on 2024/12/5.
//

#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include "qwidget.h"
#include "qlineedit.h"

class SearchWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SearchWidget(QWidget *parent = nullptr);
	~SearchWidget() override;

	signals:
		void sigSearch(const QString&);
private:
	void initUI();

	QLineEdit* m_pSearchEdit{};

};


#endif//SEARCHWIDGET_H
