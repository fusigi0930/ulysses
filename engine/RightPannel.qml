import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "qrc:/ulysses/"

Rectangle {
	id: rectRightPannel

	property var currentName: ""
	property var listM

	signal sigCleanTCList()

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

		TableViewColumn { role: "enabled"; title: "v"; width: 25; delegate: checkboxTable }
		TableViewColumn { role: "name"; title: "Name"; width: 200 }
		TableViewColumn { role: "id"; title: "id"; width: 0 }
		TableViewColumn { role: "pid"; title: "pid"; width: 0 }
	}

	Component.onCompleted: {
		console.log("table view current name: " + rectRightPannel.currentName);
		listM = listModelCreator.createObject();
		table.model = listM;
		listM.append( { "name":"YES", "enabled":"" }
					);
	}

	Component {
		id: listModelCreator
		ListModel {

		}
	}

	Component {
		id: checkboxTable
		CheckBox {
			id: cbox
			MouseArea {
				anchors.fill: parent
				onClicked: {
					cbox.checked = !cbox.checked;
				}
			}
		}
	}

}

