#ifndef CINTERFACEUI_H
#define CINTERFACEUI_H

#include <QObject>
#include <netio.h>
#include <map>
#include <QVariant>

class CInterfaceUi : public QObject
{
	Q_OBJECT
public:
	CInterfaceUi(QObject *parent = 0);
	~CInterfaceUi();

	int m_nCount;

	typedef std::map<int, int> UiMap;

	UiMap m_map;
	CNotifyRecv m_recv;
signals:
	void sigUpdateTotalCount(int nCount);
	void sigUpdateIp(QString ip);
	void sigInsertItem(QVariant item);
	void sigUpdateItem(QVariant item);

public slots:
	void slotAddNewDevice(int nPort);
};

#endif // CINTERFACEUI_H
