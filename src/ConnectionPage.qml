import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0

Page {
  id: connectionPage

  header: ChatToolBar {
    Label {
      text: qsTr("Connection Setup")
      font.pixelSize: 24
      anchors.centerIn: parent
    }

    ToolButton {
      id: conversationButton
      text: qsTr("Conversation")
      anchors.right: parent.right
      anchors.rightMargin: 10
      anchors.verticalCenter: parent.verticalCenter
      onClicked: connectionPage.StackView.view.push("qrc:/ConversationPage.qml", {})
    }
  }

  ColumnLayout {
    anchors.fill: parent
    anchors.topMargin: 0
    anchors.bottomMargin: 30
    anchors.leftMargin: 30

    ColumnLayout {
      spacing: 45

      RowLayout {
        spacing: 15

        Label {
          id: ipLabel
          font.pixelSize: 18
          text: qsTr("Your contact's IP address")
        }

        TextField {
          id: ip
          font.pixelSize: 18
          onEditingFinished: window.ip(ip.text)
        }
      }

      RowLayout {
        spacing: 15

        Label {
          id: portLabel
          font.pixelSize: 18
          text: qsTr("Optional: Port number")
        }

        TextField {
          id: port
          font.pixelSize: 18
          placeholderText: qsTr("28710")
          onEditingFinished: window.port(port.text)
        }
      }

      RowLayout {
        spacing: 15

        Label {
          id: connectionLabel
          font.pixelSize: 18
          text: qsTr("Connection status")
        }

        Label {
          text: socketConnection.status
          font.pixelSize: 18
          color: socketConnection.status === "Connected" ? "#4CAF50" : "#3F51B5"
        }
      }
    }
  }
}