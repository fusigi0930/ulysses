#ifndef CTESTLINKITEM_H
#define CTESTLINKITEM_H

#include "runitem.h"
#include "csqlitestore.h"
#include <list>

/*
------------------------------------------------------------
get test case step
------------------------------------------------------------
1. select * from `nodes_hierarchy` T1 where node_type_id=1 and name like 'android%'
// node_type_id: 1 testprojects
// get id --> 6

2. select * from `nodes_hierarchy` T1 where node_type_id=5 and parent_id=6
// node_type_id: 5 testplan
// parent_id: 6, id from last command
// get id --> 18

3. select * from `testplan_tcversions` where testplan_id=18
// testplan_id: 18, id from last command
// get tcversion_id --> 10

4. select * from `nodes_hierarchy` T1 where node_type_id=4 and id=10
// node_type_id: 4, tcversion
// id: 10, tcversion_id from last command
// get parent_id --> 9

5.
SELECT TS.id,TV.summary, TV.preconditions, TS.actions, TS.expected_results, TS.execution_type
FROM  `tcversions` TV,  `tcsteps` TS
WHERE TV.id
IN (
	SELECT id
	FROM  `nodes_hierarchy` NH
	WHERE NH.parent_id =9
)
AND TS.id
IN (
	SELECT id
	FROM  `nodes_hierarchy` NH
	WHERE NH.parent_id = TV.id
)
// NH.parent_id: 9, parent from last command
// get all tset step and id, summary, preconditions, actions, expected_results, execution_type
// execution:
// 		1: manual
//		2: auto
-------------------------------------------------------------------
get build and platform id step
-------------------------------------------------------------------
1. select * from `builds` where name like 'android%'
// get id --> 3 (get build_id)

2. select * from `platforms` where name='icm-3011' and testproject_id=6
// testproject_id: get from test case step 1
// get id --> 1 (get platform_id)

------------------------------------------------------------------
executions need info
------------------------------------------------------------------
1, build_id, platform_id, tcversion_id, testplan_id, tester_id --> get id
2. execution_tcsteps id, tcstep_id

*/
enum ETestLinkStatus {
	_TL_STATUS_NORMAL_RUN	=	0,
	_TL_STATUS_MANUAL_RUN,
	_TL_STATUS_IGNORE,
};

enum ETestLinkNodeType {
	_TL_NTYPE_TESTPROJ		=	1,
	_TL_NTYPE_TESTSUITE		=	2,
	_TL_NTYPE_TESTCASE		=	3,
	_TL_NTYPE_TC_VERSION	=	4,
	_TL_NTYPE_TESTPLAN		=	5,
	_TL_NTYPE_REQ_SPEC		=	6,
	_TL_NTYPE_REQ			=	7,
	_TL_NTYPE_REQ_VERSION	=	8,
	_TL_NTYPE_TC_STEP		=	9,
	_TL_NTYPE_REQ_REVISION	=	10,
	_TL_NTYPE_REQ_SPEC_REVISION	=	11,
	_TL_NTYPE_BUILD			=	12,
	_TL_NTYPE_PLATFORM		=	13,
	_TL_NTYPE_USER			=	14,
};

class CTestLinkRoot {
protected:
	CTestLinkRoot *m_parent;
public:
	unsigned long long m_nProjectId;

	CTestLinkRoot() {}
	virtual ~CTestLinkRoot() {

	}

	virtual CTestLinkRoot* getParent() { return NULL; }
	virtual CTestLinkRoot* getRoot() {
		if (NULL == getParent()) return this;

		return getParent()->getRoot();
	}

	virtual void duplicateInfo(CTestLinkRoot* item) {
		this->m_nProjectId=item->m_nProjectId;
		m_parent=item;
	}

	virtual bool isOpened() { return false; }
	virtual QString getDevName() { return ""; }
};

class CTestLinkItem;
class CTestLinkPlan;

class CTestLinkReader : public CTestLinkRoot {
private:
	CUlyStore m_db;
	QString m_szName;
	QString m_szDev;
	QString m_szIp;
	bool m_bIsOpened;

public:
	std::list<CTestLinkPlan*> m_listPlans;

	CTestLinkReader();
	virtual ~CTestLinkReader();

	virtual bool open(QString szFile);
	virtual void close();

	void setDevName(QString szName);
	void getPlans();

	virtual bool isOpened() { return m_bIsOpened; }
	virtual QString getDevName() { return m_szName; }

	QVariant fetchTCInfo(QVariant item);

};

class CTestLinkPlan : public CTestLinkRoot {
private:
	CUlyStore *m_db;
	QString m_szName;

public:
	unsigned long long m_nPlanId;
	std::list<QVariant> m_tcs;


	CTestLinkPlan();
	virtual ~CTestLinkPlan();

	virtual bool open(QString szFile);
	virtual void close();

	void setDB(CUlyStore *db);
	virtual CTestLinkRoot* getParent();
	QString getName();

	virtual void duplicateInfo(CTestLinkRoot *item);
	virtual bool isOpened() { getRoot()->isOpened(); }

	void getTCs();
};

class CTestLinkItem : public CRootItem, public CTestLinkRoot
{
private:
	CTestLinkPlan *m_parent;
	SItem m_currentItem;
	CUlyStore *m_db;
	QString m_szName;
	QString m_szType;
	QString m_szIp;

public:
	CTestLinkItem();
	virtual ~CTestLinkItem();

	virtual bool open(QString szFile);
	virtual bool save(QString szFile);
	virtual void close();
	virtual bool reload();
	virtual int getItemCount();
	virtual int getCurrentIndex();
	virtual void setCurrentIndex(int nIndex);
	virtual bool nextItem();
	virtual bool removeItem(int nIndex);

	SItem* getItem();
	void setDB(CUlyStore *db);
	virtual CTestLinkRoot* getParent();
	virtual void duplicateInfo(CTestLinkRoot *item);
	virtual bool isOpened() { getRoot()->isOpened(); }
};

#endif // CTESTLINKITEM_H
