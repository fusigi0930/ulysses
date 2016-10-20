import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "qrc:/ulysses/"

Rectangle {
	id: rectLeftPannel
	property var currentName: ""

	TreeView {
		id: tree
		anchors.fill: parent

		TableViewColumn { role: "enabled"; title: "v"; width: 25; delegate: checkboxTree }
		TableViewColumn { role: "name"; title: "Name"; width: 200 }
		TableViewColumn { role: "id"; title: "id"; width: 0 }
		TableViewColumn { role: "pid"; title: "pid"; width: 0 }
	}

	Component.onCompleted: {
		console.log("tree view current name: " + rectLeftPannel.currentName);
	}

	Component {
		id: checkboxTree
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

