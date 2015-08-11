#ifndef XMLRUN_H
#define XMLRUN_H

#include "xmlrun_global.h"
#include "runitem.h"

#include <QtXml/QtXml>

class XMLRUNSHARED_EXPORT CXmlRun : public CRunItem<QXmlStreamReader,
													QXmlStreamWriter>
{

public:
	CXmlRun();
	virtual ~CXmlRun();

protected:
	virtual bool parser();
	virtual bool writer();

};

#endif // XMLRUN_H
