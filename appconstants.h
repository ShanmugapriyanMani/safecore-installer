#pragma once

#include <QObject>
#include <QString>

namespace AppConstants {
inline const QString DefaultRelayUrl = "https://simple-relay.calmwave-053a3279.southcentralus.azurecontainerapps.io/api/send";
inline const QString DockerImage = "rjaat/aibox-prod:latest";
inline const QString ContainerName = "aibox";
inline const QString ServiceName = "aibox.service";
inline const QString TenantEndpoint = "/api/tenant/GetTenantDataById";
}

class AppConstantsProvider : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString dockerImage READ dockerImage CONSTANT)
    Q_PROPERTY(QString containerName READ containerName CONSTANT)
    Q_PROPERTY(QString tenantEndpoint READ tenantEndpoint CONSTANT)

public:
    explicit AppConstantsProvider(QObject *parent = nullptr) : QObject(parent) {}

    QString dockerImage() const { return AppConstants::DockerImage; }
    QString containerName() const { return AppConstants::ContainerName; }
    QString tenantEndpoint() const { return AppConstants::TenantEndpoint; }
};
