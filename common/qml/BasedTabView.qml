import QtQuick 2.2
import QtQuick.Controls 1.2
import "qrc:/ulysses"


TabView {
	id: tabView
	anchors.fill: parent
	signal sigAddHost(var item)
	signal sigRemoveHost(var item)
	signal sigFinalResult(var nameIp, var item)
	
	signal sigAddItem(var tabName, var item)
	signal sigRunItem(var tabName, var item)
	signal sigUpdateItemResult(var tabName, var item)
	
	onSigAddHost: {
		for (var i=0; i<tabView.count; i++) {
			if (tabView.getTab(i).title === "UlyssesMonitor") {
				tabView.getTab(i).item.sigAddItem(item)
				break;
			}
		}		
	}
	
	onSigRemoveHost: {
		for (var i=0; i<tabView.count; i++) {
			if (tabView.getTab(i).title === "UlyssesMonitor") {
				tabView.getTab(i).item.sigRemoveItem(item)
				break;
			}
		}	
	}

	onSigAddItem: {
		for (var i=0; i<tabView.count; i++) {
			if (tabView.getTab(i).title === tabName) {
				tabView.getTab(i).item.sigAddItem(item)
				break;
			}
		}
	}
	
	onSigRunItem: {
		for (var i=0; i<tabView.count; i++) {
			if (tabView.getTab(i).title === tabName) {
				tabView.getTab(i).item.sigRunItem(item)
				break;
			}
		}
	}	
	
	onSigUpdateItemResult: {
		for (var i=0; i<tabView.count; i++) {
			if (tabView.getTab(i).title === tabName) {
				tabView.getTab(i).item.sigFinalResult(nameIp, item)
				break;
			}
		}	
	}
	
	onSigFinalResult: {
		for (var i=0; i<tabView.count; i++) {
			if (tabView.getTab(i).title === "UlyssesMonitor") {
				tabView.getTab(i).item.sigUpdateItemResult(item)
				break;
			}
		}			
	}

	Tab {
		title: "UlyssesMonitor"

		BroadcastGridView {
		}
	}
}