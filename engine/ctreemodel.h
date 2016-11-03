#ifndef CTREEMODEL_H
#define CTREEMODEL_H

#include <QAbstractItemModel>
#include <QVariant>
#include <QModelIndex>
#include "ctreeitem.h"
#include <QVariantList>
#include "ctestlink.h"

class CTreeModel : public QAbstractItemModel
{
	Q_OBJECT

private:
	CTreeItem *rootItem;
	QHash<int, QByteArray> m_roleNameMapping;
	QString m_szQMLName;
	QString m_szDevName;
	QString m_szIp;
	CTestLinkReader m_reader;

	void getAllIndex(CTreeItem *item, QVariantList &list);

public:
	CTreeModel();
	virtual ~CTreeModel();

	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QModelIndex index(int row, int column,
					  const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
	int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QHash<int, QByteArray> roleNames() const override;
	Q_INVOKABLE QVariantList getAllIndex();
	Q_INVOKABLE QVariant getQMLName();
	Q_INVOKABLE void setQMLName(QString szName);
	Q_INVOKABLE void setDevName(QString szName);

public slots:

	void slotTest();
};

#endif // CTREEMODEL_H
