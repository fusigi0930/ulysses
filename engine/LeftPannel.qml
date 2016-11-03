import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "qrc:/ulysses/"

Rectangle {
	id: rectLeftPannel
	property var currentName: ""

	signal sigAddPlan(var item)

	onSigAddPlan: {
		tree.treeM.append(item);
	}

	TableView {
		id: tree
		anchors.fill: parent
		//model: treeModel

		property var treeM

		TableViewColumn { id: mainColumn; role: "enabled"; title: currentName; width: 135; delegate: checkboxTree }
		TableViewColumn { role: "name"; title: "Name"; width: 200 }
		TableViewColumn { role: "id"; title: "id"; width: 0; visible: false }
		TableViewColumn { role: "pid"; title: "pid"; width: 0; visible: false }
	}

	Component.onCompleted: {
		tree.treeM = treeModelCreator.createObject();
		tree.model = tree.treeM;
	}

	Component {
		id: treeModelCreator
		ListModel {

		}
	}

	Component {
		id: checkboxTree
		CheckBox {
			id: cbox
			MouseArea {
				anchors.fill: parent
				onClicked: {
					console.log("click checkbox!");
					cbox.checked = !cbox.checked;
				}
			}
		}
	}

}

