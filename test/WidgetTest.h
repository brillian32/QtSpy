//
// Created by 17305 on 2024/11/28.
//

#ifndef WIDGETTEST_H
#define WIDGETTEST_H
#include "qpushbutton.h"
#include "qwidget.h"
#include <QLabel>
#include <QVBoxLayout>

class WidgetTest: public QWidget
{
public:
	explicit WidgetTest(QWidget* parent = nullptr);
	~WidgetTest() override;
private:

};

WidgetTest::WidgetTest(QWidget *parent)
{
	setWindowTitle("Hello World!");

	resize(800, 600);
	//	widget.setWindowFlags(Qt::Tool);
	setAttribute(Qt::WA_KeyboardFocusChange);

	// 创建顶层布局
	auto *mainLayout = new QVBoxLayout(this);

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

//	//scrolltoitem 测试
	for (int i = 0; i < 100; ++i)
	{
		auto *button2 = new QPushButton("Button 2");
		button2->setObjectName("btn" + QString::number(i));
		auto *label = new QLabel("Label 2");
		label->setObjectName("lab" + QString::number(i));
		label->setParent(button2);
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
}

WidgetTest::~WidgetTest()
{
}

#endif//WIDGETTEST_H
