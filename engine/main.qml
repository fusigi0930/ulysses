import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "qrc:/ulysses/"

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
					pannelBottom.addInfoPanel("test");
				}
			}
			MenuItem {
				text: qsTr("find pannel")
				onTriggered: {
					pannelBottom.findInfoPannel("abc");
				}
			}
		}
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
