import QtQuick
import QtQuick.Controls

Rectangle {
    id: root

    // 自适应宽度
    width: Math.min(label.implicitWidth + 40, parent.width * 0.9)
    height: label.implicitHeight + 24
    color: "#333333"
    radius: 8
    opacity: 0 //默认隐藏

    // 居中显示在顶部
    anchors.top: parent.top
    anchors.topMargin: 60
    anchors.horizontalCenter: parent.horizontalCenter

    // 保证在最上层
    z: 100

    property alias text: label.text

    Text {
        id: label
        color: "white"
        font.pixelSize: 14
        anchors.centerIn: parent
        width: parent.width - 30
        wrapMode: Text.Wrap
        horizontalAlignment: Text.AlignHCenter
    }

    function show(msg) {
        text = msg
        anim.restart()
    }

    SequentialAnimation {
        id: anim
        NumberAnimation { target: root; property: "opacity"; to: 0.9; duration: 200 }
        PauseAnimation { duration: 2000 }
        NumberAnimation { target: root; property: "opacity"; to: 0; duration: 500 }
    }
}
