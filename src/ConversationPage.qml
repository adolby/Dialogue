import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
  id: conversationPage

  header: ChatToolBar {
    ToolButton {
      text: qsTr("Back")
      anchors.left: parent.left
      anchors.leftMargin: 10
      anchors.verticalCenter: parent.verticalCenter
      onClicked: conversationPage.StackView.view.pop()
    }

    Label {
      id: pageTitle
      text: qsTr("Conversation")
      font.pixelSize: 24
      anchors.centerIn: parent
    }
  }

  ColumnLayout {
    anchors.fill: parent

    ListView {
      id: listView
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.margins: inputPane.leftPadding + messageField.leftPadding
      displayMarginBeginning: 40
      displayMarginEnd: 40
      verticalLayoutDirection: ListView.BottomToTop
      spacing: 12
      model: messageModel
      delegate: Column {
        anchors.right: sentByMe ? parent.right : undefined
        spacing: 6

        readonly property bool sentByMe: model.recipient !== qsTr("Me")

        Row {
          id: messageRow
          spacing: 6
          anchors.right: sentByMe ? parent.right : undefined

          Rectangle {
            width: Math.min(messageLabel.implicitWidth + 24,
                            listView.width - 6)
            height: messageLabel.implicitHeight + 24
            radius: 12
            color: sentByMe ? "#3F51B5" : "#FF9800"

            Label {
              id: messageLabel
              font.pixelSize: 18
              text: model.messageText
              color: sentByMe ? "white" : "white"
              anchors.fill: parent
              anchors.margins: 12
              wrapMode: Label.Wrap
            }
          }
        }

        Label {
          id: timestampText
          text: Qt.formatDateTime(model.timestamp, "MMM d, hh:mm")
          color: "#3F51B5"
          anchors.right: sentByMe ? parent.right : undefined
        }
      }

      ScrollBar.vertical: ScrollBar {}
    }

    Pane {
      id: inputPane
      Layout.fillWidth: true

      RowLayout {
        width: parent.width

        TextArea {
          id: messageField
          font.pixelSize: 18
          Layout.fillWidth: true
          wrapMode: TextArea.Wrap
        }

        Button {
          id: sendButton
          text: qsTr("Send")
          font.pixelSize: 18
          onClicked: {
            if (socketConnection.status === qsTr("Connected") &&
                messageField.length > 0) {
              window.message(messageField.text);
              messageField.clear();
            }
          }
        }
      }
    }
  }
}
