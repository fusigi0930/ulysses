#ifndef CINTERFACEUI_H
#define CINTERFACEUI_H

#include <QObject>
#include <netio.h>
#include <csqlitestore.h>

class CInterfaceUi : public QObject
{
	Q_OBJECT
public:
	CInterfaceUi(QObject *parent = 0);
	~CInterfaceUi();

	CSQLiteStore m_db;

signals:
	void sigUpdateUi(QString szResult);

public slots:
	void slotUpdateResult(QString szMac, QString szSerial);
	void slotGetResult(QString szMac);
};

#endif // CINTERFACEUI_H
