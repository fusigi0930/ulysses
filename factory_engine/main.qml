import QtQuick 2.3
import QtQuick.Controls 1.2

import "qrc:/ulysses/"

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

	IconButton {
		id: testButton
		text: qsTr("YAS")
		image: "image/res/png/run.png"
		tooltip: qsTr("Test")
		onClicked: {
			console.log("test")
			//testView.sigAddItem({"name":"AA","command":"bb","itemcolor":"#F0F0F0"})
			testView.sigAddItem({"ip":"192.168.100.100","mac":"ff:ff:ff:ff:ff:ff","style":"android","itemcolor":"#f0f0f0"})
		}
	}

	Rectangle {
		x: 50
		y:80
		width: parent.width
		height: parent.height
		//FactoryGridView {
		BroadcastGridView {
			id: testView
		}
	}

}
