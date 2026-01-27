#include "appcontroller.h"
#include "appconstants.h"
#include <QClipboard>
#include <QGuiApplication>
#include <QDir>
#include <QRegularExpression>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>
#include <QFile>
#include <QDateTime>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkInterface>
#include <QTextStream>
#include <QCoreApplication>
#include <algorithm>
#include <unistd.h>

namespace {
QString normalizeMacForApi(const QString &mac)
{
    QString normalized = mac.trimmed();
    normalized.replace("-", ":");
    return normalized.toLower();
}

QString extractLayerId(const QString &line)
{
    static const QRegularExpression layerRe("^([0-9a-f]{6,}):");
    const QRegularExpressionMatch match = layerRe.match(line);
    return match.hasMatch() ? match.captured(1) : QString();
}

QString stripAnsiSequences(const QString &input)
{
    QString output = input;
    static const QRegularExpression oscRe("\x1B\\][^\u0007]*\u0007");
    static const QRegularExpression csiRe("\x1B\\[[0-9;?]*[ -/]*[@-~]");
    output.remove(oscRe);
    output.remove(csiRe);
    return output;
}

} // namespace

AppController::AppController(QObject *parent)
    : QObject(parent)
{
    m_relayUrl = AppConstants::DefaultRelayUrl;
    connect(&m_registration, &RegistrationService::keyGenerated,
            this, &AppController::onKeyGenerated);
    m_dockerWatchdog.setInterval(5000);
    m_dockerWatchdog.setSingleShot(false);
    connect(&m_dockerWatchdog, &QTimer::timeout, this, &AppController::checkDockerPullStall);
    loadSetupState();

    QTimer::singleShot(0, this, [this]() {
        QProcess *inspectProcess = new QProcess(this);
        connect(inspectProcess, &QProcess::finished, this,
                [this, inspectProcess](int exitCode, QProcess::ExitStatus exitStatus) {
                    const QString output = QString::fromUtf8(inspectProcess->readAll()).trimmed();
                    inspectProcess->deleteLater();
                    if (exitStatus == QProcess::NormalExit && exitCode == 0 && !output.isEmpty()) {
                        const QStringList parts = output.split(' ', Qt::SkipEmptyParts);
                        const QString fullId = parts.value(0);
                        const bool running = parts.value(1).trimmed().toLower() == "true";
                        setDockerOpsContainerId(fullId.left(12));
                        setDockerOpsConflict(true);
                        setDockerOpsRunning(running);
                        if (running) {
                            setDockerOpsLog(QString("SafeCore container is already running.\nContainer ID: %1\n").arg(fullId));
                        }
                        return;
                    }
                    setDockerOpsContainerId(QString());
                    setDockerOpsConflict(false);
                    setDockerOpsRunning(false);
                });
        inspectProcess->start("bash", {"-lc",
            QString("docker inspect -f '{{.Id}} {{.State.Running}}' %1 2>/dev/null").arg(AppConstants::ContainerName)});
    });
}

AppController::~AppController()
{
    if (m_dockerProcess) {
        if (m_dockerProcess->state() != QProcess::NotRunning) {
            m_dockerProcess->terminate();
            if (!m_dockerProcess->waitForFinished(2000))
                m_dockerProcess->kill();
        }
        m_dockerProcess->deleteLater();
        m_dockerProcess = nullptr;
    }

    if (m_runProcess) {
        if (m_runProcess->state() != QProcess::NotRunning) {
            m_runProcess->terminate();
            if (!m_runProcess->waitForFinished(2000))
                m_runProcess->kill();
        }
        m_runProcess->deleteLater();
        m_runProcess = nullptr;
    }

    if (m_dockerOpsProcess) {
        if (m_dockerOpsProcess->state() != QProcess::NotRunning) {
            m_dockerOpsProcess->terminate();
            if (!m_dockerOpsProcess->waitForFinished(2000))
                m_dockerOpsProcess->kill();
        }
        m_dockerOpsProcess->deleteLater();
        m_dockerOpsProcess = nullptr;
    }

    if (m_dockerOpsStopProcess) {
        if (m_dockerOpsStopProcess->state() != QProcess::NotRunning) {
            m_dockerOpsStopProcess->terminate();
            if (!m_dockerOpsStopProcess->waitForFinished(2000))
                m_dockerOpsStopProcess->kill();
        }
        m_dockerOpsStopProcess->deleteLater();
        m_dockerOpsStopProcess = nullptr;
    }

    if (m_dockerOpsLogsProcess) {
        if (m_dockerOpsLogsProcess->state() != QProcess::NotRunning) {
            m_dockerOpsLogsProcess->terminate();
            if (!m_dockerOpsLogsProcess->waitForFinished(2000))
                m_dockerOpsLogsProcess->kill();
        }
        m_dockerOpsLogsProcess->deleteLater();
        m_dockerOpsLogsProcess = nullptr;
    }

    if (m_installPrereqsProcess) {
        if (m_installPrereqsProcess->state() != QProcess::NotRunning) {
            m_installPrereqsProcess->terminate();
            if (!m_installPrereqsProcess->waitForFinished(2000))
                m_installPrereqsProcess->kill();
        }
        m_installPrereqsProcess->deleteLater();
        m_installPrereqsProcess = nullptr;
    }
}

void AppController::setInstallPath(const QString &p)
{
    if (m_installPath == p) return;
    m_installPath = p;
    emit installPathChanged();
}

void AppController::setServiceBaseUrl(const QString &url)
{
    if (m_serviceBaseUrl == url) return;
    m_serviceBaseUrl = url;
    emit serviceBaseUrlChanged();
}

void AppController::setRelayUrl(const QString &relayUrl)
{
    if (m_relayUrl == relayUrl) return;
    m_relayUrl = relayUrl;
    emit relayUrlChanged();
    if (!m_relayError.isEmpty())
        setRelayError(QString());
}

void AppController::setMacId(const QString &macId)
{
    if (m_macId == macId) return;
    m_macId = macId;
    emit macIdChanged();
    if (!m_registrationError.isEmpty())
        setRegistrationError(QString());
}

void AppController::setTenantId(const QString &tenantId)
{
    if (m_tenantId == tenantId) return;
    m_tenantId = tenantId;
    emit tenantIdChanged();
    if (!m_registrationError.isEmpty())
        setRegistrationError(QString());
}

void AppController::setBusy(bool b)
{
    if (m_busy == b) return;
    m_busy = b;
    emit busyChanged();
}

void AppController::setSyncBusy(bool b)
{
    if (m_syncBusy == b) return;
    m_syncBusy = b;
    emit syncBusyChanged();
}

void AppController::setTenantBusy(bool b)
{
    if (m_tenantBusy == b) return;
    m_tenantBusy = b;
    emit tenantBusyChanged();
}

void AppController::setTenantSuccess(bool b)
{
    if (m_tenantSuccess == b) return;
    m_tenantSuccess = b;
    emit tenantSuccessChanged();
}

void AppController::setDockerPullLog(const QString &log)
{
    if (m_dockerPullLog == log) return;
    m_dockerPullLog = log;
    emit dockerPullLogChanged();
}

void AppController::clearDockerPullLog()
{
    m_dockerPullLines.clear();
    m_dockerPullLineIndex.clear();
    m_dockerPullRemainder.clear();
    m_dockerPullLastLayerId.clear();
    setDockerPullLog(QString());
}

void AppController::clearInstallPrereqsLog()
{
    setInstallPrereqsLog(QString());
}

void AppController::setDockerPullProgress(double value)
{
    if (qFuzzyCompare(m_dockerPullProgress, value))
        return;
    m_dockerPullProgress = value;
    emit dockerPullProgressChanged();
}

void AppController::setDockerPullActive(bool value)
{
    if (m_dockerPullActive == value)
        return;
    m_dockerPullActive = value;
    emit dockerPullActiveChanged();
}

void AppController::setInstallPrereqsLog(const QString &log)
{
    if (m_installPrereqsLog == log)
        return;
    m_installPrereqsLog = log;
    emit installPrereqsLogChanged();
}

void AppController::setInstallPrereqsRunning(bool value)
{
    if (m_installPrereqsRunning == value)
        return;
    m_installPrereqsRunning = value;
    emit installPrereqsRunningChanged();
}

void AppController::setInstallPrereqsDone(bool value)
{
    if (m_installPrereqsDone == value)
        return;
    m_installPrereqsDone = value;
    emit installPrereqsDoneChanged();
}

void AppController::setDockerOpsLog(const QString &log)
{
    if (m_dockerOpsLog == log)
        return;
    m_dockerOpsLog = log;
    emit dockerOpsLogChanged();
}

void AppController::setDockerOpsFollowLog(const QString &log)
{
    if (m_dockerOpsFollowLog == log)
        return;
    m_dockerOpsFollowLog = log;
    emit dockerOpsFollowLogChanged();
}

QString AppController::dockerImage() const
{
    return AppConstants::DockerImage;
}

void AppController::setDockerOpsRunning(bool value)
{
    if (m_dockerOpsRunning == value)
        return;
    m_dockerOpsRunning = value;
    emit dockerOpsRunningChanged();
}

void AppController::setDockerOpsStarting(bool value)
{
    if (m_dockerOpsStarting == value)
        return;
    m_dockerOpsStarting = value;
    emit dockerOpsStartingChanged();
}

void AppController::setDockerOpsStopping(bool value)
{
    if (m_dockerOpsStopping == value)
        return;
    m_dockerOpsStopping = value;
    emit dockerOpsStoppingChanged();
}

void AppController::setDockerOpsConflict(bool value)
{
    if (m_dockerOpsConflict == value)
        return;
    m_dockerOpsConflict = value;
    emit dockerOpsConflictChanged();
}

void AppController::setDockerOpsContainerId(const QString &id)
{
    if (m_dockerOpsContainerId == id)
        return;
    m_dockerOpsContainerId = id;
    emit dockerOpsContainerIdChanged();
}

void AppController::setDockerOpsAutoRun(bool value)
{
    if (m_dockerOpsAutoRun == value)
        return;
    m_dockerOpsAutoRun = value;
    emit dockerOpsAutoRunChanged();
}

void AppController::appendDockerPullLog(const QString &chunk)
{
    if (chunk.isEmpty())
        return;
    QString normalized = chunk;
    normalized.replace("\r", "\n");
    normalized = stripAnsiSequences(normalized);
    const QStringList lines = normalized.split('\n');
    for (const QString &rawLine : lines) {
        QString line = rawLine;
        const QString trimmedLine = rawLine.trimmed();
        if (trimmedLine.isEmpty())
            continue;
        if (trimmedLine.contains("killing shell", Qt::CaseInsensitive)
            || trimmedLine.contains("killed.", Qt::CaseInsensitive)) {
            continue;
        }
        if (trimmedLine.startsWith("Status:") || trimmedLine.startsWith("Digest:"))
            m_dockerPullSawStatus = true;
        QString layerId = extractLayerId(trimmedLine);
        const bool isProgressLine = trimmedLine.startsWith("Downloading")
            || trimmedLine.startsWith("Extracting")
            || trimmedLine.startsWith("Waiting")
            || trimmedLine.startsWith("Pull complete")
            || trimmedLine.startsWith("Download complete")
            || trimmedLine.startsWith("Pulling fs layer");
        if (layerId.isEmpty() && isProgressLine && !m_dockerPullLastLayerId.isEmpty()) {
            layerId = m_dockerPullLastLayerId;
            line = m_dockerPullLastLayerId + ": " + trimmedLine;
        }
        if (!layerId.isEmpty())
            m_dockerPullLastLayerId = layerId;
        if (!layerId.isEmpty()) {
            const int existingIndex = m_dockerPullLineIndex.value(layerId, -1);
            if (existingIndex >= 0 && existingIndex < m_dockerPullLines.size()) {
                m_dockerPullLines[existingIndex] = line;
            } else {
                m_dockerPullLineIndex.insert(layerId, m_dockerPullLines.size());
                m_dockerPullLines.append(line);
            }
        } else {
            if (isProgressLine)
                continue;
            m_dockerPullLines.append(line);
        }
    }

    const int maxLines = 200;
    if (m_dockerPullLines.size() > maxLines) {
        m_dockerPullLines = m_dockerPullLines.mid(m_dockerPullLines.size() - maxLines);
        m_dockerPullLineIndex.clear();
        for (int i = 0; i < m_dockerPullLines.size(); ++i) {
            const QString layerId = extractLayerId(m_dockerPullLines[i]);
            if (!layerId.isEmpty())
                m_dockerPullLineIndex.insert(layerId, i);
        }
    }
    setDockerPullLog(m_dockerPullLines.join("\n"));
    updateDockerPullProgressFromChunk(normalized);
}

void AppController::resetDockerPullProgress()
{
    m_dockerPullRemainder.clear();
    m_dockerPullLines.clear();
    m_dockerPullLineIndex.clear();
    m_dockerPullLayerState.clear();
    m_dockerPullLastLayerId.clear();
    m_dockerPullSawStatus = false;
    setDockerPullProgress(0.0);
}

void AppController::updateDockerPullProgressFromChunk(const QString &chunk)
{
    QString data = m_dockerPullRemainder + chunk;
    const bool endsWithNewline = data.endsWith('\n');
    QStringList lines = data.split('\n');
    if (!endsWithNewline) {
        m_dockerPullRemainder = lines.takeLast();
    } else {
        m_dockerPullRemainder.clear();
    }

    bool changed = false;
    QString lastLayerId = m_dockerPullLastLayerId;
    for (const QString &line : lines) {
        const QString trimmed = line.trimmed();
        const int colonIdx = trimmed.indexOf(':');
        QString layerId;
        if (colonIdx > 0) {
            layerId = trimmed.left(colonIdx);
        } else {
            const bool isProgressLine = trimmed.startsWith("Downloading")
                || trimmed.startsWith("Extracting")
                || trimmed.startsWith("Waiting")
                || trimmed.startsWith("Pull complete")
                || trimmed.startsWith("Download complete")
                || trimmed.startsWith("Pulling fs layer");
            if (isProgressLine && !lastLayerId.isEmpty())
                layerId = lastLayerId;
        }
        if (layerId.isEmpty() || layerId.size() < 6)
            continue;

        lastLayerId = layerId;

        int newState = -1;
        if (trimmed.contains("Pull complete") || trimmed.contains("Already exists")
            || trimmed.contains("Download complete")) {
            newState = 2;
        } else if (trimmed.contains("Downloading") || trimmed.contains("Extracting")
                   || trimmed.contains("Pulling fs layer") || trimmed.contains("Verifying Checksum")
                   ) {
            newState = 1;
        } else if (trimmed.contains("Waiting")) {
            newState = 0;
        }

        if (newState >= 0) {
            const int prevState = m_dockerPullLayerState.value(layerId, -1);
            if (newState > prevState) {
                m_dockerPullLayerState.insert(layerId, newState);
                changed = true;
            } else if (prevState < 0) {
                m_dockerPullLayerState.insert(layerId, newState);
                changed = true;
            }
        }
    }

    m_dockerPullLastLayerId = lastLayerId;
    const int total = m_dockerPullLayerState.size();
    if (total <= 0)
        return;

    int done = 0;
    int inProgress = 0;
    for (auto it = m_dockerPullLayerState.constBegin(); it != m_dockerPullLayerState.constEnd(); ++it) {
        if (it.value() >= 2)
            ++done;
        else if (it.value() == 1)
            ++inProgress;
    }

    const double weighted = static_cast<double>(done) + (static_cast<double>(inProgress) * 0.3);
    const double ratio = weighted / static_cast<double>(total);
    const double newProgress = qMin(0.98, ratio);
    if (changed || !qFuzzyCompare(newProgress, m_dockerPullProgress)) {
        setDockerPullProgress(newProgress);
    }
}

void AppController::loadSetupState()
{
    const QString basePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/SafeCore";
    QFile inFile(basePath + "/setup_state.json");
    if (!inFile.open(QIODevice::ReadOnly))
        return;

    const QJsonDocument doc = QJsonDocument::fromJson(inFile.readAll());
    if (!doc.isObject())
        return;

    const QJsonObject obj = doc.object();
    m_registrationOk = obj.value("registrationOk").toBool(false);
    m_dockerPullOk = obj.value("dockerPullOk").toBool(false);
    m_setupComplete = obj.value("setupComplete").toBool(false);
}

void AppController::resetLocalState()
{
    const QString basePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/SafeCore";
    QDir dir(basePath);
    if (dir.exists())
        dir.removeRecursively();

    m_registrationOk = false;
    m_dockerPullOk = false;
    m_setupComplete = false;
    m_registrationAttempted = false;
    m_registrationError.clear();
    m_registrationMessage.clear();
    m_registrationKey.clear();
    m_registrationGeneratedOn.clear();
    m_relayError.clear();
    m_tenantMessage.clear();
    m_tenantSuccess = false;
    emit registrationAttemptedChanged();
    emit registrationErrorChanged();
    emit registrationMessageChanged();
    emit registrationKeyChanged();
    emit registrationGeneratedOnChanged();
    emit relayErrorChanged();
    emit tenantMessageChanged();
    emit tenantSuccessChanged();
}

void AppController::persistSetupState()
{
    const QString basePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/SafeCore";
    QDir dir(basePath);
    dir.mkpath(".");

    QFile outFile(basePath + "/setup_state.json");
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QJsonObject obj;
    obj.insert("setupComplete", m_setupComplete);
    obj.insert("registrationOk", m_registrationOk);
    obj.insert("dockerPullOk", m_dockerPullOk);
    obj.insert("savedAt", QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
    outFile.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
}

void AppController::updateSetupComplete()
{
    const bool completeNow = m_registrationOk && m_dockerPullOk;
    if (m_setupComplete != completeNow)
        m_setupComplete = completeNow;
    persistSetupState();
}

void AppController::setStatus(const QString &s)
{
    if (m_statusText == s) return;
    m_statusText = s;
    emit statusTextChanged();
}

void AppController::setProgress(double p)
{
    p = std::clamp(p, 0.0, 1.0);
    if (qFuzzyCompare(m_stepProgress, p)) return;
    m_stepProgress = p;
    emit stepProgressChanged();
}

void AppController::setStep(int step)
{
    if (m_currentStep == step) return;
    m_currentStep = step;
    emit currentStepChanged();
}

void AppController::setRegistrationError(const QString &message)
{
    if (m_registrationError == message) return;
    m_registrationError = message;
    emit registrationErrorChanged();
}

void AppController::setRegistrationMessage(const QString &message)
{
    if (m_registrationMessage == message) return;
    m_registrationMessage = message;
    emit registrationMessageChanged();
}

void AppController::setRelayError(const QString &message)
{
    if (m_relayError == message) return;
    m_relayError = message;
    emit relayErrorChanged();
}

void AppController::setTenantMessage(const QString &message)
{
    if (m_tenantMessage == message) return;
    m_tenantMessage = message;
    emit tenantMessageChanged();
}


void AppController::validateKey()
{
    const QString mac = m_macId.trimmed();
    const QString tenant = m_tenantId.trimmed();

    // Basic format validation
    static const QRegularExpression macRegex(R"(^([0-9A-Fa-f]{2}[:-]){5}[0-9A-Fa-f]{2}$)");
    static const QRegularExpression tenantRegex(R"(^[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}$)");

    if (mac.isEmpty() || tenant.isEmpty()) {
        if (m_registrationAttempted) { m_registrationAttempted = false; emit registrationAttemptedChanged(); }
        if (m_keyValid != false) { m_keyValid = false; emit keyValidChanged(); }
        setRegistrationError("MAC ID and Tenant ID are required.");
        setRegistrationMessage("MAC ID and Tenant ID are required.");
        setStatus("Please enter MAC ID and Tenant ID.");
        setProgress(0.0);
        setStep(1);
        emit registrationResult(false, QString(), QString(), "MAC ID and Tenant ID are required.");
        return;
    }

    if (!macRegex.match(mac).hasMatch() || !tenantRegex.match(tenant).hasMatch()) {
        if (m_registrationAttempted) { m_registrationAttempted = false; emit registrationAttemptedChanged(); }
        if (m_keyValid != false) { m_keyValid = false; emit keyValidChanged(); }
        setRegistrationError("Invalid MAC ID or Tenant ID format.");
        setRegistrationMessage("Invalid MAC ID or Tenant ID format.");
        setStatus("Invalid MAC ID or Tenant ID format.");
        setProgress(0.0);
        setStep(1);
        emit registrationResult(false, QString(), QString(), "Invalid MAC ID or Tenant ID format.");
        return;
    }

    setRegistrationError(QString());
    setRegistrationMessage(QString());
    setBusy(true);
    setStatus("Generating registration key...");
    setProgress(0.1);
    setStep(1);

    m_registration.generateKey(m_serviceBaseUrl, m_tenantAccessKey, normalizeMacForApi(m_macId), m_tenantId);
}

void AppController::populateMacId()
{
    QString mac;
    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : interfaces) {
        const auto flags = iface.flags();
        if (!(flags & QNetworkInterface::IsUp) || !(flags & QNetworkInterface::IsRunning))
            continue;
        if (flags & QNetworkInterface::IsLoopBack)
            continue;

        const QString hw = iface.hardwareAddress().trimmed();
        if (hw.isEmpty() || hw == "00:00:00:00:00:00")
            continue;

        mac = hw;
        break;
    }

    if (mac.isEmpty()) {
        setRegistrationError("Unable to detect MAC ID.");
        setStatus("Unable to detect MAC ID.");
        return;
    }

    mac = mac.toUpper().replace("-", ":");
    setMacId(mac);
    setRegistrationError(QString());
    setStatus("MAC ID detected.");
}

void AppController::onKeyGenerated(bool ok, const QString &message, const QString &registrationKey, const QString &generatedOn)
{
    if (!m_registrationAttempted) {
        m_registrationAttempted = true;
        emit registrationAttemptedChanged();
    }
    setBusy(false);
    setProgress(ok ? 1.0 : 0.0);

    if (m_keyValid != ok) {
        m_keyValid = ok;
        emit keyValidChanged();
    }

    if (m_registrationKey != registrationKey) {
        m_registrationKey = registrationKey;
        emit registrationKeyChanged();
    }

    if (m_registrationGeneratedOn != generatedOn) {
        m_registrationGeneratedOn = generatedOn;
        emit registrationGeneratedOnChanged();
    }

    if (ok && !m_registrationKey.isEmpty()) {
        const QString basePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/SafeCore";
        QDir dataDir(basePath);
        dataDir.mkpath("data");
        QFile outFile(dataDir.filePath("data/registration_data.json"));
        if (outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QJsonObject payload;
            payload.insert("savedAt", QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
            payload.insert("macId", m_macId.trimmed());
            payload.insert("tenantId", m_tenantId.trimmed());
            payload.insert("registrationKey", m_registrationKey);
            payload.insert("generatedOn", m_registrationGeneratedOn);
            outFile.write(QJsonDocument(payload).toJson(QJsonDocument::Indented));
        }
    }

    if (ok) {
        setRegistrationError(QString());
    } else {
        setRegistrationError(message);
    }
    setRegistrationMessage(message);
    setStatus(message);
    // Stay on Registration step even on failure; success remains on step 1 until user proceeds.
    setStep(1);
    emit registrationResult(ok, registrationKey, generatedOn, message);

    m_registrationOk = ok;
    updateSetupComplete();
}

void AppController::syncRelay()
{
    if (!m_keyValid) {
        setStatus("Register first before syncing.");
        emit syncResult(false, "Register first before syncing.");
        return;
    }

    const QString relay = m_relayUrl.trimmed();
    if (relay.isEmpty()) {
        setRelayError("Relay URL is required.");
        return;
    }

    if (!relay.startsWith("https://", Qt::CaseInsensitive)) {
        setRelayError("Relay URL must start with https://");
        return;
    }

    QUrl relayUrl(relay);
    if (!relayUrl.isValid()) {
        setRelayError("Relay URL is invalid.");
        return;
    }

    if (m_syncBusy)
        return;

    setRelayError(QString());

    QUrl baseUrl(m_serviceBaseUrl.trimmed());
    if (!baseUrl.isValid()) {
        setStatus("Service URL is invalid.");
        emit syncResult(false, "Service URL is invalid.");
        return;
    }

    setSyncBusy(true);
    setStatus("Syncing relay API...");

    QUrl url(baseUrl);
    QString path = url.path();
    if (path.endsWith('/'))
        path.chop(1);
    url.setPath(path + "/api/registration/syncdevice");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("accept", "*/*");

    QJsonObject payload;
    payload.insert("accessKey", m_tenantAccessKey);
    payload.insert("macId", normalizeMacForApi(m_macId));
    payload.insert("tenantId", m_tenantId.trimmed());
    payload.insert("relayApi", relay);

    QNetworkReply *reply = m_net.post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const QByteArray body = reply->readAll();
        const QNetworkReply::NetworkError err = reply->error();
        const QString errString = reply->errorString();
        const int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply->deleteLater();

        bool ok = false;
        QString finalMessage;

        QJsonParseError parseError{};
        const QJsonDocument doc = QJsonDocument::fromJson(body, &parseError);
        if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
            const QJsonObject root = doc.object();
            const int statusCode = root.value("statusCode").toInt(httpStatus);
            const QString status = root.value("status").toString();
            const QString message = root.value("message").toString();

            ok = statusCode >= 200 && statusCode < 300;
            if (!ok && !status.isEmpty())
                ok = status.compare("Success", Qt::CaseInsensitive) == 0;

            finalMessage = message.isEmpty() ? (ok ? "Sync completed." : "Sync failed.") : message;
        } else {
            if (err == QNetworkReply::NoError && httpStatus >= 200 && httpStatus < 300)
                finalMessage = "Sync failed: invalid response.";
            else
                finalMessage = "Sync failed: " + errString;
        }

        setSyncBusy(false);
        setStatus(finalMessage);
        emit syncResult(ok, finalMessage);
    });
}

void AppController::fetchTenantData(const QString &vertical)
{
    if (m_tenantBusy)
        return;

    setTenantSuccess(false);
    if (!m_keyValid) {
        setTenantMessage("Register first before fetching tenant data.");
        return;
    }

    const QString mac = normalizeMacForApi(m_macId);
    const QString tenant = m_tenantId.trimmed();
    const QString trimmedVertical = vertical.trimmed();

    if (mac.isEmpty() || tenant.isEmpty()) {
        setTenantMessage("MAC ID and Tenant ID are required.");
        return;
    }

    if (trimmedVertical.isEmpty()) {
        setTenantMessage("Vertical is required.");
        return;
    }

    QUrl baseUrl(m_serviceBaseUrl.trimmed());
    if (!baseUrl.isValid()) {
        setTenantMessage("Service URL is invalid.");
        return;
    }

    setTenantMessage(QString());
    setTenantBusy(true);
    setStatus("Fetching tenant data...");

    QUrl url(baseUrl);
    QString path = url.path();
    if (path.endsWith('/'))
        path.chop(1);
    url.setPath(path + "/api/tenant/GetTenantDataById");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("accept", "*/*");

    QJsonObject payload;
    payload.insert("tenantId", tenant);
    payload.insert("vertical", trimmedVertical);
    payload.insert("accessKey", m_tenantAccessKey);
    payload.insert("macId", mac);

    QNetworkReply *reply = m_net.post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const QByteArray body = reply->readAll();
        const QNetworkReply::NetworkError err = reply->error();
        const QString errString = reply->errorString();
        const int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply->deleteLater();

        bool ok = false;
        QString finalMessage;

        QJsonParseError parseError{};
        const QJsonDocument doc = QJsonDocument::fromJson(body, &parseError);
        if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
            const QJsonObject root = doc.object();
            const int statusCode = root.value("statusCode").toInt(httpStatus);
            const QString status = root.value("status").toString();
            const QString message = root.value("message").toString();

            ok = statusCode >= 200 && statusCode < 300;
            if (!ok && !status.isEmpty())
                ok = status.compare("Success", Qt::CaseInsensitive) == 0;

            if (ok) {
                const QJsonObject data = root.value("data").toObject();
                if (!data.isEmpty()) {
                    const QString basePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/SafeCore";
                    QDir dataDir(basePath);
                    dataDir.mkpath("data");
                    QFile outFile(dataDir.filePath("data/tenant_data.json"));
                    if (outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                        QJsonObject payload;
                        payload.insert("savedAt", QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
                        payload.insert("data", data);
                        outFile.write(QJsonDocument(payload).toJson(QJsonDocument::Indented));
                    }
                }
                finalMessage = message.isEmpty() ? "Tenant data retrieved successfully." : message;
            } else {
                if (message.isEmpty())
                    finalMessage = "Tenant data fetch failed.";
                else
                    finalMessage = "Tenant data fetch failed: " + message;
            }
        } else {
            if (err == QNetworkReply::NoError && httpStatus >= 200 && httpStatus < 300)
                finalMessage = "Tenant data fetch failed: invalid response.";
            else if (httpStatus >= 500)
                finalMessage = "Tenant data fetch failed: Internal Server Error.";
            else
                finalMessage = "Tenant data fetch failed: " + errString;
        }

        setTenantBusy(false);
        setTenantSuccess(ok);
        setTenantMessage(finalMessage);
        setStatus(finalMessage);
    });
}

void AppController::startInstall()
{
    if (!m_keyValid) {
        setStatus("Key is not valid. Please validate first.");
        return;
    }

    // Ensure base folder exists
    QDir dir;
    if (!dir.mkpath(m_installPath)) {
        setStatus("Cannot create install folder: " + m_installPath);
        return;
    }

    setBusy(true);
    setStep(2);
    setProgress(0.0);
    setStatus("Installation: Installing Docker...");
    m_installPhase = 0;
    runStep1_Docker();
}

void AppController::pullDockerImage()
{
    if (m_dockerProcess && m_dockerProcess->state() != QProcess::NotRunning)
        return;

    resetDockerPullProgress();
    m_dockerPullCanceled = false;
    m_dockerAwaitingNetwork = false;
    m_dockerProbeActive = false;
    setDockerPullLog("Waiting for docker output...\n");
    startDockerPullProcess(true);
}

void AppController::startInstallPrereqs()
{
    if (m_installPrereqsRunning)
        return;

    if (m_installPrereqsProcess) {
        if (m_installPrereqsProcess->state() != QProcess::NotRunning) {
            m_installPrereqsProcess->terminate();
            if (!m_installPrereqsProcess->waitForFinished(2000))
                m_installPrereqsProcess->kill();
        }
        m_installPrereqsProcess->deleteLater();
        m_installPrereqsProcess = nullptr;
    }

    setInstallPrereqsDone(false);
    setInstallPrereqsRunning(true);
    setInstallPrereqsLog("");
    m_installPrereqsCanceled = false;

    QFile resourceScript(":/scripts/install_prereqs.sh");
    if (!resourceScript.open(QIODevice::ReadOnly)) {
        setInstallPrereqsRunning(false);
        setInstallPrereqsDone(false);
        setInstallPrereqsLog("Failed to read install script.\n");
        return;
    }
    const QByteArray script = resourceScript.readAll();
    resourceScript.close();

    const QString scriptPath = QStringLiteral("/tmp/Install-Docker-NVIDIA-Container-Toolkit");
    {
        QFile scriptFile(scriptPath);
        if (!scriptFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            setInstallPrereqsRunning(false);
            setInstallPrereqsDone(false);
            setInstallPrereqsLog("Failed to create install script.\n");
            return;
        }
        scriptFile.write(script);
        scriptFile.close();
        QFile::setPermissions(scriptPath,
                              QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner |
                              QFileDevice::ReadGroup | QFileDevice::ExeGroup |
                              QFileDevice::ReadOther | QFileDevice::ExeOther);
    }

    m_installPrereqsProcess = new QProcess(this);
    m_installPrereqsProcess->setProcessChannelMode(QProcess::MergedChannels);
    m_installPrereqsProcess->start("pkexec", {scriptPath});

    QPointer<QProcess> installProcess(m_installPrereqsProcess);

    connect(m_installPrereqsProcess, &QProcess::readyRead, this, [this, installProcess]() {
        if (!installProcess)
            return;
        const QString chunk = stripAnsiSequences(QString::fromUtf8(installProcess->readAll()));
        if (!chunk.isEmpty())
            setInstallPrereqsLog(m_installPrereqsLog + chunk);
    });

    connect(m_installPrereqsProcess, &QProcess::finished, this,
            [this, installProcess](int exitCode, QProcess::ExitStatus exitStatus) {
                if (!installProcess)
                    return;
                if (m_installPrereqsCanceled) {
                    setInstallPrereqsRunning(false);
                    setInstallPrereqsDone(false);
                    installProcess->deleteLater();
                    if (m_installPrereqsProcess == installProcess)
                        m_installPrereqsProcess = nullptr;
                    return;
                }
                const QString output = stripAnsiSequences(QString::fromUtf8(installProcess->readAll())).trimmed();
                const bool ok = (exitStatus == QProcess::NormalExit && exitCode == 0);
                setInstallPrereqsRunning(false);
                setInstallPrereqsDone(ok);
                if (!output.isEmpty())
                    setInstallPrereqsLog(m_installPrereqsLog + output + "\n");
                if (!ok)
                    setInstallPrereqsLog(m_installPrereqsLog + "Install prerequisites failed.\n");
                installProcess->deleteLater();
                if (m_installPrereqsProcess == installProcess)
                    m_installPrereqsProcess = nullptr;
            });

    connect(m_installPrereqsProcess, &QProcess::errorOccurred, this,
            [this, installProcess](QProcess::ProcessError) {
                if (!installProcess)
                    return;
                if (m_installPrereqsCanceled) {
                    setInstallPrereqsRunning(false);
                    setInstallPrereqsDone(false);
                    installProcess->deleteLater();
                    if (m_installPrereqsProcess == installProcess)
                        m_installPrereqsProcess = nullptr;
                    return;
                }
                const QString output = stripAnsiSequences(QString::fromUtf8(installProcess->readAll())).trimmed();
                setInstallPrereqsRunning(false);
                setInstallPrereqsDone(false);
                if (!output.isEmpty())
                    setInstallPrereqsLog(m_installPrereqsLog + output + "\n");
                setInstallPrereqsLog(m_installPrereqsLog + "Install prerequisites failed.\n");
                installProcess->deleteLater();
                if (m_installPrereqsProcess == installProcess)
                    m_installPrereqsProcess = nullptr;
            });
}

void AppController::cancelInstallPrereqs()
{
    if (!m_installPrereqsProcess)
        return;
    m_installPrereqsCanceled = true;
    disconnect(m_installPrereqsProcess, nullptr, this, nullptr);
    if (m_installPrereqsProcess->state() != QProcess::NotRunning) {
        m_installPrereqsProcess->terminate();
        if (!m_installPrereqsProcess->waitForFinished(2000)) {
            m_installPrereqsProcess->kill();
            if (!m_installPrereqsProcess->waitForFinished(5000)) {
                QPointer<QProcess> process(m_installPrereqsProcess);
                m_installPrereqsProcess->setParent(nullptr);
                connect(m_installPrereqsProcess, &QProcess::finished, process, [process]() {
                    if (!process)
                        return;
                    process->deleteLater();
                });
                m_installPrereqsProcess = nullptr;
                setInstallPrereqsRunning(false);
                setInstallPrereqsDone(false);
                setInstallPrereqsLog(m_installPrereqsLog + "Docker installation canceled.\n");
                return;
            }
        }
    }
    m_installPrereqsProcess->deleteLater();
    m_installPrereqsProcess = nullptr;
    setInstallPrereqsRunning(false);
    setInstallPrereqsDone(false);
    setInstallPrereqsLog(m_installPrereqsLog + "Docker installation canceled.\n");
}

void AppController::startDockerPullProcess(bool resetStatus)
{
    if (m_dockerProcess) {
        m_dockerProcess->terminate();
        if (!m_dockerProcess->waitForFinished(2000))
            m_dockerProcess->kill();
        m_dockerProcess->deleteLater();
        m_dockerProcess = nullptr;
    }

    const int generation = ++m_dockerPullGeneration;
    m_dockerProcess = new QProcess(this);
    QPointer<QProcess> process(m_dockerProcess);
    m_dockerProcess->setProcessChannelMode(QProcess::MergedChannels);
    m_dockerLastOutput.restart();
    m_dockerWatchdog.start();

    connect(m_dockerProcess, &QProcess::readyRead, this, [this, process, generation]() {
        if (!process || generation != m_dockerPullGeneration)
            return;
        appendDockerPullLog(QString::fromUtf8(process->readAll()));
        m_dockerLastOutput.restart();
    });

    connect(m_dockerProcess, &QProcess::started, this, [this, process, generation]() {
        if (!process || generation != m_dockerPullGeneration)
            return;
        setStatus("Pulling Docker image...");
        setDockerPullActive(true);
        emit dockerPullStarted();
    });

    connect(m_dockerProcess, &QProcess::finished, this,
            [this, process, generation](int exitCode, QProcess::ExitStatus exitStatus) {
                if (!process)
                    return;
                if (generation != m_dockerPullGeneration) {
                    process->deleteLater();
                    return;
                }
                appendDockerPullLog(QString::fromUtf8(process->readAllStandardOutput()));
                const QString output = m_dockerPullLog.trimmed();
                process->deleteLater();
                m_dockerProcess = nullptr;
                m_dockerWatchdog.stop();

                bool ok = (!m_dockerPullCanceled && exitStatus == QProcess::NormalExit && exitCode == 0);
                if (ok && !m_dockerPullSawStatus)
                    ok = false;
                m_dockerPullOk = ok;
                updateSetupComplete();
                if (ok)
                    setDockerPullProgress(1.0);
                const QString message = m_dockerPullCanceled
                    ? QStringLiteral("Docker image pull canceled.")
                    : (ok ? QStringLiteral("Docker image pulled successfully.")
                          : (output.isEmpty() ? QStringLiteral("Docker image pull failed.") : output));
                if (!message.isEmpty() && !m_dockerPullLog.contains(message))
                    appendDockerPullLog("\n" + message + "\n");
                setStatus(message);
                setDockerPullActive(false);
                m_dockerPullCanceled = false;
                emit dockerPullFinished(ok, message);
            });

    connect(m_dockerProcess, &QProcess::errorOccurred, this,
            [this, process, generation](QProcess::ProcessError) {
                if (!process)
                    return;
                if (generation != m_dockerPullGeneration) {
                    process->deleteLater();
                    return;
                }
                appendDockerPullLog(QString::fromUtf8(process->readAllStandardOutput()));
                const QString output = m_dockerPullLog.trimmed();
                const QString message = m_dockerPullCanceled
                    ? QStringLiteral("Docker pull canceled.")
                    : (output.isEmpty() ? QStringLiteral("Docker pull failed to start.") : output);
                m_dockerPullOk = false;
                updateSetupComplete();
                process->deleteLater();
                m_dockerProcess = nullptr;
                m_dockerWatchdog.stop();
                setStatus(message);
                setDockerPullActive(false);
                m_dockerPullCanceled = false;
                emit dockerPullFinished(false, message);
            });

    if (resetStatus)
        setStatus("Waiting for authentication...");
    const QString image = "rjaat/aibox-prod:latest";
    const QString scriptPath = QStandardPaths::findExecutable("script");
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("TERM", "xterm-256color");
    env.insert("COLUMNS", "120");
    m_dockerProcess->setProcessEnvironment(env);
    if (!scriptPath.isEmpty()) {
        const QString command = QString("docker pull %1").arg(image);
        m_dockerProcess->start(scriptPath, {"-q", "-e", "-c", command, "/dev/null"});
    } else {
        m_dockerProcess->start("docker", {"pull", image});
    }
}

void AppController::checkDockerPullStall()
{
    if (!m_dockerProcess || m_dockerProcess->state() == QProcess::NotRunning) {
        m_dockerWatchdog.stop();
        return;
    }
    if (m_dockerLastOutput.isValid() && m_dockerLastOutput.elapsed() < 30000)
        return;
    if (m_dockerProbeActive)
        return;
    m_dockerProbeActive = true;
    probeDockerRegistry();
}

void AppController::probeDockerRegistry()
{
    QNetworkRequest request(QUrl("https://registry-1.docker.io/v2/"));
    request.setAttribute(QNetworkRequest::Http2AllowedAttribute, false);
    QNetworkReply *reply = m_net.head(request);
    QTimer *timeout = new QTimer(reply);
    timeout->setSingleShot(true);
    timeout->start(4000);
    connect(timeout, &QTimer::timeout, reply, &QNetworkReply::abort);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const auto error = reply->error();
        const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        bool networkUp = (error == QNetworkReply::NoError)
            || (status > 0 && status < 600)
            || (error == QNetworkReply::AuthenticationRequiredError);

        m_dockerProbeActive = false;
        if (networkUp) {
            if (m_dockerAwaitingNetwork)
                appendDockerPullLog("\nNetwork restored. Retrying docker pull...\n");
            m_dockerAwaitingNetwork = false;
            restartDockerPull();
        } else {
            if (!m_dockerAwaitingNetwork) {
                appendDockerPullLog("\nNetwork unavailable. Waiting to resume...\n");
                setStatus("Network unavailable. Waiting to resume pull...");
            }
            m_dockerAwaitingNetwork = true;
        }
        reply->deleteLater();
    });
}

void AppController::restartDockerPull()
{
    if (!m_dockerProcess || m_dockerProcess->state() == QProcess::NotRunning)
        return;
    m_dockerProcess->terminate();
    if (!m_dockerProcess->waitForFinished(2000))
        m_dockerProcess->kill();
    if (m_dockerProcess) {
        m_dockerProcess->deleteLater();
        m_dockerProcess = nullptr;
    }
    m_dockerLastOutput.restart();
    startDockerPullProcess(false);
}

void AppController::cancelDockerPull()
{
    if (!m_dockerProcess || m_dockerProcess->state() == QProcess::NotRunning)
        return;
    m_dockerPullCanceled = true;
    setStatus("Canceling Docker pull...");
    m_dockerProcess->terminate();
    if (!m_dockerProcess->waitForFinished(2000)) {
        m_dockerProcess->kill();
    }
}

void AppController::runDockerContainer()
{
    const QString image = "rjaat/aibox-prod:latest";
    const QString dockerCmd = "docker run --rm --stop-timeout=1 " + image;

    if (!isatty(STDIN_FILENO)) {
        struct Candidate {
            QString name;
            QStringList args;
        };

        const QStringList bashArgs = {"bash", "-lc", dockerCmd};
        QList<Candidate> candidates;
        auto addCandidate = [&candidates](const QString &name, const QStringList &args) {
            candidates.push_back({name, args});
        };

        QStringList args;
        args = {"--"};
        args += bashArgs;
        addCandidate("gnome-terminal", args);

        args = {"--"};
        args += bashArgs;
        addCandidate("kgx", args);

        args = {"-e"};
        args += bashArgs;
        addCandidate("x-terminal-emulator", args);

        args = {"-e"};
        args += bashArgs;
        addCandidate("konsole", args);

        args = {"-e"};
        args += bashArgs;
        addCandidate("xterm", args);

        bool launched = false;
        for (const Candidate &candidate : candidates) {
            const QString exe = QStandardPaths::findExecutable(candidate.name);
            if (exe.isEmpty())
                continue;
            if (QProcess::startDetached(exe, candidate.args)) {
                launched = true;
                break;
            }
        }

        if (!launched) {
            setStatus("Unable to launch terminal for docker run.");
            QProcess::startDetached("bash", {"-lc", dockerCmd});
        }
        return;
    }

    if (m_runProcess && m_runProcess->state() != QProcess::NotRunning)
        return;

    if (m_runProcess) {
        m_runProcess->deleteLater();
        m_runProcess = nullptr;
    }

    m_runProcess = new QProcess(this);
    m_runProcess->setProcessChannelMode(QProcess::ForwardedChannels);

    connect(m_runProcess, &QProcess::finished, this,
            [this](int exitCode, QProcess::ExitStatus exitStatus) {
                const bool ok = (exitStatus == QProcess::NormalExit && exitCode == 0);
                setStatus(ok ? "Docker run completed." : "Docker run failed.");
                emit dockerRunFinished(ok);
                if (m_quitAfterRun)
                    QCoreApplication::quit();
                m_runProcess->deleteLater();
                m_runProcess = nullptr;
                m_quitAfterRun = false;
            });

    connect(m_runProcess, &QProcess::errorOccurred, this,
            [this](QProcess::ProcessError) {
                setStatus("Docker run failed to start.");
                emit dockerRunFinished(false);
                if (m_runProcess) {
                    m_runProcess->deleteLater();
                    m_runProcess = nullptr;
                }
            });

    setStatus("Running Docker container...");
    m_runProcess->start("docker", {"run", "--rm", "--stop-timeout=1", image});
}

void AppController::runDockerOps()
{
    if (m_dockerOpsRunning)
        return;

    auto readJsonFile = [](const QString &path, QJsonObject &outObj) -> bool {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly))
            return false;
        const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isObject())
            return false;
        outObj = doc.object();
        return true;
    };

    const QString basePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/SafeCore/data";
    QJsonObject registrationObj;
    QJsonObject tenantObj;
    if (!readJsonFile(basePath + "/registration_data.json", registrationObj)) {
        setDockerOpsLog("Missing registration data. Run registration first.\n");
        return;
    }
    if (!readJsonFile(basePath + "/tenant_data.json", tenantObj)) {
        setDockerOpsLog("Missing tenant data. Fetch tenant data first.\n");
        return;
    }

    const QString tenantId = registrationObj.value("tenantId").toString().trimmed();
    const QString macId = registrationObj.value("macId").toString().trimmed();
    if (tenantId.isEmpty() || macId.isEmpty()) {
        setDockerOpsLog("Registration data is incomplete.\n");
        return;
    }

    const QJsonObject tenantData = tenantObj.value("data").toObject();
    QString domain = tenantData.value("domain").toString().trimmed();
    if (!domain.isEmpty())
        domain = domain.toUpper();

    QString safeCoreBoxId;
    const QJsonArray safeCores = tenantData.value("safeCores").toArray();
    const QString targetMac = macId.toLower();
    for (const QJsonValue &value : safeCores) {
        const QJsonObject obj = value.toObject();
        const QString entryMac = obj.value("macId").toString().trimmed().toLower();
        if (!entryMac.isEmpty() && entryMac == targetMac) {
            safeCoreBoxId = obj.value("safeCoreBoxId").toString().trimmed();
            break;
        }
    }
    if (safeCoreBoxId.isEmpty() && !safeCores.isEmpty())
        safeCoreBoxId = safeCores.first().toObject().value("safeCoreBoxId").toString().trimmed();

    if (safeCoreBoxId.isEmpty() || domain.isEmpty()) {
        setDockerOpsLog("Tenant data is missing SafeCoreBoxId or domain.\n");
        return;
    }

    const QString serviceUrl = m_serviceBaseUrl.trimmed();
    QUrl baseUrl(serviceUrl);
    if (!baseUrl.isValid() || baseUrl.scheme().isEmpty() || baseUrl.host().isEmpty()) {
        setDockerOpsLog("Service URL is invalid.\n");
        return;
    }
    const QString tenantEndpoint = AppConstants::TenantEndpoint;
    QString apiUrl;
    if (baseUrl.path().endsWith(tenantEndpoint)) {
        apiUrl = baseUrl.toString();
    } else {
        QString path = baseUrl.path();
        if (path.endsWith('/'))
            path.chop(1);
        baseUrl.setPath(path + tenantEndpoint);
        apiUrl = baseUrl.toString();
    }

    setDockerOpsLog(QString());
    setDockerOpsContainerId(QString());
    setDockerOpsConflict(false);
    setDockerOpsLog("Creating and Starting the new a container...\n");

    const bool simulateRun = qEnvironmentVariableIsSet("SAFECORE_DEV_DOCKER_OPS");
    if (simulateRun) {
        const QString id = QUuid::createUuid().toString(QUuid::WithoutBraces).remove('-').left(12);
        setDockerOpsStarting(true);
        QTimer::singleShot(800, this, [this, id]() {
            setDockerOpsLog("Starting SafeCore container...\nContainer started: " + id + "\nSafeCore container is running.\n");
            setDockerOpsStarting(false);
            setDockerOpsRunning(true);
            setDockerOpsStopping(false);
            setDockerOpsConflict(true);
            setDockerOpsContainerId(id);
            emit dockerOpsFinished(true, "SafeCore container started.");
        });
        return;
    }
    if (m_dockerOpsProcess) {
        if (m_dockerOpsProcess->state() != QProcess::NotRunning) {
            m_dockerOpsProcess->terminate();
            if (!m_dockerOpsProcess->waitForFinished(2000))
                m_dockerOpsProcess->kill();
        }
        m_dockerOpsProcess->deleteLater();
        m_dockerOpsProcess = nullptr;
    }

    setDockerOpsLog(m_dockerOpsLog + "Starting SafeCore container...\n");
    setDockerOpsStarting(true);
    setDockerOpsRunning(false);
    setDockerOpsStopping(false);

    m_dockerOpsProcess = new QProcess(this);
    m_dockerOpsProcess->setProcessChannelMode(QProcess::MergedChannels);

    connect(m_dockerOpsProcess, &QProcess::finished, this,
            [this](int exitCode, QProcess::ExitStatus exitStatus) {
                const QString output = QString::fromUtf8(m_dockerOpsProcess->readAll()).trimmed();
                const bool ok = (exitStatus == QProcess::NormalExit && exitCode == 0);
                setDockerOpsStarting(false);
                if (ok) {
                    const QString id = output.isEmpty() ? QString() : output.section('\n', 0, 0).trimmed();
                    setDockerOpsContainerId(id);
                    const QString shownId = id.isEmpty() ? QStringLiteral("(no id returned)") : id;
                    setDockerOpsLog(m_dockerOpsLog + QString("Container started: %1\nSafeCore container is running.\n").arg(shownId));
                    setDockerOpsRunning(true);
                    setDockerOpsStopping(false);
                    setDockerOpsConflict(true);
                } else {
                    setDockerOpsLog(m_dockerOpsLog + QString("Failed to start SafeCore container.\n%1\n").arg(output));
                    setDockerOpsRunning(false);
                    setDockerOpsStopping(false);
                    setDockerOpsConflict(true);
                }
                emit dockerOpsFinished(ok, ok ? "SafeCore container started." : "Failed to start SafeCore container.");
                m_dockerOpsProcess->deleteLater();
                m_dockerOpsProcess = nullptr;
            });

    connect(m_dockerOpsProcess, &QProcess::errorOccurred, this,
            [this](QProcess::ProcessError) {
                const QString output = QString::fromUtf8(m_dockerOpsProcess->readAll()).trimmed();
                setDockerOpsLog(QString("Failed to start SafeCore container.\n%1\n").arg(output));
                setDockerOpsStarting(false);
                setDockerOpsRunning(false);
                setDockerOpsStopping(false);
                setDockerOpsConflict(true);
                emit dockerOpsFinished(false, "Failed to start SafeCore container.");
                m_dockerOpsProcess->deleteLater();
                m_dockerOpsProcess = nullptr;
            });

    QStringList args;
    args << "run" << "-d"
         << "--name" << AppConstants::ContainerName
         << "--gpus" << "all"
         << "--restart" << "unless-stopped"
         << "-p" << "8080:8080"
         << "-p" << "8001:8001"
         << "-p" << "8002:8002"
         << "-p" << "8011:8011"
         << "-p" << "8012:8012"
         << "-e" << QString("CONFIG_API_URL=%1").arg(apiUrl)
         << "-e" << QString("CONFIG_API_ACCESS_KEY=%1").arg(m_tenantAccessKey)
         << "-e" << QString("SafeCoreBoxId=%1").arg(safeCoreBoxId)
         << "-e" << QString("TENANT_ID=%1").arg(tenantId)
         << "-e" << QString("DOMAIN=%1").arg(domain)
         << AppConstants::DockerImage;

    m_dockerOpsProcess->start("bash", {"-lc",
        QString("docker rm -f %1 >/dev/null 2>&1 || true; docker run -d "
                "--name %1 --gpus all --restart unless-stopped "
                "-p 8080:8080 -p 8001:8001 -p 8002:8002 -p 8011:8011 -p 8012:8012 "
                "-e CONFIG_API_URL=\"" + apiUrl + "\" "
                "-e CONFIG_API_ACCESS_KEY=\"" + m_tenantAccessKey + "\" "
                "-e SafeCoreBoxId=\"" + safeCoreBoxId + "\" "
                "-e TENANT_ID=\"" + tenantId + "\" "
                "-e DOMAIN=\"" + domain + "\" "
                "%2")
            .arg(AppConstants::ContainerName, AppConstants::DockerImage)});
}

void AppController::startDockerOps()
{
    if (m_dockerOpsRunning || !m_dockerOpsConflict)
        return;

    const bool simulateRun = qEnvironmentVariableIsSet("SAFECORE_DEV_DOCKER_OPS");
    if (simulateRun) {
        setDockerOpsLog("Starting SafeCore container...\n");
        QTimer::singleShot(600, this, [this]() {
            setDockerOpsLog("SafeCore container started.\nSafeCore container is running.\n");
            setDockerOpsRunning(true);
            setDockerOpsConflict(true);
            emit dockerOpsFinished(true, "SafeCore container started.");
        });
        return;
    }

    if (m_dockerOpsProcess) {
        if (m_dockerOpsProcess->state() != QProcess::NotRunning) {
            m_dockerOpsProcess->terminate();
            if (!m_dockerOpsProcess->waitForFinished(2000))
                m_dockerOpsProcess->kill();
        }
        m_dockerOpsProcess->deleteLater();
        m_dockerOpsProcess = nullptr;
    }

    setDockerOpsLog("Starting SafeCore container...\n");

    m_dockerOpsProcess = new QProcess(this);
    m_dockerOpsProcess->setProcessChannelMode(QProcess::MergedChannels);

    connect(m_dockerOpsProcess, &QProcess::finished, this,
            [this](int exitCode, QProcess::ExitStatus exitStatus) {
                const QString output = QString::fromUtf8(m_dockerOpsProcess->readAll()).trimmed();
                const bool ok = (exitStatus == QProcess::NormalExit && exitCode == 0);
                if (ok) {
                    const QString id = output.left(12);
                    if (!id.isEmpty())
                        setDockerOpsContainerId(id);
                    setDockerOpsLog(QString("SafeCore container started.\nSafeCore container is running.\n"));
                    setDockerOpsRunning(true);
                    setDockerOpsConflict(true);
                } else {
                    setDockerOpsLog(QString("Failed to start SafeCore container.\n%1\n").arg(output));
                    setDockerOpsRunning(false);
                }
                emit dockerOpsFinished(ok, ok ? "SafeCore container started." : "Failed to start SafeCore container.");
                m_dockerOpsProcess->deleteLater();
                m_dockerOpsProcess = nullptr;
            });

    connect(m_dockerOpsProcess, &QProcess::errorOccurred, this,
            [this](QProcess::ProcessError) {
                const QString output = QString::fromUtf8(m_dockerOpsProcess->readAll()).trimmed();
                setDockerOpsLog(QString("Failed to start SafeCore container.\n%1\n").arg(output));
                setDockerOpsRunning(false);
                emit dockerOpsFinished(false, "Failed to start SafeCore container.");
                m_dockerOpsProcess->deleteLater();
                m_dockerOpsProcess = nullptr;
            });

    m_dockerOpsProcess->start("docker", {"start", AppConstants::ContainerName});
}

void AppController::stopDockerOps()
{
    if (!m_dockerOpsRunning && !m_dockerOpsConflict)
        return;
    stopDockerOpsLogs();
    setDockerOpsStarting(false);
    setDockerOpsStopping(true);

    const bool simulateRun = qEnvironmentVariableIsSet("SAFECORE_DEV_DOCKER_OPS");
    if (simulateRun) {
        setDockerOpsLog("Stopping SafeCore container...\n");
        QTimer::singleShot(600, this, [this]() {
            setDockerOpsLog("SafeCore container stopped and removed.\n");
            setDockerOpsRunning(false);
            setDockerOpsConflict(false);
            setDockerOpsStopping(false);
            setDockerOpsContainerId(QString());
            emit dockerOpsStopped(true, "SafeCore container stopped.");
        });
        return;
    }

    if (m_dockerOpsStopProcess) {
        if (m_dockerOpsStopProcess->state() != QProcess::NotRunning) {
            m_dockerOpsStopProcess->terminate();
            if (!m_dockerOpsStopProcess->waitForFinished(2000))
                m_dockerOpsStopProcess->kill();
        }
        m_dockerOpsStopProcess->deleteLater();
        m_dockerOpsStopProcess = nullptr;
    }

    setDockerOpsLog("Stopping SafeCore container...\n");

    m_dockerOpsStopProcess = new QProcess(this);
    m_dockerOpsStopProcess->setProcessChannelMode(QProcess::MergedChannels);

    connect(m_dockerOpsStopProcess, &QProcess::finished, this,
            [this](int exitCode, QProcess::ExitStatus exitStatus) {
                const QString output = QString::fromUtf8(m_dockerOpsStopProcess->readAll()).trimmed();
                bool ok = (exitStatus == QProcess::NormalExit && exitCode == 0);
                const bool notFound = output.contains("No such container", Qt::CaseInsensitive);
                if (!ok && notFound)
                    ok = true;
                const QString message = ok
                    ? "SafeCore container stopped and removed.\n"
                    : "Failed to stop and remove SafeCore container.\n";
                setDockerOpsLog(message + (ok || output.isEmpty() ? "" : output + "\n"));
                if (ok) {
                    setDockerOpsRunning(false);
                } else {
                    setDockerOpsRunning(true);
                }
                setDockerOpsConflict(ok);
                setDockerOpsStopping(false);
                if (ok)
                    setDockerOpsContainerId(QString());
                emit dockerOpsStopped(ok, message.trimmed());
                m_dockerOpsStopProcess->deleteLater();
                m_dockerOpsStopProcess = nullptr;
            });

    connect(m_dockerOpsStopProcess, &QProcess::errorOccurred, this,
            [this](QProcess::ProcessError) {
                setDockerOpsLog("Failed to stop and remove SafeCore container.\n");
                setDockerOpsRunning(true);
                setDockerOpsConflict(true);
                setDockerOpsStopping(false);
                emit dockerOpsStopped(false, "Failed to stop and remove SafeCore container.");
                m_dockerOpsStopProcess->deleteLater();
                m_dockerOpsStopProcess = nullptr;
            });

    m_dockerOpsStopProcess->start("bash", {"-lc",
        QString("docker stop %1 && docker rm %1").arg(AppConstants::ContainerName)});
}

void AppController::startDockerOpsLogs()
{
    if (m_dockerOpsLogsProcess && m_dockerOpsLogsProcess->state() != QProcess::NotRunning)
        return;

    if (m_dockerOpsLogsProcess) {
        m_dockerOpsLogsProcess->deleteLater();
        m_dockerOpsLogsProcess = nullptr;
    }

    setDockerOpsFollowLog("Streaming container logs...\n");

    m_dockerOpsLogsProcess = new QProcess(this);
    m_dockerOpsLogsProcess->setProcessChannelMode(QProcess::MergedChannels);
    QPointer<QProcess> proc = m_dockerOpsLogsProcess;

    connect(m_dockerOpsLogsProcess, &QProcess::readyRead, this, [this, proc]() {
        if (!proc)
            return;
        setDockerOpsFollowLog(m_dockerOpsFollowLog + QString::fromUtf8(proc->readAll()));
    });

    connect(m_dockerOpsLogsProcess, &QProcess::finished, this,
            [this, proc](int, QProcess::ExitStatus) {
                if (proc)
                    proc->deleteLater();
                if (m_dockerOpsLogsProcess == proc)
                    m_dockerOpsLogsProcess = nullptr;
            });

    connect(m_dockerOpsLogsProcess, &QProcess::errorOccurred, this,
            [this, proc](QProcess::ProcessError) {
                if (!proc)
                    return;
                const QString output = QString::fromUtf8(proc->readAll()).trimmed();
                if (!output.isEmpty())
                    setDockerOpsFollowLog(m_dockerOpsFollowLog + output + "\n");
                proc->deleteLater();
                if (m_dockerOpsLogsProcess == proc)
                    m_dockerOpsLogsProcess = nullptr;
            });

    m_dockerOpsLogsProcess->start("docker", {"logs", "-f", AppConstants::ContainerName});
}

void AppController::stopDockerOpsLogs()
{
    if (!m_dockerOpsLogsProcess)
        return;
    QPointer<QProcess> proc = m_dockerOpsLogsProcess;
    m_dockerOpsLogsProcess = nullptr;
    if (!proc)
        return;
    proc->blockSignals(true);
    if (!proc)
        return;
    proc->disconnect();
    if (!proc)
        return;
    if (proc->state() != QProcess::NotRunning) {
        proc->terminate();
        if (!proc->waitForFinished(1000)) {
            proc->kill();
            proc->waitForFinished(1000);
        }
    }
    if (proc)
        proc->deleteLater();
}

bool AppController::installDockerService(bool startNow)
{
    const QString dockerPath = QStandardPaths::findExecutable("docker");
    if (dockerPath.isEmpty()) {
        setStatus("Docker not found in PATH.");
        return false;
    }

    const QString configRoot = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    if (configRoot.isEmpty()) {
        setStatus("Unable to locate user config directory.");
        return false;
    }

    QDir dir;
    const QString serviceDir = configRoot + "/systemd/user";
    if (!dir.mkpath(serviceDir)) {
        setStatus("Unable to create systemd user directory.");
        return false;
    }

    const QString serviceName = AppConstants::ServiceName;
    const QString servicePath = serviceDir + "/" + serviceName;
    const QString image = AppConstants::DockerImage;
    const QString containerName = AppConstants::ContainerName;

    QFile serviceFile(servicePath);
    if (!serviceFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        setStatus("Unable to write systemd service file.");
        return false;
    }

    QTextStream out(&serviceFile);
    out << "[Unit]\n";
    out << "Description=SafeCore Docker Service\n";
    out << "After=network-online.target\n";
    out << "Wants=network-online.target\n\n";
    out << "[Service]\n";
    out << "Type=simple\n";
    out << "ExecStartPre=-" << dockerPath << " rm -f " << containerName << "\n";
    out << "ExecStart=" << dockerPath << " run --name " << containerName << " " << image << "\n";
    out << "ExecStop=" << dockerPath << " stop " << containerName << "\n";
    out << "Restart=always\n";
    out << "RestartSec=2\n\n";
    out << "[Install]\n";
    out << "WantedBy=default.target\n";
    serviceFile.close();

    auto runSystemctl = [this](const QStringList &args, QString *errorOut) -> bool {
        QProcess proc;
        proc.setProcessChannelMode(QProcess::MergedChannels);
        proc.start("systemctl", QStringList() << "--user" << args);
        if (!proc.waitForStarted(3000)) {
            if (errorOut) *errorOut = "Failed to start systemctl.";
            return false;
        }
        proc.waitForFinished(10000);
        const QString output = QString::fromUtf8(proc.readAll()).trimmed();
        if (proc.exitStatus() != QProcess::NormalExit || proc.exitCode() != 0) {
            if (errorOut) *errorOut = output.isEmpty() ? "systemctl failed." : output;
            return false;
        }
        return true;
    };

    QString error;
    if (!runSystemctl({"daemon-reload"}, &error)) {
        setStatus("Failed to reload systemd: " + error);
        return false;
    }

    QStringList enableArgs = {"enable"};
    if (startNow)
        enableArgs << "--now";
    enableArgs << serviceName;

    if (!runSystemctl(enableArgs, &error)) {
        setStatus("Failed to enable service: " + error);
        return false;
    }

    setStatus(startNow ? "SafeCore service started." : "SafeCore service installed.");
    return true;
}

bool AppController::launchDockerOpsApp(bool autoRun)
{
    const QString appPath = QCoreApplication::applicationFilePath();
    if (appPath.isEmpty()) {
        setStatus("Unable to locate application executable.");
        return false;
    }
    QStringList args{"--docker-ops"};
    if (autoRun)
        args << "--auto-run";
    const bool ok = QProcess::startDetached(appPath, args);
    if (!ok)
        setStatus("Failed to launch Docker operations app.");
    return ok;
}

void AppController::handleSigInt()
{
    if (m_runProcess && m_runProcess->state() != QProcess::NotRunning) {
        const qint64 pid = m_runProcess->processId();
        if (pid > 0)
            ::kill(static_cast<pid_t>(pid), SIGINT);
        m_quitAfterRun = true;
        return;
    }
    QCoreApplication::quit();
}


void AppController::cancel()
{
    if (m_task) {
        m_task->cancel();
        setStatus("Cancelled.");
    }
    setBusy(false);
}

void AppController::copyInstallPath()
{
    if (QClipboard* cb = QGuiApplication::clipboard()) {
        cb->setText(m_installPath);
        setStatus("Install path copied to clipboard.");
    }
}

void AppController::goToStep(int step)
{
    if (m_busy)
        return;

    step = std::clamp(step, 0, 3);
    if (step > 1 && !m_keyValid)
        step = 1;
    setStep(step);
}

void AppController::forceStep(int step)
{
    if (m_busy)
        return;

    step = std::clamp(step, 0, 3);
    setStep(step);
}

void AppController::runStep1_Docker()
{
    delete m_task;
    m_task = new DownloadTask(this);

    connect(m_task, &DownloadTask::progress, this, &AppController::onTaskProgress);
    connect(m_task, &DownloadTask::finished, this, &AppController::onTaskFinished);

    // Example approach:
    // 1) download a tar image (or pull from registry)
    // 2) docker load -i file.tar  OR docker pull

    m_task->setMode(DownloadTask::Mode::DockerImage);
    m_task->setInstallPath(m_installPath);
    m_installPhase = 0;
    m_task->start();
}

void AppController::runStep2_Env()
{
    delete m_task;
    m_task = new DownloadTask(this);

    connect(m_task, &DownloadTask::progress, this, &AppController::onTaskProgress);
    connect(m_task, &DownloadTask::finished, this, &AppController::onTaskFinished);

    m_task->setMode(DownloadTask::Mode::EnvironmentSetup);
    m_task->setInstallPath(m_installPath);
    m_installPhase = 1;
    m_task->start();
}

void AppController::runStep3_Model()
{
    delete m_task;
    m_task = new DownloadTask(this);

    connect(m_task, &DownloadTask::progress, this, &AppController::onTaskProgress);
    connect(m_task, &DownloadTask::finished, this, &AppController::onTaskFinished);

    m_task->setMode(DownloadTask::Mode::AIModel);
    m_task->setInstallPath(m_installPath);
    m_installPhase = 2;
    m_task->start();
}

void AppController::onTaskProgress(double p)
{
    setProgress(p);
}

void AppController::onTaskFinished(bool ok, const QString &msg)
{
    if (!ok) {
        setBusy(false);
        setStatus("Failed: " + msg);
        return;
    }

    if (m_currentStep == 2) {
        if (m_installPhase == 0) {
            setStatus("Installation: Fetching env variables...");
            setProgress(0.0);
            runStep2_Env();
            return;
        }

        if (m_installPhase == 1) {
            setStatus("Installation: Downloading AI models...");
            setProgress(0.0);
            runStep3_Model();
            return;
        }

        if (m_installPhase == 2) {
            setStep(3);
            setProgress(1.0);
            setBusy(false);
            setStatus("Installation finished successfully.");
            return;
        }
    }

    // Completed
    setProgress(1.0);
    setBusy(false);
    setStatus("All steps completed successfully.");
}
