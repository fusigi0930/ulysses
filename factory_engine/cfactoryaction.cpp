#include "cfactoryaction.h"
#include "cdoaction.h"
#include "debug.h"
#include <QStringList>
#include <QVariantMap>
#include <QDateTime>
#include <vector>

#ifdef Q_OS_WIN
#include <shlobj.h>
#endif

#include <QDir>

#define IP_PREFIX "192.168.48.%d"
//#define ANOTHER_THREAD
#define BOOT_TIMEOUT 80000
#define MAX_GOING_DEV 3

//
// mac: -- mac address
// ip: -- ip address
// style: -- (android, yocto, factory, ubuntu, uboot)
// image
// itemcolor: -- color
//

#define HOST_ITEM_IP			"ip"
#define HOST_ITEM_MAC			"mac"
#define HOST_ITEM_STYLE			"style"
#define HOST_ITEM_COLOR			"itemcolor"

#define HOST_COLOR_NORMAL		"#C0C0E0"
#define HOST_COLOR_WAIT			"#F0F050"
#define HOST_COLOR_HALT			"#909090"
#define HOST_COLOR_PASS			"#70F070"
#define HOST_COLOR_ALREADY_PASS "#90FF90"
#define HOST_COLOR_FAIL			"#F07070"


CFactoryAction::CFactoryAction() : QObject() ,CBaseAction(),
	m_actionThread(this),
	m_szXmlName("items.xml"),
	m_broadcastRecv()
{
	m_bIsRunning=false;

	connect(&m_broadcastRecv, SIGNAL(sigStartNewBootDev(int)), this, SLOT(slotStartNewBootDev(int)), Qt::QueuedConnection);
	connect(&m_broadcastRecv, SIGNAL(sigStartKernel(int)), this, SLOT(slotEndBootDev(int)), Qt::QueuedConnection);
	connect(&m_broadcastRecv, SIGNAL(sigStartNewSysDev(QString)), this, SLOT(slotStartNewSysDev(QString)), Qt::QueuedConnection);
	connect(&m_broadcastRecv, SIGNAL(sigHaltSysDev(QString)), this, SLOT(slotHeltSysDev(QString)), Qt::QueuedConnection);

	connect (this, SIGNAL(sigHeltDevLater(QString)), this, SLOT(slotHeltSysDev(QString)), Qt::QueuedConnection);

#if defined(ANOTHER_THREAD)
	m_thread = new QThread(this);
	this->moveToThread(m_thread);

	m_thread->start();
#else
	m_thread=NULL;
#endif
	m_broadcastRecv.open();

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

CFactoryAction::~CFactoryAction() {
	m_db.close();
	m_broadcastRecv.close();
	if (m_thread) {
		m_thread->exit();
		delete m_thread;
		m_thread=NULL;
	}

}

int CFactoryAction::init() {
	return 0;
}

int CFactoryAction::run() {
	m_bIsRunning=true;
	return 0;
}

void CFactoryAction::destoryDev(SRunDev *dev) {
	if (dev) {
		if (dev->ncio) {
			dev->ncio->close();
			delete dev->ncio;
		}
		if (dev->tio) {
			dev->tio->close();
			delete dev->tio;
		}
		if (dev->thread) {
			if (dev->thread->isRunning())
				dev->thread->terminate();
			dev->thread->exit(0);
			delete dev->thread;
		}
		if (dev->timer) {
			dev->timer->stop();
			delete dev->timer;
		}
		if (dev->action) {
			dev->action->close();
			delete dev->action;
		}
		delete dev;
	}
}

void CFactoryAction::close() {
	for (std::map<QString, SRunDev*>::iterator pIO=m_mapDevice.begin();
		pIO != m_mapDevice.end(); pIO++) {

		destoryDev(pIO->second);
	}

	m_mapDevice.clear();

	//if (m_actionThread.isRunning()) {
	//	m_actionThread.terminate();
	//}

	m_broadcastRecv.close();
}

void CFactoryAction::slotStartNewBootDev(int nPort) {
	QString szIp;
	QString szMac;
	QString szWo;
	szIp.sprintf(IP_PREFIX, nPort);

	int nRetryTimes=3;

	DMSG("start new boot dev: %s", QSZ(szIp));
	std::map<QString, SRunDev*>::iterator pFind=m_mapDevice.find(szIp);
	if (pFind != m_mapDevice.end()) {
		DMSG("duplicate ip address!");
		return;
	}

	SRunDev *newDev=new SRunDev;
	newDev->ncio=new CNcIO();
	newDev->tio=new CTelnetIO();
	newDev->action=new CDoAction(newDev, m_szXmlName);
	dynamic_cast<CDoAction*>(newDev->action)->setDB(&m_db);
	newDev->thread=new CNetActionThread(newDev->action);
	newDev->timer=new CTimer();
	newDev->szIp=szIp;
	newDev->nRuntimeStatus=_RT_PROCESSING;

	connect(newDev->timer, SIGNAL(sigTimeout(QTimer*)), this, SLOT(slotTimerTimeout(QTimer*)), Qt::QueuedConnection);
	connect(newDev->timer, SIGNAL(sigTimeoutClose(QTimer*)), this, SLOT(slotTimeoutClose(QTimer*)), Qt::QueuedConnection);
	//connect(dynamic_cast<CNcIO*>(newDev->ncio), SIGNAL(sigStartKernel(int)), dynamic_cast<CDoAction*>(newDev->action), SLOT(slotStartKernel(int)), Qt::QueuedConnection);
	connect(dynamic_cast<CNcIO*>(newDev->ncio), SIGNAL(sigStartKernel(int)), this, SLOT(slotEndBootDev(int)), Qt::QueuedConnection);
	connect(dynamic_cast<CDoAction*>(newDev->action), SIGNAL(sigAddShowItem(QVariant)), this, SIGNAL(sigAddShowItem(QVariant)), Qt::QueuedConnection);
	connect(dynamic_cast<CDoAction*>(newDev->action), SIGNAL(sigUpdateShowItem(QVariant)), this, SIGNAL(sigUpdateShowItem(QVariant)), Qt::QueuedConnection);
	connect(dynamic_cast<CDoAction*>(newDev->action), SIGNAL(sigUpdateHost(QVariant)), this, SIGNAL(sigUpdateHost(QVariant)), Qt::QueuedConnection);

	DMSG("add to device map: %s", QSZ(szIp));
	m_mapDevice[szIp]=newDev;

	DMSG("start communicate: %s", QSZ(szIp));
	QString szBuf;

	do {
		wait(1200);
		newDev->ncio->close();
		DMSG("start connect to udp port: %d, %d", 7000+nPort, nRetryTimes);

		szBuf.sprintf("net:client:%s:%d", QSZ(szIp), 7000+nPort);
		newDev->ncio->open(QSZ(szBuf));
		wait(1500);

		newDev->ncio->setPrompt("EMBUX-GEMINI");

		DMSG("ready to get mac address");
		wait(600);

		// get mac address
		szBuf="printenv ethaddr\n";
		newDev->ncio->write(szBuf);

		newDev->ncio->waitPrompt(300);
		newDev->ncio->read(szMac);
		int nPos=szMac.lastIndexOf("=");
		int nEnd=szMac.indexOf('\n',nPos+1);
		szMac=szMac.mid(nPos+1, nEnd-nPos);
		szMac=szMac.trimmed();
		szMac.replace(":", "");
		szMac.toUpper();
		newDev->szMac=szMac;
	} while (szMac.isEmpty() && nRetryTimes--);

	// get wo number
	szBuf="printenv wonum\n";
	newDev->ncio->write(szBuf);

	newDev->ncio->waitPrompt(300);
	newDev->ncio->read(szWo);
	int nPos=szWo.lastIndexOf("=");
	int nEnd=szWo.indexOf('\n',nPos+1);
	szWo=szWo.mid(nPos+1, nEnd-nPos);
	szWo=szWo.trimmed();
	newDev->szWo=szWo;
	newDev->nStatus=_TS_RUNNING;

	DMSG("add ui item!");
	emit sigStartNewBootDev(szIp, szMac);

	if (szMac.isEmpty()) {
		QVariantMap mapItem;

		mapItem.insert(HOST_ITEM_IP, szIp);
		mapItem.insert(HOST_ITEM_COLOR, HOST_COLOR_FAIL);
		mapItem.insert(HOST_ITEM_STYLE, "boot");
		newDev->nStatus = _TS_FAIL;
		emit sigUpdateHost(QVariant::fromValue(mapItem));
		newDev->nRuntimeStatus=_RT_STANDBY;
		return;
	}

	processTarget(newDev);

	if (_TS_ALREADY_TESTED == newDev->nStatus) {
		QVariantMap mapItem;

		mapItem.insert(HOST_ITEM_IP, szIp);
		mapItem.insert(HOST_ITEM_COLOR, HOST_COLOR_ALREADY_PASS);
		mapItem.insert(HOST_ITEM_STYLE, "boot");
		emit sigUpdateHost(QVariant::fromValue(mapItem));
	}
	newDev->nRuntimeStatus=_RT_STANDBY;
}

void CFactoryAction::slotStartNewSysDev(QString ip) {
	QStringList listData=ip.split(":");
	QString szIp=listData.at(1);

	if (0 != listData.at(0).compare("factory")) {
		DMSG("not factory type");
		return;
	}

	std::map<QString, SRunDev*>::iterator pFind=m_mapDevice.find(szIp);
	if (pFind == m_mapDevice.end()) {
		DMSG("the device is not exist");
		return;
	}
	pFind->second->nRuntimeStatus=_RT_PROCESSING;

	QVariantMap mapItem;

	mapItem.insert(HOST_ITEM_IP, pFind->first);
	mapItem.insert(HOST_ITEM_COLOR, HOST_COLOR_NORMAL);
	mapItem.insert(HOST_ITEM_STYLE, listData.at(0));

	emit sigUpdateHost(QVariant::fromValue(mapItem));
	if (pFind->second->timer) {
		pFind->second->timer->stop();
	}
	if (pFind->second->ncio) {
		pFind->second->ncio->close();
	}

	wait(350);

	if (pFind->second->tio) {
		QString szCmd="\nroot\n";

		if (!pFind->second->tio->open(QSZ(pFind->first))) {
			QVariantMap mapItem;

			mapItem.insert(HOST_ITEM_IP, szIp);
			mapItem.insert(HOST_ITEM_COLOR, HOST_COLOR_FAIL);
			mapItem.insert(HOST_ITEM_STYLE, "factory");
			pFind->second->nStatus = _TS_FAIL;
			emit sigUpdateHost(QVariant::fromValue(mapItem));
			pFind->second->thread->terminate();
			pFind->second->nRuntimeStatus=_RT_STANDBY;
			return;
		}
		wait(5000);

		szCmd="\nroot\n";
		pFind->second->tio->write(szCmd);
		QThread::msleep(350);
		szCmd="export PATH=$PATH:/nfs/common/rootfs/usr/bin\n";
		pFind->second->tio->write(szCmd);
		QThread::msleep(350);
		szCmd="export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/nfs/common/rootfs/usr/lib\n";
		pFind->second->tio->write(szCmd);
		QThread::msleep(350);
		pFind->second->tio->read(szCmd);
		DMSG("init telnet res: %s", QSZ(szCmd));

		pFind->second->tio->setPrompt("#");
		pFind->second->action->actionUnblock();

	}
	pFind->second->nRuntimeStatus=_RT_STANDBY;
}

void CFactoryAction::slotHeltSysDev(QString ip) {
	QString szIp=ip;

	std::map<QString, SRunDev*>::iterator pFind=m_mapDevice.find(szIp);
	if (pFind == m_mapDevice.end()) {
		DMSG("the device is not exist");
		return;
	}

	if (_RT_PROCESSING == pFind->second->nRuntimeStatus) {
		DMSG("event still processing, kill later: %s", QSZ(pFind->first));
		emit sigHeltDevLater(pFind->first);
		return;
	}
	destoryDev(pFind->second);

	QVariantMap mapItem;

	mapItem.insert(HOST_ITEM_IP, pFind->first);

	emit sigRemoveHost(QVariant::fromValue(mapItem));

	m_mapDevice.erase(pFind);

	wait(1500);
}

void CFactoryAction::slotEndBootDev(int nPort) {
	QString szIp;
	szIp.sprintf(IP_PREFIX, nPort);

	DMSG("end boot device: %d", nPort);
	std::map<QString, SRunDev*>::iterator pFind=m_mapDevice.find(szIp);
	if (pFind == m_mapDevice.end()) {
		DMSG("the device is not exist");
		return;
	}

	QVariantMap mapItem;

	mapItem.insert(HOST_ITEM_IP, pFind->first);
	mapItem.insert(HOST_ITEM_STYLE, "boot");
	mapItem.insert(HOST_ITEM_COLOR, HOST_COLOR_WAIT);

	emit sigUpdateHost(QVariant::fromValue(mapItem));
	if (pFind->second->timer) {
		pFind->second->timer->setInterval(MAX_GOING_DEV * BOOT_TIMEOUT);
		pFind->second->timer->setSingleShot(true);
		pFind->second->timer->start();

		QString szRes;
		pFind->second->ncio->read(szRes);
		DMSG("boot msg: %s", QSZ(szRes));
	}
}

void CFactoryAction::slotClose() {
	close();
}

void CFactoryAction::slotTimerTimeout(QTimer *timer) {
	if (NULL == timer) {
		return;
	}

	for (std::map<QString, SRunDev*>::iterator pItem=m_mapDevice.begin();
		pItem != m_mapDevice.end(); pItem++) {

		if (pItem->second->timer == timer) {
			QVariantMap mapItem;

			mapItem.insert(HOST_ITEM_IP, pItem->first);
			mapItem.insert(HOST_ITEM_STYLE, "boot");
			mapItem.insert(HOST_ITEM_COLOR, HOST_COLOR_HALT);

			emit sigUpdateHost(QVariant::fromValue(mapItem));
			break;
		}
	}
}

void CFactoryAction::slotTimeoutClose(QTimer *timer) {
	if (NULL == timer) {
		return;
	}

	for (std::map<QString, SRunDev*>::iterator pItem=m_mapDevice.begin();
		pItem != m_mapDevice.end(); pItem++) {

		if (pItem->second->timer == timer) {
			QVariantMap mapItem;

			mapItem.insert(HOST_ITEM_IP, pItem->first);
			emit sigRemoveHost(QVariant::fromValue(mapItem));
			destoryDev(pItem->second);

			m_mapDevice.erase(pItem);
			break;
		}
	}
}

void CFactoryAction::slotShowItem(QString szIp) {
	std::map<QString, SRunDev*>::iterator pItem=m_mapDevice.find(szIp);
	if (pItem == m_mapDevice.end()) {
		return;
	}

	dynamic_cast<CDoAction*>(pItem->second->action)->slotShowItems();
}

void CFactoryAction::slotRemoveHost(QVariant item) {
	QMap<QString, QVariant> mapItem=item.toMap();

	std::map<QString, SRunDev*>::iterator pItem=m_mapDevice.find(mapItem["ip"].toString());

	if (pItem == m_mapDevice.end()) {
		return;
	}

	destoryDev(pItem->second);
	m_mapDevice.erase(pItem);
}

void CFactoryAction::slotRemoveFailedHosts() {
	std::map<QString, SRunDev*>::iterator pItem;
	std::vector<QString> vt;
	for (pItem = m_mapDevice.begin(); pItem != m_mapDevice.end(); pItem++) {
		if (_TS_FAIL == pItem->second->nStatus) {
			QVariantMap mapItem;

			mapItem.insert(HOST_ITEM_IP, pItem->first);

			emit sigRemoveHost(QVariant::fromValue(mapItem));

			destoryDev(pItem->second);
			vt.push_back(pItem->first);
		}
	}

	for (std::vector<QString>::iterator pIP=vt.begin(); pIP!=vt.end(); pIP++) {
		m_mapDevice.erase(*pIP);
	}

}

void CFactoryAction::slotRemovePassedHosts() {
	std::map<QString, SRunDev*>::iterator pItem;
	std::vector<QString> vt;
	for (pItem = m_mapDevice.begin(); pItem != m_mapDevice.end(); pItem++) {
		if (_TS_ALREADY_TESTED == pItem->second->nStatus) {
			QVariantMap mapItem;

			mapItem.insert(HOST_ITEM_IP, pItem->first);

			emit sigRemoveHost(QVariant::fromValue(mapItem));

			destoryDev(pItem->second);
			vt.push_back(pItem->first);
		}
	}

	for (std::vector<QString>::iterator pIP=vt.begin(); pIP!=vt.end(); pIP++) {
		m_mapDevice.erase(*pIP);
	}

}

void CFactoryAction::wait(int msec) {
#if defined(ANOTHER_THREAD)
	QThread::msleep(msec);
#else
	QDateTime startTime=QDateTime::currentDateTime();
	int nTimes=msec/30;
	while (msec > (QDateTime::currentDateTime().toMSecsSinceEpoch() - startTime.toMSecsSinceEpoch())) {
		QCoreApplication::processEvents();
		QThread::msleep(30);
		nTimes--;
		if (0 > nTimes)
			break;
	}
#endif
}

long long CFactoryAction::processTarget(SRunDev *dev) {
	if (NULL == dev) {
		DMSG("empty dev info!");
		return -1;
	}
	// check duplicate mac address
	DMSG("detect report status!");
	QString szSQL;
	std::list<QVariant> lst;

	DMSG("query database file");
	m_db.query(lst, "select * from target, board_info where "
					"target.mac='%s' and target.id=board_info.tid "
					"order by board_info.id desc limit 1;",
					QSZ(dev->szMac));

	DMSG("ready to process, target status: %d", lst.size());
	QVariantMap dbItem;

	if (0 == lst.size()) {
		dbItem.insert("type", "target");
		dbItem.insert("mac", dev->szMac);
		dbItem.insert("wonum", dev->szWo);
		dbItem.insert("unum", "-");
		dbItem.insert("board_name", "-");
		dev->info.nTime=QDateTime::currentMSecsSinceEpoch();
		dbItem.insert("sdate", dev->info.nTime);
		dev->info.nTargetID=m_db.add(QVariant::fromValue(dbItem));
		dev->thread->start();
		return dev->info.nTargetID;
	}

	int nResult=lst.begin()->toMap()["result"].toInt();
	switch (nResult) {
		default:
			dev->info.nTargetID=lst.begin()->toMap()["id"].toLongLong();
			DMSG("start factory test thread! tid: %d", dev->info.nTargetID);
			dev->thread->start();
			break;
		case _DB_RESULT_PASS:
			dev->info.nTargetID=lst.begin()->toMap()["id"].toLongLong();
			dev->nStatus=_TS_ALREADY_TESTED;
			break;
	}
	return dev->info.nTargetID;
}

void CFactoryAction::actionBlock() {

}

void CFactoryAction::actionUnblock() {

}
