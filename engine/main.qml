import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "qrc:/ulysses/"
import InterfaceUi 1.0

ApplicationWindow {
    visible: true
	title: qsTr("Thor Hammer")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
		Menu {
			title: "test"
			MenuItem {
				text: qsTr("add pannel")
				onTriggered: {
					var newTreeModel=interfaceUi.newTreeModel();
					pannelBottom.addInfoPanel(newTreeModel.getQMLName(), newTreeModel);
				}
			}
			MenuItem {
				text: qsTr("list pannel in console")
				onTriggered: {
					pannelBottom.listInfoIname();
				}
			}
			MenuItem {
				text: qsTr("active pannel")
				onTriggered: {
					var i=pannelBottom.findInfoPannel("treeModel-00000");
					pannelBottom.activateInfoPannel(i);
				}
			}
			MenuItem {
				text: qsTr("deactive all pannel")
				onTriggered: {
					pannelBottom.deactivateInfoPannels();
				}
			}
		}
    }

	InterfaceUi {
		id: interfaceUi
	}

	Component.onCompleted: {
		showMaximized();
	}

	SplitView {
		id: splitMainPannel
		anchors.fill: parent
		orientation: Qt.Vertical

		BroadcastGridView {
			id: broadcastView
			height: 150
			anchors.fill: undefined
			Layout.maximumHeight: 250
			Layout.minimumHeight: 150
		}

		BottomPannel {
			id: pannelBottom
			Layout.fillHeight: true
			Layout.fillWidth: true
		}
	}
}
