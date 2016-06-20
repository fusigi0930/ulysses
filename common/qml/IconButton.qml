import QtQuick 2.2
import QtQuick.Controls 1.2

Rectangle {
	id: iconBtn
	property alias image: pngButton.iconSource
	property alias tooltip: pngButton.tooltip
	property alias text: bottomText.text

	width:60
	height:60
	color: "transparent"

	signal clicked
	
	ToolButton {
		id: pngButton
		iconSource: ""
		tooltip: ""
		anchors.horizontalCenter: parent.horizontalCenter
		onClicked: iconBtn.clicked()
	}
	Text {
		id: bottomText
		text: ""
		font.pixelSize: 16
		font.family: "tohama"
		anchors.bottom: parent.bottom
		color: "#A0A0FF"
		anchors.horizontalCenter: parent.horizontalCenter
	}
}
