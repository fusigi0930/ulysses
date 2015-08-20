#ifndef XMLRUN_H
#define XMLRUN_H

#include "xmlrun_global.h"
#include "runitem.h"

#include <QtXml/QtXml>

class QXmlStreamReader;
class QXmlStreamWriter;

class XMLRUNSHARED_EXPORT CXmlRun : public CRunItem<QXmlStreamReader,
													QXmlStreamWriter
													>
{
protected:
	QString m_szMainNode;

public:
	CXmlRun();
	virtual ~CXmlRun();

protected:
	virtual bool parser();
	virtual bool writer();

	virtual bool getMainNode();
	virtual bool getCase();
};

#endif // XMLRUN_H
