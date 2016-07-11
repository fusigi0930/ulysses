#include "cdoaction.h"
#include "debug.h"

#define XML_FAC_ITEM			"item"
#define XML_FAC_ITEM_NAME		"name"
#define XML_FAC_ITEM_COMMAND	"command"
#define XML_FAC_ITEM_EXPECT		"expect"
#define XML_FAC_ITEM_IMAGE		"image"
#define XML_FAC_ITEM_COLOR		"itemcolor"
#define XML_FAC_ITEM_DELAY		"delay"
#define XML_FAC_ITEM_POST_COMMAND "postcmd"
#define XML_FAC_ITEM_PRE_COMMAND  "precmd"

#define XML_COMMAND_REBOOT "reboot"
#define XML_COMMAND_PAUSE "pause"
#define XML_COMMAND_DELAY "delay"
#define XML_COMMAND_BOOT "boot"
#define XML_COMMAND_WAIT "wait"

#define XML_FAC_ITEM_COLOR_NORMAL	"#F0F0F0"
#define XML_FAC_ITEM_COLOR_PASS		"#20FF20"
#define XML_FAC_ITEM_COLOR_FAIL		"#FF2020"

#define ITEM_INFO_IP "ip"

CTimer::CTimer() : QTimer() {
	connect(this, SIGNAL(timeout()), this, SLOT(slotTimeout()));
}

CTimer::~CTimer() {

}

void CTimer::slotTimeout() {
	emit sigTimeout(this);
}

CDoAction::CDoAction(SRunDev *dev) : QObject(), CBaseAction(),
	m_szXmlFile("items.xml"),
	m_nStatus(_RUN_STATUS_BOOTLOADER)
{
	m_ptrDev=dev;

	init();
}

CDoAction::CDoAction(SRunDev *dev, QString item) : QObject(), CBaseAction(),
	m_szXmlFile(item),
	m_nStatus(_RUN_STATUS_BOOTLOADER)
{
	m_ptrDev=dev;

	init();
}

CDoAction::CDoAction() : QObject(), CBaseAction(),
	m_szXmlFile("items.xml"),
	m_nStatus(_RUN_STATUS_BOOTLOADER)
{
	m_ptrDev=NULL;

	init();
}

void CDoAction::init() {
	m_xmlRun.open(m_szXmlFile);
}

CDoAction::~CDoAction() {
	close();
}

int CDoAction::run() {
	if (NULL == m_ptrDev) {
		DMSG("not have test device!");
		return 1;
	}

	CDoAction::runFunc runArray[] = {
		&CDoAction::runBootCmd,
		&CDoAction::runSysCmd,
		NULL
	};

	bool bFinalResult=true;
	do {
		SFactoryItem &item=m_xmlRun.getCurrentItem();

		DMSG("current status: %d", m_nStatus);
		if (runArray[m_nStatus]) {
			int nRet = (this->*runArray[m_nStatus])(&item);
			if (0 != nRet) {
				DMSG("run failed!");
				return nRet;
			}
		}

		QVariantMap itemMap;
		itemMap.insert(ITEM_INFO_IP, m_ptrDev->szIp);
		itemMap.insert(XML_FAC_ITEM_COLOR, (0 == item.szResult.compare("pass") ?
												XML_FAC_ITEM_COLOR_PASS : XML_FAC_ITEM_COLOR_FAIL));
		itemMap.insert("index", item.nUiIndex);
		emit sigUpdateShowItem(QVariant::fromValue(itemMap));
		bFinalResult &= (0 == item.szResult.compare("pass") ? true : false);

	} while (m_xmlRun.nextItem());

	QVariantMap itemMap;
	itemMap.insert(ITEM_INFO_IP, m_ptrDev->szIp);
	itemMap.insert(XML_FAC_ITEM_COLOR, (bFinalResult ?
					XML_FAC_ITEM_COLOR_PASS : XML_FAC_ITEM_COLOR_FAIL));

	m_ptrDev->nStatus=(bFinalResult ? _TS_PASS : _TS_FAIL);

	emit sigUpdateHost(QVariant::fromValue(itemMap));

	runFinalAlarm(bFinalResult);

	return 0;
}

void CDoAction::close() {

}

int CDoAction::runFinalAlarm(bool bResult) {
	if (NULL == m_ptrDev) return -1;

	QString szCmd;
	szCmd.sprintf("/nfs/common/rootfs/usr/bin/ebx-alarm %s\n", (bResult ? "pass" : "fail"));

	if (m_ptrDev->tio) {
		m_ptrDev->tio->write(szCmd);

	}
	return 0;
}

int CDoAction::runPrePostCmd(QString szCmd) {
	if (szCmd.isEmpty()) {
		return 0;
	}

	QStringList cmdList=szCmd.split(';');
	CRootIO *in=NULL, *out=NULL;

	switch (m_nStatus) {
		default: break;
		case _RUN_STATUS_BOOTLOADER:
			in=m_ptrDev->rio;
			out=m_ptrDev->wio;
			break;
		case _RUN_STATUS_RAMDISK:
			in=m_ptrDev->tio;
			out=m_ptrDev->rio;
			break;
	}


	for (QStringList::iterator p=cmdList.begin(); p!=cmdList.end(); p++) {
		// reboot
		if (!p->compare(XML_COMMAND_REBOOT)) {
			//
		}
		// delay
		else if (!p->left(5).compare(XML_COMMAND_DELAY)) {
			int nDelay=p->mid(6).toInt();
			DMSG("delay: %d", nDelay);;
			QThread::msleep(nDelay);
			DMSG("finish delay");
			continue;
		}
		// wait
		else if (!p->left(4).compare(XML_COMMAND_WAIT)) {
			int nTimeout=p->mid(5).toInt();
			in->waitPrompt(nTimeout);
			continue;
		}
		// boot
		else if(!p->compare(XML_COMMAND_BOOT)) {
			//
		}
		p->append("\r");

		if (0 == p->left(6).compare("setenv")) {
			p->replace(':', ';');
		}
		DMSG("send command: %s", p->toUtf8().data());
		out->write(*p);
		in->waitPrompt(100);
	}

	return 0;
}

int CDoAction::runBootCmd(SFactoryItem *item) {
	if(NULL == m_ptrDev || NULL == item) {
		return -1;
	}

	QString szRes;

	runPrePostCmd(item->szPreCmd);
	m_ptrDev->rio->waitPrompt(300);
	m_ptrDev->rio->read(szRes);

	m_ptrDev->wio->write(item->szCmd);
	DMSG("send command: %s", QSZ(item->szCmd));

	m_ptrDev->rio->waitPrompt(500+item->nDelay);
	szRes.clear();
	m_ptrDev->rio->read(szRes);
	szRes=szRes.mid(item->szCmd.length());

	DMSG("get response: %s", QSZ(szRes));

	item->szResult=(szRes.contains(item->szExpect) ? "pass" : "fail");

	runPrePostCmd(item->szPostCmd);

	return 0;
}

int CDoAction::runSysCmd(SFactoryItem *item) {
	if(NULL == m_ptrDev || NULL == item) {
		return -1;
	}

	QString szRes;

	runPrePostCmd(item->szPreCmd);
	m_ptrDev->tio->waitPrompt(300);
	m_ptrDev->tio->read(szRes);

	m_ptrDev->tio->write(item->szCmd);
	DMSG("send command: %s", QSZ(item->szCmd));
	m_ptrDev->tio->waitPrompt(500+item->nDelay);
	szRes.clear();
	m_ptrDev->tio->read(szRes);
	szRes=szRes.mid(item->szCmd.length());
	DMSG("get response: %s", QSZ(szRes));

	item->szResult=(szRes.contains(item->szExpect) ? "pass" : "fail");

	runPrePostCmd(item->szPostCmd);
	return 0;
}

void CDoAction::slotSetXMLFile(QString szFile) {
	m_szXmlFile=szFile;
}

void CDoAction::slotStartKernel(int status) {
	DMSG("start kernel-----");
	m_nStatus=_RUN_STATUS_RAMDISK;
}

void CDoAction::slotShowItems() {
	for (int i=0; i<m_xmlRun.getItemCount(); ++i) {
		SFactoryItem &testItem=m_xmlRun.getItem(i);

		QVariantMap itemMap;
		itemMap.insert(XML_FAC_ITEM_NAME, testItem.szName);
		itemMap.insert(XML_FAC_ITEM_PRE_COMMAND, testItem.szPreCmd);
		itemMap.insert(XML_FAC_ITEM_COMMAND, testItem.szCmd);
		itemMap.insert(XML_FAC_ITEM_POST_COMMAND, testItem.szPostCmd);
		itemMap.insert(XML_FAC_ITEM_IMAGE, testItem.szImage);
		itemMap.insert(XML_FAC_ITEM_EXPECT, testItem.szExpect);
		if (0 == testItem.szResult.compare("pass")) {
			itemMap.insert(XML_FAC_ITEM_COLOR, XML_FAC_ITEM_COLOR_PASS);
		}
		else if (0 == testItem.szResult.compare("fail")) {
			itemMap.insert(XML_FAC_ITEM_COLOR, XML_FAC_ITEM_COLOR_FAIL);
		}
		else {
			itemMap.insert(XML_FAC_ITEM_COLOR, XML_FAC_ITEM_COLOR_NORMAL);
		}
		itemMap.insert(ITEM_INFO_IP, m_ptrDev->szIp);
		emit sigAddShowItem(QVariant::fromValue(itemMap));
		testItem.nUiIndex=i;
	}
}
