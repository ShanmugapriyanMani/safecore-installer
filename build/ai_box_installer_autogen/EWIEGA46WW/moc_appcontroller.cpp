/****************************************************************************
** Meta object code from reading C++ file 'appcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../appcontroller.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'appcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
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
struct qt_meta_tag_ZN13AppControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto AppController::qt_create_metaobjectdata<qt_meta_tag_ZN13AppControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AppController",
        "installPathChanged",
        "",
        "serviceBaseUrlChanged",
        "macIdChanged",
        "tenantIdChanged",
        "registrationKeyChanged",
        "registrationGeneratedOnChanged",
        "registrationAttemptedChanged",
        "registrationErrorChanged",
        "registrationMessageChanged",
        "relayUrlChanged",
        "relayErrorChanged",
        "registrationResult",
        "ok",
        "registrationKey",
        "generatedOn",
        "message",
        "syncBusyChanged",
        "syncResult",
        "tenantBusyChanged",
        "tenantMessageChanged",
        "tenantSuccessChanged",
        "dockerPullLogChanged",
        "dockerPullProgressChanged",
        "dockerPullActiveChanged",
        "installPrereqsLogChanged",
        "installPrereqsRunningChanged",
        "installPrereqsDoneChanged",
        "dockerPullStarted",
        "dockerPullFinished",
        "dockerRunFinished",
        "dockerOpsLogChanged",
        "dockerOpsFollowLogChanged",
        "dockerOpsRunningChanged",
        "dockerOpsStartingChanged",
        "dockerOpsStoppingChanged",
        "dockerOpsConflictChanged",
        "dockerOpsContainerIdChanged",
        "dockerOpsAutoRunChanged",
        "dockerOpsFinished",
        "dockerOpsStopped",
        "upgradeLogChanged",
        "upgradeRunningChanged",
        "upgradeProgressChanged",
        "upgradeFinished",
        "hasUpdate",
        "currentStepChanged",
        "stepProgressChanged",
        "statusTextChanged",
        "busyChanged",
        "keyValidChanged",
        "onKeyGenerated",
        "runStep1_Docker",
        "runStep2_Env",
        "runStep3_Model",
        "onTaskProgress",
        "p",
        "onTaskFinished",
        "msg",
        "validateKey",
        "populateMacId",
        "syncRelay",
        "fetchTenantData",
        "vertical",
        "pullDockerImage",
        "cancelDockerPull",
        "clearDockerPullLog",
        "startInstallPrereqs",
        "cancelInstallPrereqs",
        "clearInstallPrereqsLog",
        "runDockerContainer",
        "runDockerOps",
        "restartDockerOps",
        "stopDockerOps",
        "startDockerOpsLogs",
        "stopDockerOpsLogs",
        "installDockerService",
        "startNow",
        "launchDockerOpsApp",
        "autoRun",
        "handleSigInt",
        "startInstall",
        "cancel",
        "copyInstallPath",
        "goToStep",
        "step",
        "forceStep",
        "startUpgrade",
        "cancelUpgrade",
        "installPath",
        "serviceBaseUrl",
        "macId",
        "tenantId",
        "registrationGeneratedOn",
        "registrationAttempted",
        "registrationError",
        "registrationMessage",
        "relayUrl",
        "relayError",
        "syncBusy",
        "tenantBusy",
        "tenantMessage",
        "tenantSuccess",
        "dockerPullLog",
        "dockerPullProgress",
        "dockerPullActive",
        "installPrereqsLog",
        "installPrereqsRunning",
        "installPrereqsDone",
        "dockerOpsLog",
        "dockerOpsFollowLog",
        "dockerImage",
        "dockerOpsRunning",
        "dockerOpsStarting",
        "dockerOpsStopping",
        "dockerOpsConflict",
        "dockerOpsContainerId",
        "dockerOpsAutoRun",
        "upgradeLog",
        "upgradeRunning",
        "upgradeProgress",
        "currentStep",
        "stepProgress",
        "statusText",
        "busy",
        "keyValid"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'installPathChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'serviceBaseUrlChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'macIdChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'tenantIdChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'registrationKeyChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'registrationGeneratedOnChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'registrationAttemptedChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'registrationErrorChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'registrationMessageChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'relayUrlChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'relayErrorChanged'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'registrationResult'
        QtMocHelpers::SignalData<void(bool, const QString &, const QString &, const QString &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 14 }, { QMetaType::QString, 15 }, { QMetaType::QString, 16 }, { QMetaType::QString, 17 },
        }}),
        // Signal 'syncBusyChanged'
        QtMocHelpers::SignalData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'syncResult'
        QtMocHelpers::SignalData<void(bool, const QString &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 14 }, { QMetaType::QString, 17 },
        }}),
        // Signal 'tenantBusyChanged'
        QtMocHelpers::SignalData<void()>(20, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'tenantMessageChanged'
        QtMocHelpers::SignalData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'tenantSuccessChanged'
        QtMocHelpers::SignalData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockerPullLogChanged'
        QtMocHelpers::SignalData<void()>(23, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockerPullProgressChanged'
        QtMocHelpers::SignalData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockerPullActiveChanged'
        QtMocHelpers::SignalData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'installPrereqsLogChanged'
        QtMocHelpers::SignalData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'installPrereqsRunningChanged'
        QtMocHelpers::SignalData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'installPrereqsDoneChanged'
        QtMocHelpers::SignalData<void()>(28, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockerPullStarted'
        QtMocHelpers::SignalData<void()>(29, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockerPullFinished'
        QtMocHelpers::SignalData<void(bool, const QString &)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 14 }, { QMetaType::QString, 17 },
        }}),
        // Signal 'dockerRunFinished'
        QtMocHelpers::SignalData<void(bool)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 14 },
        }}),
        // Signal 'dockerOpsLogChanged'
        QtMocHelpers::SignalData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockerOpsFollowLogChanged'
        QtMocHelpers::SignalData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockerOpsRunningChanged'
        QtMocHelpers::SignalData<void()>(34, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockerOpsStartingChanged'
        QtMocHelpers::SignalData<void()>(35, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockerOpsStoppingChanged'
        QtMocHelpers::SignalData<void()>(36, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockerOpsConflictChanged'
        QtMocHelpers::SignalData<void()>(37, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockerOpsContainerIdChanged'
        QtMocHelpers::SignalData<void()>(38, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockerOpsAutoRunChanged'
        QtMocHelpers::SignalData<void()>(39, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockerOpsFinished'
        QtMocHelpers::SignalData<void(bool, const QString &)>(40, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 14 }, { QMetaType::QString, 17 },
        }}),
        // Signal 'dockerOpsStopped'
        QtMocHelpers::SignalData<void(bool, const QString &)>(41, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 14 }, { QMetaType::QString, 17 },
        }}),
        // Signal 'upgradeLogChanged'
        QtMocHelpers::SignalData<void()>(42, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'upgradeRunningChanged'
        QtMocHelpers::SignalData<void()>(43, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'upgradeProgressChanged'
        QtMocHelpers::SignalData<void()>(44, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'upgradeFinished'
        QtMocHelpers::SignalData<void(bool, const QString &)>(45, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 46 }, { QMetaType::QString, 17 },
        }}),
        // Signal 'currentStepChanged'
        QtMocHelpers::SignalData<void()>(47, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'stepProgressChanged'
        QtMocHelpers::SignalData<void()>(48, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'statusTextChanged'
        QtMocHelpers::SignalData<void()>(49, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'busyChanged'
        QtMocHelpers::SignalData<void()>(50, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'keyValidChanged'
        QtMocHelpers::SignalData<void()>(51, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onKeyGenerated'
        QtMocHelpers::SlotData<void(bool, const QString &, const QString &, const QString &)>(52, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 14 }, { QMetaType::QString, 17 }, { QMetaType::QString, 15 }, { QMetaType::QString, 16 },
        }}),
        // Slot 'runStep1_Docker'
        QtMocHelpers::SlotData<void()>(53, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'runStep2_Env'
        QtMocHelpers::SlotData<void()>(54, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'runStep3_Model'
        QtMocHelpers::SlotData<void()>(55, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTaskProgress'
        QtMocHelpers::SlotData<void(double)>(56, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 57 },
        }}),
        // Slot 'onTaskFinished'
        QtMocHelpers::SlotData<void(bool, const QString &)>(58, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 14 }, { QMetaType::QString, 59 },
        }}),
        // Method 'validateKey'
        QtMocHelpers::MethodData<void()>(60, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'populateMacId'
        QtMocHelpers::MethodData<void()>(61, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'syncRelay'
        QtMocHelpers::MethodData<void()>(62, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'fetchTenantData'
        QtMocHelpers::MethodData<void(const QString &)>(63, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 64 },
        }}),
        // Method 'pullDockerImage'
        QtMocHelpers::MethodData<void()>(65, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'cancelDockerPull'
        QtMocHelpers::MethodData<void()>(66, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'clearDockerPullLog'
        QtMocHelpers::MethodData<void()>(67, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'startInstallPrereqs'
        QtMocHelpers::MethodData<void()>(68, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'cancelInstallPrereqs'
        QtMocHelpers::MethodData<void()>(69, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'clearInstallPrereqsLog'
        QtMocHelpers::MethodData<void()>(70, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'runDockerContainer'
        QtMocHelpers::MethodData<void()>(71, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'runDockerOps'
        QtMocHelpers::MethodData<void()>(72, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'restartDockerOps'
        QtMocHelpers::MethodData<void()>(73, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'stopDockerOps'
        QtMocHelpers::MethodData<void()>(74, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'startDockerOpsLogs'
        QtMocHelpers::MethodData<void()>(75, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'stopDockerOpsLogs'
        QtMocHelpers::MethodData<void()>(76, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'installDockerService'
        QtMocHelpers::MethodData<bool(bool)>(77, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Bool, 78 },
        }}),
        // Method 'launchDockerOpsApp'
        QtMocHelpers::MethodData<bool(bool)>(79, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Bool, 80 },
        }}),
        // Method 'launchDockerOpsApp'
        QtMocHelpers::MethodData<bool()>(79, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Bool),
        // Method 'handleSigInt'
        QtMocHelpers::MethodData<void()>(81, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'startInstall'
        QtMocHelpers::MethodData<void()>(82, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'cancel'
        QtMocHelpers::MethodData<void()>(83, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'copyInstallPath'
        QtMocHelpers::MethodData<void()>(84, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'goToStep'
        QtMocHelpers::MethodData<void(int)>(85, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 86 },
        }}),
        // Method 'forceStep'
        QtMocHelpers::MethodData<void(int)>(87, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 86 },
        }}),
        // Method 'startUpgrade'
        QtMocHelpers::MethodData<void()>(88, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'cancelUpgrade'
        QtMocHelpers::MethodData<void()>(89, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'installPath'
        QtMocHelpers::PropertyData<QString>(90, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'serviceBaseUrl'
        QtMocHelpers::PropertyData<QString>(91, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'macId'
        QtMocHelpers::PropertyData<QString>(92, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'tenantId'
        QtMocHelpers::PropertyData<QString>(93, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'registrationKey'
        QtMocHelpers::PropertyData<QString>(15, QMetaType::QString, QMC::DefaultPropertyFlags, 4),
        // property 'registrationGeneratedOn'
        QtMocHelpers::PropertyData<QString>(94, QMetaType::QString, QMC::DefaultPropertyFlags, 5),
        // property 'registrationAttempted'
        QtMocHelpers::PropertyData<bool>(95, QMetaType::Bool, QMC::DefaultPropertyFlags, 6),
        // property 'registrationError'
        QtMocHelpers::PropertyData<QString>(96, QMetaType::QString, QMC::DefaultPropertyFlags, 7),
        // property 'registrationMessage'
        QtMocHelpers::PropertyData<QString>(97, QMetaType::QString, QMC::DefaultPropertyFlags, 8),
        // property 'relayUrl'
        QtMocHelpers::PropertyData<QString>(98, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 9),
        // property 'relayError'
        QtMocHelpers::PropertyData<QString>(99, QMetaType::QString, QMC::DefaultPropertyFlags, 10),
        // property 'syncBusy'
        QtMocHelpers::PropertyData<bool>(100, QMetaType::Bool, QMC::DefaultPropertyFlags, 12),
        // property 'tenantBusy'
        QtMocHelpers::PropertyData<bool>(101, QMetaType::Bool, QMC::DefaultPropertyFlags, 14),
        // property 'tenantMessage'
        QtMocHelpers::PropertyData<QString>(102, QMetaType::QString, QMC::DefaultPropertyFlags, 15),
        // property 'tenantSuccess'
        QtMocHelpers::PropertyData<bool>(103, QMetaType::Bool, QMC::DefaultPropertyFlags, 16),
        // property 'dockerPullLog'
        QtMocHelpers::PropertyData<QString>(104, QMetaType::QString, QMC::DefaultPropertyFlags, 17),
        // property 'dockerPullProgress'
        QtMocHelpers::PropertyData<double>(105, QMetaType::Double, QMC::DefaultPropertyFlags, 18),
        // property 'dockerPullActive'
        QtMocHelpers::PropertyData<bool>(106, QMetaType::Bool, QMC::DefaultPropertyFlags, 19),
        // property 'installPrereqsLog'
        QtMocHelpers::PropertyData<QString>(107, QMetaType::QString, QMC::DefaultPropertyFlags, 20),
        // property 'installPrereqsRunning'
        QtMocHelpers::PropertyData<bool>(108, QMetaType::Bool, QMC::DefaultPropertyFlags, 21),
        // property 'installPrereqsDone'
        QtMocHelpers::PropertyData<bool>(109, QMetaType::Bool, QMC::DefaultPropertyFlags, 22),
        // property 'dockerOpsLog'
        QtMocHelpers::PropertyData<QString>(110, QMetaType::QString, QMC::DefaultPropertyFlags, 26),
        // property 'dockerOpsFollowLog'
        QtMocHelpers::PropertyData<QString>(111, QMetaType::QString, QMC::DefaultPropertyFlags, 27),
        // property 'dockerImage'
        QtMocHelpers::PropertyData<QString>(112, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'dockerOpsRunning'
        QtMocHelpers::PropertyData<bool>(113, QMetaType::Bool, QMC::DefaultPropertyFlags, 28),
        // property 'dockerOpsStarting'
        QtMocHelpers::PropertyData<bool>(114, QMetaType::Bool, QMC::DefaultPropertyFlags, 29),
        // property 'dockerOpsStopping'
        QtMocHelpers::PropertyData<bool>(115, QMetaType::Bool, QMC::DefaultPropertyFlags, 30),
        // property 'dockerOpsConflict'
        QtMocHelpers::PropertyData<bool>(116, QMetaType::Bool, QMC::DefaultPropertyFlags, 31),
        // property 'dockerOpsContainerId'
        QtMocHelpers::PropertyData<QString>(117, QMetaType::QString, QMC::DefaultPropertyFlags, 32),
        // property 'dockerOpsAutoRun'
        QtMocHelpers::PropertyData<bool>(118, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 33),
        // property 'upgradeLog'
        QtMocHelpers::PropertyData<QString>(119, QMetaType::QString, QMC::DefaultPropertyFlags, 36),
        // property 'upgradeRunning'
        QtMocHelpers::PropertyData<bool>(120, QMetaType::Bool, QMC::DefaultPropertyFlags, 37),
        // property 'upgradeProgress'
        QtMocHelpers::PropertyData<double>(121, QMetaType::Double, QMC::DefaultPropertyFlags, 38),
        // property 'currentStep'
        QtMocHelpers::PropertyData<int>(122, QMetaType::Int, QMC::DefaultPropertyFlags, 40),
        // property 'stepProgress'
        QtMocHelpers::PropertyData<double>(123, QMetaType::Double, QMC::DefaultPropertyFlags, 41),
        // property 'statusText'
        QtMocHelpers::PropertyData<QString>(124, QMetaType::QString, QMC::DefaultPropertyFlags, 42),
        // property 'busy'
        QtMocHelpers::PropertyData<bool>(125, QMetaType::Bool, QMC::DefaultPropertyFlags, 43),
        // property 'keyValid'
        QtMocHelpers::PropertyData<bool>(126, QMetaType::Bool, QMC::DefaultPropertyFlags, 44),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AppController, qt_meta_tag_ZN13AppControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AppController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AppControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AppControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13AppControllerE_t>.metaTypes,
    nullptr
} };

void AppController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AppController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->installPathChanged(); break;
        case 1: _t->serviceBaseUrlChanged(); break;
        case 2: _t->macIdChanged(); break;
        case 3: _t->tenantIdChanged(); break;
        case 4: _t->registrationKeyChanged(); break;
        case 5: _t->registrationGeneratedOnChanged(); break;
        case 6: _t->registrationAttemptedChanged(); break;
        case 7: _t->registrationErrorChanged(); break;
        case 8: _t->registrationMessageChanged(); break;
        case 9: _t->relayUrlChanged(); break;
        case 10: _t->relayErrorChanged(); break;
        case 11: _t->registrationResult((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4]))); break;
        case 12: _t->syncBusyChanged(); break;
        case 13: _t->syncResult((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 14: _t->tenantBusyChanged(); break;
        case 15: _t->tenantMessageChanged(); break;
        case 16: _t->tenantSuccessChanged(); break;
        case 17: _t->dockerPullLogChanged(); break;
        case 18: _t->dockerPullProgressChanged(); break;
        case 19: _t->dockerPullActiveChanged(); break;
        case 20: _t->installPrereqsLogChanged(); break;
        case 21: _t->installPrereqsRunningChanged(); break;
        case 22: _t->installPrereqsDoneChanged(); break;
        case 23: _t->dockerPullStarted(); break;
        case 24: _t->dockerPullFinished((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 25: _t->dockerRunFinished((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 26: _t->dockerOpsLogChanged(); break;
        case 27: _t->dockerOpsFollowLogChanged(); break;
        case 28: _t->dockerOpsRunningChanged(); break;
        case 29: _t->dockerOpsStartingChanged(); break;
        case 30: _t->dockerOpsStoppingChanged(); break;
        case 31: _t->dockerOpsConflictChanged(); break;
        case 32: _t->dockerOpsContainerIdChanged(); break;
        case 33: _t->dockerOpsAutoRunChanged(); break;
        case 34: _t->dockerOpsFinished((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 35: _t->dockerOpsStopped((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 36: _t->upgradeLogChanged(); break;
        case 37: _t->upgradeRunningChanged(); break;
        case 38: _t->upgradeProgressChanged(); break;
        case 39: _t->upgradeFinished((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 40: _t->currentStepChanged(); break;
        case 41: _t->stepProgressChanged(); break;
        case 42: _t->statusTextChanged(); break;
        case 43: _t->busyChanged(); break;
        case 44: _t->keyValidChanged(); break;
        case 45: _t->onKeyGenerated((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4]))); break;
        case 46: _t->runStep1_Docker(); break;
        case 47: _t->runStep2_Env(); break;
        case 48: _t->runStep3_Model(); break;
        case 49: _t->onTaskProgress((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 50: _t->onTaskFinished((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 51: _t->validateKey(); break;
        case 52: _t->populateMacId(); break;
        case 53: _t->syncRelay(); break;
        case 54: _t->fetchTenantData((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 55: _t->pullDockerImage(); break;
        case 56: _t->cancelDockerPull(); break;
        case 57: _t->clearDockerPullLog(); break;
        case 58: _t->startInstallPrereqs(); break;
        case 59: _t->cancelInstallPrereqs(); break;
        case 60: _t->clearInstallPrereqsLog(); break;
        case 61: _t->runDockerContainer(); break;
        case 62: _t->runDockerOps(); break;
        case 63: _t->restartDockerOps(); break;
        case 64: _t->stopDockerOps(); break;
        case 65: _t->startDockerOpsLogs(); break;
        case 66: _t->stopDockerOpsLogs(); break;
        case 67: { bool _r = _t->installDockerService((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 68: { bool _r = _t->launchDockerOpsApp((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 69: { bool _r = _t->launchDockerOpsApp();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 70: _t->handleSigInt(); break;
        case 71: _t->startInstall(); break;
        case 72: _t->cancel(); break;
        case 73: _t->copyInstallPath(); break;
        case 74: _t->goToStep((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 75: _t->forceStep((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 76: _t->startUpgrade(); break;
        case 77: _t->cancelUpgrade(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::installPathChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::serviceBaseUrlChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::macIdChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::tenantIdChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::registrationKeyChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::registrationGeneratedOnChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::registrationAttemptedChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::registrationErrorChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::registrationMessageChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::relayUrlChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::relayErrorChanged, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)(bool , const QString & , const QString & , const QString & )>(_a, &AppController::registrationResult, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::syncBusyChanged, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)(bool , const QString & )>(_a, &AppController::syncResult, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::tenantBusyChanged, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::tenantMessageChanged, 15))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::tenantSuccessChanged, 16))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::dockerPullLogChanged, 17))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::dockerPullProgressChanged, 18))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::dockerPullActiveChanged, 19))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::installPrereqsLogChanged, 20))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::installPrereqsRunningChanged, 21))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::installPrereqsDoneChanged, 22))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::dockerPullStarted, 23))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)(bool , const QString & )>(_a, &AppController::dockerPullFinished, 24))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)(bool )>(_a, &AppController::dockerRunFinished, 25))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::dockerOpsLogChanged, 26))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::dockerOpsFollowLogChanged, 27))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::dockerOpsRunningChanged, 28))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::dockerOpsStartingChanged, 29))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::dockerOpsStoppingChanged, 30))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::dockerOpsConflictChanged, 31))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::dockerOpsContainerIdChanged, 32))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::dockerOpsAutoRunChanged, 33))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)(bool , const QString & )>(_a, &AppController::dockerOpsFinished, 34))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)(bool , const QString & )>(_a, &AppController::dockerOpsStopped, 35))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::upgradeLogChanged, 36))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::upgradeRunningChanged, 37))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::upgradeProgressChanged, 38))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)(bool , const QString & )>(_a, &AppController::upgradeFinished, 39))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::currentStepChanged, 40))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::stepProgressChanged, 41))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::statusTextChanged, 42))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::busyChanged, 43))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::keyValidChanged, 44))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->installPath(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->serviceBaseUrl(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->macId(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->tenantId(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->registrationKey(); break;
        case 5: *reinterpret_cast<QString*>(_v) = _t->registrationGeneratedOn(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->registrationAttempted(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->registrationError(); break;
        case 8: *reinterpret_cast<QString*>(_v) = _t->registrationMessage(); break;
        case 9: *reinterpret_cast<QString*>(_v) = _t->relayUrl(); break;
        case 10: *reinterpret_cast<QString*>(_v) = _t->relayError(); break;
        case 11: *reinterpret_cast<bool*>(_v) = _t->syncBusy(); break;
        case 12: *reinterpret_cast<bool*>(_v) = _t->tenantBusy(); break;
        case 13: *reinterpret_cast<QString*>(_v) = _t->tenantMessage(); break;
        case 14: *reinterpret_cast<bool*>(_v) = _t->tenantSuccess(); break;
        case 15: *reinterpret_cast<QString*>(_v) = _t->dockerPullLog(); break;
        case 16: *reinterpret_cast<double*>(_v) = _t->dockerPullProgress(); break;
        case 17: *reinterpret_cast<bool*>(_v) = _t->dockerPullActive(); break;
        case 18: *reinterpret_cast<QString*>(_v) = _t->installPrereqsLog(); break;
        case 19: *reinterpret_cast<bool*>(_v) = _t->installPrereqsRunning(); break;
        case 20: *reinterpret_cast<bool*>(_v) = _t->installPrereqsDone(); break;
        case 21: *reinterpret_cast<QString*>(_v) = _t->dockerOpsLog(); break;
        case 22: *reinterpret_cast<QString*>(_v) = _t->dockerOpsFollowLog(); break;
        case 23: *reinterpret_cast<QString*>(_v) = _t->dockerImage(); break;
        case 24: *reinterpret_cast<bool*>(_v) = _t->dockerOpsRunning(); break;
        case 25: *reinterpret_cast<bool*>(_v) = _t->dockerOpsStarting(); break;
        case 26: *reinterpret_cast<bool*>(_v) = _t->dockerOpsStopping(); break;
        case 27: *reinterpret_cast<bool*>(_v) = _t->dockerOpsConflict(); break;
        case 28: *reinterpret_cast<QString*>(_v) = _t->dockerOpsContainerId(); break;
        case 29: *reinterpret_cast<bool*>(_v) = _t->dockerOpsAutoRun(); break;
        case 30: *reinterpret_cast<QString*>(_v) = _t->upgradeLog(); break;
        case 31: *reinterpret_cast<bool*>(_v) = _t->upgradeRunning(); break;
        case 32: *reinterpret_cast<double*>(_v) = _t->upgradeProgress(); break;
        case 33: *reinterpret_cast<int*>(_v) = _t->currentStep(); break;
        case 34: *reinterpret_cast<double*>(_v) = _t->stepProgress(); break;
        case 35: *reinterpret_cast<QString*>(_v) = _t->statusText(); break;
        case 36: *reinterpret_cast<bool*>(_v) = _t->busy(); break;
        case 37: *reinterpret_cast<bool*>(_v) = _t->keyValid(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setInstallPath(*reinterpret_cast<QString*>(_v)); break;
        case 1: _t->setServiceBaseUrl(*reinterpret_cast<QString*>(_v)); break;
        case 2: _t->setMacId(*reinterpret_cast<QString*>(_v)); break;
        case 3: _t->setTenantId(*reinterpret_cast<QString*>(_v)); break;
        case 9: _t->setRelayUrl(*reinterpret_cast<QString*>(_v)); break;
        case 29: _t->setDockerOpsAutoRun(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *AppController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AppController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AppControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AppController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 78)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 78;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 78)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 78;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    }
    return _id;
}

// SIGNAL 0
void AppController::installPathChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AppController::serviceBaseUrlChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void AppController::macIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void AppController::tenantIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void AppController::registrationKeyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void AppController::registrationGeneratedOnChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void AppController::registrationAttemptedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void AppController::registrationErrorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void AppController::registrationMessageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void AppController::relayUrlChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void AppController::relayErrorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void AppController::registrationResult(bool _t1, const QString & _t2, const QString & _t3, const QString & _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 12
void AppController::syncBusyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void AppController::syncResult(bool _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1, _t2);
}

// SIGNAL 14
void AppController::tenantBusyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void AppController::tenantMessageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void AppController::tenantSuccessChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void AppController::dockerPullLogChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void AppController::dockerPullProgressChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void AppController::dockerPullActiveChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}

// SIGNAL 20
void AppController::installPrereqsLogChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 20, nullptr);
}

// SIGNAL 21
void AppController::installPrereqsRunningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void AppController::installPrereqsDoneChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 22, nullptr);
}

// SIGNAL 23
void AppController::dockerPullStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 23, nullptr);
}

// SIGNAL 24
void AppController::dockerPullFinished(bool _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 24, nullptr, _t1, _t2);
}

// SIGNAL 25
void AppController::dockerRunFinished(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 25, nullptr, _t1);
}

// SIGNAL 26
void AppController::dockerOpsLogChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 26, nullptr);
}

// SIGNAL 27
void AppController::dockerOpsFollowLogChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 27, nullptr);
}

// SIGNAL 28
void AppController::dockerOpsRunningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 28, nullptr);
}

// SIGNAL 29
void AppController::dockerOpsStartingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 29, nullptr);
}

// SIGNAL 30
void AppController::dockerOpsStoppingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 30, nullptr);
}

// SIGNAL 31
void AppController::dockerOpsConflictChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 31, nullptr);
}

// SIGNAL 32
void AppController::dockerOpsContainerIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 32, nullptr);
}

// SIGNAL 33
void AppController::dockerOpsAutoRunChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 33, nullptr);
}

// SIGNAL 34
void AppController::dockerOpsFinished(bool _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 34, nullptr, _t1, _t2);
}

// SIGNAL 35
void AppController::dockerOpsStopped(bool _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 35, nullptr, _t1, _t2);
}

// SIGNAL 36
void AppController::upgradeLogChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 36, nullptr);
}

// SIGNAL 37
void AppController::upgradeRunningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 37, nullptr);
}

// SIGNAL 38
void AppController::upgradeProgressChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 38, nullptr);
}

// SIGNAL 39
void AppController::upgradeFinished(bool _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 39, nullptr, _t1, _t2);
}

// SIGNAL 40
void AppController::currentStepChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 40, nullptr);
}

// SIGNAL 41
void AppController::stepProgressChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 41, nullptr);
}

// SIGNAL 42
void AppController::statusTextChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 42, nullptr);
}

// SIGNAL 43
void AppController::busyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 43, nullptr);
}

// SIGNAL 44
void AppController::keyValidChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 44, nullptr);
}
QT_WARNING_POP
