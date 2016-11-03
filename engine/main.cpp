#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QResource>

#include "ctreemodel.h"

#include "debug.h"
#include "cinterfaceui.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	qmlRegisterType<CInterfaceUi>("InterfaceUi", 1, 0, "InterfaceUi");

    QQmlApplicationEngine engine;
	QResource::registerResource("ulysses_common.rcc");

	//CTreeModel model;

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	//engine.rootContext()->setContextProperty("treeModel", &model);

    return app.exec();
}
