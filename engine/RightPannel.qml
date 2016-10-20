import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "qrc:/ulysses/"

Rectangle {
	id: rectRightPannel

	property var currentName: ""

	TableView {
		id: table
		anchors.fill: parent

		TableViewColumn { role: "enabled"; title: "v"; width: 25; delegate: checkboxTable }
		TableViewColumn { role: "name"; title: "Name"; width: 200 }
		TableViewColumn { role: "id"; title: "id"; width: 0 }
		TableViewColumn { role: "pid"; title: "pid"; width: 0 }
	}

	Component.onCompleted: {
		console.log("table view current name: " + rectRightPannel.currentName);
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

