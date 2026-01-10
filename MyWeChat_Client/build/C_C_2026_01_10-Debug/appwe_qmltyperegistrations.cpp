/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#if __has_include(<AudioManager.h>)
#  include <AudioManager.h>
#endif
#if __has_include(<ChatSession.h>)
#  include <ChatSession.h>
#endif
#if __has_include(<CommunicationController.h>)
#  include <CommunicationController.h>
#endif
#if __has_include(<ComplianceController.h>)
#  include <ComplianceController.h>
#endif
#if __has_include(<MediaSessionController.h>)
#  include <MediaSessionController.h>
#endif
#if __has_include(<Netizen.h>)
#  include <Netizen.h>
#endif
#if __has_include(<VideoItem.h>)
#  include <VideoItem.h>
#endif


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_we()
{
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    qmlRegisterTypesAndRevisions<AudioManager>("we", 1);
    qmlRegisterTypesAndRevisions<ChatSession>("we", 1);
    qmlRegisterTypesAndRevisions<CommunicationController>("we", 1);
    qmlRegisterTypesAndRevisions<ComplianceController>("we", 1);
    qmlRegisterTypesAndRevisions<MediaSessionController>("we", 1);
    qmlRegisterTypesAndRevisions<Netizen>("we", 1);
    qmlRegisterTypesAndRevisions<VideoDecoder>("we", 1);
    qmlRegisterTypesAndRevisions<VideoEncoder>("we", 1);
    qmlRegisterTypesAndRevisions<VideoItem>("we", 1);
    qmlRegisterAnonymousType<QQuickItem, 254>("we", 1);
    QT_WARNING_POP
    qmlRegisterModule("we", 1, 0);
}

static const QQmlModuleRegistration weRegistration("we", qml_register_types_we);
