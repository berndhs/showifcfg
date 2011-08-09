
import Qt 4.7
import net.sf.moui 1.0

Rectangle {
  id: shellDisplay
  color: "yellow"
  property bool holding: false

  ShowShell {
    id: showShell
    onNewData: {
      console.log ("have new data")
      displayText.text = data
    }
  }
  Flickable {
    id: flickArea
    interactive: true
    height: parent.height; width: parent.width
    contentWidth: displayBox.width
    contentHeight: displayBox.height
    Rectangle {
      id: displayBox
      height: displayText.height
      width: displayText.width
      color: Qt.lighter (shellDisplay.color)
      TextEdit {
        id: displayText
        readOnly: true
        text: qsTr ("no data")
      }
    }
  }
  Rectangle {
    id: refreshButton
    width: 96; height: 28
    color: "#77ff77"
    opacity: 0.7
    radius: 0.5*height
    anchors {
      right: parent.right
      top:parent.top
    }
    MouseArea {
      anchors.fill: parent
      onClicked: { 
        showShell.runShell() 
      }
    }
    Text { 
      id: refreshLabel
      anchors.centerIn: parent
      text: qsTr ("Refresh")
    }
  }
  Rectangle {
    id: holdButton
    width: 96; height: 28
    color: "#ff7777"
    opacity: 0.7
    radius: 0.5*height
    anchors {
      right: refreshButton.left
       top: parent.top
    }
    property string holdLabelText: shellDisplay.holding ? qsTr ("Run") : qsTr ("Hold")
    MouseArea {
      anchors.fill: parent
      onClicked: {
        shellDisplay.holding = !shellDisplay.holding
        flickArea.interactive = !shellDisplay.holding
        if (shellDisplay.holding) {
          showShell.stopTimer ()
        } else {
          showShell.startTimer (60)
        }
      }
    }
    Text {
      id: holdLabel
      anchors.centerIn: parent
      text: holdButton.holdLabelText
    }
  }
  Component.onCompleted: {
    showShell.startTimer (60)
  }
}