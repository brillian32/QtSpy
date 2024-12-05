//
// Created by 17305 on 2024/12/5.
//

#include "SearchWidget.h"
#include "QHBoxLayout"

SearchWidget::SearchWidget(QWidget *parent) : QWidget(parent)
{
	initUI();
	connect(m_pSearchEdit, &QLineEdit::editingFinished, this, [=]() {
		emit sigSearch(m_pSearchEdit->text());
	});
}
SearchWidget::~SearchWidget()
{
}
void SearchWidget::initUI()
{
	m_pSearchEdit = new QLineEdit(this);
	m_pSearchEdit->setPlaceholderText("Search");
	m_pSearchEdit->setClearButtonEnabled(true);
	m_pSearchEdit->setFocus();

	auto *layout = new QHBoxLayout(this);
	layout->setContentsMargins(10, 0, 10, 0);
	layout->addWidget(m_pSearchEdit);

}
