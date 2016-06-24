#include "cfactoryaction.h"
#include "cdoaction.h"
#include "debug.h"

CFactoryAction::CFactoryAction() : QObject() ,CBaseAction(),
	m_actionThread(this),
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
				delete pIO->second->rio;
			}
			if (pIO->second->wio) {
				delete pIO->second->wio;
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
	szIp.sprintf("192.168.0.%d", nPort);

	std::map<QString, SRunDev*>::iterator pFind=m_mapDevice.find(szIp);
	if (pFind != m_mapDevice.end()) {
		DMSG("duplicate ip address!");
		return;
	}

	SRunDev *newDev=new SRunDev;
	newDev->rio=new CNetcatIO();
	newDev->wio=new CNetcatIO();
	newDev->action=new CDoAction();
	newDev->thread=new CNetActionThread(newDev->action);

	m_mapDevice[szIp]=newDev;

	QString szBuf;
	szBuf.sprintf("net:client:%s:%d", QSZ(szIp), 7000+nPort);
	newDev->wio->open(QSZ(szBuf));
	szBuf.sprintf("net:server:%d", 7000+nPort);
	newDev->rio->open(QSZ(szBuf));

	QThread::msleep(50);

	szBuf="printenv ethaddr\n";
	newDev->wio->write(szBuf);

	QThread::msleep(50);
	newDev->rio->read(szMac);
	int nPos=szMac.lastIndexOf("=");
	int nEnd=szMac.indexOf('\n',nPos+1);
	szMac=szMac.mid(nPos+1, nEnd-nPos);

	emit sigStartNewBootDev(szIp, szMac);
}

void CFactoryAction::slotStartNewSysDev(QString ip) {

}

void CFactoryAction::slotHeltSysDev(QString ip) {

}

void CFactoryAction::slotEndBootDev(int nPort) {

}

void CFactoryAction::slotClose() {
	close();
}
