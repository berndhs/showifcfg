
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

import Qt 4.7
import net.sf.moui 1.0

Rectangle {
  id: shellDisplay
  color: "#f0fafa"
  property bool holding: false

  ShowShell {
    id: showShell
    onNewData: {
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
      color: shellDisplay.color
      TextEdit {
        id: displayText
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
    property string holdLabelText: shellDisplay.holding ? 
                                    qsTr ("Run") : qsTr ("Hold")
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
