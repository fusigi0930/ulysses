import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4
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
	
	signal sigAddTab(var ip, var comp)
	signal sigRemoveTab(var ip)
	
	signal sigStartClient(var ip)
	signal sigEndClient(var ip)
	
	property var defaultMonitorTabName: "UlyssesMonitor"
	
	onSigAddHost: {
		for (var i=0; i<tabView.count; i++) {
			if (tabView.getTab(i).title === defaultMonitorTabName) {
				tabView.getTab(i).item.sigAddItem(item)
				break;
			}
		}		
	}
	
	onSigRemoveHost: {
		for (var i=0; i<tabView.count; i++) {
			if (tabView.getTab(i).title === defaultMonitorTabName) {
				tabView.sigRemoveTab(item.ip)
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
			if (tabView.getTab(i).title === defaultMonitorTabName) {
				tabView.getTab(i).item.sigUpdateItemResult(item)
				break;
			}
		}			
	}
	
	onSigAddTab: {
		tabView.addTab(ip, Qt.createComponent(comp))
		tabView.getTab(tabView.count - 1).active = true	
	}
	
	onSigRemoveTab: {
		if (ip === "UlyssesMonitor")
			return
		for (var i=0; i<tabView.count; i++) {
			if (tabView.getTab(i).title === ip) {
				console.log("remove ip: " + ip);
				tabView.removeTab(i)
				break;
			}
		}	
	}
	
	style: TabViewStyle {
		frameOverlap: 1
		tab: Rectangle {
			color: styleData.selected ? "steelblue" :"lightsteelblue"
			border.color:  "steelblue"
			implicitWidth: Math.max(text.width + 42, 120)
			implicitHeight: 30
			radius: 5
			
			Text {
				id: text
				anchors.centerIn: parent
				text: styleData.title
				color: styleData.selected ? "white" : "#404040"
				font.pixelSize: 16
				font.family: "Tohama"		
			}
			
			Rectangle {
				anchors.right: parent.right
				anchors.rightMargin: 2
				anchors.verticalCenter: parent.verticalCenter
				
				implicitWidth: 12
				implicitHeight: 12
				radius: width/2
				
				border.color: "gray"
				color: "#e0e0e0"
				visible: text.text === defaultMonitorTabName ? false : true
				
				MouseArea {
					anchors.fill: parent
					
					onClicked: {
						tabView.removeTab(styleData.index)
					}
					
					onPressed: color="#c0c0c0"
				}
			}
		}
		frame: Rectangle { color: "#F0F0C0" }	
	}

	Tab {
		title: defaultMonitorTabName

		BroadcastGridView {
			onSigStartClient: {
				tabView.sigStartClient(ip)
			}
			
			onSigEndClient: {
				tabView.sigEndClient(ip)
				tabView.sigRemoveTab(ip)
			}
		}
	}
}