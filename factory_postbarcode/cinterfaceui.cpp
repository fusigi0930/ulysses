#include "cinterfaceui.h"
#include "debug.h"

#ifdef Q_OS_WIN
#include <shlobj.h>
#endif

#include <QDir>

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
	if (!QDir().exists(szFolder)) {
		QDir().mkpath(szFolder);
	}

	m_db.open(QSZ(szDBFile));
}

CInterfaceUi::~CInterfaceUi() {
	m_db.close();
}

void CInterfaceUi::slotUpdateResult(QString szMac, QString szSerial) {
	QVariantMap dbItem;

	QString szCurrentMac;
	szCurrentMac.sprintf("%s:%s:%s:%s:%s:%s", QSZ(szMac.mid(0,2)), QSZ(szMac.mid(2,2)), QSZ(szMac.mid(4,2)),
												QSZ(szMac.mid(6,2)), QSZ(szMac.mid(8,2)), QSZ(szMac.mid(10,2)));

	szCurrentMac=szCurrentMac.toLower();

	dbItem.insert("type", "target");
	dbItem.insert("mac", szMac);
	dbItem.insert("unum", szSerial);
	m_db.update(QVariant::fromValue(dbItem));
}

void CInterfaceUi::slotGetResult(QString szMac) {
	std::list<QVariant> lst;
	QString szCurrentMac;
	szCurrentMac.sprintf("%s:%s:%s:%s:%s:%s", QSZ(szMac.mid(0,2)), QSZ(szMac.mid(2,2)), QSZ(szMac.mid(4,2)),
												QSZ(szMac.mid(6,2)), QSZ(szMac.mid(8,2)), QSZ(szMac.mid(10,2)));

	szCurrentMac=szCurrentMac.toLower();

	DMSG("current Mac: %s", QSZ(szCurrentMac));

	m_db.query(lst, "select * from target, board_info where "
					"target.mac='%s' and target.id=board_info.tid "
					"order by board_info.id desc limit 1;",
					QSZ(szCurrentMac));

	if (0 == lst.size()) {
		emit sigUpdateUi("NA");
		return;
	}

	switch(lst.begin()->toMap()["result"].toLongLong()) {
		default:
		case _DB_RESULT_NA:
			emit sigUpdateUi("NA");
			break;
		case _DB_RESULT_PASS:
			emit sigUpdateUi("PASS");
			break;
		case _DB_RESULT_FAIL:
			emit sigUpdateUi("FAIL");
			break;

	}
}
