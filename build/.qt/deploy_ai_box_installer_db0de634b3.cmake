include("/home/sandy/Documents/v1.0/safecore-installer/build/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/ai_box_installer-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "/home/sandy/Documents/v1.0/safecore-installer/build/Safecore"
    GENERATE_QT_CONF
EXCLUDE_PLUGINS;qtiff)
