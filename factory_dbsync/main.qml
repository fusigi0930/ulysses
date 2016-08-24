import QtQuick 2.1
import QtQuick.Controls 1.2
import "qrc:/ulysses/"
import InterfaceUi 1.0

ApplicationWindow {
	visible: true
	width: 360
	height: 360

	property var isMac;
	property var isSerial;

	id: winDBSync;
	title: qsTr("Ulysses - Corridor") // 艦隊

	DBSync {
		id: syncBlock

	}

	InterfaceUi {
		id: interfaceUi

	}
}
