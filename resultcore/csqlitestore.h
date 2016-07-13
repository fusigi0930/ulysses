#ifndef CSQLITESTORE_H
#define CSQLITESTORE_H

#include "resultcore_global.h"
#include "base_store.h"
#include <QSqlDatabase>
#include <QSqlQuery>

class RESULTCORESHARED_EXPORT CSQLiteStore : public CBaseStore
{
private:
	long long addTarget(const QVariantMap &item);
	long long addBoard(const QVariantMap &item);
	long long addItem(const QVariantMap &item);

	bool updateTarget(const QVariantMap &item);
	bool updateBoard(const QVariantMap &item);
	bool updateItem(const QVariantMap &item);

	void removeTarget(const QVariantMap &item);
	void removeBoard(const QVariantMap &item);
	void removeItem(const QVariantMap &item);

protected:
	QString m_szFile;
	QSqlDatabase m_db;

	virtual bool initDB();

public:
	CSQLiteStore();
	~CSQLiteStore();

	virtual bool open(char *szFile);

	virtual long long add(const QVariant &item);

	virtual bool update(const QVariant &item);

	virtual void remove(const QVariant &item);

	virtual bool query(std::list<QVariant> &result, char *fmt, ...);

	virtual bool query(QSqlQuery &q, char *fmt, ...);

	virtual void close();
};

#endif // CSQLITESTORE_H
