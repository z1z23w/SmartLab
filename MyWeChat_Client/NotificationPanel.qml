import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: root
    title: "系统通知"
    width: 320
    height: 180
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    modal: true
    standardButtons: Dialog.Ok

    function show(t, m) {
        root.title = t
        msgLabel.text = m
        root.open()
    }

    contentItem: ColumnLayout {
        Label {
            id: msgLabel
            text: ""
            wrapMode: Text.Wrap
            Layout.maximumWidth: 300
            Layout.alignment: Qt.AlignHCenter
            padding: 20
        }
    }

    background: Rectangle {
        color: "white"
        border.color: "#eeeeee"
        border.width: 1
        radius: 8
    }
}
