#include "cfactoryaction.h"
#include "cdoaction.h"
#include "debug.h"
#include <QStringList>
#include <QVariantMap>
#include <QDateTime>

#define IP_PREFIX "192.168.48.%d"

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
#define HOST_COLOR_FAIL			"#F07070"


CFactoryAction::CFactoryAction() : QObject() ,CBaseAction(),
	m_actionThread(this),
	m_szXmlName("items.xml"),
	m_broadcastRecv()
{
	m_bIsRunning=false;

	connect(&m_broadcastRecv, SIGNAL(sigStartNewBootDev(int)), this, SLOT(slotStartNewBootDev(int)));
	connect(&m_broadcastRecv, SIGNAL(sigStartKernel(int)), this, SLOT(slotEndBootDev(int)));
	connect(&m_broadcastRecv, SIGNAL(sigStartNewSysDev(QString)), this, SLOT(slotStartNewSysDev(QString)));
	connect(&m_broadcastRecv, SIGNAL(sigHaltSysDev(QString)), this, SLOT(slotHeltSysDev(QString)));

	m_broadcastRecv.open();
}

CFactoryAction::~CFactoryAction() {

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
		if (dev->thread) {
			if (dev->thread->isRunning())
				dev->thread->terminate();
			delete dev->thread;
		}
		if (dev->rio) {
			dev->rio->close();
			delete dev->rio;
		}
		if (dev->wio) {
			dev->tio->close();
			delete dev->wio;
		}
		if (dev->tio) {
			dev->tio->close();
			delete dev->tio;
		}
		if (dev->timer) {
			dev->timer->stop();
			delete dev->timer;
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

	std::map<QString, SRunDev*>::iterator pFind=m_mapDevice.find(szIp);
	if (pFind != m_mapDevice.end()) {
		DMSG("duplicate ip address!");
		return;
	}

	SRunDev *newDev=new SRunDev;
	newDev->rio=new CNetcatIO();
	newDev->wio=new CNetcatIO();
	newDev->tio=new CTelnetIO();
	newDev->action=new CDoAction(newDev, m_szXmlName);
	newDev->thread=new CNetActionThread(newDev->action);
	newDev->timer=new CTimer();
	newDev->szIp=szIp;

	connect(newDev->timer, SIGNAL(sigTimeout(QTimer*)), this, SLOT(slotTimerTimeout(QTimer*)));
	connect(newDev->timer, SIGNAL(sigTimeoutClose(QTimer*)), this, SLOT(slotTimeoutClose(QTimer*)));
	connect(dynamic_cast<CNetcatIO*>(newDev->wio), SIGNAL(sigStartKernel(int)), dynamic_cast<CDoAction*>(newDev->action), SLOT(slotStartKernel(int)));
	connect(dynamic_cast<CNetcatIO*>(newDev->wio), SIGNAL(sigStartKernel(int)), this, SLOT(slotEndBootDev(int)));
	connect(dynamic_cast<CDoAction*>(newDev->action), SIGNAL(sigAddShowItem(QVariant)), this, SIGNAL(sigAddShowItem(QVariant)));
	connect(dynamic_cast<CDoAction*>(newDev->action), SIGNAL(sigUpdateShowItem(QVariant)), this, SIGNAL(sigUpdateShowItem(QVariant)));
	connect(dynamic_cast<CDoAction*>(newDev->action), SIGNAL(sigUpdateHost(QVariant)), this, SIGNAL(sigUpdateHost(QVariant)));

	m_mapDevice[szIp]=newDev;

	QString szBuf;
	wait(600);

	szBuf.sprintf("net:client:%s:%d", QSZ(szIp), 7000+nPort);
	newDev->wio->open(QSZ(szBuf));
	szBuf.sprintf("net:server:%d", 7000+nPort);
	newDev->rio->open(QSZ(szBuf));

	newDev->rio->setPrompt("EMBUX-TAURUS");

	wait(500);

	// get mac address
	szBuf="printenv ethaddr\n";
	newDev->wio->write(szBuf);

	newDev->rio->waitPrompt(300);
	newDev->rio->read(szMac);
	int nPos=szMac.lastIndexOf("=");
	int nEnd=szMac.indexOf('\n',nPos+1);
	szMac=szMac.mid(nPos+1, nEnd-nPos);
	szMac=szMac.trimmed();
	newDev->szMac=szMac;

	// get wo number
	szBuf="printenv wonum\n";
	newDev->wio->write(szBuf);

	newDev->rio->waitPrompt(300);
	newDev->rio->read(szWo);
	nPos=szWo.lastIndexOf("=");
	nEnd=szWo.indexOf('\n',nPos+1);
	szWo=szWo.mid(nPos+1, nEnd-nPos);
	szWo=szWo.trimmed();
	newDev->szWo=szWo;
	newDev->nStatus=_TS_RUNNING;

	emit sigStartNewBootDev(szIp, szMac);

	if (szMac.isEmpty()) {
		QVariantMap mapItem;

		mapItem.insert(HOST_ITEM_IP, szIp);
		mapItem.insert(HOST_ITEM_COLOR, HOST_COLOR_FAIL);
		newDev->nStatus = _TS_FAIL;
		emit sigUpdateHost(QVariant::fromValue(mapItem));
		return;
	}

	newDev->thread->start();
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

	QVariantMap mapItem;

	mapItem.insert(HOST_ITEM_IP, pFind->first);
	mapItem.insert(HOST_ITEM_COLOR, HOST_COLOR_NORMAL);
	mapItem.insert(HOST_ITEM_STYLE, listData.at(0));

	emit sigUpdateHost(QVariant::fromValue(mapItem));
	if (pFind->second->timer) {
		pFind->second->timer->stop();
	}

	QThread::msleep(350);

	if (pFind->second->tio) {
		QString szCmd="\nroot\n";

		pFind->second->tio->open(QSZ(pFind->first));
		wait(3000);

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

#if 0
		if (szCmd.isEmpty()) {
			QVariantMap mapItem;

			mapItem.insert(HOST_ITEM_IP, szIp);
			mapItem.insert(HOST_ITEM_COLOR, HOST_COLOR_FAIL);
			emit sigUpdateHost(QVariant::fromValue(mapItem));
			pFind->second->thread->terminate();
		}
#endif
	}
}

void CFactoryAction::slotHeltSysDev(QString ip) {
	QString szIp=ip;

	std::map<QString, SRunDev*>::iterator pFind=m_mapDevice.find(szIp);
	if (pFind == m_mapDevice.end()) {
		DMSG("the device is not exist");
		return;
	}

	destoryDev(pFind->second);

	QVariantMap mapItem;

	mapItem.insert(HOST_ITEM_IP, pFind->first);

	emit sigRemoveHost(QVariant::fromValue(mapItem));

	m_mapDevice.erase(pFind);
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
	mapItem.insert(HOST_ITEM_COLOR, HOST_COLOR_WAIT);

	emit sigUpdateHost(QVariant::fromValue(mapItem));
	if (pFind->second->timer) {
		pFind->second->timer->setInterval(80000);
		pFind->second->timer->setSingleShot(true);
		pFind->second->timer->start();

		pFind->second->rio->close();
		pFind->second->wio->close();
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
	for (pItem = m_mapDevice.begin(); pItem != m_mapDevice.end(); pItem++) {
		if (_TS_FAIL == pItem->second->nStatus) {
			QVariantMap mapItem;

			mapItem.insert(HOST_ITEM_IP, pItem->first);

			emit sigRemoveHost(QVariant::fromValue(mapItem));

			destoryDev(pItem->second);
			m_mapDevice.erase(pItem);
		}
	}

}

void CFactoryAction::wait(int msec) {
	QDateTime startTime=QDateTime::currentDateTime();
	while (msec > (QDateTime::currentDateTime().toMSecsSinceEpoch() - startTime.toMSecsSinceEpoch())) {
		QCoreApplication::processEvents();
		QThread::msleep(30);
	}
}
