//
// Created by 17305 on 2024/11/17.
//

#ifndef IQSPYINTERFACE_H
#define IQSPYINTERFACE_H

#include <QConstOverload>
#include <QObject>
class IQSpyInterface
{
public:
	virtual ~IQSpyInterface()  = default;
	virtual void start() = 0;
};

QT_BEGIN_NAMESPACE
#define IQSpyInterface_iid "com.example.IQSpyInterface.1.0"
Q_DECLARE_INTERFACE(IQSpyInterface, IQSpyInterface_iid)
QT_END_NAMESPACE

IQSpyInterface* createQSpy();


#endif//IQSPYINTERFACE_H
