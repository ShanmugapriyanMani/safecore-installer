#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QSocketNotifier>
#include <QLocalServer>
#include <QLocalSocket>
#include <QWindow>
#include <QString>
#include <csignal>
#include <unistd.h>
#include "appcontroller.h"
#include "appconstants.h"

namespace {
int sigintFd[2];
const QString SOCKET_NAME = "SafeCoreInstallerInstance";

void signalHandler(int)
{
    char ch = 1;
    ::write(sigintFd[0], &ch, sizeof(ch));
}

// Bring window to front
void raiseWindow(QQmlApplicationEngine* engine)
{
    if (!engine)
        return;
    const auto rootObjects = engine->rootObjects();
    for (QObject* obj : rootObjects) {
        if (QWindow* window = qobject_cast<QWindow*>(obj)) {
            window->show();
            window->raise();
            window->requestActivate();
            break;
        }
    }
}
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM"))
        qputenv("QT_QPA_PLATFORM", "xcb");
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
        qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");
    QGuiApplication app(argc, argv);

    app.setOrganizationName("SafeCore");
    app.setApplicationName("SafeCore");
    QGuiApplication::setDesktopFileName("safespace-global");
    app.setWindowIcon(QIcon("qrc:/images/launch/Safespace.png"));

    // Single instance check - try to connect to existing instance
    {
        QLocalSocket socket;
        socket.connectToServer(SOCKET_NAME);
        if (socket.waitForConnected(500)) {
            // Another instance exists - send activate signal and exit
            socket.write("activate");
            socket.waitForBytesWritten(1000);
            socket.disconnectFromServer();
            qInfo() << "SafeCore is already running. Activating existing window.";
            return 0;
        }
    }

    // Remove stale socket file if exists
    QLocalServer::removeServer(SOCKET_NAME);

    bool dockerOpsMode = false;
    bool forceInstaller = false;
    bool resetState = false;
    bool autoRunDockerOps = false;
    for (int i = 1; i < argc; ++i) {
        const QString arg = QString::fromLocal8Bit(argv[i]);
        if (arg == "--docker-ops") {
            dockerOpsMode = true;
            continue;
        }
        if (arg == "--installer") {
            forceInstaller = true;
        }
        if (arg == "--reset") {
            resetState = true;
            forceInstaller = true;
        }
        if (arg == "--auto-run") {
            autoRunDockerOps = true;
        }
    }
    AppController controller;
    if (resetState)
        controller.resetLocalState();
    controller.setDockerOpsAutoRun(autoRunDockerOps);
    QQmlApplicationEngine engine;
    AppConstantsProvider appConstants;
    engine.rootContext()->setContextProperty("AppConstants", &appConstants);
    engine.rootContext()->setContextProperty("AppController", &controller);

    engine.rootContext()->setContextProperty("controller", &controller);
    if (!dockerOpsMode && !forceInstaller && controller.setupComplete())
        dockerOpsMode = true;
    if (dockerOpsMode)
        app.setApplicationName("SafeCore");
    const char *qmlPath = dockerOpsMode ? "qrc:/safecore_ops.qml" : "qrc:/main.qml";
    const QUrl url(QString::fromLatin1(qmlPath));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    if (::pipe(sigintFd) == 0) {
        struct sigaction sa {};
        sa.sa_handler = signalHandler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sigaction(SIGINT, &sa, nullptr);
        sigaction(SIGTERM, &sa, nullptr);

        QSocketNotifier notifier(sigintFd[1], QSocketNotifier::Read);
        QObject::connect(&notifier, &QSocketNotifier::activated, &app, [&](int) {
            char tmp;
            ::read(sigintFd[1], &tmp, sizeof(tmp));
            controller.handleSigInt();
        });
    }

    engine.load(url);

    // Start local server for single instance detection
    QLocalServer localServer;
    if (localServer.listen(SOCKET_NAME)) {
        QObject::connect(&localServer, &QLocalServer::newConnection, [&]() {
            QLocalSocket* clientSocket = localServer.nextPendingConnection();
            if (clientSocket) {
                clientSocket->waitForReadyRead(1000);
                QByteArray message = clientSocket->readAll();
                if (message == "activate") {
                    raiseWindow(&engine);
                }
                clientSocket->disconnectFromServer();
                clientSocket->deleteLater();
            }
        });
    }

    return app.exec();
}
