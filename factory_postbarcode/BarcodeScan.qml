import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import QtQuick.Layouts 1.0

Rectangle {
	id: rectScan

	border.width: 1;
	border.color: "#7070E0"
	color: "#F0F0F0";
	width: parent.width-10;
	height: parent.height-10;
	anchors.horizontalCenter: parent.horizontalCenter;
	anchors.verticalCenter: parent.verticalCenter;

	signal sigUpdateResult(string mac, string serial)
	signal sigGetResult(string mac)

	Text {
		id: textPrompt;
		width: parent.width-10;
		height: 52;
		font.pixelSize: 46;
		font.family: "tohama";
		color: "#8080F0";
		text: "Please Scan..."

		horizontalAlignment: Text.AlignHCenter;
		anchors.horizontalCenter: parent.horizontalCenter;
	}

	Rectangle {
		id: rectInfo;
		width: textPrompt.width;
		height: 78
		color: "transparent";
		anchors.top: textPrompt.bottom
		anchors.topMargin: 10

		Text {
			id: textMacLabel
			x: 5;
			y: 5;
			font.pixelSize: 28;
			font.family: "tohama";
			color: "#C08080";
			text: "Mac: "
		}

		Text {
			id: textMac;
			x: textMacLabel.x + textMacLabel.paintedWidth;
			y: textMacLabel.y;
			font.pixelSize: 28;
			font.family: "tohama";
			color: "#6060E0";
		}

		Text {
			id: textSerialLabel;
			x: textMacLabel.x;
			y: textMacLabel.y + textMacLabel.paintedHeight + 5;
			font.pixelSize: 28;
			font.family: "tohama";
			color: "#C08080";
			text: "Serial: "
		}

		Text {
			id: textSerial;
			x: textSerialLabel.x + textSerialLabel.paintedWidth;
			y: textSerialLabel.y;
			font.pixelSize: 28;
			font.family: "tohama";
			color: "#6060E0";
		}
	}

	Rectangle {
		id: rectResult
		width: textPrompt.width;
		height: 78

		color: "transparent";
		anchors.top: rectInfo.bottom
		anchors.topMargin: 10
		Text {
			id: textResult
			x: 5;
			y: 5;
			font.pixelSize: 28;
			font.family: "tohama";
			color: "#C01080";
		}
	}

	TextInput {
		id: editBarcode;
		width: parent.width-10;
		height: 28;
		font.pixelSize: 24
		font.family: "tohama";
		focus: true;
		color: "#8080F0";
		horizontalAlignment: TextInput.AlignHCenter;

		anchors.bottom: parent.bottom;
		anchors.bottomMargin: 10;

		Keys.onPressed: {
			if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
				event.accepted=true;
				detectBarcode(editBarcode.text);
				if (isMac === 1 && isSerial === 1) {
					isMac = 0;
					isSerial = 0;
					rectScan.sigUpdateResult(textMac.text, textSerial.text);
				}
			}
		}
	}

	function detectBarcode(data) {
		editBarcode.text="";
		var dataLeng=data.length;

		if (isMac === 0 && isSerial === 0) {
			textSerial.text="";
			textMac.text="";
			textResult.text="";
		}

		if (dataLeng === 11) {
			textSerial.text=data.toUpperCase()
			isSerial=1;
		}
		else if (dataLeng === 6) {
			textMac.text=("F80278"+data).toUpperCase();
			isMac=1;
			rectScan.sigGetResult(textMac.text)
		}
	}

	function updateUiResult(result) {
		textResult.text=result
	}
}

