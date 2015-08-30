#include <QCoreApplication>
#include <QTest>

bool testCXmlRun();

struct SGroup {
	char *szGroup;
	char *szItem;
	bool (*func) ();
};

SGroup g_testGroup[] = {
	{ "class", "CXmlRun", testCXmlRun },
	{ NULL, NULL, NULL },
};

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	if (3 == argc) {
		for (int i=0; g_testGroup[i].szGroup != NULL; i++) {
			if (0 == strcmp(argv[1], g_testGroup[i].szGroup) &&
				0 == strcmp(argv[2], g_testGroup[i].szItem)) {
				// run test function
				g_testGroup[i].func();
			}
		}
	}
	return a.exec();
}

