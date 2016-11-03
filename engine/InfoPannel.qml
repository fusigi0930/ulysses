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

	signal sigAddPlan(var item)

	onSigAddPlan: {
		pannelLeft.sigAddPlan(item);
	}

	LeftPannel {
		id: pannelLeft
		width: 300
		Layout.maximumWidth: 550
		Layout.minimumWidth: 150
	}

	RightPannel {
		id: pannelRight
		Layout.fillWidth: true
		Layout.minimumWidth: 150
		color: "#F0F0F0"
	}

	Component.onCompleted: {
		console.log("objName: " + objectName);
	}

}
