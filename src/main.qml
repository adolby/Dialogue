import QtQuick 2.7
import QtQuick.Controls 2.1
import Qt.labs.settings 1.0

ApplicationWindow {
  property string status: "Not connected"

  signal message(string msg)
  signal ip(string ip)
  signal port(int port)

  function updateStatus(status) {
    window.status = status;
  }

  id: window
  visible: true
  title: qsTr("Dialogue")
  width: 800
  height: 600

  Settings {
    property alias x: window.x
    property alias y: window.y
    property alias width: window.width
    property alias height: window.height
  }

  StackView {
    id: stackView
    anchors.fill: parent
    initialItem: ConnectionPage {}
  }
}
