import QtQuick 2.3
import QtQuick.Controls 1.2

import "qrc:/ulysses/"

ApplicationWindow {
    visible: true
    width: 640
    height: 480
	title: qsTr("Ulysses - factory")
	visibility: "Maximized"

	toolBar: FactoryToolBar {
		id: factoryToolBar

		onSigClickButton: {
			if (button == "run") {
				factoryToolBar.sigSignal("access")
				baseTabHost.sigAddHost({"mac":"ff:ff:ff:ff:ff:ff","ip":"192.168.100.200","style":"android","itemcolor":"#F0F0F0"})
			}
			else if (button == "stop") {
				factoryToolBar.sigSignal("wait")
				baseTabHost.sigRemoveHost({"mac":"ff:ff:ff:ff:ff:ff","ip":"192.168.100.200","style":"android","itemcolor":"#F0F0F0"})
			}
			else if (button == "reset") {
				factoryToolBar.sigSignal("none")
				baseTabHost.sigAddItem("192.168.100.200", {"name":"= =+","itemcolor":"#F0F0F0"})
			}
		}
	}

	BasedTabView {
		id: baseTabHost

		onSigStartClient: {
			baseTabHost.sigAddTab(ip, "qrc:/ulysses/FactoryGridView.qml")
		}
	}

	//BroadcastGridView {
	//	id: testView
	//}


}
