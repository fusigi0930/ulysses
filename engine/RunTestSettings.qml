import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
import QtQuick.Controls.Styles 1.2
import "qrc:/ulysses/"

Dialog {
	id: dlgTestSetting
	modality: Qt.WindowModal
	width: Screen.width / 1.5
	height: Screen.height / 1.5
	visible: false
	title: ""
	standardButtons: StandardButton.Ok | StandardButton.Cancel

	property int build_id: -1
	property int platform_id: -1
	property var listBuildM
	property var listPlatformM

	signal sigShowSetting(var item)
	signal sigSetBuildId(var item)

	onSigShowSetting: {
		console.log("dlg show setting!");
		for (var i=0; i<item.length; i++) {
			if (item[i].type === "build") {
				var infoBuild = {
					"text": item[i].name,
					"planid": item[i].testplan_id,
					"id": item[i].id
				}
				console.log("append build: " + item[i].name);
				dlgTestSetting.listBuildM.append(infoBuild);
				console.log("build list count: " + listBuildM.count);
			}
			else if (item[i].type === "platform") {
				var infoPlat = {
					"text": item[i].name,
					"planid": -1,
					"id": item[i].id
				}
				console.log("append platform: " + item[i].name);
				dlgTestSetting.listPlatformM.append(infoPlat);
			}
		}
		// refresh ui
		cboxBuild.currentIndex=1;
		cboxPlatform.currentIndex=1;
		cboxBuild.currentIndex=0;
		cboxPlatform.currentIndex=0;
	}

	Rectangle {
		id: rectMainInfo

		width: dlgTestSetting.width - 20
		height: dlgTestSetting.height - 60
		color: "transparent"

		Rectangle {
			id: rectBuild
			width: parent.width
			height: parent.height / 2.2
			color: "transparent"

			Text {
				id: labelBuild
				text: "Build:"
				font.pixelSize: 28
				font.bold: true
				height: paintedHeight
			}
			ComboBox {
				id: cboxBuild
				anchors.top: labelBuild.bottom
				anchors.topMargin: 5
				width: parent.width

				textRole: "text"
			}
		}

		Rectangle {
			id: rectPlatform
			width: parent.width
			height: parent.height / 2.2
			anchors.top: rectBuild.bottom
			anchors.topMargin: 5
			color: "transparent"

			Text {
				id: labelPlatform
				text: "Platrom:"
				font.pixelSize: 28
				font.bold: true
				height: paintedHeight
			}
			ComboBox {
				id: cboxPlatform
				anchors.top: labelPlatform.bottom
				anchors.topMargin: 5
				width: parent.width
				textRole: "text"
			}
		}
	}

	onAccepted: {
		console.log("counts: " + listBuildM.count + " , " + listPlatformM.count);
		console.log("build index: " + cboxBuild.currentIndex);
		console.log("plat index: " + cboxPlatform.currentIndex);
		if (listBuildM.count === 0 || listPlatformM.count === 0) {
			return;
		}

		console.log("build: " + JSON.stringify(listBuildM.get(0)));

		build_id=dlgTestSetting.listBuildM.get(cboxBuild.currentIndex).id;
		platform_id=dlgTestSetting.listPlatformM.get(cboxPlatform.currentIndex).id;
		console.log("build id: " + build_id);
		console.log("platform id: " + platform_id);
		var item = {
			"buildid": listBuildM.get(cboxBuild.currentIndex).id,
			"planid": listBuildM.get(cboxBuild.currentIndex).planid,
			"platid": listPlatformM.get(cboxPlatform.currentIndex).id
		}

		dlgTestSetting.sigSetBuildId(item);
	}

	onRejected: {

	}

	Component.onCompleted: {
		dlgTestSetting.listBuildM = listBuildCreator.createObject();
		dlgTestSetting.listPlatformM = listPlatCreator.createObject();

		console.log("setting model var");
		cboxBuild.model = dlgTestSetting.listBuildM;
		cboxPlatform.model = dlgTestSetting.listPlatformM;

		var item = {
			"text": "Not specific",
			"id": -1
		}

		listBuildM.append(item);
		listPlatformM.append(item);
	}

	Component {
		id: listBuildCreator
		ListModel {

		}
	}

	Component {
		id: listPlatCreator
		ListModel {

		}
	}

}

