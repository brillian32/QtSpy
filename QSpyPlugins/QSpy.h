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
#include "SwitchToolWidget.h"

//不参与窗口调试
const QList<QString> gNoListenObjList{
//	"infoTreeWidget" ,
	"TreeObjMenu",
	"DrawInfoObj",
	"SwitchToolWidgetObj"
};

class QSpy: public QObject , public IQSpyInterface
{
	Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.example.IQSpyInterface.1.0")
    Q_INTERFACES(IQSpyInterface)



public:
	QSpy();
	virtual ~QSpy() override;
	void start() override;

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;

private:
	static bool isListen(QObject *obj);

private:
	QScopedPointer<ObjTreeWidget> m_treeWidget{new ObjTreeWidget};
	QScopedPointer<DrawInfoWidget> m_drawInfoWidget{new DrawInfoWidget};
	QScopedPointer<SwitchToolWidget> m_switchToolWidget{new SwitchToolWidget};
	bool m_switchState{false};
};

#endif//QSPY_H
