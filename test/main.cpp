#include "IQSpyInterface.h"
#include "QWidget"
#include "WidgetTest.h"
#include "qdebug.h"
#include <QApplication>
#include <QDir>
#include <QLabel>
#include <QPluginLoader>
#include <QPushButton>
#include <QVBoxLayout>
#include "GraphicsViewTest.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	WidgetTest widget;

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
	QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
#endif
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#  if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
	QApplication::setHighDpiScaleFactorRoundingPolicy(
		Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#  endif
#endif

#ifdef QT_DEBUG
	qDebug() << "Current build is Debug";
#elif QT_NO_DEBUG
	qDebug() << "Current build is Release";
#else
	qDebug() << "Unknown build type";
#endif

	QDir pluginsDir(QApplication::applicationDirPath());
	auto files = pluginsDir.entryList(QDir::Files);
	foreach (QString fileName, files)
	{
		QPluginLoader pluginLoader(fileName);
		QObject *plugin = pluginLoader.instance();
		if (plugin)
		{
			// 获取插件名称
			QString pluginName = plugin->metaObject()->className();
			qDebug() << "Plugin name:" << pluginName;
			if (pluginName == "QSpy")
			{
				// 对插件初始化
				auto plugs = qobject_cast<IQSpyInterface *>(plugin);
				plugs->start();
				break;
			}
		}
		else
		{
			qDebug() << "Error:" << pluginLoader.errorString();
		}
	}

	GraphicsViewTest view;
	view.setWindowTitle("QGraphicsView Example");
	view.resize(600, 400);
	view.show();

	widget.show();
	return QApplication::exec();
}
