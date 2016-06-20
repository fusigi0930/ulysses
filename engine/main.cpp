#include <QApplication>
#include <QQmlApplicationEngine>
#include <QResource>

#include "debug.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
	QResource::registerResource("ulysses_common.rcc");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
