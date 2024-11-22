//
// Created by 17305 on 2024/11/22.
//

#ifndef TREEOBJMENU_H
#define TREEOBJMENU_H

#include <QMenu>


QT_BEGIN_NAMESPACE
namespace Ui
{
class TreeObjMenu;
}
QT_END_NAMESPACE

class TreeObjMenu: public QMenu
{
	Q_OBJECT

public:
	explicit TreeObjMenu(QWidget *parent = nullptr);
	~TreeObjMenu() override;

	void setWinInfo(QList<QPair<QString, QString>>&);

private:
	Ui::TreeObjMenu *ui;

	QList<QPair<QString, QString>> m_winInfo{};
};


#endif//TREEOBJMENU_H
