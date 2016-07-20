import QtQuick 2.1
import QtQuick.Controls 1.2
import "qrc:/ulysses/"
import InterfaceUi 1.0

ApplicationWindow {
	visible: true
	title: qsTr("Ulysses - dhcp test viewer")
	visibility: "Maximized"

	signal sigUpdateCurrentIp(var ip)



	InterfaceUi {
		id: interUi

		onSigUpdateTotalCount: {
			textCount.text=nCount
		}

		onSigUpdateIp: {
			textCurrentIP.text=ip
		}

		onSigUpdateItem: {
			var data=dhcpView.get(item)
			console.log("item index: " + data.index)
			dhcpView.set(data.index, item)
		}

		onSigInsertItem: {
			dhcpView.addItem(item)
		}
	}

	Rectangle {
		id: rectCount
		border.width: 2
		border.color: "#209020"
		anchors.left: parent.left
		anchors.leftMargin: 10
		anchors.top: parent.top
		anchors.topMargin: 10

		width: 300
		height: 50

		Label {
			id: labelCount
			anchors.left: parent.left
			anchors.leftMargin: 10
			anchors.top: parent.top
			anchors.topMargin: 10

			font.pixelSize: 26
			font.family: "Tohama"
			color: "#5050FF"
			text: "Total Count:"
		}

		Text {
			id: textCount
			anchors.left: labelCount.right
			anchors.leftMargin: 10
			anchors.top: labelCount.top

			font.pixelSize: 26
			font.family: "Tohama"
			color: "#5050FF"
			text: "0"
		}
	}

	Rectangle {
		id: rectCurrentIP
		border.width: 2
		border.color: "#209020"
		anchors.left: rectCount.right
		anchors.leftMargin: 10
		anchors.top: parent.top
		anchors.topMargin: 10

		width: 300
		height: 50

		Label {
			id: labelCurrentIP
			anchors.left: parent.left
			anchors.leftMargin: 10
			anchors.top: parent.top
			anchors.topMargin: 10

			font.pixelSize: 26
			font.family: "Tohama"
			color: "#5050FF"
			text: "Current IP:"
		}

		Text {
			id: textCurrentIP
			anchors.left: labelCurrentIP.right
			anchors.leftMargin: 10
			anchors.top: labelCurrentIP.top

			font.pixelSize: 26
			font.family: "Tohama"
			color: "#5050FF"
			text: "0.0.0.0"
		}
	}

	DhcpView {
		id: dhcpView

		anchors.left: parent.left
		anchors.leftMargin: 5
		anchors.right: parent.right
		anchors.rightMargin: 5
		anchors.top: rectCount.bottom
		anchors.topMargin: 10
		anchors.bottom: parent.bottom
	}
}
