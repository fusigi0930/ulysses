#ifndef CSQLITESTORE_H
#define CSQLITESTORE_H

#include "resultcore_global.h"
#include "base_store.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMutex>

enum EDBResult {
	_DB_RESULT_NA,
	_DB_RESULT_PASS,
	_DB_RESULT_FAIL,
	_DB_RESULT_UNKNOW
};

enum EDBType {
	_DB_TYPE_ADD_TARGET,
	_DB_TYPE_ADD_BOARD,
};

class RESULTCORESHARED_EXPORT CSQLiteStore : public CBaseStore
{
private:
	long long addTarget(const QVariantMap &item);
	long long addBoard(const QVariantMap &item);
	long long addItem(const QVariantMap &item);
	long long addSync(const QVariantMap &item);

	bool updateTarget(const QVariantMap &item);
	bool updateBoard(const QVariantMap &item);
	bool updateItem(const QVariantMap &item);
	bool updateSync(const QVariantMap &item);

	void removeTarget(const QVariantMap &item);
	void removeBoard(const QVariantMap &item);
	void removeItem(const QVariantMap &item);
	void removeSync(const QVariantMap &item);

protected:
	QString m_szFile;
	QSqlDatabase m_db;
	QMutex m_mutex;

	virtual bool initDB();

	void insertItemMap(QVariantMap &item, QSqlRecord &rinfo, QSqlQuery &query, int nIndex, int nCount=0);

public:
	CSQLiteStore();
	virtual ~CSQLiteStore();

	virtual bool open(char *szFile);

	virtual long long add(const QVariant &item);

	virtual bool update(const QVariant &item);

	virtual void remove(const QVariant &item);

	virtual bool query(std::list<QVariant> &result, char *fmt, ...);

	virtual bool query(QSqlQuery &q, char *fmt, ...);

	virtual void close();
};

class RESULTCORESHARED_EXPORT CUlyStore : public CSQLiteStore {
protected:
	virtual bool initDB();

public:
	CUlyStore();
	virtual ~CUlyStore();

	virtual long long add(const QVariant &item);

	virtual bool update(const QVariant &item);

	virtual void remove(const QVariant &item);
};

class RESULTCORESHARED_EXPORT CDBSyncStore : public CSQLiteStore {
protected:
	virtual bool initDB();

public:
	CDBSyncStore();
	virtual ~CDBSyncStore();

	virtual long long add(const QVariant &item);

	virtual bool update(const QVariant &item);

	virtual void remove(const QVariant &item);
};

#endif // CSQLITESTORE_H
