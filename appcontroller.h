#pragma once
#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QProcess>
#include <QHash>
#include <QSet>
#include <QElapsedTimer>
#include <QTimer>
#include <QStandardPaths>
#include <QPointer>
#include <QStringList>
#include <csignal>
#include <functional>

#include "registrationservice.h"
#include "downloadtask.h"
#include "appconstants.h"

class AppController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString installPath READ installPath WRITE setInstallPath NOTIFY installPathChanged)
    Q_PROPERTY(QString serviceBaseUrl READ serviceBaseUrl WRITE setServiceBaseUrl NOTIFY serviceBaseUrlChanged)
    Q_PROPERTY(QString macId READ macId WRITE setMacId NOTIFY macIdChanged)
    Q_PROPERTY(QString tenantId READ tenantId WRITE setTenantId NOTIFY tenantIdChanged)
    Q_PROPERTY(QString registrationKey READ registrationKey NOTIFY registrationKeyChanged)
    Q_PROPERTY(QString registrationGeneratedOn READ registrationGeneratedOn NOTIFY registrationGeneratedOnChanged)
    Q_PROPERTY(bool registrationAttempted READ registrationAttempted NOTIFY registrationAttemptedChanged)
    Q_PROPERTY(QString registrationError READ registrationError NOTIFY registrationErrorChanged)
    Q_PROPERTY(QString registrationMessage READ registrationMessage NOTIFY registrationMessageChanged)
    Q_PROPERTY(QString relayUrl READ relayUrl WRITE setRelayUrl NOTIFY relayUrlChanged)
    Q_PROPERTY(QString relayError READ relayError NOTIFY relayErrorChanged)
    Q_PROPERTY(bool syncBusy READ syncBusy NOTIFY syncBusyChanged)
    Q_PROPERTY(bool tenantBusy READ tenantBusy NOTIFY tenantBusyChanged)
    Q_PROPERTY(QString tenantMessage READ tenantMessage NOTIFY tenantMessageChanged)
    Q_PROPERTY(bool tenantSuccess READ tenantSuccess NOTIFY tenantSuccessChanged)
    Q_PROPERTY(QString dockerPullLog READ dockerPullLog NOTIFY dockerPullLogChanged)
    Q_PROPERTY(double dockerPullProgress READ dockerPullProgress NOTIFY dockerPullProgressChanged)
    Q_PROPERTY(bool dockerPullActive READ dockerPullActive NOTIFY dockerPullActiveChanged)
    Q_PROPERTY(QString installPrereqsLog READ installPrereqsLog NOTIFY installPrereqsLogChanged)
    Q_PROPERTY(bool installPrereqsRunning READ installPrereqsRunning NOTIFY installPrereqsRunningChanged)
    Q_PROPERTY(bool installPrereqsDone READ installPrereqsDone NOTIFY installPrereqsDoneChanged)
    Q_PROPERTY(QString dockerOpsLog READ dockerOpsLog NOTIFY dockerOpsLogChanged)
    Q_PROPERTY(QString dockerOpsFollowLog READ dockerOpsFollowLog NOTIFY dockerOpsFollowLogChanged)
    Q_PROPERTY(QString dockerImage READ dockerImage CONSTANT)
    Q_PROPERTY(bool dockerOpsRunning READ dockerOpsRunning NOTIFY dockerOpsRunningChanged)
    Q_PROPERTY(bool dockerOpsStarting READ dockerOpsStarting NOTIFY dockerOpsStartingChanged)
    Q_PROPERTY(bool dockerOpsStopping READ dockerOpsStopping NOTIFY dockerOpsStoppingChanged)
    Q_PROPERTY(bool dockerOpsConflict READ dockerOpsConflict NOTIFY dockerOpsConflictChanged)
    Q_PROPERTY(QString dockerOpsContainerId READ dockerOpsContainerId NOTIFY dockerOpsContainerIdChanged)
    Q_PROPERTY(bool dockerOpsAutoRun READ dockerOpsAutoRun WRITE setDockerOpsAutoRun NOTIFY dockerOpsAutoRunChanged)

    Q_PROPERTY(QString upgradeLog READ upgradeLog NOTIFY upgradeLogChanged)
    Q_PROPERTY(bool upgradeRunning READ upgradeRunning NOTIFY upgradeRunningChanged)
    Q_PROPERTY(double upgradeProgress READ upgradeProgress NOTIFY upgradeProgressChanged)

    Q_PROPERTY(int currentStep READ currentStep NOTIFY currentStepChanged)          // 0..3
    Q_PROPERTY(double stepProgress READ stepProgress NOTIFY stepProgressChanged)   // 0..1
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(bool keyValid READ keyValid NOTIFY keyValidChanged)

public:
    explicit AppController(QObject *parent = nullptr);
    ~AppController() override;

    QString installPath() const { return m_installPath; }
    void setInstallPath(const QString& p);

    QString serviceBaseUrl() const { return m_serviceBaseUrl; }
    void setServiceBaseUrl(const QString& url);

    QString macId() const { return m_macId; }
    void setMacId(const QString& macId);

    QString tenantId() const { return m_tenantId; }
    void setTenantId(const QString& tenantId);

    QString registrationKey() const { return m_registrationKey; }
    QString registrationGeneratedOn() const { return m_registrationGeneratedOn; }
    bool registrationAttempted() const { return m_registrationAttempted; }
    QString registrationError() const { return m_registrationError; }
    QString registrationMessage() const { return m_registrationMessage; }
    QString relayUrl() const { return m_relayUrl; }
    QString relayError() const { return m_relayError; }
    bool syncBusy() const { return m_syncBusy; }
    bool tenantBusy() const { return m_tenantBusy; }
    QString tenantMessage() const { return m_tenantMessage; }
    bool tenantSuccess() const { return m_tenantSuccess; }
    QString dockerPullLog() const { return m_dockerPullLog; }
    double dockerPullProgress() const { return m_dockerPullProgress; }
    bool dockerPullActive() const { return m_dockerPullActive; }
    QString installPrereqsLog() const { return m_installPrereqsLog; }
    bool installPrereqsRunning() const { return m_installPrereqsRunning; }
    bool installPrereqsDone() const { return m_installPrereqsDone; }
    QString dockerOpsLog() const { return m_dockerOpsLog; }
    QString dockerOpsFollowLog() const { return m_dockerOpsFollowLog; }
    QString dockerImage() const;
    bool dockerOpsRunning() const { return m_dockerOpsRunning; }
    bool dockerOpsStarting() const { return m_dockerOpsStarting; }
    bool dockerOpsStopping() const { return m_dockerOpsStopping; }
    bool dockerOpsConflict() const { return m_dockerOpsConflict; }
    QString dockerOpsContainerId() const { return m_dockerOpsContainerId; }
    bool dockerOpsAutoRun() const { return m_dockerOpsAutoRun; }
    void setDockerOpsAutoRun(bool value);
    bool setupComplete() const { return m_setupComplete; }
    QString upgradeLog() const { return m_upgradeLog; }
    bool upgradeRunning() const { return m_upgradeRunning; }
    double upgradeProgress() const { return m_upgradeProgress; }

    int currentStep() const { return m_currentStep; }
    double stepProgress() const { return m_stepProgress; }
    QString statusText() const { return m_statusText; }
    bool busy() const { return m_busy; }
    bool keyValid() const { return m_keyValid; }

    Q_INVOKABLE void validateKey();
    Q_INVOKABLE void populateMacId();
    Q_INVOKABLE void syncRelay();
    Q_INVOKABLE void fetchTenantData(const QString& vertical);
    Q_INVOKABLE void pullDockerImage();
    Q_INVOKABLE void cancelDockerPull();
    Q_INVOKABLE void clearDockerPullLog();
    Q_INVOKABLE void startInstallPrereqs();
    Q_INVOKABLE void cancelInstallPrereqs();
    Q_INVOKABLE void clearInstallPrereqsLog();
    void resetLocalState();
    Q_INVOKABLE void runDockerContainer();
    Q_INVOKABLE void runDockerOps();
    Q_INVOKABLE void restartDockerOps();
    Q_INVOKABLE void stopDockerOps();
    Q_INVOKABLE void startDockerOpsLogs();
    Q_INVOKABLE void stopDockerOpsLogs();
    Q_INVOKABLE bool installDockerService(bool startNow);
    Q_INVOKABLE bool launchDockerOpsApp(bool autoRun = false);
    Q_INVOKABLE void handleSigInt();
    Q_INVOKABLE void startInstall();
    Q_INVOKABLE void cancel();
    Q_INVOKABLE void copyInstallPath();
    Q_INVOKABLE void goToStep(int step);
    Q_INVOKABLE void forceStep(int step);
    Q_INVOKABLE void startUpgrade();
    Q_INVOKABLE void cancelUpgrade();

signals:
    void installPathChanged();
    void serviceBaseUrlChanged();
    void macIdChanged();
    void tenantIdChanged();
    void registrationKeyChanged();
    void registrationGeneratedOnChanged();
    void registrationAttemptedChanged();
    void registrationErrorChanged();
    void registrationMessageChanged();
    void relayUrlChanged();
    void relayErrorChanged();
    void registrationResult(bool ok, const QString& registrationKey, const QString& generatedOn, const QString& message);
    void syncBusyChanged();
    void syncResult(bool ok, const QString& message);
    void tenantBusyChanged();
    void tenantMessageChanged();
    void tenantSuccessChanged();
    void dockerPullLogChanged();
    void dockerPullProgressChanged();
    void dockerPullActiveChanged();
    void installPrereqsLogChanged();
    void installPrereqsRunningChanged();
    void installPrereqsDoneChanged();
    void dockerPullStarted();
    void dockerPullFinished(bool ok, const QString& message);
    void dockerRunFinished(bool ok);
    void dockerOpsLogChanged();
    void dockerOpsFollowLogChanged();
    void dockerOpsRunningChanged();
    void dockerOpsStartingChanged();
    void dockerOpsStoppingChanged();
    void dockerOpsConflictChanged();
    void dockerOpsContainerIdChanged();
    void dockerOpsAutoRunChanged();
    void dockerOpsFinished(bool ok, const QString& message);
    void dockerOpsStopped(bool ok, const QString& message);

    void upgradeLogChanged();
    void upgradeRunningChanged();
    void upgradeProgressChanged();
    void upgradeFinished(bool hasUpdate, const QString& message);

    void currentStepChanged();
    void stepProgressChanged();
    void statusTextChanged();

    void busyChanged();
    void keyValidChanged();

private slots:
    void onKeyGenerated(bool ok, const QString& message, const QString& registrationKey, const QString& generatedOn);

    void runStep1_Docker();
    void runStep2_Env();
    void runStep3_Model();

    void onTaskProgress(double p);
    void onTaskFinished(bool ok, const QString& msg);

private:
    void setBusy(bool b);
    void setSyncBusy(bool b);
    void setTenantBusy(bool b);
    void setTenantSuccess(bool b);
    void setDockerPullLog(const QString& log);
    void setDockerPullProgress(double value);
    void setDockerPullActive(bool value);
    void setInstallPrereqsLog(const QString& log);
    void setInstallPrereqsRunning(bool value);
    void setInstallPrereqsDone(bool value);
    void appendDockerPullLog(const QString& chunk);
    void resetDockerPullProgress();
    void updateDockerPullProgressFromChunk(const QString& chunk);
    void appendUpgradeLog(const QString& chunk);
    void resetUpgradeProgress();
    void updateUpgradeProgressFromChunk(const QString& chunk);
    void setUpgradeProgress(double value);
    void startUpgradePull();
    void setDockerOpsLog(const QString& log);
    void setDockerOpsFollowLog(const QString& log);
    void setDockerOpsRunning(bool value);
    void setDockerOpsStarting(bool value);
    void setDockerOpsStopping(bool value);
    void setDockerOpsConflict(bool value);
    void setDockerOpsContainerId(const QString& id);
    void startDockerPullProcess(bool resetStatus);
    void performDockerLogin(std::function<void(bool)> callback);
    void checkDockerPullStall();
    void probeDockerRegistry();
    void restartDockerPull();
    void loadSetupState();
    void persistSetupState();
    void updateSetupComplete();
    void setStatus(const QString& s);
    void setProgress(double p);
    void setStep(int step);
    void setRegistrationError(const QString& message);
    void setRegistrationMessage(const QString& message);
    void setRelayUrl(const QString& relayUrl);
    void setRelayError(const QString& message);
    void setTenantMessage(const QString& message);

private:
    QString m_installPath = AppConstants::DefaultInstallPath;
    QString m_serviceBaseUrl = AppConstants::ServiceBaseUrl;
    QString m_macId;
    QString m_tenantId;
    QString m_registrationKey;
    QString m_registrationGeneratedOn;
    bool m_registrationAttempted = false;
    QString m_registrationError;
    QString m_registrationMessage;
    QString m_relayUrl;
    QString m_relayError;
    bool m_syncBusy = false;
    bool m_tenantBusy = false;
    QString m_tenantMessage;
    bool m_tenantSuccess = false;
    QString m_dockerPullLog;
    QString m_dockerPullRemainder;
    QStringList m_dockerPullLines;
    QHash<QString, int> m_dockerPullLineIndex;
    QHash<QString, int> m_dockerPullLayerState;
    QString m_dockerPullLastLayerId;
    bool m_dockerPullSawStatus = false;
    double m_dockerPullProgress = 0.0;
    bool m_dockerPullActive = false;
    bool m_dockerPullCanceled = false;
    QString m_installPrereqsLog;
    bool m_installPrereqsRunning = false;
    bool m_installPrereqsDone = false;
    bool m_installPrereqsCanceled = false;
    int m_dockerPullGeneration = 0;
    QElapsedTimer m_dockerLastOutput;
    QTimer m_dockerWatchdog;
    bool m_dockerAwaitingNetwork = false;
    bool m_dockerProbeActive = false;
    QString m_dockerOpsLog;
    QString m_dockerOpsFollowLog;
    bool m_dockerOpsRunning = false;
    bool m_dockerOpsStarting = false;
    bool m_dockerOpsStopping = false;
    bool m_dockerOpsConflict = false;
    QString m_dockerOpsContainerId;
    bool m_dockerOpsAutoRun = false;
    QString m_dockerOpsApiUrl;
    QString m_dockerOpsSafeCoreBoxId;
    QString m_dockerOpsTenantId;
    QString m_dockerOpsDomain;
    QString m_tenantAccessKey = AppConstants::TenantAccessKey;

    QString m_upgradeLog;
    QString m_upgradeRemainder;
    QStringList m_upgradeLines;
    QHash<QString, int> m_upgradeLineIndex;
    QHash<QString, int> m_upgradeLayerState;
    QString m_upgradeLastLayerId;
    bool m_upgradeSawStatus = false;
    double m_upgradeProgress = 0.0;
    bool m_upgradeRunning = false;
    bool m_upgradeCanceled = false;
    QProcess* m_upgradeProcess = nullptr;

    int m_currentStep = 0;
    double m_stepProgress = 0.0;
    QString m_statusText = "Ready.";

    bool m_busy = false;
    bool m_keyValid = false;
    int m_installPhase = 0;
    bool m_registrationOk = false;
    bool m_dockerPullOk = false;
    bool m_setupComplete = false;

    RegistrationService m_registration;
    DownloadTask* m_task = nullptr; // current step task
    QNetworkAccessManager m_net;
    QProcess* m_dockerProcess = nullptr;
    QProcess* m_runProcess = nullptr;
    QProcess* m_dockerOpsProcess = nullptr;
    QProcess* m_dockerOpsStopProcess = nullptr;
    QProcess* m_dockerOpsLogsProcess = nullptr;
    QProcess* m_installPrereqsProcess = nullptr;
    bool m_quitAfterRun = false;
};
