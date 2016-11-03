import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "qrc:/ulysses/"

Rectangle {
	id: rectBottomPanel

	property int nCount: 0
	property var lstItems: []
	property var compInfoPannel

	signal sigAddPlan(var item)

	onSigAddPlan: {
		for (var i=0; i<lstItems.length; i++) {
			if (item.objName === lstItems[i].objectName) {
				console.log("bottom onSigAddPlan: " + item);
				lstItems[i].sigAddPlan(item);
				return;
			}
		}
	}

	Component.onCompleted: {
		compInfoPannel=Qt.createComponent("InfoPannel.qml");
	}

	function addInfoPanel(name) {
		// name format:
		// <system>:<ip>

		for (var i=0; i<lstItems.length; i++) {
			if (name === lstItems[i].objectName) {
				console.log("duplicate device: " + name);
				return;
			}
		}

		var object=compInfoPannel.createObject(rectBottomPanel);
		deactivateInfoPannels();
		console.log("add pannel name: " + name)
		object.objectName=name;
		object.visible=true;
		object.treeCurrentName=name;
		object.listCurrentName=name;
		lstItems.push(object);
	}

	function removeInfoPanel(name) {
		for (var i = 0; i < lstItems.length; i++) {
			if (name === lstItems[i].objectName) {
				lstItems.splice(i, 1);
				return;
			}
		}
		if (lstItems.length > 0) {
			lstItems[lstItems.length-1].visible=true;
		}
	}

	function findInfoPannel(name) {
		for (var i = 0; i < lstItems.length; i++) {
			console.log("item name: " + lstItems[i].objectName);
			if (name === lstItems[i].objectName) {
				return i;
			}
		}
		return -1;
	}

	function listInfoIname() {
		for (var i = 0; i < lstItems.length; i++) {
			console.log("item name: " + lstItems[i].objectName);
		}
	}

	function deactivateInfoPannels() {
		for (var i = 0; i < lstItems.length; i++) {
			lstItems[i].visible=false;
		}
	}

	function activateInfoPannel(index) {
		deactivateInfoPannels();
		lstItems[index].visible=true;
	}

	function clear() {
		lstItems.erase();
	}
}
