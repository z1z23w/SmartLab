import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import we 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 400
    height: 700
    title: "WeChat Pro"
    Material.theme: Material.Light
    Material.accent: "#07c160"

    //Control
    CommunicationController {
        id: controller
        onNotificationTriggered: (title, msg) => notifPanel.show(title, msg)

        onMessageReceived: (from, content) => {
            if (controller.currentSession.currentTarget === from) {
                chatModel.append({"sender": "other", "text": content})
                chatView.positionViewAtEnd()
            } else {
                toast.show("收到 " + from + " 的消息: " + content)
            }
        }

        onHistoryLoaded: (friendName, history) => {
            if (controller.currentSession.currentTarget === friendName) {
                chatModel.clear()
                for(var i=0; i<history.length; i++) {
                    var msg = history[i]
                    if (msg.type === "msg") chatModel.append({"sender": msg.sender, "text": msg.text})
                }
                chatView.positionViewAtEnd()
            }
        }

        //呼叫信号
        onIncomingCall: (from, mode) => {
            console.log("UI Incoming Call: " + from + " mode: " + mode)
            callDialog.callState = "incoming"
            callDialog.remoteUser = from
            callDialog.callMode = mode
            callDialog.open()
        }
        onCallAccepted: (mode) => {
            console.log("UI Call Connected")
            callDialog.callState = "connected"
        }
        onCallRejected: () => {
            toast.show("对方拒绝通话")
            callDialog.close()
        }
        onCallEnded: () => {
            toast.show("通话结束")
            callDialog.close()
        }
    }

    NotificationPanel { id: notifPanel }
    Toast { id: toast }

    CallDialog {
        id: callDialog
        onOpened: controller.setRemoteVideoItem(callDialog.videoItem)

        //接听时强制更新状态，确保视频画面显示
        onAcceptClicked: {
            controller.acceptCall()
            callDialog.callState = "connected"
        }

        onRejectClicked: { controller.rejectCall(); callDialog.close() }
        onHangupClicked: { controller.endCall(); callDialog.close() }
    }

    //登录界面
    ColumnLayout {
        anchors.centerIn: parent
        visible: controller.currentUser.status !== "Online"
        spacing: 15
        width: 300
        TextField { id: ipField; text: "127.0.0.1"; placeholderText: "IP"; Layout.fillWidth: true }
        TextField { id: userField; placeholderText: "账号"; Layout.fillWidth: true }
        TextField { id: passField; placeholderText: "密码"; echoMode: TextInput.Password; Layout.fillWidth: true }
        RowLayout {
            Button { text: "登录"; Layout.fillWidth: true; onClicked: { controller.connectToServer(ipField.text); controller.login(userField.text, passField.text) } }
            Button { text: "注册"; Layout.fillWidth: true; onClicked: { controller.connectToServer(ipField.text); controller.registerUser(userField.text, passField.text) } }
        }
    }

    //主界面
    Page {
        anchors.fill: parent
        visible: controller.currentUser.status === "Online"

        header: ToolBar {
            background: Rectangle { color: Material.accent }

            // 修复：重新组织导航栏布局，确保标题居中
            RowLayout {
                anchors.fill: parent

                // 左侧按钮容器（占固定宽度，保证标题居中）
                Item {
                    Layout.preferredWidth: 40
                    Layout.preferredHeight: 40
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

                    ToolButton {
                        id: backButton
                        visible: swipe.currentIndex === 1
                        text: "←"
                        font.pixelSize: 20
                        onClicked: {
                            swipe.currentIndex = 0
                            controller.clearCurrentSession()
                        }
                        background: Rectangle {
                            color: "transparent"
                            radius: 20
                        }
                        contentItem: Label {
                            text: parent.text
                            color: "white"
                            anchors.centerIn: parent
                        }
                    }
                    ToolButton {
                        visible: swipe.currentIndex === 0
                        text: "通信录"
                        onClicked: swipe.currentIndex = 0
                        contentItem: Label {
                            text: parent.text
                            color: "white"
                            font.bold: true
                        }
                    }
                }

                Label {
                    text: swipe.currentIndex === 0 ? "联系人" : controller.currentSession.currentTarget
                    color: "white"
                    font.bold: true
                    Layout.fillWidth: true  // 填充剩余宽度
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter  // 水平+垂直居中
                    horizontalAlignment: Text.AlignHCenter  // 文字自身也居中
                }

                // 右侧按钮容器（占固定宽度，保证标题居中）
                Item {
                    Layout.preferredWidth: 40
                    Layout.preferredHeight: 40
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                    ToolButton {
                        text: "+"
                        visible: swipe.currentIndex === 0
                        onClicked: searchDialog.open()

                        Layout.alignment: Qt.AlignCenter  // 按钮在容器内居中
                        background: Rectangle {
                            color: "transparent"
                            radius: 20
                        }
                        contentItem: Label {
                            text: parent.text
                            color: "white"
                            font.pixelSize: 20
                            anchors.centerIn: parent
                        }
                    }
                }
            }
        }

        SwipeView {
            id: swipe
            anchors.fill: parent
            interactive: false

            ListView {
                id: friendListView
                model: controller.friendList
                spacing: 0

                delegate: Item {
                    width: parent.width
                    height: 70
                    // 点击区域
                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        onClicked: {
                            controller.selectFriend(modelData.username)
                            // 移除：controller.clearUnread(modelData.username)
                            chatModel.clear()
                            controller.getHistory(modelData.username)
                            swipe.currentIndex = 1
                        }
                        // 悬停效果
                        hoverEnabled: true
                    }

                    // 背景色（包含悬停/选中状态）
                    Rectangle {
                        anchors.fill: parent
                        color: {
                            if (mouseArea.containsMouse) return "#f0f0f0"  // 悬停色
                            else if (controller.currentSession.currentTarget === modelData.username) return "#e6f7ea"  // 选中色
                            else return "white"  // 默认色
                        }
                        // 底部分割线
                        Rectangle {
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 1
                            color: "#eeeeee"
                        }
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        // 头像
                        Rectangle {
                            width: 50
                            height: 50
                            radius: 25  // 圆形头像
                            color: "#07c160"  // 微信绿作为默认头像背景
                            Label {
                                anchors.centerIn: parent
                                text: modelData.username.substring(0,1)  // 取用户名第一个字作为头像文字
                                color: "white"
                                font.pixelSize: 20
                                font.bold: true
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter

                            RowLayout {
                                Layout.fillWidth: true

                                Label {
                                    text: modelData.username
                                    font.pixelSize: 16
                                    font.bold: true
                                    color: "#333333"
                                }

                                Item { Layout.fillWidth: true }

                            }

                        }
                    }
                }
            }

            //聊天窗口
            ColumnLayout {
                ListView {
                    id: chatView
                    Layout.fillWidth: true; Layout.fillHeight: true
                    model: ListModel { id: chatModel }
                    clip: true
                    delegate: Row {
                        width: chatView.width; spacing: 10; padding: 10
                        layoutDirection: model.sender==="me"?Qt.RightToLeft:Qt.LeftToRight
                        Rectangle { width: 35; height: 35; color: "gray"; radius: 4 }
                        Rectangle {
                            width: Math.min(Math.max(txt.implicitWidth+20, 40), chatView.width*0.7)
                            height: txt.implicitHeight+20; color: model.sender==="me"?"#95ec69":"white"; radius: 5
                            Text { id: txt; text: model.text; anchors.centerIn: parent; width: parent.width-20; wrapMode: Text.WrapAnywhere }
                        }
                    }
                }

                RowLayout {
                    TextField { id: input; Layout.fillWidth: true }

                    Button {
                        text: "📞"
                        onClicked: {
                            var target = controller.currentSession.currentTarget
                            if (target === "") return
                            callDialog.callMode = "voice"
                            callDialog.callState = "outgoing"
                            callDialog.remoteUser = target
                            callDialog.open()
                            controller.requestCall(target, "voice")
                        }
                    }

                    Button {
                        text: "📹"
                        onClicked: {
                            var target = controller.currentSession.currentTarget
                            if (target === "") return
                            callDialog.callMode = "video"
                            callDialog.callState = "outgoing"
                            callDialog.remoteUser = target
                            callDialog.open()
                            controller.requestCall(target, "video")
                        }
                    }

                    Button {
                        text: "发送"
                        onClicked: {
                            var safeText = controller.sendMessage(input.text)
                            if (safeText !== "") {
                                chatModel.append({"sender": "me", "text": safeText})
                                input.text = ""; chatView.positionViewAtEnd()
                            }
                        }
                    }
                }
            }
        }
    }

    Dialog {
        id: searchDialog
        title: "添加好友"
        x: (parent.width-width)/2; y: (parent.height-height)/2
        modal: true
        standardButtons: Dialog.Cancel
        ColumnLayout {
            TextField { id: sInput; placeholderText: "用户名" }
            Button { text: "搜索"; onClicked: controller.searchUser(sInput.text) }
            Button { text: "添加"; onClicked: { controller.addFriend(sInput.text); searchDialog.close() } }
        }
    }
}
