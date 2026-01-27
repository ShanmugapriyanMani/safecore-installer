#pragma once
#include <QObject>
#include <QString>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DownloadTask : public QObject
{
    Q_OBJECT
public:
    enum class Mode { DockerImage, EnvironmentSetup, AIModel };
    Q_ENUM(Mode)

    explicit DownloadTask(QObject* parent = nullptr);

    void setMode(Mode m) { m_mode = m; }
    void setInstallPath(const QString& p) { m_installPath = p; }

    void start();
    void cancel();

signals:
    void progress(double p);                 // 0..1
    void finished(bool ok, const QString& msg);

private:
    void emitFail(const QString& msg);
    void emitOk(const QString& msg);

    void stepDownloadFile(const QUrl& url, const QString& outFile);
    void stepRunCommand(const QString& program, const QStringList& args, const QString& workDir = QString());
    void simulateDemoInstall(const QString& successMessage);

private:
    Mode m_mode = Mode::DockerImage;
    QString m_installPath;

    bool m_cancelled = false;

    QNetworkAccessManager m_net;
    QNetworkReply* m_reply = nullptr;

    QProcess* m_proc = nullptr;
};
