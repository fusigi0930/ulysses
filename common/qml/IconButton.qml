import QtQuick 2.2
import QtQuick.Controls 1.2

Rectangle {
	id: iconBtn
	property alias image: buttonImage.source
	property alias tooltip: pngButton.tooltip
	property alias text: bottomText.text
	property alias buttonWidth: pngButton.width
	property alias buttonHeight: pngButton.height

	width:60
	height:60
	color: "transparent"

	signal clicked
	
	ToolButton {
		id: pngButton
		width: 48
		height: 48
		tooltip: ""
		anchors.horizontalCenter: parent.horizontalCenter
		onClicked: iconBtn.clicked()
		Image {
			id: buttonImage
			anchors.fill: parent
			anchors.margins: 5
			source: "qrc:/image/res/png/run.png"
		}
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
