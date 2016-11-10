import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "qrc:/ulysses/"

Rectangle {
	id: rectRightPannel

	property var currentName: ""
	property var listM
	property var dlgTestCase
	property var dlgTestSettings
	property int plan_id: -1

	signal sigCleanTCList()
	signal sigAddTC(var item)

	signal sigFetchTCInfo(var item)
	signal sigShowTCInfo(var item)

	signal sigFetchBuild(var item)
	signal sigShowSetting(var item)

	signal sigSetBuildId(var item)

	signal sigStart(var item)
	signal sigStop(var item)

	onSigAddTC: {
		//console.log("right pannel add tc item: " + item.name);
		//console.log("right pannel add tc summary: " + item.summary);
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
				for (var i=0; i<listM.count; i++) {
					listM.get(i)["enabled"]=true;
				}
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
				for (var i=0; i<listM.count; i++) {
					listM.get(i)["enabled"]=false;
				}
			}
		}
		IconButton {
			id: buttonSetting
			anchors.left: buttonUncheckAll.right
			anchors.leftMargin: 10
			text: qsTr("test settings")
			image: "qrc:/image/res/png/setting.png"
			tooltip: qsTr("test settings")
			onClicked: {
				var dlg=dlgTestSettings.createObject(rectRightPannel);
				dlg.sigSetBuildId.connect(rectRightPannel.sigSetBuildId);
				rectRightPannel.sigShowSetting.connect(dlg.sigShowSetting);
				var item = {
					"tlname": rectRightPannel.currentName,
					"planid": rectRightPannel.plan_id,
				};
				rectRightPannel.sigFetchBuild(item);
				dlg.open();
			}
		}
		IconButton {
			id: buttonStartTest
			anchors.left: buttonSetting.right
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

		signal sigChangeRowHeight(var row, var newHeight)

		TableViewColumn { role: "enabled"; title: "v"; width: 35; delegate: CheckBox {
				id: cbox
				anchors.fill: parent
				anchors.leftMargin: 5
				anchors.verticalCenter: parent.verticalCenter

				checked: listM.get(styleData.row).enabled === undefined ? false : listM.get(styleData.row).enabled

				onCheckedChanged: {
					listM.get(styleData.row).enabled=checked;
				}
			}
		}
		TableViewColumn { role: "name"; title: "Name"; width: 200 }
		TableViewColumn { role: "summary"; title: "Summary"; width: 600;
			delegate: Rectangle {
				color: "transparent"
				anchors.fill: parent

				Text {
					id: textColumn
					width: parent.width
					anchors {
						leftMargin: 5
					}
					text: styleData.value !== undefined ? styleData.value : ""
					wrapMode: Text.WordWrap

					signal sigChangeRowHeight(var row, var newHeight)

					onTextChanged: {
						//console.log("painterHeight: " + paintedHeight);
						//sigChangeRowHeight(styleData.row, paintedHeight);
						listM.get(styleData.row)["s_num"]=paintedHeight;
					}

					Component.onCompleted: {
						//sigChangeRowHeight.connect(table.sigChangeRowHeight);
					}
				}
			}

		}
		TableViewColumn { role: "result"; title: "Result"; width: 200 }
		TableViewColumn { role: "id"; title: "id"; width: 0; visible: false }
		TableViewColumn { role: "pid"; title: "pid"; width: 0; visible: false }
		TableViewColumn { role: "s_num"; title: "pid"; width: 0; visible: false }


		onDoubleClicked: {
			//console.log("click row: " + table.currentRow);
			var tc=listM.get(table.currentRow);
			var tcItem = {
				"id": tc["id"],
				"pid": tc["pid"],
				"name": tc["name"],
				"tlname": rectRightPannel.currentName
			}
			var dlg=dlgTestCase.createObject(rectRightPannel);
			dlg.sigInitData(tcItem);
			rectRightPannel.sigShowTCInfo.connect(dlg.sigShowTCInfo);
			rectRightPannel.sigFetchTCInfo(tcItem);
			dlg.open();
		}

		rowDelegate: Rectangle {
			id: rowDeleRect
			height: listM.get(styleData.row).s_num === undefined ? 10 : listM.get(styleData.row).s_num
			width: parent.width
			color: styleData.row !== undefined ? (styleData.row % 2 ? "#E0FFE0" : "#FFE0FF") : "#FFFFFF"
			border.color: "#F0F0F0"
			border.width: 1

			signal sigChangeHeight(var row, var newHeight)

			onSigChangeHeight: {
				//console.log("data: " + row + ", " + newHeight);
				//console.log("yes! " + listM.get(row)["summary"]);
			}

			Component.onCompleted: {
				table.sigChangeRowHeight.connect(sigChangeHeight);
			}
		}

	}

	Component.onCompleted: {
		console.log("table view current name: " + rectRightPannel.currentName);
		dlgTestCase=Qt.createComponent("TestCaseInfo.qml");
		dlgTestSettings=Qt.createComponent("RunTestSettings.qml");
		listM = listModelCreator.createObject();
		table.model = listM;
	}

	Component {
		id: listModelCreator
		ListModel {

		}
	}


}

