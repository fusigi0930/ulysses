#ifndef XMLCFG_H
#define XMLCFG_H

#include "xmlcfg_global.h"
#include "runitem.h"
#include <QString>

#include <QtXml/QtXml>

class QXmlStreamReader;
class QXmlStreamWriter;

struct SConfig {
	QString m_szMethod;
};

class XMLCFGSHARED_EXPORT CXmlConfig : public CBaseItem<QXmlStreamReader,
														QXmlStreamWriter,
														SConfig>
{

public:
	CXmlConfig();
	virtual ~CXmlConfig();
};

#endif // XMLCFG_H
