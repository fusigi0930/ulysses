#ifndef CINTERFACEUI_H
#define CINTERFACEUI_H

#include <QObject>
#include <netio.h>
#include <csqlitestore.h>

class CInterfaceUi : public QObject
{
	Q_OBJECT
protected:
	bool checkDB(int nType);


public:
	CInterfaceUi(QObject *parent = 0);
	~CInterfaceUi();

	CSQLiteStore m_Localdb;
	CDBSyncStore m_Remotedb;

	QString m_szDBFile;
signals:
	void sigUpdateInfo(QString szInfo);
	void sigUpdateProgress(QVariant vInfo);

public slots:
	void slotSetFilePath(QString szFile);
	void slotStartSync();
};

#endif // CINTERFACEUI_H
