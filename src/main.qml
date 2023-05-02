import QtQuick
import QtQuick.Controls
import Qt.labs.settings 1.0

ApplicationWindow {
  id: window
  visible: true
  title: qsTr("Dialogue")
  width: 800
  height: 600

  signal message(string msg)
  signal ip(string ip)
  signal port(int port)

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
