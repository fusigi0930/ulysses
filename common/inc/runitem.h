#ifndef __RUNITEM_H__
#define __RUNITEM_H__

#include <QString>
#include <QList>
#include <QFile>

#include "debug.h"

template <typename T_T, typename O_O>
class CRunItem {
protected:
	T_T *m_reader;
	O_O *m_writer;


public:
	struct SItem {
		QString szName;
		QString szProc;
		QString szPreProc;
		QString szPostProc;
		QString szDesc;
		QString szExpect;
		unsigned int nCmdType;
		unsigned int nIndex;
		unsigned int nDelay;
	};

protected:
	QList<CRunItem::SItem> m_items;
	QFile m_sourceFile;
	QFile m_destFile;

	virtual bool parser() {
		return true;
	}

	virtual bool writer() {
		return true;
	}

public:
	CRunItem() {

	}

	virtual ~CRunItem() {
		m_items.clear();

		if (m_sourceFile.isOpen())
			m_sourceFile.close();

		if (m_destFile.isOpen())
			m_destFile.close();
	}

	virtual bool open(QString szFile) {
		if (m_sourceFile.isOpen()) {
			DMSG("file is already opened");
			m_sourceFile.close();
		}

		m_sourceFile.setFileName(szFile);
		if (!m_sourceFile.open(QFile::ReadOnly)) {
			DMSG("open file failed");
			return false;
		}

		return parser();
	}

	virtual bool save(QString szFile) {
		if (m_destFile.isOpen()) {
			DMSG("file is already opened");
			m_destFile.close();
		}

		m_destFile.setFileName(szFile);
		if (!m_destFile.open(QFile::ReadWrite)) {
			DMSG("open file failed");
			return false;
		}

		return writer();
	}

	virtual void close() {
		if (m_sourceFile.isOpen()) {
			m_sourceFile.close();
		}
		if (m_destFile.isOpen()) {
			m_destFile.close();
		}

		m_items.clear();
	}

	virtual bool reload() {
		this->close();
		return this->open(m_sourceFile.fileName());
	}


};

#endif // __RUNITEM_H__

