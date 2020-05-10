#ifndef QIPADDRESSWIDGET_H
#define QIPADDRESSWIDGET_H

#include <QFrame>
#include <QLineEdit>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QFont>
#include <QLabel>
#include <QKeyEvent>
#include <QHostAddress>

class QIPAddressWidget : public QFrame
{
	Q_OBJECT

public:
    QIPAddressWidget(QWidget *parent = 0);
    ~QIPAddressWidget();

	virtual bool eventFilter(QObject *obj, QEvent *event);
    bool getIP(QHostAddress *haddr);
    QHostAddress getIP();
	public slots:
	void slotTextChanged(QLineEdit* pEdit);
	void lineEditChanged();
    void setDefaultValue(QString first, QString second, QString third, QString forth);
signals:
	void signalTextChanged(QLineEdit* pEdit);

private:
	enum
	{
        QTUTL_IP_SIZE = 4,//
		MAX_DIGITS = 3 //
	};

	QLineEdit *(m_pLineEdit[QTUTL_IP_SIZE]);
    QLabel *(m_pDot[QTUTL_IP_SIZE - 1]);
    QHBoxLayout* pLayout;
	void MoveNextLineEdit(int i);
	void MovePrevLineEdit(int i);

};
#endif
