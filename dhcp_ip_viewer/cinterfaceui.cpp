#include "cinterfaceui.h"
#include "debug.h"

#define IP_PREFIX "192.168.48."

CInterfaceUi::CInterfaceUi(QObject *parent) : QObject(parent)
{
	connect(&m_recv, SIGNAL(sigStartNewBootDev(int)), this, SLOT(slotAddNewDevice(int)), Qt::QueuedConnection);

	m_nCount=0;
	m_recv.open();
}

CInterfaceUi::~CInterfaceUi() {
	m_map.clear();
}

void CInterfaceUi::slotAddNewDevice(int nPort) {
	UiMap::iterator pItem=m_map.find(nPort);

	m_nCount++;
	emit sigUpdateTotalCount(m_nCount);

	QString szBuf;
	szBuf.sprintf("%s%d", IP_PREFIX, nPort);

	emit sigUpdateIp(szBuf);

	if (pItem != m_map.end()) {
		pItem->second++;

		QVariantMap mapItem;
		mapItem.insert("ip", szBuf);
		mapItem.insert("count", pItem->second);

		emit sigUpdateItem(QVariant::fromValue(mapItem));
	}
	else {
		m_map[nPort]=1;

		QVariantMap mapItem;
		mapItem.insert("ip", szBuf);
		mapItem.insert("count", 1);

		emit sigInsertItem(QVariant::fromValue(mapItem));
	}

}
