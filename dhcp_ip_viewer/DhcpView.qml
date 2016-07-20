import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0

Rectangle {
	id: rectList

	border.width: 2
	border.color: "#505050"

	TableView {
		id: tableList
		anchors.fill: parent

		model: listModel

		TableViewColumn {role: "ip"; title: "IP address"; width: 200 }
		TableViewColumn {role: "count"; title: "Count"; width: 50 }
		TableViewColumn {role: "mac"; title: "Mac address"; width: 500 }
	}

	ListModel {
		id: listModel
	}

	function addItem(item) {
		listModel.append(item)
	}

	function count() {
		return listModel.count
	}

	function get(item) {
		for (var i=0; i<listModel.count; i++) {
			var data=listModel.get(i)
			console.log("list item: " + data.ip)
			if (item.ip === data.ip) {
				data.index=i
				return data
			}
		}
	}

	function set(index, item) {
		listModel.set(index, item)
	}
}

