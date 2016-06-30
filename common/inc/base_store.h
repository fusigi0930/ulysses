#ifndef __BASE_STORE_H__
#define __BASE_STORE_H__

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVariantMap>
#include <list>

/////////////////////////////////


class CBaseStore {
public:
	CBaseStore() {

	}

	virtual ~CBaseStore() {

	}
	
	virtual bool open(char *szFile) = 0;
	
	virtual unsigned long long add(const QVariant &item) = 0;
	
	virtual bool update(const QVariant &item) = 0;
	
	virtual void remove(const QVariant &item) = 0;
	
	virtual bool query(std::list<QVariant> &result, char *fmt, ...) = 0;

	virtual void close() = 0;
};

#endif // __BASE_STORE_H__

