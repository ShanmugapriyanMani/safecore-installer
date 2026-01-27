#include "downloadtask.h"
#include <QDir>
#include <QFile>
#include <QNetworkRequest>
#include <QTimer>

static QString ensureDir(const QString& base, const QString& sub)
{
    QDir d(base);
    d.mkpath(sub);
    return d.filePath(sub);
}

static bool isDemoUrl(const QUrl& url)
{
    return url.host() == "example.com";
}

DownloadTask::DownloadTask(QObject *parent)
    : QObject(parent)
{}

void DownloadTask::cancel()
{
    m_cancelled = true;

    if (m_reply) {
        m_reply->abort();
    }
    if (m_proc) {
        m_proc->kill();
    }
}

void DownloadTask::start()
{
    m_cancelled = false;
    emit progress(0.0);

    // NOTE: Replace these URLs and commands with your real artifacts.
    // You can store these in a config file too.

    if (m_mode == Mode::DockerImage) {
        // Option A: docker pull (no file download)
        // Option B: download image.tar then docker load -i image.tar
        // Here we show download + docker load.

        const QString cacheDir = ensureDir(m_installPath, "cache");
        const QString tarPath  = QDir(cacheDir).filePath("ai_box_image.tar");

        // TODO: replace with your real URL
        const QUrl url("https://example.com/artifacts/ai_box_image.tar");
        if (isDemoUrl(url)) {
            simulateDemoInstall("Docker image installed (demo). Configure real URL in downloadtask.cpp.");
            return;
        }
        stepDownloadFile(url, tarPath);
        return;
    }

    if (m_mode == Mode::EnvironmentSetup) {
        const QString cacheDir = ensureDir(m_installPath, "cache");
        const QString shPath   = QDir(cacheDir).filePath("env_setup.sh");

        // TODO: replace with your real URL
        const QUrl url("https://example.com/artifacts/env_setup.sh");
        if (isDemoUrl(url)) {
            simulateDemoInstall("Environment installed (demo). Configure real URL in downloadtask.cpp.");
            return;
        }
        stepDownloadFile(url, shPath);
        return;
    }

    if (m_mode == Mode::AIModel) {
        const QString modelDir = ensureDir(m_installPath, "models");
        const QString modelPath = QDir(modelDir).filePath("model.bin");

        // TODO: replace with your real URL
        const QUrl url("https://example.com/artifacts/model.bin");
        if (isDemoUrl(url)) {
            simulateDemoInstall("Model downloaded (demo). Configure real URL in downloadtask.cpp.");
            return;
        }
        stepDownloadFile(url, modelPath);
        return;
    }
}

void DownloadTask::simulateDemoInstall(const QString &successMessage)
{
    if (m_cancelled)
        return emitFail("Cancelled.");

    emit progress(0.2);

    QTimer::singleShot(200, this, [this]() {
        if (!m_cancelled)
            emit progress(0.6);
    });

    QTimer::singleShot(500, this, [this, successMessage]() {
        if (m_cancelled)
            return emitFail("Cancelled.");
        emitOk(successMessage);
    });
}

void DownloadTask::stepDownloadFile(const QUrl &url, const QString &outFile)
{
    if (m_cancelled) return emitFail("Cancelled.");

    QNetworkRequest req(url);
    m_reply = m_net.get(req);

    QFile* file = new QFile(outFile);
    if (!file->open(QIODevice::WriteOnly)) {
        m_reply->abort();
        m_reply->deleteLater();
        m_reply = nullptr;
        delete file;
        return emitFail("Cannot write file: " + outFile);
    }

    connect(m_reply, &QNetworkReply::readyRead, this, [this, file]() {
        file->write(m_reply->readAll());
    });

    connect(m_reply, &QNetworkReply::downloadProgress, this, [this](qint64 rec, qint64 total) {
        if (total <= 0) return;
        // Make download phase be 0..0.7
        const double p = 0.7 * (double(rec) / double(total));
        emit progress(p);
    });

    connect(m_reply, &QNetworkReply::finished, this, [this, file, outFile]() {
        file->flush();
        file->close();
        file->deleteLater();

        if (m_cancelled) {
            m_reply->deleteLater();
            m_reply = nullptr;
            return emitFail("Cancelled.");
        }

        if (m_reply->error() != QNetworkReply::NoError) {
            const QString err = m_reply->errorString();
            m_reply->deleteLater();
            m_reply = nullptr;
            return emitFail("Download failed: " + err);
        }

        m_reply->deleteLater();
        m_reply = nullptr;

        emit progress(0.75);

        // After download, do install command depending on mode
        if (m_mode == Mode::DockerImage) {
            // docker load -i <tar>
            stepRunCommand("docker", {"load", "-i", outFile});
            return;
        }

        if (m_mode == Mode::EnvironmentSetup) {
#ifdef LINUX_BUILD
            // chmod +x and run script
            stepRunCommand("chmod", {"+x", outFile});
            // run as: bash env_setup.sh --prefix <installPath>
            // (update your script to accept --prefix)
            stepRunCommand("bash", {outFile, "--prefix", m_installPath}, m_installPath);
#else
            // On Windows you would do different env installation logic
            emitOk("Env setup downloaded (Windows install logic not implemented in sample).");
#endif
            return;
        }

        if (m_mode == Mode::AIModel) {
            emit progress(1.0);
            emitOk("Model downloaded.");
            return;
        }
    });
}

void DownloadTask::stepRunCommand(const QString &program, const QStringList &args, const QString &workDir)
{
    if (m_cancelled) return emitFail("Cancelled.");

    if (m_proc) {
        m_proc->deleteLater();
        m_proc = nullptr;
    }

    m_proc = new QProcess(this);
    if (!workDir.isEmpty())
        m_proc->setWorkingDirectory(workDir);

    connect(m_proc, &QProcess::readyReadStandardOutput, this, [this]() {
        // If you want: parse output and update progress
        // For now we just ignore logs
    });

    connect(m_proc, &QProcess::readyReadStandardError, this, [this]() {
        // ignore or log
    });

    connect(m_proc, qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
            this, [this, program](int exitCode, QProcess::ExitStatus status) {

                if (m_cancelled) return emitFail("Cancelled.");

                if (status != QProcess::NormalExit || exitCode != 0) {
                    return emitFail(program + " failed (exit code " + QString::number(exitCode) + ").");
                }

                // Command success:
                // Make command phase be 0.75..1.0
                emit progress(1.0);

                if (m_mode == Mode::DockerImage) {
                    emitOk("Docker image installed.");
                    return;
                }
                if (m_mode == Mode::EnvironmentSetup) {
                    emitOk("Environment installed.");
                    return;
                }

                emitOk("Done.");
            });

    emit progress(0.85);
    m_proc->start(program, args);
}

void DownloadTask::emitFail(const QString &msg)
{
    emit progress(0.0);
    emit finished(false, msg);
}

void DownloadTask::emitOk(const QString &msg)
{
    emit progress(1.0);
    emit finished(true, msg);
}
