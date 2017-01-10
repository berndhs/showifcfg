import QtQuick 2.0

Rectangle {
    id: greenBox;
    objectName: "GreenBox";
    width: 100;
    height: 100;
    color: "green";
    x: 0; y: 0;
    Text {
        anchors.centerIn: parent;
        font.pixelSize: 14;
        text: "Please Wait...";
    }

}
