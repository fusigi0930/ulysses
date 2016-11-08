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
	signal sigAddTC(var item)
	signal sigGetTC(var item)
	signal sigCleanTCList()

	signal sigFetchTCInfo(var item)
	signal sigShowTCInfo(var item)

	onSigAddPlan: {
		pannelLeft.sigAddPlan(item);
	}

	onSigAddTC: {
		console.log("info pannel add tc item: ", item.name);
		pannelRight.sigAddTC(item);
	}

	onSigCleanTCList: {
		pannelRight.sigCleanTCList();
	}

	onSigShowTCInfo: {
		pannelRight.sigShowTCInfo(item);
	}

	LeftPannel {
		id: pannelLeft
		width: 300
		Layout.maximumWidth: 550
		Layout.minimumWidth: 150

		onSigGetTC: {
			splitBottomFrame.sigGetTC(item);
		}
	}

	RightPannel {
		id: pannelRight
		Layout.fillWidth: true
		Layout.minimumWidth: 150
		color: "#F0F0F0"

		onSigFetchTCInfo: {
			splitBottomFrame.sigFetchTCInfo(item);
		}
	}

	Component.onCompleted: {
		console.log("objName: " + objectName);
	}

}
