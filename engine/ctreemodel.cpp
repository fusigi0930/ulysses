#include "ctreemodel.h"
#include "debug.h"

#define DEF_COLUMN_COUNT 4
#define ROLE_NAME		Qt::UserRole+1
#define ROLE_ENABLE		Qt::UserRole+2
#define ROLE_ID			Qt::UserRole+3
#define ROLE_PID		Qt::UserRole+4

CTreeModel::CTreeModel()
{
	m_roleNameMapping[ROLE_NAME]="name";
	m_roleNameMapping[ROLE_ENABLE]="enable";
	m_roleNameMapping[ROLE_ID]="id";
	m_roleNameMapping[ROLE_PID]="pid";
	rootItem=new CTreeItem();
}

CTreeModel::~CTreeModel() {
	rootItem->cleanChilds();
	delete rootItem;
}

QVariant CTreeModel::data(const QModelIndex &index, int role) const {
	if(!index.isValid()) return QVariant();

	CTreeItem *item=static_cast<CTreeItem*>(index.internalPointer());
	switch (role) {
		default:
			break;
		case ROLE_NAME:
			return item->data("name");
		case ROLE_ENABLE:
			return item->data("enable");
		case ROLE_ID:
			return item->data("id");
		case ROLE_PID:
			return item->data("pid");
	}

	return QVariant();
}

Qt::ItemFlags CTreeModel::flags(const QModelIndex &index) const {
	if (!index.isValid()) return 0;

	return QAbstractItemModel::flags(index);
}

QVariant CTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
	DMSG("s: %d, o: %d, r: %d", section, orientation, role);

	return QVariant();
}

QModelIndex CTreeModel::index(int row, int column, const QModelIndex &parent) const {
	if (!hasIndex(row, column, parent)) return QModelIndex();

	CTreeItem *parentItem;
	if(!parent.isValid()) {
		parentItem=const_cast<CTreeItem*>(rootItem);
	}
	else {
		parentItem=static_cast<CTreeItem*>(parent.internalPointer());
	}

	CTreeItem *childItem = parentItem->getChild(row);
	if (childItem) {
		return createIndex(row, column, childItem);
	}

	return QModelIndex();
}

QModelIndex CTreeModel::parent(const QModelIndex &index) const {
	if (!index.isValid()) return QModelIndex();

	CTreeItem *child=static_cast<CTreeItem*>(index.internalPointer());
	CTreeItem *parent=child->getParent();

	if (parent == rootItem) return QModelIndex();

	return createIndex(parent->getRow(), 0, parent);
}

int CTreeModel::rowCount(const QModelIndex &parent) const {
	CTreeItem *parentItem;
	//if (0 < parent.column()) return 0;

	if(!parent.isValid()) {
		parentItem=const_cast<CTreeItem*>(rootItem);
	}
	else {
		parentItem=static_cast<CTreeItem*>(parent.internalPointer());
	}

	return parentItem->getChildCount();
}

int CTreeModel::columnCount(const QModelIndex &parent) const {
	return DEF_COLUMN_COUNT;
}

QHash<int, QByteArray> CTreeModel::roleNames() const {
	return m_roleNameMapping;
}

void CTreeModel::slotTest() {
	DMSG("yes!");
	if (!m_reader.isOpened()) {
		DMSG("not opend database!");
		return;
	}
	m_reader.getPlans();
	beginInsertRows(QModelIndex(), rootItem->getChildCount(), m_reader.m_listPlans.size()-1);
	for (std::list<CTestLinkPlan*>::iterator pItem=m_reader.m_listPlans.begin(); pItem != m_reader.m_listPlans.end(); pItem++) {
		CTreeItem *item=new CTreeItem(rootItem);
		QVariantMap mapItem;

		mapItem.insert("name", (*pItem)->getName());
		item->setData(mapItem);
		rootItem->addChild(item);
	}
	endInsertRows();
}

QVariantList CTreeModel::getAllIndex() {
	QVariantList listIndex;
	getAllIndex(rootItem, listIndex);
	DMSG("list count: %d", listIndex.size());
	return listIndex;
}

void CTreeModel::getAllIndex(CTreeItem *item, QVariantList &list) {
	if (NULL == item) return;

	for (int i=0; i<item->getChildCount(); i++) {
		CTreeItem *child=item->getChild(i);
		list.append(createIndex(i, 0, static_cast<CTreeItem*>(child)));
		getAllIndex(child, list);
	}
}

void CTreeModel::setQMLName(QString szName) {
	m_szQMLName=szName;
	DMSG("qmlname: %s", QSZ(szName));
}

void CTreeModel::setDevName(QString szName) {
	m_szDevName=szName;
	DMSG("devname: %s", QSZ(szName));
	m_reader.setDevName(szName.split(":").at(0));
	m_reader.open(m_szQMLName);
}

QVariant CTreeModel::getQMLName() {
	return m_szQMLName;
}
