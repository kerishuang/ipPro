#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QIPAddress.hpp"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QIPAddressWidget testIP;

};
#endif // WIDGET_H
