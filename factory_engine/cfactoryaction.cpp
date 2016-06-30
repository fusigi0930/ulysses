#include "cfactoryaction.h"
#include "cdoaction.h"
#include "debug.h"
#include <QStringList>
#include <QVariantMap>

#define IP_PREFIX "192.168.0.%d"

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


CFactoryAction::CFactoryAction() : QObject() ,CBaseAction(),
	m_actionThread(this),
	m_broadcastRecv()
{
	m_bIsRunning=false;

	connect(&m_broadcastRecv, SIGNAL(sigStartNewBootDev(int)), this, SLOT(slotStartNewBootDev(int)));
	connect(&m_broadcastRecv, SIGNAL(sigStartKernel(int)), this, SLOT(slotEndBootDev(int)));
	connect(&m_broadcastRecv, SIGNAL(sigStartNewSysDev(QString)), this, SLOT(slotStartNewSysDev(QString)));
	connect(&m_broadcastRecv, SIGNAL(sigHaltSysDev(QString)), this, SLOT(slotHeltSysDev(QString)));

	// test code
	connect(this, SIGNAL(sigTest(int)), &m_broadcastRecv, SIGNAL(sigStartKernel(int)));

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

void CFactoryAction::close() {
	for (std::map<QString, SRunDev*>::iterator pIO=m_mapDevice.begin();
		pIO != m_mapDevice.end(); pIO++) {

		if(pIO->second) {
			if (pIO->second->thread) {
				if (pIO->second->thread->isRunning())
					pIO->second->thread->terminate();
				delete pIO->second->thread;
			}
			if (pIO->second->rio) {
				pIO->second->rio->close();
				delete pIO->second->rio;
			}
			if (pIO->second->wio) {
				pIO->second->wio->close();
				delete pIO->second->wio;
			}
			if (pIO->second->tio) {
				pIO->second->tio->close();
				delete pIO->second->tio;
			}
			if (pIO->second->timer) {
				pIO->second->timer->stop();
				delete pIO->second->timer;
			}
			delete pIO->second;
		}
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
	newDev->action=new CDoAction(newDev);
	newDev->thread=new CNetActionThread(newDev->action);
	newDev->timer=new CTimer();
	newDev->szIp=szIp;

	connect(newDev->timer, SIGNAL(sigTimeout(QTimer*)), this, SLOT(slotTimerTimeout(QTimer*)));
	connect(newDev->timer, SIGNAL(sigTimeoutClose(QTimer*)), this, SLOT(slotTimeoutClose(QTimer*)));
	connect(&m_broadcastRecv, SIGNAL(sigStartKernel(int)), dynamic_cast<CDoAction*>(newDev->action), SLOT(slotStartKernel(int)));
	connect(dynamic_cast<CDoAction*>(newDev->action), SIGNAL(sigAddShowItem(QVariant)), this, SIGNAL(sigAddShowItem(QVariant)));

	m_mapDevice[szIp]=newDev;

	QString szBuf;
	szBuf.sprintf("net:client:%s:%d", QSZ(szIp), 7000+nPort);
	newDev->wio->open(QSZ(szBuf));
	szBuf.sprintf("net:server:%d", 7000+nPort);
	newDev->rio->open(QSZ(szBuf));

	newDev->rio->setPrompt("EMBUX-TAURUS");

	QThread::msleep(500);

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

	emit sigStartNewBootDev(szIp, szMac);
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

}

void CFactoryAction::slotHeltSysDev(QString ip) {
	QString szIp=ip;

	std::map<QString, SRunDev*>::iterator pFind=m_mapDevice.find(szIp);
	if (pFind == m_mapDevice.end()) {
		DMSG("the device is not exist");
		return;
	}
}

void CFactoryAction::slotEndBootDev(int nPort) {
	QString szIp;
	szIp.sprintf(IP_PREFIX, nPort);

	std::map<QString, SRunDev*>::iterator pFind=m_mapDevice.find(szIp);
	if (pFind == m_mapDevice.end()) {
		DMSG("the device is not exist");
		return;
	}

	QVariantMap mapItem;

	mapItem.insert(HOST_ITEM_IP, pFind->first);
	mapItem.insert(HOST_ITEM_COLOR, "#F0F050");

	emit sigUpdateHost(QVariant::fromValue(mapItem));
	if (pFind->second->timer) {
		pFind->second->timer->setInterval(5000);
		pFind->second->timer->setSingleShot(true);
		pFind->second->timer->start();
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
			mapItem.insert(HOST_ITEM_COLOR, "#F05050");

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

	m_mapDevice.erase(mapItem["ip"].toString());
}
