#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(300,100);
    testIP.setParent(this);
    testIP.move(100,20);
    testIP.show();
}

Widget::~Widget()
{
}

