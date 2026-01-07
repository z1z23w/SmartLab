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

    // --- Control ---
    CommunicationController {
        id: controller
        onNotificationTriggered: (title, msg) => notifPanel.show(title, msg)

        // 【核心修改】收到消息的处理逻辑
        onMessageReceived: (from, content) => {
            // 情况A: 正在和这个人聊天 -> 直接显示在屏幕上
            if (controller.currentSession.currentTarget === from) {
                chatModel.append({"sender": "other", "text": content})
                chatView.positionViewAtEnd()
            }
            // 情况B: 没在和他聊天 -> 顶部弹出提示框
            else {
                toast.show("收到 " + from + " 的新消息: " + content)
            }
        }

        onHistoryLoaded: (friendName, history) => {
            if (controller.currentSession.currentTarget === friendName) {
                chatModel.clear()
                for(var i=0; i<history.length; i++) {
                    var msg = history[i]
                    if (msg.type === "msg") {
                        chatModel.append({"sender": msg.sender, "text": msg.text})
                    }
                }
                chatView.positionViewAtEnd()
            }
        }
    }

    // --- Boundary Components ---
    NotificationPanel { id: notifPanel }
    Toast { id: toast } // 【新增】实例化 Toast

    // --- Login View ---
    ColumnLayout {
        anchors.centerIn: parent
        visible: controller.currentUser.status !== "Online"
        spacing: 15
        width: 300

        TextField { id: ipField; text: "127.0.0.1"; placeholderText: "IP"; Layout.fillWidth: true }
        TextField { id: userField; placeholderText: "账号"; Layout.fillWidth: true }
        TextField { id: passField; placeholderText: "密码"; echoMode: TextInput.Password; Layout.fillWidth: true }

        RowLayout {
            Button {
                text: "登录"
                Layout.fillWidth: true
                onClicked: {
                    controller.connectToServer(ipField.text)
                    controller.login(userField.text, passField.text)
                }
            }
            Button {
                text: "注册"
                Layout.fillWidth: true
                onClicked: {
                    controller.connectToServer(ipField.text)
                    controller.registerUser(userField.text, passField.text)
                }
            }
        }
    }

    // --- Main Chat View ---
    Page {
        anchors.fill: parent
        visible: controller.currentUser.status === "Online"

        header: ToolBar {
            RowLayout {
                anchors.fill: parent
                ToolButton { text: "通信录"; onClicked: swipe.currentIndex = 0 }
                Label {
                    text: swipe.currentIndex === 0 ? "联系人" : controller.currentSession.currentTarget
                    color: "white"; font.bold: true
                    Layout.alignment: Qt.AlignCenter
                }
                ToolButton { text: "+"; visible: swipe.currentIndex===0; onClicked: searchDialog.open() }
            }
        }

        SwipeView {
            id: swipe
            anchors.fill: parent
            interactive: false

            // 1. 联系人列表
            ListView {
                model: controller.friendList
                delegate: ItemDelegate {
                    width: parent.width
                    text: modelData.username + (modelData.unread ? " 🔴" : "")

                    onClicked: {
                        controller.selectFriend(modelData.username)
                        controller.clearUnread(modelData.username) // 清红点
                        chatModel.clear()
                        controller.getHistory(modelData.username)  // 加载历史
                        swipe.currentIndex = 1
                    }
                }
            }

            // 2. 聊天窗口
            ColumnLayout {
                ListView {
                    id: chatView
                    Layout.fillWidth: true; Layout.fillHeight: true
                    model: ListModel { id: chatModel }
                    clip: true
                    delegate: Row {
                        width: chatView.width
                        layoutDirection: model.sender==="me"?Qt.RightToLeft:Qt.LeftToRight
                        spacing: 10; padding: 10
                        Rectangle { width: 35; height: 35; color: "gray"; radius: 4 }
                        Rectangle {
                            width: Math.min(Math.max(txt.implicitWidth+20, 40), chatView.width*0.7)
                            height: txt.implicitHeight+20
                            color: model.sender==="me"?"#95ec69":"white"; radius: 5
                            Text {
                                id: txt; text: model.text;
                                anchors.centerIn: parent; width: parent.width-20; wrapMode: Text.WrapAnywhere
                            }
                        }
                    }
                }

                RowLayout {
                    TextField { id: input; Layout.fillWidth: true }
                    Button {
                        text: "按住说话"
                        onPressed: controller.startMediaSession()
                        onReleased: controller.endMediaSession()
                    }
                    Button {
                        text: "发送"
                        onClicked: {
                            var safeText = controller.sendMessage(input.text)
                            if (safeText !== "") {
                                chatModel.append({"sender": "me", "text": safeText})
                                input.text = ""
                                chatView.positionViewAtEnd()
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
            Button {
                text: "搜索"
                onClicked: controller.searchUser(sInput.text)
            }
            Button {
                text: "添加"
                onClicked: { controller.addFriend(sInput.text); searchDialog.close() }
            }
        }
    }
}
