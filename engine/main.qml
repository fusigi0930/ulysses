import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import "qrc:/ulysses/"
import InterfaceUi 1.0

ApplicationWindow {
	id: mainWnd
	visible: true
	title: qsTr("Thor Hammer")

	property var dlgTestCase

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
		Menu {
			title: "test"
			MenuItem {
				text: qsTr("list pannel in console")
				onTriggered: {
					pannelBottom.listInfoIname();
				}
			}
			MenuItem {
				text: qsTr("show test case dialog")
				onTriggered: {
					var dlg=dlgTestCase.createObject(mainWnd);
				}
			}
		}
    }

	//Loader {
	//	id: loaderDlgTestCase
	//}

	InterfaceUi {
		id: interfaceUi

		onSigNewDev: {
			broadcastView.sigAddItem(item);
		}

		onSigHeltDev: {
			broadcastView.sigRemoveItem(item);
		}

		onSigAddPlan: {
			pannelBottom.sigAddPlan(item);
		}

		onSigAddTC: {
			console.log("add tc: " + item.name + " pannel name: " + item.tlname);
			pannelBottom.sigAddTC(item);
		}

		onSigShowTCInfo: {
			console.log("show tc item: " + item[0].tlname);
			pannelBottom.sigShowTCInfo(item);
		}

		onSigShowBuildPlatform: {
			console.log("main show build item: " + item.length);
			console.log("main show build: " + JSON.stringify(item));
			pannelBottom.sigShowSetting(item);
		}
	}

	Component.onCompleted: {
		showMaximized();
		dlgTestCase=Qt.createComponent("TestCaseInfo.qml");
	}

	SplitView {
		id: splitMainPannel
		anchors.fill: parent
		orientation: Qt.Vertical

		BroadcastGridView {
			id: broadcastView
			height: 150
			anchors.fill: undefined
			Layout.maximumHeight: 250
			Layout.minimumHeight: 150

			onSigOpenClient: {
				console.log("item: " + item.ip + ", " + item.style);
				var name=item.style+":"+item.ip;
				var index=pannelBottom.findInfoPannel(name);
				if (-1 === index) {
					console.log("open client: " + name)
					interfaceUi.newDBReader(name);
					pannelBottom.addInfoPanel(name);
					interfaceUi.getTestPlan(name);
				}
				else {
					pannelBottom.activateInfoPannel(index);
				}
			}
		}

		BottomPannel {
			id: pannelBottom
			Layout.fillHeight: true
			Layout.fillWidth: true

			onSigGetTC: {
				console.log("main tlname: " + item.tlname);
				console.log("main name: " + item.name);
				interfaceUi.reqGetTC(item);
				pannelBottom.sigCleanTCList(item);
			}

			onSigFetchTCInfo: {
				console.log("main tlname: " + item.tlname);
				interfaceUi.reqFetchTCInfo(item);
			}

			onSigFetchBuild: {
				console.log("fetch build: " + item.tlname);
				interfaceUi.reqFetchBuild(item);
			}
		}
	}
}
