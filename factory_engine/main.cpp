#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QResource>

#include "debug.h"
#include "cfactoryaction.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

	qmlRegisterType<CFactoryAction>("FactoryAction", 1, 0, "FactoryAction");

    QQmlApplicationEngine engine;

	QResource::registerResource("ulysses_common.rcc");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	int nRet=app.exec();

	return nRet;
}
