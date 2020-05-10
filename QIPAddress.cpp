#include "QIPAddress.hpp"
#include <QDebug>
#include <QMessageBox>
QIPAddressWidget::QIPAddressWidget(QWidget *parent) : QFrame(parent)
{

    //background-color: rgb(68, 73, 79);
    //color: white;
    setStyleSheet("background-color:white");
	setFrameShape(QFrame::StyledPanel);
	setFrameShadow(QFrame::Sunken);

    pLayout = new QHBoxLayout(this);
    pLayout->setMargin(0);
	pLayout->setContentsMargins(0, 0, 0, 0);
	pLayout->setSpacing(0);

	for (int i = 0; i != QTUTL_IP_SIZE; ++i)
	{
		if (i != 0)
		{
            m_pDot[i - 1] = new QLabel(".");
            m_pDot[i - 1]->setMargin(0);
//            m_pDot[i - 1]->setStyleSheet("background: white");
			pLayout->addWidget(m_pDot[i - 1]);
            pLayout->setStretch(pLayout->count(), 0);
		}

        m_pLineEdit[i] = new QLineEdit();
		QLineEdit* pEdit = m_pLineEdit[i];
        pEdit->setMaximumWidth(30);
        pEdit->setTextMargins(0, 0, 0, 0);
		pEdit->installEventFilter(this);

		pLayout->addWidget(pEdit);
		pLayout->setStretch(pLayout->count(), 1);

        pEdit->setFrame(false);
		pEdit->setAlignment(Qt::AlignCenter);

		QFont font = pEdit->font();
		font.setStyleHint(QFont::Monospace);
		font.setFixedPitch(true);
		pEdit->setFont(font);

		QRegExp rx("^(0|[1-9]|[1-9][0-9]|1[0-9][0-9]|2([0-4][0-9]|5[0-5]))$");
		QValidator *validator = new QRegExpValidator(rx, pEdit);
		pEdit->setValidator(validator);

	}

    setMaximumWidth(60 * QTUTL_IP_SIZE);
    setMaximumHeight(30);
	connect(this, SIGNAL(signalTextChanged(QLineEdit*)),
		this, SLOT(slotTextChanged(QLineEdit*)),
		Qt::QueuedConnection);
	for (int i = 0; i<QTUTL_IP_SIZE; i++)
	{
		connect(m_pLineEdit[i], SIGNAL(textChanged(QString)), this, SLOT(lineEditChanged()));
	}
    setLayout(pLayout);
}

QIPAddressWidget::~QIPAddressWidget()
{
	delete[] * m_pDot;
	delete[] * m_pLineEdit;
}


void QIPAddressWidget::lineEditChanged()
{
	QLineEdit* pEdit = qobject_cast<QLineEdit*>(sender());
	for (unsigned int i = 0; i != QTUTL_IP_SIZE; ++i)
	{
		if (pEdit == m_pLineEdit[i])
		{
			if (pEdit->text().size() == MAX_DIGITS&&pEdit->text().size() == pEdit->cursorPosition())
			{
				if (i + 1 != QTUTL_IP_SIZE)
				{
					m_pLineEdit[i + 1]->setFocus();
					m_pLineEdit[i + 1]->selectAll();
				}
			}
		}
    }
}

void QIPAddressWidget::setDefaultValue(QString first, QString second, QString third, QString forth)
{
    m_pLineEdit[0]->setText(first);
    m_pLineEdit[1]->setText(second);
    m_pLineEdit[2]->setText(third);
    m_pLineEdit[3]->setText(forth);
}

void QIPAddressWidget::slotTextChanged(QLineEdit* pEdit)
{
	for (unsigned int i = 0; i != QTUTL_IP_SIZE; ++i)
	{
		if (pEdit == m_pLineEdit[i])
		{
			if ((pEdit->text().size() == MAX_DIGITS &&  pEdit->text().size() == pEdit->cursorPosition()) || (pEdit->text() == "0"))
			{
				if (i + 1 != QTUTL_IP_SIZE)
				{
					m_pLineEdit[i + 1]->setFocus();
					m_pLineEdit[i + 1]->selectAll();
				}
			}
		}
	}
}

bool QIPAddressWidget::eventFilter(QObject *obj, QEvent *event)
{
	bool bRes = QFrame::eventFilter(obj, event);

	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent* pEvent = dynamic_cast<QKeyEvent*>(event);
		if (pEvent)
		{
			for (unsigned int i = 0; i != QTUTL_IP_SIZE; ++i)
			{
				QLineEdit* pEdit = m_pLineEdit[i];
				if (pEdit == obj)
				{
					switch (pEvent->key())
					{
					case Qt::Key_Left:
						if (pEdit->cursorPosition() == 0)
						{
							// user wants to move to previous item
							MovePrevLineEdit(i);
						}
						break;

					case Qt::Key_Right:
						if (pEdit->text().isEmpty() || (pEdit->text().size() == pEdit->cursorPosition()))
						{
							// user wants to move to next item
							MoveNextLineEdit(i);
						}
						break;

					case Qt::Key_0:
						if (pEdit->text().isEmpty() || pEdit->text() == "0")
						{
							pEdit->setText("0");
							// user wants to move to next item
							MoveNextLineEdit(i);
						}
						emit signalTextChanged(pEdit);
						break;

					case Qt::Key_Backspace:
						if (pEdit->text().isEmpty() || pEdit->cursorPosition() == 0)
						{
							// user wants to move to previous item
							MovePrevLineEdit(i);
						}
						break;

					case Qt::Key_Comma:
					case Qt::Key_Period:
						MoveNextLineEdit(i);
						break;

					default:
						emit signalTextChanged(pEdit);
						break;

					}
				}
			}
		}
	}

	return bRes;
}

void QIPAddressWidget::MoveNextLineEdit(int i)
{
	if (i + 1 != QTUTL_IP_SIZE)
	{
		m_pLineEdit[i + 1]->setFocus();
		m_pLineEdit[i + 1]->setCursorPosition(0);
		m_pLineEdit[i + 1]->selectAll();
	}
}

void QIPAddressWidget::MovePrevLineEdit(int i)
{
	if (i != 0)
	{
		m_pLineEdit[i - 1]->setFocus();
		m_pLineEdit[i - 1]->setCursorPosition(m_pLineEdit[i - 1]->text().size());
	}
}

bool QIPAddressWidget::getIP(QHostAddress *haddr)
{
	QString ip_str;
	ip_str.clear();
	for (int i = 0; i<QTUTL_IP_SIZE; i++)
	{
		if (i != 0)
			ip_str.append(".");
		if (m_pLineEdit[i]->text().isEmpty())
		{
			return false;
		}

		ip_str.append(m_pLineEdit[i]->text());
	}
	*haddr = QHostAddress(ip_str);
    return true;
}

QHostAddress QIPAddressWidget::getIP()
{
    QHostAddress addr;
    getIP(&addr);
    return addr;
}
