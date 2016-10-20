import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "qrc:/ulysses/"

SplitView {
	id: splitBottomFrame
	orientation: Qt.Horizontal
	anchors.fill: parent

	property alias treeCurrentName: pannelLeft.currentName
	property alias listCurrentName: pannelRight.currentName

	LeftPannel {
		id: pannelLeft
		width: 200
		Layout.maximumWidth: 250
		Layout.minimumWidth: 150
		color: "blue"
	}

	RightPannel {
		id: pannelRight
		Layout.fillWidth: true
		Layout.minimumWidth: 150
		color: "red"
	}

	Component.onCompleted: {
		console.log("objName: " + objectName);
	}

}
