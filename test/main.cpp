#include "IQSpyInterface.h"
#include "QWidget"
#include "qdebug.h"
#include <QApplication>
#include <QDir>
#include <QLabel>
#include <QPluginLoader>
#include <QPushButton>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QWidget widget;
	widget.setWindowTitle("Hello World!");

	widget.resize(800, 600);
	//	widget.setWindowFlags(Qt::Tool);
	widget.setAttribute(Qt::WA_KeyboardFocusChange);

	// 创建顶层布局
	auto *mainLayout = new QVBoxLayout(&widget);

	// 第一层: 水平布局
	auto *firstLayer = new QHBoxLayout();
	mainLayout->addLayout(firstLayer);

	// 第二层: 左侧垂直布局
	auto *secondLayerLeft = new QVBoxLayout();
	firstLayer->addLayout(secondLayerLeft);

	// 第三层: 左侧上部水平布局
	auto *thirdLayerTopLeft = new QHBoxLayout();
	secondLayerLeft->addLayout(thirdLayerTopLeft);

	// 第四层: 左侧上部第一个按钮
	auto *button1 = new QPushButton("Button 1");
	thirdLayerTopLeft->addWidget(button1);

	// 第四层: 左侧上部第二个按钮
	auto *button2 = new QPushButton("Button 2");
	thirdLayerTopLeft->addWidget(button2);

//scrolltoitem 测试
	for (int i = 0; i < 100; ++i)
	{

		auto *button2 = new QPushButton("Button 2");
		thirdLayerTopLeft->addWidget(button2);
	}


	// 第三层: 左侧下部标签
	auto *label1 = new QLabel("Label 1");
	secondLayerLeft->addWidget(label1);

	// 第二层: 右侧垂直布局
	auto *secondLayerRight = new QVBoxLayout();
	firstLayer->addLayout(secondLayerRight);

	// 第三层: 右侧上部水平布局
	auto *thirdLayerTopRight = new QHBoxLayout();
	secondLayerRight->addLayout(thirdLayerTopRight);

	// 第四层: 右侧上部第一个按钮
	auto *button3 = new QPushButton("Button 3");
	thirdLayerTopRight->addWidget(button3);

	// 第四层: 右侧上部第二个按钮
	auto *button4 = new QPushButton("Button 4");
	thirdLayerTopRight->addWidget(button4);

	// 第三层: 右侧下部标签
	auto *label2 = new QLabel("Label 2");
	secondLayerRight->addWidget(label2);

	auto *label3 = new QLabel(label1);
	label3->setGeometry(0, 0, 10, 10);


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
			qWarning() << "Plugin name:" << pluginName;
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

	widget.show();
	return QApplication::exec();
}
