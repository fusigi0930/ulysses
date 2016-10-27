#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QResource>

#include "debug.h"
#include "cinterfaceui.h"
#include "imm.h"

int main(int argc, char *argv[])
{
	ImmDisableIME(-1);
    QApplication app(argc, argv);

	qmlRegisterType<CInterfaceUi>("InterfaceUi", 1, 0, "InterfaceUi");

    QQmlApplicationEngine engine;

	QResource::registerResource("ulysses_common.rcc");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
