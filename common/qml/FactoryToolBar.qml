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
	id: toolBar
	signal sigClickButton(var button)
	signal sigSignal(var sig)
	
	property var itemName
	
	RowLayout {
		IconButton {
			id: buttonRun
			text: qsTr("clean pass")
			image: "qrc:/image/res/png/clean_pass.png"
			tooltip: qsTr("run")
			onClicked: {
				toolBar.sigClickButton("clean_pass")
			}
		}
		IconButton {
			id: buttonStop
			text: qsTr("clean fail")
			image: "qrc:/image/res/png/clean_fail.png"
			tooltip: qsTr("stop")
			onClicked: {
				toolBar.sigClickButton("clean_fail")
			}
		}
		IconButton {
			id: buttonReload
			text: qsTr("open xml")
			image: "qrc:/image/res/png/reset.png"
			tooltip: qsTr("change a xml file")
			onClicked: {
				toolBar.sigClickButton("open-xml")
			}
		}
		Rectangle {
			anchors.left: buttonReload.right
			anchors.leftMargin: 30
			anchors.verticalCenter: buttonReload.verticalCenter
			
			Text {
				id: labelFilename
				text: qsTr("Item:")
				font.pixelSize: 32
				font.family: "tohama"
				anchors.verticalCenter: parent.verticalCenter
				color: "#A0A0FF"
			}
			Text {
				id: textItemName
				anchors.left: labelFilename.right
				anchors.leftMargin: 5
				text: itemName
				font.pixelSize: 32
				font.family: "tohama"
				anchors.verticalCenter: parent.verticalCenter
				color: "#A0A0FF"
			}			
		}
	}
}