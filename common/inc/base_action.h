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

    virtual void actionBlock() = 0;

    virtual void actionUnblock() = 0;
};

#endif // __BASE_ACTION_H__

