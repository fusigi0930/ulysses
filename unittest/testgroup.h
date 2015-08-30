#ifndef __TESTGROUP_H__
#define __TESTGROUP_H__

#include <QObject>

class CTestGroup : public QObject {
	Q_OBJECT

public:
	CTestGroup(QObject *parent=NULL) : QObject(parent) {

	}

	virtual ~CTestGroup() {

	}

private slots:
	void slotCXmlRun_saveFile();
	void slotCXmlRun_openFile();
};

#endif // __TESTGROUP_H__

