import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "qrc:/ulysses/"

Rectangle {
	id: rectLeftPannel
	property var currentName: ""

	signal sigNewTreeModel(var newTreeModel)

	onSigNewTreeModel: {
		tree.model=newTreeModel;
		tree.treeM=newTreeModel;

		tree.treeM.slotTest();
		var indexs=tree.treeM.getAllIndex();
		console.log(indexs);
		for (var i=0; i<indexs.length; i++) {
			tree.expand(indexs[i]);
		}

		tree.resizeColumnsToContents();
	}

	TreeView {
		id: tree
		anchors.fill: parent
		//model: treeModel

		property var treeM

		TableViewColumn { id: mainColumn; role: "enabled"; title: currentName; width: 150; delegate: checkboxTree }
		TableViewColumn { role: "name"; title: "Name"; width: 200 }
		TableViewColumn { role: "id"; title: "id"; width: 0; visible: false }
		TableViewColumn { role: "pid"; title: "pid"; width: 0; visible: false }
	}

	Component.onCompleted: {

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

