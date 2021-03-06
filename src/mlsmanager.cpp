#include "mlsmanager.h"

#include "constants.h"

MlsManager::MlsManager(QObject *parent) :
    QObject(parent)
{
    // start background thread and move objects to it
    m_backgroundThread->start();

    connect(this, &MlsManager::requestInstallPackage, m_manager, &PackageManager::installPackages, Qt::QueuedConnection);
    connect(this, &MlsManager::requestRemovePackage, m_manager, &PackageManager::removePackages, Qt::QueuedConnection);
    connect(this, &MlsManager::requestUpdatePackage, m_manager, &PackageManager::updatePackages, Qt::QueuedConnection);
    connect(this, &MlsManager::requestRefreshCache, m_manager, &PackageManager::refreshCache, Qt::QueuedConnection);
    connect(this, &MlsManager::requestRefreshPackages, m_manager, &PackageManager::refreshPackages, Qt::QueuedConnection);
    connect(this, &MlsManager::requestRefreshRepo, m_manager, &PackageManager::refreshRepo, Qt::QueuedConnection);

    connect(m_manager, &PackageManager::packagesAvailable, m_packagesModel, &PackagesModel::setPackages, Qt::QueuedConnection);

    m_manager->moveToThread(m_backgroundThread);
}

MlsManager::~MlsManager()
{
    m_backgroundThread->quit();
    m_backgroundThread->wait();
}

void MlsManager::initialize()
{
    emit requestRefreshRepo();
}

PackagesModel *MlsManager::packagesModel()
{
    return m_packagesModel;
}

void MlsManager::installPackage(const QString &code)
{
    Package package = m_packagesModel->packageByCode(code);

    if (!package.isValid())
        return;

    m_packagesModel->setLoading(true);
    emit requestInstallPackage(QStringList() << package.packageId(Package::Install));
}

void MlsManager::removePackage(const QString &code)
{
    Package package = m_packagesModel->packageByCode(code);

    if (!package.isValid())
        return;

    m_packagesModel->setLoading(true);
    emit requestRemovePackage(QStringList() << package.packageId(Package::Remove));
}

void MlsManager::updatePackage(const QString &code)
{
    Package package = m_packagesModel->packageByCode(code);

    if (!package.isValid())
        return;

    qDebug() << package.packageId();

    m_packagesModel->setLoading(true);
    emit requestUpdatePackage(QStringList() << package.packageId(Package::Update));
}

void MlsManager::getUpdates()
{

}

void MlsManager::refreshCache()
{

}

void MlsManager::refreshRepo()
{
    m_packagesModel->setLoading(true);
    emit requestRefreshRepo();
}
