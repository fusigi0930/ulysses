#ifndef CDOACTION_H
#define CDOACTION_H

#include "base_action.h"

class CDoAction : public QObject, public CBaseAction
{
	Q_OBJECT
public:
	CDoAction();
	~CDoAction();

	virtual void close();
	virtual int run();
};

#endif // CDOACTION_H
