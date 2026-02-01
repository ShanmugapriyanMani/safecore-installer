#pragma once

#include <QObject>
#include <QString>
#include <QStringList>

namespace AppConstants {

// API Configuration
inline const QString ServiceBaseUrl = "https://ss-qa-eus2-api-admin-h8ayfhffdfffa4bp.eastus2-01.azurewebsites.net";
inline const QString DefaultRelayUrl = "https://simple-relay.calmwave-053a3279.southcentralus.azurecontainerapps.io/api/send";
inline const QString TenantEndpoint = "/api/tenant/GetTenantDataById";
inline const QString TenantAccessKey = "7YCGe7FmFyrLTLD6qMDTWcWZx1q9Gxk9";

// Docker Configuration
inline const QString DockerImage = "ssaiboxacr.azurecr.io/aibox-prod:latest";
inline const QString ContainerName = "aibox";
inline const QString DockerRegistryUrl = "https://ssaiboxacr.azurecr.io/v2/";
inline const QString DockerRegistryHost = "ssaiboxacr.azurecr.io";
inline const QString DockerRegistryUser = "pull-ssaibox";
inline const QString DockerRegistryPassword = "A9YZmplEIveTu5krRJYAjzPm47Ak1klOTJZEK1FBTiBIxc8Ikj4ZJQQJ99CAACHYHv6Eqg7NAAABAZCRCcZ1";

// Container Ports
inline const QStringList ContainerPorts = {
    "8080:8080",
    "8085:8085",
    "8090:8090",
    "8008:8008",
    "8009:8009",
    "8010:8010",
    "8011:8011",
    "8012:8012",
    "8013:8013",
    "8111:8111",
    "5432:5432",
    "6379:6379"
};

// Container Volumes
inline const QStringList ContainerVolumes = {
    "/home/aibox/postgres:/var/lib/postgresql",
    "/home/aibox/data:/app/data",
    "/home/aibox/logs:/app/logs",
    "aibox_weapons:/app/downloads/models/weapons"
};

// NVIDIA Driver Capabilities
inline const QString NvidiaDriverCapabilities = "compute,utility,video";

// Service Configuration
inline const QString ServiceName = "aibox.service";
inline const QString DefaultInstallPath = "/opt/ai_box";
inline const QString PrereqScriptPath = "/tmp/Install-Docker-NVIDIA-Container-Toolkit";

// Application Info
inline const QString AppName = "SafeCore";
inline const QString OrgName = "SafeCore";

}

class AppConstantsProvider : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString dockerImage READ dockerImage CONSTANT)
    Q_PROPERTY(QString containerName READ containerName CONSTANT)
    Q_PROPERTY(QString tenantEndpoint READ tenantEndpoint CONSTANT)
    Q_PROPERTY(QString serviceBaseUrl READ serviceBaseUrl CONSTANT)
    Q_PROPERTY(QString defaultRelayUrl READ defaultRelayUrl CONSTANT)
    Q_PROPERTY(QString defaultInstallPath READ defaultInstallPath CONSTANT)
    Q_PROPERTY(QString appName READ appName CONSTANT)

public:
    explicit AppConstantsProvider(QObject *parent = nullptr) : QObject(parent) {}

    QString dockerImage() const { return AppConstants::DockerImage; }
    QString containerName() const { return AppConstants::ContainerName; }
    QString tenantEndpoint() const { return AppConstants::TenantEndpoint; }
    QString serviceBaseUrl() const { return AppConstants::ServiceBaseUrl; }
    QString defaultRelayUrl() const { return AppConstants::DefaultRelayUrl; }
    QString defaultInstallPath() const { return AppConstants::DefaultInstallPath; }
    QString appName() const { return AppConstants::AppName; }
};
