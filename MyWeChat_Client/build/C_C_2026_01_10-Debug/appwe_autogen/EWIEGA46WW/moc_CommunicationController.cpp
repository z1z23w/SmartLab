/****************************************************************************
** Meta object code from reading C++ file 'CommunicationController.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../CommunicationController.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CommunicationController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN23CommunicationControllerE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN23CommunicationControllerE = QtMocHelpers::stringData(
    "CommunicationController",
    "QML.Element",
    "auto",
    "friendListChanged",
    "",
    "connectionStatusChanged",
    "messageReceived",
    "from",
    "content",
    "notificationTriggered",
    "title",
    "msg",
    "historyLoaded",
    "friendName",
    "history",
    "incomingCall",
    "fromUser",
    "mode",
    "callAccepted",
    "callRejected",
    "callEnded",
    "onReadyRead",
    "onConnected",
    "connectToServer",
    "ip",
    "login",
    "u",
    "p",
    "registerUser",
    "sendMessage",
    "requestCall",
    "targetUser",
    "acceptCall",
    "rejectCall",
    "endCall",
    "setRemoteVideoItem",
    "VideoItem*",
    "item",
    "selectFriend",
    "name",
    "searchUser",
    "keyword",
    "addFriend",
    "getHistory",
    "clearUnread",
    "currentUser",
    "Netizen*",
    "currentSession",
    "ChatSession*",
    "friendList",
    "isConnected"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN23CommunicationControllerE[] = {

 // content:
      12,       // revision
       0,       // classname
       1,   14, // classinfo
      25,   16, // methods
       4,  237, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,  166,    4, 0x06,    5 /* Public */,
       5,    0,  167,    4, 0x06,    6 /* Public */,
       6,    2,  168,    4, 0x06,    7 /* Public */,
       9,    2,  173,    4, 0x06,   10 /* Public */,
      12,    2,  178,    4, 0x06,   13 /* Public */,
      15,    2,  183,    4, 0x06,   16 /* Public */,
      18,    1,  188,    4, 0x06,   19 /* Public */,
      19,    0,  191,    4, 0x06,   21 /* Public */,
      20,    0,  192,    4, 0x06,   22 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      21,    0,  193,    4, 0x08,   23 /* Private */,
      22,    0,  194,    4, 0x08,   24 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      23,    1,  195,    4, 0x02,   25 /* Public */,
      25,    2,  198,    4, 0x02,   27 /* Public */,
      28,    2,  203,    4, 0x02,   30 /* Public */,
      29,    1,  208,    4, 0x02,   33 /* Public */,
      30,    2,  211,    4, 0x02,   35 /* Public */,
      32,    0,  216,    4, 0x02,   38 /* Public */,
      33,    0,  217,    4, 0x02,   39 /* Public */,
      34,    0,  218,    4, 0x02,   40 /* Public */,
      35,    1,  219,    4, 0x02,   41 /* Public */,
      38,    1,  222,    4, 0x02,   43 /* Public */,
      40,    1,  225,    4, 0x02,   45 /* Public */,
      42,    1,  228,    4, 0x02,   47 /* Public */,
      43,    1,  231,    4, 0x02,   49 /* Public */,
      44,    1,  234,    4, 0x02,   51 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   10,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonArray,   13,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   16,   17,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   26,   27,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   26,   27,
    QMetaType::QString, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   31,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 36,   37,
    QMetaType::Void, QMetaType::QString,   39,
    QMetaType::Void, QMetaType::QString,   41,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   13,

 // properties: name, type, flags, notifyId, revision
      45, 0x80000000 | 46, 0x00015409, uint(-1), 0,
      47, 0x80000000 | 48, 0x00015409, uint(-1), 0,
      49, QMetaType::QJsonArray, 0x00015001, uint(0), 0,
      50, QMetaType::Bool, 0x00015001, uint(1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject CommunicationController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN23CommunicationControllerE.offsetsAndSizes,
    qt_meta_data_ZN23CommunicationControllerE,
    qt_static_metacall,
    nullptr,
    qt_metaTypeArray<
        // property 'currentUser'
        Netizen*,
        // property 'currentSession'
        ChatSession*,
        // property 'friendList'
        QJsonArray,
        // property 'isConnected'
        bool,
        // Q_OBJECT / Q_GADGET
        CommunicationController,
        // method 'friendListChanged'
        void,
        // method 'connectionStatusChanged'
        void,
        // method 'messageReceived'
        void,
        QString,
        QString,
        // method 'notificationTriggered'
        void,
        QString,
        QString,
        // method 'historyLoaded'
        void,
        QString,
        QJsonArray,
        // method 'incomingCall'
        void,
        QString,
        QString,
        // method 'callAccepted'
        void,
        QString,
        // method 'callRejected'
        void,
        // method 'callEnded'
        void,
        // method 'onReadyRead'
        void,
        // method 'onConnected'
        void,
        // method 'connectToServer'
        void,
        QString,
        // method 'login'
        void,
        QString,
        QString,
        // method 'registerUser'
        void,
        QString,
        QString,
        // method 'sendMessage'
        QString,
        QString,
        // method 'requestCall'
        void,
        QString,
        QString,
        // method 'acceptCall'
        void,
        // method 'rejectCall'
        void,
        // method 'endCall'
        void,
        // method 'setRemoteVideoItem'
        void,
        VideoItem *,
        // method 'selectFriend'
        void,
        QString,
        // method 'searchUser'
        void,
        QString,
        // method 'addFriend'
        void,
        QString,
        // method 'getHistory'
        void,
        QString,
        // method 'clearUnread'
        void,
        QString
    >,
    nullptr
} };

void CommunicationController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CommunicationController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->friendListChanged(); break;
        case 1: _t->connectionStatusChanged(); break;
        case 2: _t->messageReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 3: _t->notificationTriggered((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->historyLoaded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[2]))); break;
        case 5: _t->incomingCall((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 6: _t->callAccepted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->callRejected(); break;
        case 8: _t->callEnded(); break;
        case 9: _t->onReadyRead(); break;
        case 10: _t->onConnected(); break;
        case 11: _t->connectToServer((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->login((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 13: _t->registerUser((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 14: { QString _r = _t->sendMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 15: _t->requestCall((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 16: _t->acceptCall(); break;
        case 17: _t->rejectCall(); break;
        case 18: _t->endCall(); break;
        case 19: _t->setRemoteVideoItem((*reinterpret_cast< std::add_pointer_t<VideoItem*>>(_a[1]))); break;
        case 20: _t->selectFriend((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 21: _t->searchUser((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 22: _t->addFriend((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 23: _t->getHistory((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 24: _t->clearUnread((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 19:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< VideoItem* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (CommunicationController::*)();
            if (_q_method_type _q_method = &CommunicationController::friendListChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (CommunicationController::*)();
            if (_q_method_type _q_method = &CommunicationController::connectionStatusChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (CommunicationController::*)(QString , QString );
            if (_q_method_type _q_method = &CommunicationController::messageReceived; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (CommunicationController::*)(QString , QString );
            if (_q_method_type _q_method = &CommunicationController::notificationTriggered; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (CommunicationController::*)(QString , QJsonArray );
            if (_q_method_type _q_method = &CommunicationController::historyLoaded; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (CommunicationController::*)(QString , QString );
            if (_q_method_type _q_method = &CommunicationController::incomingCall; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _q_method_type = void (CommunicationController::*)(QString );
            if (_q_method_type _q_method = &CommunicationController::callAccepted; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _q_method_type = void (CommunicationController::*)();
            if (_q_method_type _q_method = &CommunicationController::callRejected; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _q_method_type = void (CommunicationController::*)();
            if (_q_method_type _q_method = &CommunicationController::callEnded; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ChatSession* >(); break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Netizen* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Netizen**>(_v) = _t->currentUser(); break;
        case 1: *reinterpret_cast< ChatSession**>(_v) = _t->currentSession(); break;
        case 2: *reinterpret_cast< QJsonArray*>(_v) = _t->friendList(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->isConnected(); break;
        default: break;
        }
    }
}

const QMetaObject *CommunicationController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommunicationController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN23CommunicationControllerE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CommunicationController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CommunicationController::friendListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CommunicationController::connectionStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CommunicationController::messageReceived(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CommunicationController::notificationTriggered(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CommunicationController::historyLoaded(QString _t1, QJsonArray _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CommunicationController::incomingCall(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CommunicationController::callAccepted(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CommunicationController::callRejected()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void CommunicationController::callEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}
QT_WARNING_POP
