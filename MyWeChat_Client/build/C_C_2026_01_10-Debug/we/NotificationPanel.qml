import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    title: "系统通知"

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
}
