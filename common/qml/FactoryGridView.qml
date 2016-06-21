import QtQuick 2.2
import QtQuick.Controls 1.2

//
// name: -- item name
// image: -- item image
// command
// expect
// itemcolor -- item color
//

GridView {
	id: gridItems
	anchors.fill: parent
	cellWidth: 250
	cellHeight: 70

	model: listItems
	delegate: itemShower
	highlight: itemHighlight
	
	signal sigRunItem(int nItem)
	signal sigAddItem(var item)
	signal sigUpdateItemResult(var color)
	signal sigFinalResult(int nItem, var color)
	
	onSigAddItem: {
		listItems.append(item);
	}
	
	onUpdateItemResult: {
		listItems.get(nItem).itemcolor=color;
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
			border.color: "#8080FF"
			border.width: 1
			width: gridItems.cellWidth
			height: gridItems.cellHeight
			color: itemcolor //"#F0F0F0"

			Item {
				property alias iImage: itemImage.source
				property alias iText: itemText.text
				
				anchors.fill: parent
				Image {
					id: itemImage
					anchors.verticalCenter: parent.verticalCenter
					x: 4
					source: image
				}

				Text {
					id: itemText
					anchors.left: itemImage.right
					anchors.leftMargin: 5
					anchors.verticalCenter: itemImage.verticalCenter
					font.pixelSize: 40
					font.family: "Tohama"
					color: "#5050FF"
					text: name
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
