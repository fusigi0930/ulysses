import QtQuick 2.3
import QtQuick.Controls 1.2

import "qrc:/ulysses/"

import FactoryAction 1.0

ApplicationWindow {
    visible: true
	title: qsTr("Ulysses - factory")
	visibility: "Maximized"

	onClosing: {
		actionFactory.slotClose()
	}

	toolBar: FactoryToolBar {
		id: factoryToolBar

		onSigClickButton: {
			if (button == "run") {
				factoryToolBar.sigSignal("access")
				actionFactory.sigTest(152)
			}
			else if (button == "stop") {
				factoryToolBar.sigSignal("wait")
				actionFactory.sigTestKernel("factory:192.168.0.152")
			}
			else if (button == "reset") {
				factoryToolBar.sigSignal("none")

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
			baseTabHost.sigAddHost({"mac":szMac,"ip":szIp,"style":"boot","itemcolor":"#C0C0E0"})
			baseTabHost.sigStartClient(szIp)
		}

		onSigUpdateHost: {
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
