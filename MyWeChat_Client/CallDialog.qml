import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import we 1.0

Dialog {
    id: root
    width: 360
    height: 640
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    modal: true
    closePolicy: Popup.NoAutoClose

    property string callMode: "voice"
    property string callState: "idle"
    property string remoteUser: ""
    property alias videoItem: remoteVideo

    signal acceptClicked()
    signal rejectClicked()
    signal hangupClicked()

    background: Rectangle { color: "black" }

    onClosed: {
        videoItem.visible = false
    }

    contentItem: Item {
        anchors.fill: parent

        VideoItem {
            id: remoteVideo
            anchors.fill: parent
            visible: root.callMode === "video" && root.callState === "connected"
        }

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 20
            //只要视频没显示，就显示头像
            visible: !remoteVideo.visible

            Rectangle {
                width: 100; height: 100
                radius: 50; color: "#555"
                Label {
                    text: root.remoteUser.substring(0, 1).toUpperCase()
                    anchors.centerIn: parent
                    font.pixelSize: 40; color: "white"
                }
            }
            Label {
                text: {
                    var typeStr = (root.callMode === "video") ? "视频通话" : "语音通话"
                    if (root.callState === "outgoing") return "正在呼叫 " + root.remoteUser
                    if (root.callState === "incoming") return root.remoteUser + " 邀请通话"
                    if (root.callState === "connected") return "通话中..."
                    return ""
                }
                color: "white"
                font.pixelSize: 18
            }
        }

        RowLayout {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 50
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 60

            RoundButton {
                width: 70; height: 70
                text: "挂断"
                background: Rectangle { radius: 35; color: "#fa5151" }
                contentItem: Label { text: parent.text; color: "white"; anchors.centerIn: parent }
                onClicked: {
                    if (root.callState === "incoming") root.rejectClicked()
                    else root.hangupClicked()
                }
            }

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
