#ifndef CTREEITEM_H
#define CTREEITEM_H

#include <list>
#include <QVariant>
#include <QVariantMap>

class CTreeItem
{
private:
	std::list<CTreeItem *> m_childs;
	QVariant m_data;
	CTreeItem *m_parent;

public:
	CTreeItem(CTreeItem *parent=NULL);
	virtual ~CTreeItem();

	CTreeItem *getParent();
	void addChild(CTreeItem *item);
	CTreeItem *getChild(int nIndex);
	int getChildCount();
	int getRow();
	void cleanChilds();
	void deleteChilds(CTreeItem *item);

	QVariant data(QString szRole);
	QVariant data(int column);
	void setData(QVariant data);
	void setData(QVariantMap data);

};

#endif // CTREEITEM_H
