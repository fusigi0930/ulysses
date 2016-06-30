#ifndef CSQLITESTORE_H
#define CSQLITESTORE_H

#include "resultcore_global.h"
#include "base_store.h"

class RESULTCORESHARED_EXPORT CSQLiteStore : public CBaseStore
{
protected:
	QString m_szFile;

public:
	CSQLiteStore();
	~CSQLiteStore();

	virtual bool open(char *szFile);

	virtual unsigned long long add(const QVariant &item);

	virtual bool update(const QVariant &item);

	virtual void remove(const QVariant &item);

	virtual bool query(std::list<QVariant> &result, char *fmt, ...);

	virtual void close();
};

#endif // CSQLITESTORE_H
