import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "qrc:/ulysses/"

Rectangle {
	id: rectLeftPannel
	property var currentName: ""

	signal sigAddPlan(var item)
	signal sigGetTC(var item)

	onSigAddPlan: {
		console.log("planid: " + item.planid + " projid: " + item.projid);
		tree.treeM.append(item);
	}

	TableView {
		id: tree
		anchors.fill: parent
		//model: treeModel

		property var treeM

		TableViewColumn { id: mainColumn; role: "enabled"; title: currentName; width: 135; delegate: CheckBox {
				id: ckbox
				anchors.fill: parent

				checked: tree.treeM.get(styleData.row).enabled

				onCheckedChanged: {
					tree.treeM.get(styleData.row).enabled=checked;
				}
			}
		}
		TableViewColumn { role: "name"; title: "Name"; width: 200 }
		TableViewColumn { role: "planid"; title: "planid"; width: 0; visible: false }
		TableViewColumn { role: "projid"; title: "projid"; width: 0; visible: false }

		onDoubleClicked: {
			var currentPlan=treeM.get(tree.currentRow);
			console.log("running status: " + currentPlan.enabled);
			var plan = {
				"tlname": rectLeftPannel.currentName,
				"name": currentPlan.name,
				"projid": currentPlan.projid,
				"planid": currentPlan.planid
			}

			rectLeftPannel.sigGetTC(plan);
		}
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

}

