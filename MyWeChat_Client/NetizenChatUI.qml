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
            RowLayout {
                anchors.fill: parent
                ToolButton { text: "通信录"; onClicked: swipe.currentIndex = 0 }
                Label {
                    text: swipe.currentIndex === 0 ? "联系人" : controller.currentSession.currentTarget
                    color: "white"; font.bold: true; Layout.alignment: Qt.AlignCenter
                }
                ToolButton { text: "+"; visible: swipe.currentIndex===0; onClicked: searchDialog.open() }
            }
        }

        SwipeView {
            id: swipe
            anchors.fill: parent
            interactive: false

            //联系人列表
            ListView {
                model: controller.friendList
                delegate: ItemDelegate {
                    width: parent.width
                    text: modelData.username + (modelData.unread ? " 🔴" : "")
                    onClicked: {
                        controller.selectFriend(modelData.username)
                        controller.clearUnread(modelData.username)
                        chatModel.clear()
                        controller.getHistory(modelData.username)
                        swipe.currentIndex = 1
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
