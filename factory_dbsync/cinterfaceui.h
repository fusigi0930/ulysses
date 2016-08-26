#ifndef CINTERFACEUI_H
#define CINTERFACEUI_H

#include <QObject>
#include <netio.h>
#include <csqlitestore.h>

class CInterfaceUi;

class CTaskThread : public QThread {
	Q_OBJECT
protected:
	CInterfaceUi *m_ui;

signals:

public slots:

public:
	CTaskThread(CInterfaceUi *ui);

	~CTaskThread();

	virtual void run();
};

class CInterfaceUi : public QObject
{
	Q_OBJECT
protected:
	CTaskThread *m_thread;

	bool checkDB(int nType);
	long long getLastSyncInfo(CBaseStore *db, std::list<QVariant> &lst);
	long long checkSyncInfo(QVariant info);

	long long syncTarget(QVariant local);
	long long syncBoard(QVariant local);

	long long replaceBoard(QVariant replaceInfo);
	long long replaceItem(QVariant replaceInfo);

	void updateUiInfo();

public:
	CInterfaceUi(QObject *parent = 0);
	~CInterfaceUi();

	CSQLiteStore m_Localdb;
	CDBSyncStore m_Remotedb;

	long long m_nTotalCount;
	long long m_nCurrentCount;

	QString m_szDBFile;

	void testProg();
	long long startSync();
signals:
	void sigUpdateInfo(QString szInfo);
	void sigUpdateProgress(QVariant vInfo);

public slots:
	void slotSetFilePath(QString szFile);
	void slotStartSync();
	void slotTestProg();
};

#endif // CINTERFACEUI_H
