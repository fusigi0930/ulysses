import QtQuick 2.3
import QtQuick.Controls 1.2

import "qrc:/ulysses/"

import FactoryAction 1.0

ApplicationWindow {
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
			baseTabHost.sigUpdateItemResult(item.ip, item)
		}
	}
}
