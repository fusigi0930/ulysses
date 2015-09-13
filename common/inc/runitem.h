#ifndef __RUNITEM_H__
#define __RUNITEM_H__

#include <QString>
#include <QList>
#include <QFile>

#include "debug.h"

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

class CRootItem {
public:
	CRootItem() {}
	virtual ~CRootItem() {}
	virtual bool open(QString szFile)=0;
	virtual bool save(QString szFile)=0;
	virtual void close()=0;
	virtual bool reload()=0;
	virtual int getItemCount()=0;
	virtual int getCurrentIndex()=0;
	virtual void setCurrentIndex(int nIndex)=0;
	virtual bool removeItem(int nIndex)=0;
};

template <typename T_T, typename O_O, typename OQO>
class CBaseItem : public CRootItem {
protected:
	T_T *m_reader;
	O_O *m_writer;

public:

protected:
	int m_nIndex;
	QList<OQO> m_items;
	QFile m_sourceFile;
	QFile m_destFile;

	virtual bool parser() {
		return true;
	}

	virtual bool writer() {
		return true;
	}

public:
	CBaseItem() : CRootItem() {
		m_nIndex=0;
	}

	virtual ~CBaseItem() {
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

		bool bRet=parser();
		m_sourceFile.close();
		return bRet;
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

		bool bRet=writer();
		m_destFile.close();
		return bRet;
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

	virtual int getItemCount() {
		return m_items.length();
	}

	virtual QList<OQO> &getItems() {
		return m_items;
	}

	virtual int getCurrentIndex() {
		return m_nIndex;
	}

	virtual OQO getCurrentItem() {
		return m_items.at(m_nIndex);
	}
	
	virtual OQO& getItem(int i) {
		return m_items[i];
	}
	
	virtual OQO& replaceCurrentItem() {
		return m_items[m_nIndex];
	}

	virtual void setCurrentIndex(int nIndex) {
		m_nIndex=nIndex;
	}

	virtual int addItem(OQO item) {
		m_items.append(item);
		return m_items.length();
	}

	virtual bool removeItem(int nIndex) {
		if (0 > nIndex || m_items.length() < nIndex)
			return false;
		m_items.removeAt(nIndex);
		return true;
	}
};

template <typename T_T, typename O_O>
class CRunItem : public CBaseItem<T_T, O_O, SItem> {
public:
	CRunItem() : CBaseItem<T_T, O_O, SItem>() {

	}

	virtual ~CRunItem() {

	}
};

#endif // __RUNITEM_H__

