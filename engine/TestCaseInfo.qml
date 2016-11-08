import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
import QtQuick.Controls.Styles 1.2
import "qrc:/ulysses/"

Dialog {
	id: dlgTestCase

	modality: Qt.ApplicationModal
	width: Screen.width / 1.5
	height: Screen.height / 1.5
	x: Screen.width / 6
	y: Screen.height / 6
	visible: true
	title: ""

	standardButtons: StandardButton.Ok

	property var id
	property var pid
	property var tcname

	property var listSummaryM
	property var listActionM

	signal sigInitData(var item)
	signal sigShowTCInfo(var item)

	onSigInitData: {
		console.log("dialog id: " + item.id);
		console.log("dialog name: " + item.name);
		id=item.id;
		pid=item.pid;
		tcname=item.name;
		dlgTestCase.title=item.name;
	}

	onSigShowTCInfo: {
		console.log("step count: " + item.length);

		if (item.length >= 1 && item[0].tcid !== id) return;

		listSummaryM.append(item[0]);

		for (var i=0; i<item.length; i++) {
			console.log("add item " + item[i].actions);
			listActionM.append(item[i]);
		}
	}

	onAccepted: {
		console.log("tc info dialog ok");
	}


	Rectangle {
		id: rectMainInfo
		width: dlgTestCase.width - 20
		height: dlgTestCase.height - 60

		anchors.verticalCenter: parent.verticalCenter
		anchors.horizontalCenter: parent.horizontalCenter

		color: "transparent"

		Rectangle {
			id: rectTitle
			color: "transparent"
			anchors.top: rectMainInfo.top
			anchors.left: rectMainInfo.left
			anchors.right: rectMainInfo.right
			height: labelTitle.paintedHeight + 1

			Text {
				id: labelTitle
				text: "Title: " + tcname
				font.pixelSize: 28
				font.bold: true

				anchors.left: parent.left
			}
		}

		TableView {
			id: tableDescript
			anchors.top: rectTitle.bottom
			anchors.topMargin: 10
			anchors.left: rectMainInfo.left
			anchors.right: rectMainInfo.right

			TableViewColumn { role: "summary"; title: "Summary"; width: tableDescript.width / 2 - 2;
				delegate: Rectangle {
					color: "transparent"
					anchors.fill: parent

					Text {
						id: textSummary
						width: parent.width
						font.pixelSize: 20
						anchors {
							leftMargin: 5
						}
						text: styleData.value !== undefined ? styleData.value : ""
						wrapMode: Text.WordWrap

						onTextChanged: {
							var num=listSummaryM.get(styleData.row)["s_num"];
							if (num === undefined) num = 0;
							listSummaryM.get(styleData.row)["s_num"]=paintedHeight > num ? paintedHeight : num;
						}
					}
				}
			}
			TableViewColumn { role: "preconditions"; title: "Precondition"; width: tableDescript.width / 2 - 2;
				delegate: Rectangle {
					color: "transparent"
					anchors.fill: parent

					Text {
						id: textPrecondition
						width: parent.width
						font.pixelSize: 20
						anchors {
							leftMargin: 5
						}
						text: styleData.value !== undefined ? styleData.value : ""
						wrapMode: Text.WordWrap

						onTextChanged: {
							var num=listSummaryM.get(styleData.row)["s_num"];
							if (num === undefined) num = 0;
							listSummaryM.get(styleData.row)["s_num"]=paintedHeight > num ? paintedHeight : num;
						}
					}
				}
			}

			TableViewColumn { role: "s_num"; title: "yes"; width: 0; visible: false }

			rowDelegate: Rectangle {
				id: rowDeleSummary
				height: listSummaryM.get(styleData.row).s_num === undefined ? 10 : listSummaryM.get(styleData.row).s_num
				width: parent.width
				color: "#E0E0FF"
				border.width: 0
			}

			style: TableViewStyle {
				backgroundColor: "#E0E0FF"
			}
		}

		TableView {
			id: tableActions
			anchors.top: tableDescript.bottom
			anchors.topMargin: 10
			anchors.left: rectMainInfo.left
			anchors.right: rectMainInfo.right
			anchors.bottom: rectMainInfo.bottom

			rowDelegate: Rectangle {
				id: rowDeleRect
				height: listActionM.get(styleData.row).s_num === undefined ? 10 : listActionM.get(styleData.row).s_num
				width: parent.width
				color: styleData.row !== undefined ? (styleData.row % 2 ? "#E0FFE0" : "#FFE0FF") : "#FFFFFF"
				border.color: "#F0F0F0"
				border.width: 1
			}

			TableViewColumn { role: "actions"; title: "Step"; width: tableActions.width / 2 - 51;
				delegate: Rectangle {
					color: "transparent"
					anchors.fill: parent

					Text {
						id: textAction
						width: parent.width
						font.pixelSize: 16
						anchors {
							leftMargin: 5
						}
						text: styleData.value !== undefined ? styleData.value : ""
						wrapMode: Text.WordWrap

						onTextChanged: {
							var num=listActionM.get(styleData.row)["s_num"];
							if (num === undefined) num = 0;
							listActionM.get(styleData.row)["s_num"]=paintedHeight > num ? paintedHeight : num;
						}
					}
				}
			}

			TableViewColumn { role: "expected_results"; title: "Expectation"; width: tableActions.width / 2 - 51;
				delegate: Rectangle {
					color: "transparent"
					anchors.fill: parent

					Text {
						id: textExpect
						width: parent.width
						font.pixelSize: 16
						anchors {
							leftMargin: 5
						}
						text: styleData.value !== undefined ? styleData.value : ""
						wrapMode: Text.WordWrap

						onTextChanged: {
							var num=listActionM.get(styleData.row)["s_num"];
							if (num === undefined) num = 0;
							listActionM.get(styleData.row)["s_num"]=paintedHeight > num ? paintedHeight : num;
						}
					}
				}
			}
			TableViewColumn { role: "execution_type"; title: "type"; width: 0; visible: false }

			TableViewColumn { role: "result"; title: "Result"; width: 95 }

			TableViewColumn { role: "id"; title: "yes"; width: 0; visible: false }
			TableViewColumn { role: "s_num"; title: "yes"; width: 0; visible: false }
		}

	}

	Component.onCompleted: {
		listSummaryM = listModelCreator.createObject();
		listActionM = listModelCreator.createObject();
		tableActions.model = listActionM;
		tableDescript.model = listSummaryM;
	}

	Component {
		id: listModelCreator
		ListModel {

		}
	}


}

