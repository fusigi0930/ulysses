#include "cinterfaceui.h"
#include "debug.h"

#ifdef Q_OS_WIN
#include <shlobj.h>
#endif

#include <QDir>

enum EDBTYPE {
	DB_LOCAL,
	DB_REMOTE,
};

#define DB_HOST "192.168.48.2"
#define DB_NAME "test_product_report"
#define DB_USER "fortress"
#define DB_PASS "fortress"

CInterfaceUi::CInterfaceUi(QObject *parent) : QObject(parent)
{
	QString szFolder;
	QString szDBFile;
#ifdef Q_OS_WIN
	char szAppFolder[MAX_PATH]={0};
	SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, szAppFolder);
	szFolder.sprintf("%s/embux", szAppFolder);
#else
	szFolder.sprintf("~/.embux");
#endif
	szDBFile.sprintf("%s/product.db", QSZ(szFolder));
	m_szDBFile=szDBFile;
	if (!QFile::exists(m_szDBFile)) {
		emit sigUpdateInfo("default file is not exist");
	}
}

CInterfaceUi::~CInterfaceUi() {
	m_Localdb.close();
	m_Remotedb.close();
}

void CInterfaceUi::slotSetFilePath(QString szFile) {
	m_szDBFile=szFile;
}

void CInterfaceUi::slotStartSync() {
	QVariantMap item;
	// check database status
	if (!checkDB(DB_LOCAL) || !checkDB(DB_REMOTE)) {
		return;
	}


	// success
	emit sigUpdateInfo("Sync Completed");
}

bool CInterfaceUi::checkDB(int nType) {
	switch (nType) {
		default: return false;
		case DB_LOCAL:
			if (!m_Localdb.open(QSZ(m_szDBFile))) {
				emit sigUpdateInfo("open local db failed");
				return false;
			}
			return true;
		case DB_REMOTE:
			// <ip>::<database>::<user>::<password>
			QString szBuf;
			szBuf.sprintf("%s::%s::%s::%s", DB_HOST, DB_NAME, DB_USER, DB_PASS);
			if (!m_Remotedb.open(QSZ(szBuf))) {
				emit sigUpdateInfo("open remote db failed");
				return false;
			}
			return true;
	}

	return false;
}
