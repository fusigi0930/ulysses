#include "cinterfaceui.h"
#include "debug.h"

#ifdef Q_OS_WIN
#include <shlobj.h>
#endif

#include <QDir>
#include <typeinfo>

enum EDBTYPE {
	DB_LOCAL,
	DB_REMOTE,
};

#define DB_HOST "192.168.48.2"
#define DB_NAME "product_report"
#define DB_USER "fortress"
#define DB_PASS "fortress"

//////////////////////////////////////////////
/// \brief CTaskThread::CTaskThread
/// \param ui
///

CTaskThread::CTaskThread(CInterfaceUi *ui) {
	m_ui=ui;
}

CTaskThread::~CTaskThread() {

}

void CTaskThread::run() {
	if (!m_ui) return;

	m_ui->startSync();
}

//////////////////////////////////////////////////////
/// \brief CInterfaceUi::CInterfaceUi
/// \param parent
///

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
	m_thread=new CTaskThread(this);
}

CInterfaceUi::~CInterfaceUi() {
	m_Localdb.close();
	m_Remotedb.close();

	if (m_thread && m_thread->isRunning()) {
		m_thread->terminate();
	}
	if (m_thread) {
		delete m_thread;
	}
}

void CInterfaceUi::slotSetFilePath(QString szFile) {
	m_szDBFile=szFile;
}

void CInterfaceUi::slotStartSync() {
	if (m_thread) {
		m_thread->start();
	}
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

long long CInterfaceUi::getLastSyncInfo(CBaseStore *db, std::list<QVariant> &lst) {
	long long nRet=0;
	if (NULL == db) {
		emit sigUpdateInfo("db error!");
		return -1;
	}

	lst.clear();
	QString szInfo;
	if (!db->query(lst, "select * from sync_info order by id desc limit 1;")) {
		szInfo.sprintf("%s query failed", typeid(*db).name());
		emit sigUpdateInfo(szInfo);
		return -1;
	}

	if (0 == lst.size()) {
		szInfo.sprintf("%s no data", typeid(*db).name());
		emit sigUpdateInfo(szInfo);
		return 0;
	}

	nRet=lst.begin()->toMap()["id"].toLongLong();

	return nRet;
}

long long CInterfaceUi::checkSyncInfo(QVariant info) {
	std::list<QVariant> lst;
	if (!m_Remotedb.query(lst, "select * from sync_info where id=%lld and sdate=%lld;",
						  info.toMap()["sid"].toLongLong(),info.toMap()["sdate"].toLongLong()))
	{
		emit sigUpdateInfo("remote record not found");
		return -1;
	}

	if (0 == lst.size()) {
		emit sigUpdateInfo("remote record not found");
		return -1;
	}

	std::list<QVariant> lstLocal;
	QString szQuery;

	switch (lst.begin()->toMap()["type"].toLongLong()) {
		default:
			emit sigUpdateInfo("sync type error");
			return -1;
		case _DB_TYPE_ADD_TARGET:
			szQuery.sprintf("select * from target where id=%lld;", lst.begin()->toMap()["value"].toLongLong());
			break;
		case _DB_TYPE_ADD_BOARD:
			szQuery.sprintf("select * from board_info where id=%lld;", lst.begin()->toMap()["value"].toLongLong());
			break;
	}

	if (!m_Localdb.query(lstLocal, QSZ(szQuery))) {
		emit sigUpdateInfo("local record not found");
		return -1;
	}

	if (0 == lstLocal.size()) {
		emit sigUpdateInfo("local record not found");
		return -1;
	}

	return lst.begin()->toMap()["id"].toLongLong();
}

long long CInterfaceUi::startSync() {
	QVariantMap item;
	long long nRet=-1;
	// check database status
	if (!checkDB(DB_LOCAL) || !checkDB(DB_REMOTE)) {
		return -1;
	}

	std::list<QVariant> recordLocal, recordRemote;
	long long nLocalSyncId=0, nRemoteSyncId=0;

	do {
		nLocalSyncId=getLastSyncInfo(&m_Localdb, recordLocal);

		if (0 > nLocalSyncId) {
			return -1;
		}

		if (0 == nLocalSyncId) {
			// not have sync data, sync directly
			break;
		}

		// check is remote id exist?
		if (0 > (nRemoteSyncId = checkSyncInfo(*recordLocal.begin())))
		{
			// remote dose not have this record, remove local
			item.clear();
			item.insert("type", "sync");
			item.insert("id", nLocalSyncId);
			m_Localdb.remove(item);
			continue;
		}

	} while (0 > nLocalSyncId || 0 > nRemoteSyncId);

	QVariant local;
	std::list<QVariant> lstTarget;
	if (0 <recordLocal.size()) {
		local=*recordLocal.begin();
	}
	else {
		QVariantMap map;
		map.insert("sdate", 0);
		local=map;
	}
	emit sigUpdateInfo("Updating...");

	// get total sync records number
	if (!m_Localdb.query(lstTarget,
						 "select * from target where sdate>%lld;", local.toMap()["sdate"].toLongLong()))
	{
		emit sigUpdateInfo("fetch target error");
		return -1;
	}

	if (0 < lstTarget.size()) {
		nRet=syncTarget(local);
		addSyncRecord(_DB_TYPE_ADD_TARGET);
	}
	else {
		nRet=syncBoard(local);
		if (m_nTotalCount > 0)
			addSyncRecord(_DB_TYPE_ADD_BOARD);
	}

	// success
	emit sigUpdateInfo("Sync Completed");
	return nRet;
}

void CInterfaceUi::slotTestProg() {
	if (m_thread) {
		m_thread->start();
	}
}

void CInterfaceUi::testProg() {
	QVariantMap info;
	int nMax=100;
	for (int i=0; i<nMax; i++) {
		info.clear();
		info.insert("max", nMax);
		info.insert("current", i);
		QThread::msleep(10);
		emit sigUpdateProgress(info);
	}
}

long long CInterfaceUi::syncTarget(QVariant local) {
	std::list<QVariant> lstTarget, lstBoard, lstItem;
	if (!m_Localdb.query(lstTarget,
						 "select * from target where sdate>%lld;", local.toMap()["sdate"].toLongLong()))
	{
		emit sigUpdateInfo("fetch target error");
		return -1;
	}

	if (!m_Localdb.query(lstBoard,
						 "select * from board_info where idate>%lld;", local.toMap()["sdate"].toLongLong()))
	{
		emit sigUpdateInfo("fetch board_info error");
		return -1;
	}

	if (!m_Localdb.query(lstItem,
						 "select * from item_info where tdate>%lld;", local.toMap()["sdate"].toLongLong()))
	{
		emit sigUpdateInfo("fetch item error");
		return -1;
	}

	m_nTotalCount=lstTarget.size() + lstBoard.size() + lstItem.size();
	m_nCurrentCount=0;

	// start update information
	std::list<QVariant>::iterator pData;
	for (pData=lstTarget.begin(); pData != lstTarget.end(); pData++) {
		QVariantMap item;
		item.insert("type", "target");
		item.insert("mac", pData->toMap()["mac"]);
		item.insert("wonum", pData->toMap()["wonum"]);
		item.insert("unum", pData->toMap()["unum"]);
		item.insert("board_name", pData->toMap()["board_name"]);
		item.insert("sdate", pData->toMap()["sdate"]);
		long long nRemoteTargetId=m_Remotedb.add(item);
		m_nCurrentCount++;
		updateUiInfo();

		item.clear();
		item.insert("o_tid", pData->toMap()["id"]);
		item.insert("n_tid", nRemoteTargetId);

		replaceBoard(item);
	}


	return 0;
}

long long CInterfaceUi::syncBoard(QVariant local) {
	std::list<QVariant> lstBoard, lstItem;
	if (!m_Localdb.query(lstBoard,
						 "select * from board_info where idate>%lld;", local.toMap()["sdate"].toLongLong()))
	{
		emit sigUpdateInfo("fetch board_info error");
		return -1;
	}

	if (!m_Localdb.query(lstItem,
						 "select * from item_info where tdate>%lld;", local.toMap()["sdate"].toLongLong()))
	{
		emit sigUpdateInfo("fetch item error");
		return -1;
	}

	m_nTotalCount = lstBoard.size() + lstItem.size();
	m_nCurrentCount=0;

	// start update information
	std::list<QVariant>::iterator pData;
	for (pData=lstBoard.begin(); pData != lstBoard.end(); pData++) {
		QVariantMap item;

		item.insert("o_tid", pData->toMap()["tid"]);
		std::list<QVariant> lstTemp;
		if (!m_Localdb.query(lstTemp, "select * from target where id=%lld;", pData->toMap()["tid"].toLongLong())) {
			continue;
		}
		if (0 == lstTemp.size()) {
			continue;
		}

		QString szMac=lstTemp.begin()->toMap()["mac"].toString();
		lstTemp.clear();
		if (!m_Remotedb.query(lstTemp, "select * from target where mac=%s;"), QSZ(szMac)) {
			continue;
		}
		if (0 == lstTemp.size()) {
			continue;
		}

		item.insert("n_tid", lstTemp.begin()->toMap()["id"]);

		replaceBoard(item);
	}
	return 0;
}

long long CInterfaceUi::replaceBoard(QVariant replaceInfo) {
	// info fields:
	// o_tid: original local target id
	// n_tid: new remote target id
	std::list<QVariant> lstBoard;

	if (!m_Localdb.query(lstBoard,
						 "select * from board_info where tid=%lld;", replaceInfo.toMap()["o_tid"].toLongLong()))
	{
		emit sigUpdateInfo("fetch board_info error");
		return -1;
	}

	// start update information
	std::list<QVariant>::iterator pData;
	for (pData=lstBoard.begin(); pData != lstBoard.end(); pData++) {
		QVariantMap item;
		item.insert("type", "board");
		item.insert("tid", replaceInfo.toMap()["n_tid"]);
		item.insert("result", pData->toMap()["result"]);
		item.insert("mode", pData->toMap()["mode"]);
		item.insert("idate", pData->toMap()["idate"]);

		long long nRemoteBoardId=m_Remotedb.add(item);
		m_nCurrentCount++;
		updateUiInfo();

		item.clear();
		item.insert("n_tid", replaceInfo.toMap()["n_tid"]);
		item.insert("o_bid", pData->toMap()["id"]);
		item.insert("n_bid", nRemoteBoardId);

		replaceItem(item);
	}
	return 0;
}

long long CInterfaceUi::replaceItem(QVariant replaceInfo) {
	// info fields:
	// n_tid: new remote target id
	// o_bid: original local board_id
	// n_bid: new remote board id

	std::list<QVariant> lstItem;

	if (!m_Localdb.query(lstItem,
						 "select * from item_info where bid=%lld;", replaceInfo.toMap()["o_bid"].toLongLong()))
	{
		emit sigUpdateInfo("fetch item_info error");
		return -1;
	}

	// start update information
	std::list<QVariant>::iterator pData;
	for (pData=lstItem.begin(); pData != lstItem.end(); pData++) {
		QVariantMap item;
		item.insert("type", "item");
		item.insert("tid", replaceInfo.toMap()["n_tid"]);
		item.insert("bid", replaceInfo.toMap()["n_bid"]);
		item.insert("name", pData->toMap()["name"]);
		item.insert("result", pData->toMap()["result"]);
		item.insert("tdate", pData->toMap()["tdate"]);

		m_Remotedb.add(item);
		m_nCurrentCount++;
		updateUiInfo();
	}

	return 0;
}

void CInterfaceUi::updateUiInfo() {
	QVariantMap info;
	info.insert("max", m_nTotalCount);
	info.insert("current", m_nCurrentCount);
	emit sigUpdateProgress(info);
}

long long CInterfaceUi::addSyncRecord(int nType) {
	QVariantMap item;
	std::list<QVariant> lst;
	qint64 nDate=QDateTime::currentMSecsSinceEpoch();
	item.insert("type", "sync");
	item.insert("stype", nType);
	item.insert("sdate", nDate);
	long long nLocalSyncId=m_Localdb.add(item);

	item.clear();
	item.insert("type", "sync");
	item.insert("stype", nType);
	item.insert("sdate", nDate);
	switch (nType) {
		default: break;
		case _DB_TYPE_ADD_BOARD:
			m_Localdb.query(lst, "select * from board_info order by id desc limit 1;");
			break;
		case _DB_TYPE_ADD_TARGET:
			m_Localdb.query(lst, "select * from target order by id desc limit 1;");
			break;
	}
	if (0 < lst.size()) {
		item.insert("value", lst.begin()->toMap()["id"]);
	}
	long long nRemoteSyncId=m_Remotedb.add(item);

	item.clear();
	item.insert("type", "sync");
	item.insert("id", nLocalSyncId);
	item.insert("sid", nRemoteSyncId);
	m_Localdb.update(item);
	return nLocalSyncId;
}
