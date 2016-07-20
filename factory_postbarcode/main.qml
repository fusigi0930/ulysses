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

	id: winBarcodeScan;
	title: qsTr("Ulysses - Fleet") // 艦隊

	BarcodeScan {
		id: scanDialog

		onSigUpdateResult: {
			interfaceUi.slotUpdateResult(mac, serial)
		}

		onSigGetResult: {
			interfaceUi.slotGetResult(mac)
		}
	}

	InterfaceUi {
		id: interfaceUi

		onSigUpdateUi: {
			scanDialog.updateUiResult(szResult)
		}
	}
}
