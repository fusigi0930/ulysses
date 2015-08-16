#include "xmlrun.h"
#include "xml_common.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#define XML_MAIN_NODE "testcase"

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

bool CXmlRun::getMainNode() {
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

	return CRunItem::parser();
}

bool CXmlRun::writer() {
	if (!m_destFile.isOpen()) {
		DMSG("destination file is not opened!");
		return false;
	}
	return CRunItem::parser();
}
