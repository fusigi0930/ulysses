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

	virtual bool writeCase();
};

struct SFactoryItem {
	QString szName;
	QString szCmd;
	QString szImage;
	QString szExpect;
	int nUiIndex;
	int nDelay;
	QString szPostCmd;
	QString szPreCmd;
	QString szResult;
};

struct SFactoryConfig {
	QString szSerial;
	QString szBoardName;
};

class XMLRUNSHARED_EXPORT CXmlFactoryRun : public CBaseItem<QXmlStreamReader,
															QXmlStreamWriter,
															SFactoryItem>
{
public:
	SFactoryConfig m_Config;
public:
	CXmlFactoryRun();
	virtual ~CXmlFactoryRun();

protected:
	virtual bool parser();
	virtual bool writer();

	virtual bool getFactory();
	virtual bool getFactoryItem();
	virtual bool getScrpit();
	virtual bool getSettings();

	virtual bool writeItems();
	virtual bool writeSettings();
};

#endif // XMLRUN_H
