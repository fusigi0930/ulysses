#ifndef __BASE_INTERPRETER_H__
#define __BASE_INTERPRETER_H__

#include <QString>
#include <QList>
#include <QFile>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

#include "debug.h"

class CRootInterp {
protected:
	QString m_szFile;
	QFile m_file;
	
    QFuture<int> m_thread;
    QFutureWatcher<int> m_threadWatcher;
	
public:
	CRootInterp() {}
	virtual ~CRootInterp() {}
	virtual bool open(QString szFile) {
		m_szFile=szFile;
		return false;
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
	
	static int runThread(CRootInterp *interp, QString szFile) {
		if (NULL == interp || szFile.isEmpty()) {
			return -1;
		}
		
		if (!interp->open(szFile)) {
			return -2;
		}
		
		return interp->run();
		
	}
	
	virtual int create(QString szFile) {
		if (szFile.isEmpty()) {
			return -1;
		}
		
		m_thread=QtConcurrent::run (
						CRootInterp::runThread, this, szFile);
						
		m_threadWatcher.setFuture(m_thread);
						
		return 0;
	}
	
	virtual int run() {
		return 0;
	}
	
	virtual int stop() {
		if (m_thread.isRunning())
            m_thread.cancel();
		return 0;
	}
	virtual int suspend() {
		if (m_thread.isRunning())
			m_thread.pause();
		return 0;
	}
	virtual int resume() {
		if (m_thread.isPaused())
			m_thread.resume();
		return 0;
	}
};



#endif // __BASE_INTERPRETER_H__

