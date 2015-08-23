#include "xmlrun.h"
#include "xml_common.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#define XML_MAIN_NODE "testcase"

#define XML_CASE		"case"

#define XML_CASE_ITEM	"item"
#define XML_CASE_PRECMD "precmd"
#define XML_CASE_POSTCMD "postcmd"
#define XML_CASE_CMD	"command"
#define XML_CASE_EXPECT	"expect"
#define XML_CASE_COMMENT "comment"
#define XML_CASE_DELAY	"delay"
#define XML_CASE_TYPE	"type"

#define XML_CASE_TYPE_AUTO		"auto"
#define XML_CASE_TYPE_MANUAL	"manual"
#define XML_CASE_TYPE_HAUTO		"helfauto"

enum ETYPE {
	CMDTYPE_AUTO = 0,
	CMDTYPE_MANUAL,
	CMDTYPE_HAUTO,
};

CXmlRun::CXmlRun() : CRunItem<QXmlStreamReader, QXmlStreamWriter>(),
					m_szMainNode(XML_MAIN_NODE)
{
	m_reader = new QXmlStreamReader;
	m_writer = new QXmlStreamWriter;

}

CXmlRun::~CXmlRun() {
	if (m_reader) {
		delete m_reader;
		m_reader = NULL;
	}
	if (m_writer) {
		delete m_writer;
		m_writer = NULL;
	}
}

bool CXmlRun::getCase() {
	SItem item;
	item.nCmdType=CMDTYPE_AUTO;
	while (!m_reader->atEnd() && ! m_reader->hasError()) {
		QXmlStreamReader::TokenType readToken=m_reader->readNext();

		switch(readToken) {
			default:
				continue;
			case QXmlStreamReader::StartElement:
				XML_GET_VAULE_TEXT((*m_reader), XML_CASE_ITEM, item.szName);
				XML_GET_VAULE_TEXT((*m_reader), XML_CASE_PRECMD, item.szPreProc);
				XML_GET_VAULE_TEXT((*m_reader), XML_CASE_POSTCMD, item.szPostProc);
				XML_GET_VAULE_TEXT((*m_reader), XML_CASE_CMD, item.szProc);
				XML_GET_VAULE_TEXT((*m_reader), XML_CASE_EXPECT, item.szExpect);
				XML_GET_VAULE_TEXT((*m_reader), XML_CASE_COMMENT, item.szDesc);
				XML_GET_VAULE_INT((*m_reader), XML_CASE_DELAY, item.nDelay);
				XML_GET_VALUE_TEXT_BEGIN((*m_reader), XML_CASE_TYPE)
					if (0 == szBuf.toLower().compare(XML_CASE_TYPE_AUTO)) {
						item.nCmdType=static_cast<int>(CMDTYPE_AUTO);
					}
					else if (0 == szBuf.toLower().compare(XML_CASE_TYPE_MANUAL)) {
						item.nCmdType=static_cast<int>(CMDTYPE_MANUAL);
					}
					else if (0 == szBuf.toLower().compare(XML_CASE_TYPE_HAUTO)) {
						item.nCmdType=static_cast<int>(CMDTYPE_HAUTO);
					}
				XML_GET_VALUE_TEXT_END;
				break;
			case QXmlStreamReader::EndElement:
				if (0 == m_reader->name().toString().compare(XML_CASE)) {
					m_items.push_back(item);
					return true;
				}
				break;
		}
	}
	return false;
}

bool CXmlRun::getMainNode() {
	while (!m_reader->atEnd() && ! m_reader->hasError()) {
		QXmlStreamReader::TokenType readToken=m_reader->readNext();

		switch(readToken) {
			default:
				continue;
			case QXmlStreamReader::StartElement:
				if (0 == m_reader->name().toString().compare(XML_CASE)) {
					if (!getCase()) {
						DMSG("get case: %d failed!", m_reader->lineNumber());
						return false;
					}
				}
				break;
			case QXmlStreamReader::EndElement:
				break;
		}
	}
	return true;
}

bool CXmlRun::parser() {
	if (!m_sourceFile.isOpen()) {
		DMSG("source file is not opened!");
		return false;
	}

	m_reader->setDevice(&m_sourceFile);

	while (!m_reader->atEnd() && ! m_reader->hasError()) {
		QXmlStreamReader::TokenType readToken=m_reader->readNext();

		switch(readToken) {
			default:
				continue;
			case QXmlStreamReader::StartElement:
				if (0 == m_reader->name().toString().compare(m_szMainNode)) {
					if (!getMainNode()) {
						DMSG("get main node: %s failed!", QSZ(m_szMainNode));
						return false;
					}
				}
				break;
			case QXmlStreamReader::EndElement:
				break;
		}
	}

	return true;
}

bool CXmlRun::writer() {
	if (!m_destFile.isOpen()) {
		DMSG("destination file is not opened!");
		return false;
	}
	return true;
}

////////////////////////////////////////////////////
///
/// Factory Runner 2.0
///

CXmlFactoryRun::CXmlFactoryRun() : CBaseItem<QXmlStreamReader, QXmlStreamWriter, SFactoryItem>() {
	m_reader=new QXmlStreamReader();
	m_writer=new QXmlStreamWriter();
}

CXmlFactoryRun::~CXmlFactoryRun() {
	if (m_reader) {
		delete m_reader;
		m_reader=NULL;
	}
	if (m_writer) {
		delete m_writer;
		m_writer=NULL;
	}
}

bool CXmlFactoryRun::parser() {
	return true;
}

bool CXmlFactoryRun::writer() {
	return false;
}
