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

	if (!m_xmlRun.open(m_szXmlFile)) {
		DMSG("open %s failed!", QSZ(m_szXmlFile));
	}

}

CDoAction::CDoAction() : QObject(), CBaseAction(),
	m_szXmlFile("items.xml"),
	m_nStatus(_RUN_STATUS_BOOTLOADER)
{
	m_ptrDev=NULL;

	if (!m_xmlRun.open(m_szXmlFile)) {
		DMSG("open %s failed!", QSZ(m_szXmlFile));
	}
}

CDoAction::~CDoAction() {
	close();
}

int CDoAction::run() {
	if (NULL == m_ptrDev) {
		DMSG("not have test device!");
		return 1;
	}

	switch(m_nStatus) {
		default:
			DMSG("not support!!");
			return 3;
			break;
		case _RUN_STATUS_BOOTLOADER:
			return runBootCmd();
			break;
		case _RUN_STATUS_RAMDISK:
			return runSysCmd();
			break;
	}

	return 0;
}

void CDoAction::close() {

}

int CDoAction::runBootCmd() {
	return 0;
}

int CDoAction::runSysCmd() {
	return 0;
}

void CDoAction::slotSetXMLFile(QString szFile) {
	m_szXmlFile=szFile;
}

void CDoAction::slotStartKernel(int status) {
	m_nStatus=_RUN_STATUS_RAMDISK;
}

void CDoAction::slotShowItems() {
	for (int i=0; i<m_xmlRun.getItemCount(); ++i) {
		SFactoryItem testItem=m_xmlRun.getItem(i);

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
