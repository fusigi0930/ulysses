#include "xmlcfg.h"
#include "debug.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

CXmlConfig::CXmlConfig() : CBaseItem<QXmlStreamReader, QXmlStreamWriter, SConfig>()
{
	m_reader=new QXmlStreamReader();
	m_writer=new QXmlStreamWriter();
}

CXmlConfig::~CXmlConfig() {
	if (m_reader) {
		delete m_reader;
		m_reader=NULL;
	}
	if (m_writer) {
		delete m_writer;
		m_writer=NULL;
	}
}

bool CXmlConfig::parser() {
	return true;
}

bool CXmlConfig::writer() {
	return true;
}
