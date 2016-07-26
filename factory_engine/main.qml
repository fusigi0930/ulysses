import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2

import "qrc:/ulysses/"

import FactoryAction 1.0

ApplicationWindow {
	id: mainWnd
    visible: true
	title: qsTr("Ulysses - Fortress") // 要塞
	visibility: "Maximized"

	onClosing: {
		actionFactory.slotClose()
	}

	toolBar: FactoryToolBar {
		id: factoryToolBar

		onSigClickButton: {
			if (button == "clean_pass") {
				actionFactory.slotRemovePassedHosts()
			}
			else if (button == "clean_fail") {
				actionFactory.slotRemoveFailedHosts()
			}
			else if (button == "open-xml") {
				fileDialog.open()
			}
		}
	}

	FileDialog {
		id: fileDialog
		modality: Qt.WindowModal
		selectMultiple: false
		selectFolder: false
		selectExisting: true
		title: qsTr("Select a factory test xml file...")

		nameFilters: [ "XML files (*.xml)", "All files (*.*)" ]

		onAccepted: {
			var file=fileDialog.fileUrl.toString()
			var n=file.lastIndexOf("/")
			file=file.substring(n+1)
			console.log("filename: " + file)
			actionFactory.xmlFile=file
		}
	}

	BasedTabView {
		id: baseTabHost

		onSigStartClient: {
			baseTabHost.sigAddTab(ip, "qrc:/ulysses/FactoryGridView.qml")
			actionFactory.slotShowItem(ip)
		}

	}

	FactoryAction {
		id: actionFactory
		xmlFile: "items.xml"

		Component.onCompleted: {
			if (Qt.application.arguments[1].length !== 0)
				xmlFile=Qt.application.arguments[1]
			else
				xmlFile="items.xml"
		}

		onSigStartNewBootDev: {
			console.log("start new boot dev: " + szIp)
			baseTabHost.sigAddHost({"mac":szMac,"ip":szIp,"style":"boot","itemcolor":"#C0C0E0"})
			baseTabHost.sigStartClient(szIp)
		}

		onSigUpdateHost: {
			console.log("main update host!")
			baseTabHost.sigUpdateHost(item)
		}

		onSigRemoveHost: {
			baseTabHost.sigRemoveHost(item)
		}

		onSigAddShowItem: {
			baseTabHost.sigAddItem(item.ip, item)
		}

		onSigUpdateShowItem: {
			console.log("update show item: " + item.ip)
			baseTabHost.sigUpdateItemResult(item.ip, item)
		}
	}
}
