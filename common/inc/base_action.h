#ifndef __BASE_ACTION_H__
#define __BASE_ACTION_H__

#include <QObject>
#include <QString>

/////////////////////////////////


class CBaseAction {
public:
	CBaseAction() {

	}

	virtual ~CBaseAction() {

	}

	virtual void close() = 0;
	
	virtual int run() = 0;
};

#endif // __BASE_ACTION_H__

