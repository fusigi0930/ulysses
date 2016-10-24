#include "ctreeitem.h"
#include <debug.h>
#include <QVariantMap>

CTreeItem::CTreeItem(CTreeItem *parent)
{
	m_parent=parent;
}

CTreeItem::~CTreeItem() {
	cleanChilds();
}

void CTreeItem::deleteChilds(CTreeItem *item) {
	if (NULL == item) return;
	std::list<CTreeItem*>::iterator pItem;
	for (pItem = item->m_childs.begin(); pItem != item->m_childs.end(); pItem++) {
		deleteChilds(*pItem);
	}
	item->m_childs.clear();
	DMSG("delete address: 0x%x", item);
	delete item;
}

void CTreeItem::cleanChilds() {
	std::list<CTreeItem*>::iterator pItem;
	for (pItem = m_childs.begin(); pItem != m_childs.end(); pItem++) {
		deleteChilds(*pItem);
	}
	m_childs.clear();
}

void CTreeItem::addChild(CTreeItem *item) {
	if (NULL == item) return;

	DMSG("add address: 0x%x", item);
	m_childs.push_back(item);
}

CTreeItem* CTreeItem::getChild(int nIndex) {
	if (nIndex >= getChildCount()) return NULL;

	std::list<CTreeItem*>::iterator pItem = m_childs.begin();
	while (0 < nIndex) {
		pItem++;
	}
	return *pItem;
}

CTreeItem* CTreeItem::getParent() {
	return m_parent;
}

int CTreeItem::getChildCount() {
	return m_childs.size();
}

QVariant CTreeItem::data(QString szRole) {
	QVariantMap map=m_data.toMap();
	return map[szRole];
}

QVariant CTreeItem::data(int column) {
	DMSG("get data!!");
	return QVariant();
}

void CTreeItem::setData(QVariant data) {
	m_data = data;
}

void CTreeItem::setData(QVariantMap data) {
	m_data = QVariant::fromValue(data);
}

int CTreeItem::getRow() {
	if (m_parent) {
		std::list<CTreeItem*>::iterator pItem=m_parent->m_childs.begin();
		int nIndex=0;
		while (this != *pItem && pItem != m_parent->m_childs.end()) {
			nIndex++;
		}
		return nIndex;
	}
	return 0;
}
