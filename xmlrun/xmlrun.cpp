#include "xmlrun.h"

CXmlRun::CXmlRun() {
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

bool CXmlRun::parser() {
	return CRunItem::parser();
}

bool CXmlRun::writer() {
	return CRunItem::parser();
}
