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
	signal sigAddTC(var item)
	signal sigGetTC(var item)
	signal sigCleanTCList(var item)

	signal sigFetchTCInfo(var item)
	signal sigShowTCInfo(var item)

	signal sigFetchBuild(var item)
	signal sigShowSetting(var item)

	onSigAddPlan: {
		for (var i=0; i<lstItems.length; i++) {
			if (item.objName === lstItems[i].objectName) {
				console.log("bottom onSigAddPlan: " + item);
				lstItems[i].sigAddPlan(item);
				return;
			}
		}
	}

	onSigCleanTCList: {
		for (var i=0; i<lstItems.length; i++) {
			if (item.tlname === lstItems[i].objectName) {
				lstItems[i].sigCleanTCList();
				return;
			}
		}
	}

	onSigAddTC: {
		for (var i=0; i<lstItems.length; i++) {
			if (item.tlname === lstItems[i].objectName) {
				lstItems[i].sigAddTC(item);
				return;
			}
		}
	}

	onSigShowTCInfo: {
		console.log("bottom panel show tc item: " + item.tlname);
		for (var i=0; i<lstItems.length; i++) {
			if (item[0].tlname === lstItems[i].objectName) {
				console.log("bottom panel show item");
				lstItems[i].sigShowTCInfo(item);
				return;
			}
		}
	}

	onSigShowSetting: {
		console.log("bottom pannel show setting: " + item[0].tlname);
		for (var i=0; i<lstItems.length; i++) {
			if (item[0].tlname === lstItems[i].objectName) {
				console.log("bottom panel show build and platfrom");
				lstItems[i].sigShowSetting(item);
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
		object.sigGetTC.connect(reqGetTC);
		object.sigFetchTCInfo.connect(regFetchTCInfo);
		object.sigFetchBuild.connect(sigFetchBuild);
		lstItems.push(object);
	}

	function reqGetTC(item) {
		console.log("tlname: " + item.tlname);
		rectBottomPanel.sigGetTC(item);
	}

	function regFetchTCInfo(item) {
		rectBottomPanel.sigFetchTCInfo(item);
	}

	function removeInfoPanel(name) {
		for (var i = 0; i < lstItems.length; i++) {
			if (name === lstItems[i].objectName) {
				lstItems[i].sigGetTC.disconnect(reqGetTC);
				lstItems[i].sigFetchTCInfo.disconnect(regFetchTCInfo);
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
