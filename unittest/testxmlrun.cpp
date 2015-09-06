#include <QTest>
#include "xmlrun.h"
#include "runitem.h"
#include <QList>
#include "debug.h"
#include "testgroup.h"

void CTestGroup::testCXmlRun_saveFile() {
	CXmlRun xmlrun;
	SItem item1;
	item1.szName="item1";
	item1.szDesc="test item 1";
	item1.szPreProc="preProc";
	item1.szProc="Proc";
	item1.szPostProc="postProc";
	item1.szExpect="test expect";
	item1.nDelay=100;
	item1.nCmdType=0;

	xmlrun.getItems().push_back(item1);
	if (!xmlrun.save("test.xml")) {
		QFAIL("save file failed!");
	}

	QFile file;
	file.setFileName("test.xml");
	if (!file.open(QFile::ReadOnly)) {
		QFAIL("open file failed!");
	}
	QString szContent=file.readAll().toStdString().c_str();
	file.close();
	QCOMPARE(szContent, QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?><testcase><case><item>item1</item><comment>test item 1</comment><precmd>preProc</precmd><command>Proc</command><postcmd>postProc</postcmd><delay>100</delay><expect>test expect</expect><type>auto</type></case></testcase>\n"));
}

void CTestGroup::testCXmlRun_openFile() {
	CXmlRun xmlrun;
	if (!xmlrun.open("test.xml")) {
		QFAIL("open test file failed!");
	}

	QCOMPARE(xmlrun.getItemCount(), 1);
	xmlrun.setCurrentIndex(0);
	SItem item=xmlrun.getCurrentItem();

	QCOMPARE(item.szName, QString("item1"));
	QCOMPARE(item.szDesc, QString("test item 1"));
	QCOMPARE(item.szPreProc, QString("preProc"));
	QCOMPARE(item.szProc, QString("Proc"));
	QCOMPARE(item.szPostProc, QString("postProc"));
	QCOMPARE(item.szExpect, QString("test expect"));
	QCOMPARE(static_cast<int>(item.nDelay), 100);
	QCOMPARE(static_cast<int>(item.nCmdType), 0);
	return;
}

void CTestGroup::testCXmlRun_replaceItem() {
	CXmlRun xmlrun;
	if (!xmlrun.open("test.xml")) {
		QFAIL("open test file failed!");
	}

	SItem &item=xmlrun.getItem(0);
	item.szName="replace1";

	if (!xmlrun.save("replace.xml")) {
		QFAIL("save file failed!");
	}
	CXmlRun xmlrun1;
	if (!xmlrun1.open("replace.xml")) {
		QFAIL("open test file failed!");
	}
	SItem item1=xmlrun1.getCurrentItem();
	QCOMPARE(item1.szName, QString("replace1"));
	QFile::remove("test.xml");
	QFile::remove("replace.xml");
}

void CTestGroup::testCXmlRun_escapeCharSave() {
	CXmlRun xmlrun;
	SItem item1;
	item1.szName="escape";
	item1.szDesc="&><\"'";
	item1.szPreProc="";
	item1.szProc="Proc";
	item1.szPostProc="postProc";
	item1.szExpect="test expect";
	item1.nDelay=100;
	item1.nCmdType=0;

	xmlrun.getItems().push_back(item1);
	if (!xmlrun.save("escape.xml")) {
		QFAIL("save file failed!");
	}

	CXmlRun xmlrun1;
	if (!xmlrun1.open("escape.xml")) {
		QFAIL("open test file failed!");
	}

	QCOMPARE(xmlrun1.getCurrentItem().szDesc, QString("&><\"'"));
	QFile::remove("escape.xml");
}

void CTestGroup::testCXmlRun_Utf8CharSave() {
	CXmlRun xmlrun;
	SItem item1;
	item1.szName="utf8";
	item1.szDesc="中文UTF8測試";
	item1.szPreProc="";
	item1.szProc="Proc";
	item1.szPostProc="postProc";
	item1.szExpect="test expect";
	item1.nDelay=100;
	item1.nCmdType=0;

	xmlrun.getItems().push_back(item1);
	if (!xmlrun.save("utf8.xml")) {
		QFAIL("save file failed!");
	}
}

void CTestGroup::testCXmlRun_Utf8CharRead() {
	CXmlRun xmlrun;
	if (!xmlrun.open("utf8.xml")) {
		QFAIL("open test file failed!");
	}

	QCOMPARE(xmlrun.getCurrentItem().szDesc, QString("中文UTF8測試"));
	QFile::remove("utf8.xml");
}

void CTestGroup::testCXmlRun_multipleItem() {
	CXmlRun xmlrun;
	for (int i=0; i<100; i++) {
		SItem item;
		item.szName.sprintf("item%03d", i);
		item.szDesc.sprintf("desc%03d", i);
		item.szPreProc.sprintf("precmd%03d", i);
		item.szProc.sprintf("cmd%03d", i);
		item.szPostProc.sprintf("postcmd%03d", i);
		item.szExpect.sprintf("expect%03d", i);
		item.nDelay=i;
		item.nCmdType=i%3;
		xmlrun.getItems().push_back(item);
	}
	if (!xmlrun.save("multiple.xml")) {
		QFAIL("save test file failed!");
	}
	CXmlRun xmlrun1;
	if (!xmlrun1.open("multiple.xml")) {
		QFAIL("open file failed");
	}

	QCOMPARE(xmlrun1.getItemCount(), xmlrun.getItemCount());
	for (int i=0; i<xmlrun.getItemCount(); i++) {
		QCOMPARE(xmlrun1.getItem(i).szName, QString().sprintf("item%03d", i));
		QCOMPARE(xmlrun1.getItem(i).szDesc, QString().sprintf("desc%03d", i));
		QCOMPARE(xmlrun1.getItem(i).szPreProc, QString().sprintf("precmd%03d", i));
		QCOMPARE(xmlrun1.getItem(i).szProc, QString().sprintf("cmd%03d", i));
		QCOMPARE(xmlrun1.getItem(i).szPostProc, QString().sprintf("postcmd%03d", i));
		QCOMPARE(xmlrun1.getItem(i).szExpect, QString().sprintf("expect%03d", i));
		QCOMPARE(xmlrun1.getItem(i).nDelay == i, true);
		QCOMPARE(xmlrun1.getItem(i).nCmdType == i%3, true);
	}

	QFile::remove("multiple.xml");
}

void CTestGroup::testCXmlFactoryRun_saveFile() {
	CXmlFactoryRun run;
	SFactoryItem item;
	item.szName="item1";
	item.nDelay=100;
	item.szCmd="cmd";
	item.szExpect="expect";
	item.szImage="image";
	item.szPostCmd="post cmd";
	item.szPreCmd="pre cmd";

	run.getItems().push_back(item);
	if (!run.save("factorytest.xml")) {
		QFAIL("save xml file failed");

	}

	QFile file;
	file.setFileName("factorytest.xml");
	if (!file.open(QFile::ReadOnly)) {
		QFAIL("open file failed!");
	}
	QByteArray buf=file.readAll();
	file.close();

	QCOMPARE(-1 != buf.indexOf("<factory>"), true);
	QCOMPARE(-1 != buf.indexOf("</factory>"), true);
	QCOMPARE(-1 != buf.indexOf("<settings>"), true);
	QCOMPARE(-1 != buf.indexOf("</settings>"), true);
	QCOMPARE(-1 != buf.indexOf("<serial>COM3</serial>"), true);
	QCOMPARE(-1 != buf.indexOf("<item>"), true);
	QCOMPARE(-1 != buf.indexOf("</item>"), true);
	QCOMPARE(-1 != buf.indexOf("<name>item1</name>"), true);
	QCOMPARE(-1 != buf.indexOf("<delay>100</delay>"), true);
	QCOMPARE(-1 != buf.indexOf("<command>cmd</command>"), true);
	QCOMPARE(-1 != buf.indexOf("<precmd>pre cmd</precmd>"), true);
	QCOMPARE(-1 != buf.indexOf("<postcmd>post cmd</postcmd>"), true);
	QCOMPARE(-1 != buf.indexOf("<expect>expect</expect>"), true);
	QCOMPARE(-1 != buf.indexOf("<image>image</image>"), true);
}

void CTestGroup::testCXmlFactoryRun_openFile() {
	CXmlFactoryRun run;
	if (!run.open("factorytest.xml")) {
		QFAIL("cannot open file");
	}

	SFactoryItem item=run.getCurrentItem();
	QCOMPARE(0==item.szName.compare("item1"), true);
	QCOMPARE(item.nDelay, 100);
	QCOMPARE(0 == item.szCmd.compare("cmd\r"), true);
	QCOMPARE(0 == item.szExpect.compare("expect"), true);
	QString szImage;
	szImage.sprintf("file:///%s/image", QSZ(QDir::currentPath()));
	QCOMPARE(0 == item.szImage.compare(szImage), true);
	QCOMPARE(0 == item.szPostCmd.compare("post cmd"), true);
	QCOMPARE(0 == item.szPreCmd.compare("pre cmd"), true);
	QCOMPARE(0 == run.m_Config.szSerial.compare("COM3"), true);
}

void CTestGroup::testCXmlFactoryRun_replaceItem() {
	CXmlFactoryRun run;
	if (!run.open("factorytest.xml")) {
		QFAIL("cannot open file");
	}

	SFactoryItem &item=run.getItem(0);
	item.szName="replace1";

	if (!run.save("replace.xml")) {
		QFAIL("save file failed!");
	}
	CXmlFactoryRun xmlrun1;
	if (!xmlrun1.open("replace.xml")) {
		QFAIL("open test file failed!");
	}
	SFactoryItem item1=xmlrun1.getCurrentItem();
	QCOMPARE(item1.szName, QString("replace1"));
	QFile::remove("testfactorytestxml");
	QFile::remove("replace.xml");
}

void CTestGroup::testCXmlFactoryRun_escapeCharSave() {
	CXmlFactoryRun run;
	SFactoryItem item;
	item.szName="&<>\"";
	item.nDelay=100;
	item.szCmd="cmd";
	item.szExpect="expect";
	item.szImage="image";
	item.szPostCmd="post cmd";
	item.szPreCmd="pre cmd";

	run.getItems().push_back(item);
	if (!run.save("factoryescape.xml")) {
		QFAIL("save xml file failed");

	}

	CXmlFactoryRun xmlrun1;
	if (!xmlrun1.open("factoryescape.xml")) {
		QFAIL("open xml file failed");
	}
	QCOMPARE(0 == xmlrun1.getCurrentItem().szName.compare("&<>\""), true);
}

void CTestGroup::testCXmlFactoryRun_Utf8CharSave() {
	CXmlFactoryRun run;
	SFactoryItem item;
	item.szName="&<>\"";
	item.nDelay=100;
	item.szCmd="cmd";
	item.szExpect="expect";
	item.szImage="image";
	item.szPostCmd="post cmd";
	item.szPreCmd="pre cmd";

	run.getItems().push_back(item);
	if (!run.save("factoryescape.xml")) {
		QFAIL("save xml file failed");

	}

	CXmlFactoryRun xmlrun1;
	if (!xmlrun1.open("factoryescape.xml")) {
		QFAIL("open xml file failed");
	}
	QCOMPARE(0 == xmlrun1.getCurrentItem().szName.compare("&<>\""), true);
	QFile::remove("factoryescape.xml");
}

void CTestGroup::testCXmlFactoryRun_Utf8CharRead() {
	CXmlFactoryRun run;
	SFactoryItem item;
	item.szName="這是中文UTF8";
	item.nDelay=100;
	item.szCmd="cmd";
	item.szExpect="expect";
	item.szImage="image";
	item.szPostCmd="post cmd";
	item.szPreCmd="pre cmd";

	run.getItems().push_back(item);
	if (!run.save("utf8factory.xml")) {
		QFAIL("save xml file failed");

	}

	CXmlFactoryRun xmlrun1;
	if (!xmlrun1.open("utf8factory.xml")) {
		QFAIL("open xml file failed");
	}
	QCOMPARE(0 == xmlrun1.getCurrentItem().szName.compare("這是中文UTF8"), true);
	QFile::remove("utf8factory.xml");
}

void CTestGroup::testCXmlFactoryRun_multipleItem() {
	CXmlFactoryRun xmlrun;
	for (int i=0; i<100; i++) {
		SFactoryItem item;
		item.szName.sprintf("item%03d", i);
		item.szCmd.sprintf("cmd%03d", i);
		item.szPreCmd.sprintf("precmd%03d", i);
		item.szPostCmd.sprintf("postcmd%03d", i);
		item.szImage.sprintf("image%03d", i);
		item.szExpect.sprintf("expect%03d", i);
		item.nDelay=i;;
		xmlrun.getItems().push_back(item);
	}
	if (!xmlrun.save("factorymultiple.xml")) {
		QFAIL("save test file failed!");
	}
	CXmlFactoryRun xmlrun1;
	if (!xmlrun1.open("factorymultiple.xml")) {
		QFAIL("open file failed");
	}

	QCOMPARE(xmlrun1.getItemCount(), xmlrun.getItemCount());
	for (int i=0; i<xmlrun.getItemCount(); i++) {
		QCOMPARE(xmlrun1.getItem(i).szName, QString().sprintf("item%03d", i));
		QCOMPARE(xmlrun1.getItem(i).szCmd, QString().sprintf("cmd%03d\r", i));
		QCOMPARE(xmlrun1.getItem(i).szPreCmd, QString().sprintf("precmd%03d", i));
		QCOMPARE(xmlrun1.getItem(i).szPostCmd, QString().sprintf("postcmd%03d", i));
		QCOMPARE(xmlrun1.getItem(i).szImage, QString().sprintf("file:///%s/image%03d", QSZ(QDir::currentPath()), i));
		QCOMPARE(xmlrun1.getItem(i).szExpect, QString().sprintf("expect%03d", i));
		QCOMPARE(xmlrun1.getItem(i).nDelay == i, true);
	}

	QFile::remove("multiple.xml");
}
