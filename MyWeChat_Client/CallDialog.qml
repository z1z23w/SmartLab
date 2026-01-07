import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    width: 300
    height: 400
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    modal: true
    closePolicy: Popup.NoAutoClose // 强制手动操作

    // 状态: outgoing(呼出), incoming(来电), connected(通话中)
    property string callState: "idle"
    property string remoteUser: ""

    signal acceptClicked()
    signal rejectClicked()
    signal hangupClicked()

    background: Rectangle {
        color: "#2b2b2b"
        radius: 12
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 30

        // 头像
        Rectangle {
            width: 100; height: 100
            radius: 50
            color: "#555"
            Label {
                text: root.remoteUser.substring(0, 1).toUpperCase()
                anchors.centerIn: parent
                font.pixelSize: 40; color: "white"
            }
        }

        // 提示文本
        Label {
            text: {
                if (root.callState === "outgoing") return "正在呼叫 " + root.remoteUser + "..."
                if (root.callState === "incoming") return root.remoteUser + " 邀请通话"
                if (root.callState === "connected") return "通话中..."
                return ""
            }
            color: "white"
            font.pixelSize: 18
            Layout.alignment: Qt.AlignHCenter
        }

        // 按钮组
        RowLayout {
            spacing: 40
            Layout.alignment: Qt.AlignHCenter

            // 红色按钮 (挂断/拒绝)
            RoundButton {
                width: 70; height: 70
                text: root.callState === "incoming" ? "拒绝" : "挂断"
                background: Rectangle { radius: 35; color: "#fa5151" }
                contentItem: Label { text: parent.text; color: "white"; anchors.centerIn: parent }
                onClicked: {
                    if (root.callState === "incoming") root.rejectClicked()
                    else root.hangupClicked()
                }
            }

            // 绿色按钮 (接听) - 仅来电显示
            RoundButton {
                visible: root.callState === "incoming"
                width: 70; height: 70
                text: "接听"
                background: Rectangle { radius: 35; color: "#07c160" }
                contentItem: Label { text: parent.text; color: "white"; anchors.centerIn: parent }
                onClicked: root.acceptClicked()
            }
        }
    }
}
