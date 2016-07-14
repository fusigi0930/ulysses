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

bool CXmlRun::writeCase() {
	QString szCmdType[] = {
		XML_CASE_TYPE_AUTO,
		XML_CASE_TYPE_MANUAL,
		XML_CASE_TYPE_HAUTO,
	};
	for (QList<SItem>::iterator pItem=m_items.begin(); pItem != m_items.end(); pItem++) {
		XML_SET_ATTR_BEGIN((*m_writer), XML_CASE);
		XML_SET_VAULE((*m_writer), XML_CASE_ITEM, pItem->szName);
		XML_SET_VAULE((*m_writer), XML_CASE_COMMENT, pItem->szDesc);
		XML_SET_VAULE((*m_writer), XML_CASE_PRECMD, pItem->szPreProc);
		XML_SET_VAULE((*m_writer), XML_CASE_CMD, pItem->szProc);
		XML_SET_VAULE((*m_writer), XML_CASE_POSTCMD, pItem->szPostProc);
		XML_SET_VAULE((*m_writer), XML_CASE_DELAY, QString().sprintf("%d", pItem->nDelay));
		XML_SET_VAULE((*m_writer), XML_CASE_EXPECT, pItem->szExpect);
		XML_SET_VAULE((*m_writer), XML_CASE_TYPE, szCmdType[pItem->nCmdType]);
		XML_SET_ATTR_END((*m_writer)); // XML_CASE
	}
	return true;
}

bool CXmlRun::writer() {
	if (!m_destFile.isOpen()) {
		DMSG("destination file is not opened!");
		return false;
	}

	m_writer->setDevice(&m_destFile);
	m_writer->writeStartDocument();
	XML_SET_ATTR_BEGIN((*m_writer), XML_MAIN_NODE);

	if (!writeCase()) {
		DMSG("write xml file failed!");
		return false;
	}

	XML_SET_ATTR_END((*m_writer)); // XML_MAIN_NODE
	m_writer->writeEndDocument();
	return true;
}

////////////////////////////////////////////////////
///
/// Factory Runner 2.0
///

#define DEFAULT_XML_NAME		"items.xml"
#define XML_FACTORY				"factory"

#define XML_FAC_ITEM			"item"
#define XML_FAC_ITEM_NAME		"name"
#define XML_FAC_ITEM_COMMAND	"command"
#define XML_FAC_ITEM_EXPECT		"expect"
#define XML_FAC_ITEM_IMAGE		"image"
#define XML_FAC_ITEM_COLOR		"itemcolor"
#define XML_FAC_ITEM_DELAY		"delay"
#define XML_FAC_ITEM_POST_COMMAND "postcmd"
#define XML_FAC_ITEM_PRE_COMMAND  "precmd"

#define XML_COMMAND_REBOOT "reboot"
#define XML_COMMAND_PAUSE "pause"
#define XML_COMMAND_DELAY "delay"
#define XML_COMMAND_BOOT "boot"

#define XML_FAC_ITEM_COLOR_NORMAL	"#F0F0F0"
#define XML_FAC_ITEM_COLOR_PASS		"#20FF20"
#define XML_FAC_ITEM_COLOR_FAIL		"#FF2020"

#define XML_SCRIPT              "script"
#define XML_SCRIPT_NAME         "name"
#define XML_SCRIPT_CONTENT      "content"
#define XML_SCRIPT_COMMENT      "comment"

#define XML_SETTINGS			"settings"

#define XML_SETT_SERIAL			"serial"
#define XML_SETT_BOARDNAME		"board_name"
#define XML_SETT_MAC_FILE		"mac_file"
#define XML_SETT_ID_FILE		"id_file"

CXmlFactoryRun::CXmlFactoryRun() : CBaseItem<QXmlStreamReader, QXmlStreamWriter, SFactoryItem>() {
	m_reader=new QXmlStreamReader();
	m_writer=new QXmlStreamWriter();
	m_Config.szSerial="COM3";
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
				if (0 == m_reader->name().toString().compare(XML_FACTORY)) {
					if (!getFactory()) {
						DMSG("get main node: %s failed!", XML_FACTORY);
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

bool CXmlFactoryRun::getFactory() {
	while (!m_reader->atEnd() && ! m_reader->hasError()) {
		QXmlStreamReader::TokenType readToken=m_reader->readNext();

		switch(readToken) {
			default:
				continue;
			case QXmlStreamReader::StartElement:
				if (0 == m_reader->name().toString().compare(XML_FAC_ITEM)) {
					if (!getFactoryItem()) {
						DMSG("get factory item: %d failed!", m_reader->lineNumber());
						return false;
					}
				}
				else if (0 == m_reader->name().toString().compare(XML_SCRIPT)) {
					if (!getScrpit()) {
						DMSG("get script: %d failed!", m_reader->lineNumber());
						return false;
					}
				}
				else if (0 == m_reader->name().toString().compare(XML_SETTINGS)) {
					if (!getSettings()) {
						DMSG("get settings: %d failed!", m_reader->lineNumber());
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

bool CXmlFactoryRun::getFactoryItem() {
	SFactoryItem info;
	info.nDelay=0;
	while (!m_reader->atEnd() && !m_reader->hasError()) {
		QXmlStreamReader::TokenType readToken=m_reader->readNext();

		switch (readToken) {
			default:
				continue;
			case QXmlStreamReader::StartElement:
				XML_GET_VAULE_TEXT((*m_reader), XML_FAC_ITEM_NAME, info.szName);
				XML_GET_VAULE_TEXT((*m_reader), XML_FAC_ITEM_COMMAND, info.szCmd);
				XML_GET_VAULE_TEXT((*m_reader), XML_FAC_ITEM_IMAGE, info.szImage);
				XML_GET_VAULE_TEXT((*m_reader), XML_FAC_ITEM_EXPECT, info.szExpect);
				XML_GET_VAULE_INT((*m_reader), XML_FAC_ITEM_DELAY, info.nDelay);
				XML_GET_VAULE_TEXT((*m_reader), XML_FAC_ITEM_POST_COMMAND, info.szPostCmd);
				XML_GET_VAULE_TEXT((*m_reader), XML_FAC_ITEM_PRE_COMMAND, info.szPreCmd);
				break;
			case QXmlStreamReader::EndElement:
				if (!m_reader->name().toString().compare(XML_FAC_ITEM)) {
					// post processo for the parser datas
					info.szCmd.append('\r');
					if(0 != info.szImage.left(7).compare("file://")) {
						info.szImage="file:///" + QDir::currentPath() + "/" + info.szImage;
					}
					m_items.push_back(info);
					return true;
				}
				break;
		}
	}
	return true;
}

bool CXmlFactoryRun::getScrpit() {
	// removed it function content, because not used
	return true;
}

bool CXmlFactoryRun::getSettings() {
	while (!m_reader->atEnd() && !m_reader->hasError()) {
		QXmlStreamReader::TokenType readToken=m_reader->readNext();

		switch (readToken) {
			default:
				continue;
			case QXmlStreamReader::StartElement:
				XML_GET_VAULE_TEXT((*m_reader), XML_SETT_SERIAL, m_Config.szSerial);
				XML_GET_VAULE_TEXT((*m_reader), XML_SETT_BOARDNAME, m_Config.szBoardName);
				break;
			case QXmlStreamReader::EndElement:
				break;
		}
	}
	return true;
}

bool CXmlFactoryRun::writer() {
	if (!m_destFile.isOpen()) {
		DMSG("destination file is not opened!");
		return false;
	}

	m_writer->setDevice(&m_destFile);
	m_writer->writeStartDocument();
	XML_SET_ATTR_BEGIN((*m_writer), XML_FACTORY);

	if (!writeItems()) {
		DMSG("write xml file failed -- items!");
		return false;
	}
	if (!writeSettings()) {
		DMSG("write xml failed -- settings");
		return false;
	}

	XML_SET_ATTR_END((*m_writer)); // XML_MAIN_NODE
	m_writer->writeEndDocument();
	return true;
}

bool CXmlFactoryRun::writeItems() {
	for (QList<SFactoryItem>::iterator pItem=m_items.begin(); pItem != m_items.end(); pItem++) {
		XML_SET_ATTR_BEGIN((*m_writer), XML_FAC_ITEM);
		XML_SET_VAULE((*m_writer), XML_FAC_ITEM_NAME, pItem->szName);
		XML_SET_VAULE((*m_writer), XML_FAC_ITEM_IMAGE, pItem->szImage);
		XML_SET_VAULE((*m_writer), XML_FAC_ITEM_PRE_COMMAND, pItem->szPreCmd);
		if (0 == pItem->szCmd.right(1).compare("\r")) {
			pItem->szCmd.remove(pItem->szCmd.length()-1);
		}
		XML_SET_VAULE((*m_writer), XML_FAC_ITEM_COMMAND, pItem->szCmd);
		XML_SET_VAULE((*m_writer), XML_FAC_ITEM_POST_COMMAND, pItem->szPostCmd);
		XML_SET_VAULE((*m_writer), XML_FAC_ITEM_DELAY, QString().sprintf("%d", pItem->nDelay));
		XML_SET_VAULE((*m_writer), XML_FAC_ITEM_EXPECT, pItem->szExpect);;
		XML_SET_ATTR_END((*m_writer)); // XML_FAC_ITEM
	}
	return true;
}

bool CXmlFactoryRun::writeSettings() {
	XML_SET_ATTR_BEGIN((*m_writer), XML_SETTINGS);
	XML_SET_VAULE((*m_writer), XML_SETT_SERIAL, m_Config.szSerial);
	XML_SET_VAULE((*m_writer), XML_SETT_BOARDNAME, m_Config.szBoardName);
	XML_SET_ATTR_END((*m_writer)); // XML_FAC_ITEM
	return true;
}
