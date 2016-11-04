import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "qrc:/ulysses/"

Rectangle {
	id: rectRightPannel

	property var currentName: ""
	property var listM

	signal sigCleanTCList()
	signal sigAddTC(var item)

	onSigAddTC: {
		console.log("right pannel add tc item: " + item.name);
		console.log("right pannel add tc summary: " + item.summary);
		listM.append(item);
	}

	onSigCleanTCList: {
		console.log("right clear!");
		listM.clear();
	}

	RowLayout {
		id: layoutTool
		anchors.left: parent.left
		anchors.leftMargin: 10
		anchors.right: parent.right
		anchors.rightMargin: 10
		anchors.top: parent.top
		height: 80

		IconButton {
			id: buttonCheckAll
			anchors.left: parent.left
			anchors.leftMargin: 10
			text: qsTr("check all")
			image: "qrc:/image/res/png/check-all.png"
			tooltip: qsTr("check all")
			onClicked: {
				console.log("check all");
			}
		}
		IconButton {
			id: buttonUncheckAll
			anchors.left: buttonCheckAll.right
			anchors.leftMargin: 10
			text: qsTr("unclick all")
			image: "qrc:/image/res/png/uncheck-all.png"
			tooltip: qsTr("uncheck all")
			onClicked: {
				console.log("uncheck all");
			}
		}
		IconButton {
			id: buttonStartTest
			anchors.left: buttonUncheckAll.right
			anchors.leftMargin: 10
			text: qsTr("Start test")
			image: "qrc:/image/res/png/run.png"
			tooltip: qsTr("starting test procedure")
			onClicked: {
				console.log("starting test procedure");
			}
		}
		IconButton {
			id: buttonStopTest
			anchors.left: buttonStartTest.right
			anchors.leftMargin: 10
			text: qsTr("Stop test")
			image: "qrc:/image/res/png/stop.png"
			tooltip: qsTr("stop running test procedure")
			onClicked: {
				console.log("stop test procedure");
			}
		}
	}


	TableView {
		id: table
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: layoutTool.bottom
		anchors.bottom: parent.bottom

		TableViewColumn { role: "enabled"; title: "v"; width: 35; delegate: CheckBox {
				id: cbox
				anchors.fill: parent
				anchors.leftMargin: 5

				checked: listM.get(styleData.row).enabled

				onCheckedChanged: {
					listM.get(styleData.row).enabled=checked;
				}
			}
		}
		TableViewColumn { role: "name"; title: "Name"; width: 200 }
		TableViewColumn { role: "summary"; title: "Summary"; width: 600 }
		TableViewColumn { role: "id"; title: "id"; width: 0; visible: false }
		TableViewColumn { role: "pid"; title: "pid"; width: 0; visible: false }

	}

	Component.onCompleted: {
		console.log("table view current name: " + rectRightPannel.currentName);
		listM = listModelCreator.createObject();
		table.model = listM;
	}

	Component {
		id: listModelCreator
		ListModel {

		}
	}


}

