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
	signal sigRemoveItem(var item)
	signal sigUpdateItem(var item)
	signal sigFinalResult(var nameIp, var item)
	signal sigStartClient(var ip)
	signal sigEndClient(var ip)
			
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
	
	onSigRemoveItem: {
		for (var i=0; i<listItems.count; i++) {
			if (listItems.get(i).ip === item.ip) {
				listItems.remove(i)
				break;
			}
		}
	}
	
	onSigUpdateItem: {
		for (var i=0; i<listItems.count; i++) {
			if (listItems.get(i).ip === item.ip) {
				if (item.style == "android") {
					listItems.get(i).image  = "qrc:/image/res/png/icon-android.png"
				}
				else if (item.style == "boot") {
					listItems.get(i).image = "qrc:/image/res/png/icon-boot.png"
				}
				else if (item.style == "factory") {
					listItems.get(i).image = "qrc:/image/res/png/icon-factory.png"
				}
				else if (item.style == "ubuntu") {
					listItems.get(i).image = "qrc:/image/res/png/icon-ubuntu.png"
				}
				else if (item.style == "yocto") {
					listItems.get(i).image = "qrc:/image/res/png/icon-yocto.png"
				}	
				listItems.get(i).itemcolor=item.itemcolor
				listItems.get(i).style=item.style
				break;
			}
		}	
	}
	
	Component.onCompleted: {
	}	

	MouseArea {
		anchors.fill: parent
		acceptedButtons: Qt.LeftButton | Qt.RightButton
		
		onDoubleClicked: {
			var nItem=gridItems.indexAt(mouse.x, mouse.y);
			if (nItem !== -1 && mouse.button === Qt.LeftButton) {
				console.log("ip: " + listItems.get(nItem).ip)
				gridItems.sigStartClient(listItems.get(nItem).ip)
			}
		}
		
		onClicked: {
			var nItem=gridItems.indexAt(mouse.x, mouse.y);
			if (nItem !== -1 && mouse.button === Qt.RightButton) {
				if (listItems.get(nItem).itemcolor !== "#8080FF") {
					gridItems.sigEndClient(listItems.get(nItem).ip)
				}
			}		
		}
	}

	Component {
		id: itemShower
		Rectangle {
			border.color: "#80C0FF"
			border.width: 0
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
					opacity: 0.1
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
					color: "#3060FF"
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
