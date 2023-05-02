import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
  id: connectionPage

  header: ChatToolBar {
    Label {
      text: qsTr("Connection")
      font.pixelSize: 24
      anchors.centerIn: parent
    }

    ToolButton {
      id: conversationButton
      text: qsTr("Messages")
      anchors.right: parent.right
      anchors.rightMargin: 10
      anchors.verticalCenter: parent.verticalCenter
      onClicked: connectionPage.StackView.view.push("qrc:/ConversationPage.qml",
                                                    {})
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
          text: qsTr("Contact's IP address")
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
          text: qsTr("Port number")
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
          text: qsTr("Status")
        }

        Label {
          text: socketConnection.status
          font.pixelSize: 18
          color: socketConnection.status === qsTr("Connected") ?
                                             "#4CAF50" :
                                             "#3F51B5"
        }
      }
    }
  }
}
