import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

Rectangle {
	id: rectSync

	border.width: 1;
	border.color: "#7070E0"
	color: "#F0F0F0";
	width: parent.width-10;
	height: parent.height-10;
	anchors.horizontalCenter: parent.horizontalCenter;
	anchors.verticalCenter: parent.verticalCenter;

	signal sigSetFilename(string filename)
	signal sigStartSync()

	property var szFilename: ""

	FileDialog {
		id: fileDialog
		modality: Qt.WindowModal
		selectMultiple: false
		selectFolder: false
		selectExisting: true
		title: qsTr("Select a factory test result file...")

		nameFilters: [ "Database files (*.db)", "All files (*.*)" ]

		onAccepted: {
			szFilename=fileDialog.fileUrl.toString()
			sigSetFilename(szFilename)
		}
	}

	Item {
		id: keyHandle
		focus: true
		enabled: true
		Keys.onPressed: {
			if(event.modifiers & Qt.ControlModifier && event.key === Qt.Key_O) {
				fileDialog.open()
			}
			else if (event.modifiers & Qt.AltModifier && event.modifiers & Qt.ControlModifier && event.key === Qt.Key_U) {
				sigStartSync()
			}
		}
	}

	Text {
		id: textPrompt;
		width: parent.width-10;
		height: 52;
		font.pixelSize: 32;
		font.family: "tohama";
		color: "#8080F0";
		text: "Fortress Database Sync"

		horizontalAlignment: Text.AlignHCenter;
		anchors.horizontalCenter: parent.horizontalCenter;
	}

	Rectangle {
		id: rectInfo;
		width: textPrompt.width;
		height: 78
		color: "transparent";
		anchors.top: textPrompt.bottom
		anchors.topMargin: 30
		anchors.horizontalCenter: parent.horizontalCenter

		Text {
			id: textInfo
			horizontalAlignment: Text.AlignHCenter;
			anchors.top: parent.top
			anchors.topMargin: 20
			anchors.horizontalCenter: parent.horizontalCenter;

			font.pixelSize: 28;
			font.family: "tohama";
			color: "#C08080";
			text: "progress"
		}

		ProgressBar {
			id: progInfo
			width: parent.width
			height: 20
			anchors.top: textInfo.bottom
			anchors.topMargin: 20
			anchors.horizontalCenter: parent.horizontalCenter;

		}
	}

	Rectangle {
		id: rectResult
		width: textPrompt.width;
		height: 78

		color: "transparent";
		anchors.top: rectInfo.bottom
		anchors.topMargin: 60
		Text {
			id: textResult
			horizontalAlignment: Text.AlignHCenter;
			anchors.top: parent.top
			anchors.topMargin: 20
			anchors.horizontalCenter: parent.horizontalCenter;
			font.pixelSize: 28;
			font.family: "tohama";
			color: "#C01080";
			text: "result"
		}
	}

	function updateInfo(info) {
		textResult.text=info
	}
}

