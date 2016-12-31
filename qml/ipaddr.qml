import QtQuick 2.7
import moui.geuzen.utils.static 1.0


/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2011, Bernd Stramm
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/


Rectangle {
    id: mainBox
    objectName: "MainBox";
    color: "transparent"
    property color mainColor: "yellow"
    property int rowHeight: (isProbablyPhone ? 64 : 42)
    property bool isPortrait: isProbablyPhone
    property real mainWidth: isPortrait ? height : width
    property real mainHeight: isPortrait ? width : height
    rotation: isPortrait ? -90 : 0
    GeuzenIpList {
        id: ipList
        objectName: "RealIpList";
        css: ".iplist {font-style:italic;font-size:small; color:white; background-color:green}"
        onFreshData: {
            console.log("fresh data dignal caught in ",ipList);
            var i;
            for (i=0; i<ipList.count(); ++i) {
                ipListView.updateInterface(i);
            }
        }
    }
    Component {
        id: interfaceDelegate

        Column {
            Row {
                id: interfaceRow
                spacing: 4
                Rectangle {
                    id: interfaceNameBox
                    height: rowHeight
                    width: mainWidth * 0.2
                    radius: 0.4 * height
                    color: "green"
                    border.color: Qt.lighter (color, 1.1)
                    border.width: 2
                    Text {
                        id: interfaceNameText
                        anchors { centerIn: parent }
                        text: name
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: { interfaceDetailBox.toggleVisible () }
                    }
                } // Rect

                Rectangle {
                    id: interfaceAttrBox
                    height: rowHeight ;
                    width: mainWidth - interfaceNameBox.width - interfaceRow.spacing
                    radius: 0.1 * height
                    color: Qt.lighter (mainColor)
                    Text {
                        id: interfaceAttrText
                        wrapMode: Text.Wrap
                        width: parent.width
                        font.pixelSize: 14;
                        anchors { centerIn: parent }
                        z : parent.z + 5;
                        text: attributes
                    } // Text
                    MouseArea {
                        anchors.fill: parent
                        onClicked: { interfaceDetailBox.toggleVisible () }
                    }
                } // Rect
            } // Row
            Rectangle {
                id: interfaceDetailBox
                width: mainWidth
                color: Qt.darker (mainColor)
                property bool isHidden:true;
                property int numDetailLines: lines;
                property string detailColor: "white";
                function toggleVisible () {
                    isHidden = !isHidden
//                    ipList.populateText (index, detailTextBrowser, !isHidden)
                }
                Rectangle {
                    id: detailListBox
                    visible: !interfaceDetailBox.isHidden
                    width: parent.width
                    height: (detailTextBrowser.font.pixelSize +3) * interfaceDetailBox. numDetailLines;
                    color: interfaceDetailBox.detailColor;
                    Flickable {
                        flickableDirection: Flickable.HorizontalAndVerticalFlick;
                        Text {
                            id: detailTextBrowser;
                            width: detailListBox.width/2;
                            anchors {
                                right: detailListBox.right;
                            }

                            color: "white"
                            wrapMode: Text.Wrap
                            font.pixelSize: 16;
                            text: attributes;
                        }
                    }
                }

                states: [
                    State {
                        name: "hidden"
                        when: interfaceDetailBox.isHidden
                        PropertyChanges {
                            target: interfaceDetailBox
                            height: 3
                            color: Qt.darker (mainColor)
                        }
                    },
                    State {
                        name: "shown"
                        when: ! interfaceDetailBox.isHidden
                        PropertyChanges {
                            target: interfaceDetailBox
                            height: detailTextBrowser.height
                            detailColor: "blue";
                        }
                    }
                ]
            }
        } // Col
    } // Component
    Rectangle {
        id: refreshBox
        height: rowHeight; width: mainWidth
        property string normalColor: "#f0f0f0";
        property string darkColor: "#f0a0a0";
        color: normalColor;
        anchors { top: mainBox.top; horizontalCenter: mainBox.horizontalCenter }
        Row {
            spacing: 4
            anchors {centerIn:parent}
            Rectangle {
                id: refreshButton
                height: refreshBox.height - 8
                width: refreshBox.width * 0.45
                radius: height * 0.4
                color: refreshBox.normalColor;
                border.color: Qt.lighter (color, 1.1)
                border.width: 2
                Text {
                    anchors.centerIn: parent
                    text: qsTr("Refresh")
                }
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        refreshButton.color = refreshBox.darkColor;
                    }
                    onReleased: {
                        refreshButton.color = refreshBox.normalColor;
                        ipList.read();
//                        lieAboutFresh.start();
                    }
                }
            }
            Rectangle {
                id: doneButton
                height: refreshBox.height - 8
                width: refreshBox.width * 0.45
                radius: height * 0.4
                color: refreshBox.normalColor;
                border.color: Qt.lighter (color, 1.1)
                border.width: 2
                Text {
                    id: doneText;
                    wrapMode: Text.Wrap
                    anchors.centerIn: parent
                    text: qsTr("Done")
                }
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        doneButton.color = refreshBox.darkColor;
                    }
                    onReleased: {
                        doneButton.color = refreshBox.normalColor;
                        doneText.text = "EXITING";
                        Qt.quit();
                    }
                }
            }
        }
    }
    Rectangle {
        id: listBox
        width: mainWidth; height: mainHeight - refreshBox.height
        anchors { top: refreshBox.bottom; horizontalCenter: mainBox.horizontalCenter }
        clip: true
        ListView {
            id: ipListView
            height: mainHeight
            width: mainWidth
            anchors {
                fill: parent
            }
            function updateInterface (ifIndex) {
                console.log("updating interface "+ifIndex);
//                ipList.populateText (ifIndex, ipListView, false)
            }
            delegate: interfaceDelegate
            model: ipList;
        }
    }
    Component.onCompleted: {
        ipList.read ()
    }


}
