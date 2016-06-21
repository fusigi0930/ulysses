import QtQuick 2.2
import QtQuick.Controls 1.2
import "qrc:/ulysses/"

//
// mac: -- mac address
// ip: -- ip address
// style: -- (android, yocto, factory, ubuntu, uboot)
// image
// itemcolor: -- color
//

GridView {
	id: gridItems
	anchors.fill: parent
	cellWidth: 300
	cellHeight: 150

	model: listItems
	delegate: itemShower
	highlight: itemHighlight
	
	property var itemContent
	
	signal sigRunItem(var item)
	signal sigAddItem(var item)
	signal sigFinalResult(int nItem, var color)
	
	Component.onCompleted: {
	}	
	
	onSigAddItem: {
		if (item.style == "android") {
			item.image  = "qrc:/image/res/png/icon-android.png"
		}
		else if (item.style == "boot") {
			item.image = "qrc:/image/res/png/icon-boot.png"
		}
		else if (item.style == "factory") {
			item.image = "qrc:/image/res/png/icon-factory.png"
		}
		else if (item.style == "ubuntu") {
			item.image = "qrc:/image/res/png/icon-ubuntu.png"
		}
		else if (item.style == "yocto") {
			item.image = "qrc:/image/res/png/icon-yocto.png"
		}	
		listItems.append(item);
	}

	MouseArea {
		anchors.fill: parent
		onDoubleClicked: {
			var nItem=gridItems.indexAt(mouse.x, mouse.y);
			if (nItem !== -1) {
				gridItems.sigRunItem(nItem);
			}
		}
	}

	Component {
		id: itemShower
		Rectangle {
			border.color: "#80C0FF"
			border.width: 2
			width: gridItems.cellWidth
			height: gridItems.cellHeight
			color: itemcolor //"#F0F0F0"

			Item {
				property alias iImage: itemImage.source
				property alias ipText: itemIp.text
				property alias macText: itemMac.text
				
				anchors.fill: parent
				Image {
					id: itemImage
					anchors.verticalCenter: parent.verticalCenter
					x: 4
					source: image
				}
				Image {
					id: backImate
					anchors.left: itemImage.right
					anchors.right: parent.right
					anchors.top: parent.top
					anchors.bottom: parent.bottom
					
					source: "qrc:/image/res/png/back-device.png"
					opacity: 0.04
				}
				Text {
					id: itemMac
					anchors.left: itemImage.right
					anchors.top: parent.top
					anchors.leftMargin: 5
					anchors.topMargin: 34
					font.pixelSize: 28
					font.family: "Tohama"
					font.bold: true
					color: "#3030FF"
					text: mac
				}				

				Text {
					id: itemIp
					anchors.left: itemImage.right
					anchors.top: itemMac.bottom
					anchors.leftMargin: 5
					anchors.topMargin: 15
					font.pixelSize: 28
					font.family: "Tohama"
					font.bold: true
					color: "#3030FF"
					text: ip
				}				
			}
		}
	}

	Component {
		id: itemHighlight
		Rectangle {
			border.color: "#80FFFF"
			border.width: 3
			width: gridItems.cellWidth
			height: gridItems.cellHeight
		}
	}

	ListModel {
		id: listItems
		property int nIndex: 0
	}
}
