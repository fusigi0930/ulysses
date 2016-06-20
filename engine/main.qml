import QtQuick 2.1
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
			setIcon("/image/res/png/run.png");
		}
	}

    Label {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
}
