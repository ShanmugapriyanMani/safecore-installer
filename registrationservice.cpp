#include "registrationservice.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QTimer>

namespace {
QUrl buildEndpointUrl(const QUrl &baseUrl, const QString &endpoint)
{
    QUrl url(baseUrl);
    QString path = url.path();
    if (!path.endsWith(endpoint)) {
        if (path.endsWith('/'))
            path.chop(1);
        url.setPath(path + endpoint);
    }
    return url;
}
} // namespace

RegistrationService::RegistrationService(QObject *parent)
    : QObject(parent)
{}

void RegistrationService::generateKey(const QString &baseUrl, const QString &accessKey, const QString &macId, const QString &tenantId)
{
    const QString trimmedBase = baseUrl.trimmed();
    const QString trimmedAccessKey = accessKey.trimmed();
    const QString trimmedMac = macId.trimmed();
    const QString trimmedTenant = tenantId.trimmed();

    if (trimmedMac.isEmpty() || trimmedTenant.isEmpty()) {
        emit keyGenerated(false, "MAC ID and Tenant ID are required.", QString(), QString());
        return;
    }

    if (trimmedAccessKey.isEmpty()) {
        emit keyGenerated(false, "Access key is not configured.", QString(), QString());
        return;
    }

    if (trimmedBase.isEmpty()) {
        emit keyGenerated(false, "Service base URL is not configured.", QString(), QString());
        return;
    }

    QUrl baseUrlUrl(trimmedBase);
    if (!baseUrlUrl.isValid()) {
        emit keyGenerated(false, "Service URL is invalid.", QString(), QString());
        return;
    }

    postRegistration(baseUrlUrl, trimmedAccessKey, trimmedMac, trimmedTenant);
}

void RegistrationService::postRegistration(const QUrl &baseUrl, const QString &accessKey, const QString &macId, const QString &tenantId)
{
    const QUrl url = buildEndpointUrl(baseUrl, "/api/registration/generate");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("accept", "*/*");

    QJsonObject payload;
    payload.insert("accessKey", accessKey);
    payload.insert("macId", macId);
    payload.insert("tenantId", tenantId);

    QNetworkReply *reply = m_net.post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));

    // Set a timeout for the request (30 seconds)
    QTimer *timeout = new QTimer(reply);
    timeout->setSingleShot(true);
    timeout->setInterval(30000);
    connect(timeout, &QTimer::timeout, reply, [reply]() {
        reply->abort();
    });
    timeout->start();

    connect(reply, &QNetworkReply::finished, this, [this, reply, timeout]() {
        timeout->stop();
        const QByteArray body = reply->readAll();
        const QNetworkReply::NetworkError err = reply->error();
        const QString errString = reply->errorString();
        const int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply->deleteLater();

        QJsonParseError parseError{};
        const QJsonDocument doc = QJsonDocument::fromJson(body, &parseError);
        const bool isServerError = (httpStatus >= 500)
            || errString.contains("Internal Server Error", Qt::CaseInsensitive);

        if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
            QString fallback;
            if (isServerError) {
                fallback = "Registration Failed: Internal Server Error.";
            } else if (err == QNetworkReply::NoError && httpStatus >= 200 && httpStatus < 300) {
                fallback = "Registration Failed: Invalid response.";
            } else {
                fallback = "Registration Failed: " + errString;
            }
            emit keyGenerated(false, fallback, QString(), QString());
            return;
        }

        const QJsonObject root = doc.object();
        const int statusCode = root.value("statusCode").toInt();
        const QString status = root.value("status").toString();
        const QString message = root.value("message").toString();
        const QJsonObject data = root.value("data").toObject();

        const QString registrationKey = data.value("registrationKey").toString();
        const QString generatedOn = data.value("generatedOn").toString();

        bool ok = statusCode >= 200 && statusCode < 300;
        if (!ok && !status.isEmpty())
            ok = status.compare("Success", Qt::CaseInsensitive) == 0;
        if (ok && registrationKey.isEmpty())
            ok = false;

        QString finalMessage;
        if (ok) {
            finalMessage = message.isEmpty() ? "Registration Succeeded." : message;
        } else if (message == "DEVICE_NOT_AVAILABLE") {
            finalMessage = "Registration Failed: DEVICE_NOT_AVAILABLE";
        } else if (message == "TENANT_NOT_FOUND_OR_INACTIVE") {
            finalMessage = "Registration Failed: TENANT_NOT_FOUND_OR_INACTIVE";
        } else if (statusCode >= 500 || message.contains("Internal Server Error", Qt::CaseInsensitive) || isServerError) {
            finalMessage = "Registration Failed: Internal Server Error.";
        } else {
            finalMessage = message.isEmpty() ? "Registration failed." : message;
        }

        emit keyGenerated(ok,
                          finalMessage,
                          registrationKey,
                          generatedOn);
    });
}
