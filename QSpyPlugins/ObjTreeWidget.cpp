//
// Created by 17305 on 2024/11/17.
//

#include "ObjTreeWidget.h"
#include "qapplication.h"
#include "qdebug.h"
#include <QDesktopWidget>

ObjTreeWidget::ObjTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
	QDesktopWidget *desktop = QApplication::desktop();
	QRect screenRect = desktop->screenGeometry();
	int x = screenRect.center().x();
	int y = screenRect.center().y();
	setGeometry(x, y, 300, 300);

	setObjectName("infoTreeWidget");
	setHeaderLabel("Widget Tree:");

	connect(this, &ObjTreeWidget::itemClicked, this, [this](QTreeWidgetItem *item, int column) {
		auto data = item->data(column, Qt::UserRole);
		if (data.canConvert<QWidget *>())
		{
			auto wid = data.value<QWidget *>();
			qDebug() << "clicked:" << wid;
			sigUpdateCurWidget(wid);
		}
	});
}

void ObjTreeWidget::updateTree(QWidget *widget)
{
	clear();
	updateObjectTree(widget, this);
	expandAll();
}

template<typename T>
void ObjTreeWidget::updateObjectTree(QWidget *widget, T parentItem)
{
	auto *item = new QTreeWidgetItem(parentItem);
	item->setText(0, widget->metaObject()->className() + QString(" ") + widget->objectName());
	item->setData(0, Qt::UserRole, QVariant::fromValue(widget));
	for (QObject *child : widget->children())
	{
		if (QWidget *childWidget = qobject_cast<QWidget *>(child))
		{
			updateObjectTree(childWidget, item);
		}
	}
}

ObjTreeWidget::~ObjTreeWidget() = default;
