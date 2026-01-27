#pragma once
#include <QObject>
#include <QNetworkAccessManager>

class RegistrationService : public QObject
{
    Q_OBJECT
public:
    explicit RegistrationService(QObject* parent = nullptr);

    void generateKey(const QString& baseUrl, const QString& accessKey, const QString& macId, const QString& tenantId);

signals:
    void keyGenerated(bool ok, const QString& message, const QString& registrationKey, const QString& generatedOn);

private:
    void postRegistration(const QUrl& baseUrl, const QString& accessKey, const QString& macId, const QString& tenantId);
    QNetworkAccessManager m_net;
};
