import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Window 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.2
import "qrc:/ulysses/"

//
// mac: -- mac address
// ip: -- ip address
// style: -- (android, yocto, factory, ubuntu, uboot)
// image
// itemcolor: -- color
//

ToolBar {
	property var counterVal;

	id: toolBar
	signal sigClickButton(var button)
	signal sigSignal(var sig)
	
	onSigSignal: {
		if (sig == "access") {
			timerSignal.start()
		}
		else if (sig == "wait") {
			timerSignal.stop()
			rectRxSignal.on = 1
			rectRxSignal.color = "#1010F0"
		}
		else {
			timerSignal.stop()
			rectRxSignal.on = 0
			rectRxSignal.color = "transparent"
		}
	}	
	
	RowLayout {
		IconButton {
			id: buttonRun
			text: qsTr("run")
			image: "qrc:/image/res/png/run.png"
			tooltip: qsTr("run")
			onClicked: {
				toolBar.sigClickButton("run")
				counterVal=0
				timerRunning.start()
			}
		}
		IconButton {
			id: buttonStop
			text: qsTr("stop")
			image: "qrc:/image/res/png/stop.png"
			tooltip: qsTr("stop")
			onClicked: {
				toolBar.sigClickButton("stop")
				timerRunning.stop()
			}
		}
		IconButton {
			id: buttonReset
			text: qsTr("reset")
			image: "qrc:/image/res/png/reset.png"
			tooltip: qsTr("reset")
			onClicked: {
				counterVal=0
				toolBar.sigClickButton("reset")
				textCounter.text = "0"
			}
		}

		Rectangle {
			width:60
			height:60
			color: "transparent"
			Image {
				id: statusIcon
				source: ""
				anchors.horizontalCenter: parent.horizontalCenter
			}
			Text {
				text: qsTr("status")
				font.pixelSize: 16
				font.family: "tohama"
				anchors.bottom: parent.bottom
				color: "#A0A0FF"
				anchors.horizontalCenter: parent.horizontalCenter
			}
		}

		Rectangle {
			id: rectProductInfo
			width:460
			height:60
			color: "transparent"
			anchors.left: statusIcon.right
			anchors.leftMargin: 10
			Text {
				id: labelMac
				text: qsTr("MAC:")
				font.pixelSize: 32
				font.family: "tohama"
				anchors.verticalCenter: parent.verticalCenter
				color: "#A0A0FF"
			}
			Text {
				id: textCurrentMac

				width: 180
				height: 30
				anchors.verticalCenter: parent.verticalCenter
				anchors.left: labelMac.right
				anchors.leftMargin: 5
				font.pixelSize: 24
				font.family: "tohama"
				color: "#40407F"
			}
			Text {
				id: labelSerial
				text: qsTr("Serial:")
				font.pixelSize: 32
				font.family: "tohama"
				anchors.verticalCenter: parent.verticalCenter
				anchors.left: textCurrentMac.right
				anchors.leftMargin: 10
				color: "#A0A0FF"
			}
			Text {
				height: 30
				width: 180
				id: textCurrentSerial
				anchors.left: labelSerial.right
				anchors.leftMargin: 5
				anchors.verticalCenter: parent.verticalCenter
				font.pixelSize: 24
				font.family: "tohama"
				color: "#40407F"
			}
		}

		Rectangle {
			id: rectCounter
			width:120
			height:60
			color: "transparent"
			anchors.left: rectProductInfo.right
			anchors.leftMargin: 160

			Text {
				id: labelCounter
				text: qsTr("Counter:")
				font.pixelSize: 32
				font.family: "tohama"
				anchors.verticalCenter: parent.verticalCenter
				color: "#A0A0FF"
			}

			Timer {
				id: timerRunning
				interval: 1000
				running: false
				repeat: true
				onTriggered: {
					counterVal=counterVal+1;
					textCounter.text=counterVal;
				}
			}

			Text {
				id: textCounter
				height: 30
				width: 180

				anchors.left: labelCounter.right
				anchors.leftMargin: 5
				anchors.verticalCenter: parent.verticalCenter
				font.pixelSize: 24
				font.family: "tohama"
				color: "#40407F"
				text: qsTr("0")
			}
		}
		
		Rectangle {
			property var on: 0
			id: rectRxSignal
			width:32
			height:32
			color: "transparent"
			anchors.left: rectCounter.right
			anchors.leftMargin: 160
			border.color: "#808080"
			border.width: 2
	
			Timer {
				id: timerSignal
				interval: 60
				running: false
				repeat: true
				onTriggered: {
					if (rectRxSignal.on == 0) {
						rectRxSignal.on = 1
						rectRxSignal.color = "#F01010"
					}
					else {
						rectRxSignal.on = 0
						rectRxSignal.color = "transparent"
					}
				}
			}	

		}
	}
}