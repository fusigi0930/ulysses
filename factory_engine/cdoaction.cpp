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
	connect(this, SIGNAL(timeout()), this, SLOT(slotTimeout()), Qt::QueuedConnection);
}

CTimer::~CTimer() {

}

void CTimer::slotTimeout() {
	emit sigTimeout(this);
	QTimer::singleShot(2000, this, SLOT(slotTimeoutClose()));
}

void CTimer::slotTimeoutClose() {
	emit sigTimeoutClose(this);
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
	m_nStatus(_RUN_STATUS_BOOTLOADER),
	m_pDB(NULL)
{
	m_ptrDev=dev;

	init();
}

CDoAction::CDoAction() : QObject(), CBaseAction(),
	m_szXmlFile("items.xml"),
	m_nStatus(_RUN_STATUS_BOOTLOADER),
	m_pDB(NULL)
{
	m_ptrDev=NULL;

	init();
}

void CDoAction::init() {
	m_xmlRun.open(m_szXmlFile);
	connect(this, SIGNAL(sigStartKernel(int)), this, SLOT(slotStartKernel(int)), Qt::QueuedConnection);
}

CDoAction::~CDoAction() {
	close();
}

int CDoAction::run() {
	if (NULL == m_ptrDev || NULL == m_pDB) {
		DMSG("not have test device!");
		return 1;
	}

	CDoAction::runFunc runArray[] = {
		&CDoAction::runBootCmd,
		&CDoAction::runSysCmd,
		NULL
	};

	// add board info
	QVariantMap dbItem;
	dbItem.insert("type", "board");
	dbItem.insert("tid", m_ptrDev->info.nTargetID);
	dbItem.insert("result", _DB_RESULT_NA);
	m_ptrDev->info.nBoardID=m_pDB->add(QVariant::fromValue(dbItem));

	// update board name
	dbItem.clear();
	dbItem.insert("type", "target");
	dbItem.insert("id", m_ptrDev->info.nTargetID);
	dbItem.insert("board_name", m_xmlRun.m_Config.szBoardName);
	m_pDB->update(QVariant::fromValue(dbItem));

	bool bFinalResult=true;
	do {
		m_mutex.lock();
		SFactoryItem &item=m_xmlRun.getCurrentItem();

		DMSG("current status: %d", m_nStatus);
		if (runArray[m_nStatus]) {
			int nRet = (this->*runArray[m_nStatus])(&item);
			if (0 != nRet) {
				DMSG("run failed!");
				m_mutex.unlock();
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

		dbItem.clear();
		dbItem.insert("type", "item");
		dbItem.insert("tid", m_ptrDev->info.nTargetID);
		dbItem.insert("bid", m_ptrDev->info.nBoardID);
		dbItem.insert("name", item.szName);
		if (item.szResult.isEmpty()) {
			dbItem.insert("result", _DB_RESULT_NA);
		}
		else if (0 == item.szResult.compare("pass")) {
			dbItem.insert("result", _DB_RESULT_PASS);
		}
		else if (0 == item.szResult.compare("fail")) {
			dbItem.insert("result", _DB_RESULT_FAIL);
		}
		else {
			dbItem.insert("result", _DB_RESULT_NA);
		}
		dbItem.insert("tdate", QDateTime::currentMSecsSinceEpoch());
		m_pDB->add(QVariant::fromValue(dbItem));
		m_mutex.unlock();
	} while (m_xmlRun.nextItem());

	dbItem.clear();
	dbItem.insert("type", "board");
	dbItem.insert("id", m_ptrDev->info.nBoardID);
	dbItem.insert("result", bFinalResult ? _DB_RESULT_PASS : _DB_RESULT_FAIL);
	m_pDB->update(QVariant::fromValue(dbItem));

	runFinalAlarm(bFinalResult);

	QVariantMap itemMap;
	itemMap.insert(ITEM_INFO_IP, m_ptrDev->szIp);
	itemMap.insert(XML_FAC_ITEM_COLOR, (bFinalResult ?
					XML_FAC_ITEM_COLOR_PASS : XML_FAC_ITEM_COLOR_FAIL));

	m_ptrDev->nStatus=(bFinalResult ? _TS_PASS : _TS_FAIL);

	emit sigUpdateHost(QVariant::fromValue(itemMap));

	return 0;
}

void CDoAction::close() {

}

int CDoAction::runFinalAlarm(bool bResult) {
	if (NULL == m_ptrDev) return -1;

	QString szCmd;
	szCmd.sprintf("/nfs/common/rootfs/usr/bin/ebx-alarm %s &\n", (bResult ? "pass" : "fail"));

	if (m_ptrDev->tio) {
		m_ptrDev->tio->write(szCmd);
		m_ptrDev->tio->waitPrompt(3000);
	}


	szCmd.sprintf("cat /nfs/media/image/raw/%s > /dev/fb0\n", (bResult ? "pass" : "fail"));

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
			in=m_ptrDev->ncio;
			out=m_ptrDev->ncio;
			break;
		case _RUN_STATUS_RAMDISK:
			in=m_ptrDev->tio;
			out=m_ptrDev->tio;
			break;
	}

	QStringList::iterator p;
	for (p=cmdList.begin(); p!=cmdList.end(); p++) {
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
		else if(!p->compare(XML_COMMAND_BOOT) || !p->compare("run mmcboot")) {
			//
			DMSG("---- run boot command");
			emit sigStartKernel(0);
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
	m_ptrDev->ncio->waitPrompt(300);
	m_ptrDev->ncio->read(szRes);

	m_ptrDev->ncio->write(item->szCmd);
	DMSG("send command: %s", QSZ(item->szCmd));

	m_ptrDev->ncio->waitPrompt(500+item->nDelay);
	szRes.clear();
	m_ptrDev->ncio->read(szRes);
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
	m_mutex.lock();
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

void CDoAction::setDB(CBaseStore *ptr) {
	m_pDB=ptr;
}

void CDoAction::actionBlock() {
	m_mutex.lock();
}

void CDoAction::actionUnblock() {
	m_mutex.unlock();
}
