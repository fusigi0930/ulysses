#ifndef __BASE_INTERPRETER_H__
#define __BASE_INTERPRETER_H__

#include <QString>
#include <QList>
#include <QFile>
#include <QThread>

#include "debug.h"

class CRootInterp {
protected:
	QString m_szFile;
	QFile m_file;
	
    QThread *m_thread;
	
public:
    CRootInterp() { m_thread = NULL; }
	virtual ~CRootInterp() {}
	virtual bool open(QString szFile) {
		m_szFile=szFile;
		return true;
	}
	
	virtual void close() {
		if (m_file.isOpen()) {
			m_file.close();
		}
	}
	
	virtual bool reload() {
		close();
		if (m_szFile.isEmpty())
			return false;
		
        return open(m_szFile);
	}
	
	virtual int create(QString szFile) {
		if (szFile.isEmpty()) {
			return -1;
		}
		
        m_szFile=szFile;

        if (m_thread)
            m_thread->start();
						
		return 0;
	}
	
	virtual int run() {
		return 0;
	}
	
	virtual int stop() {
        if (m_thread->isRunning())
            m_thread->terminate();
		return 0;
	}

    virtual void setThread(QThread *thread) {
        m_thread=thread;
    }

    virtual QThread* getThread() {
        return m_thread;
    }
};



#endif // __BASE_INTERPRETER_H__

