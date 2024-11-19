//
// Created by 17305 on 2024/11/15.
//

#ifndef QSPY_H
#define QSPY_H
#include "DrawInfoWidget.h"
#include "IQSpyInterface.h"
#include "ObjTreeWidget.h"
#include <QObject>
#include <QScopedPointer>

class QSpy: public QObject , IQSpyInterface
{
	Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.example.IQSpyInterface.1.0")
    Q_INTERFACES(IQSpyInterface)
public:
	void start() override;

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;

private:
	QScopedPointer<ObjTreeWidget> m_treeWidget{new ObjTreeWidget};
	QScopedPointer<DrawInfoWidget> m_drawInfoWidget{new DrawInfoWidget};
};




#endif//QSPY_H
