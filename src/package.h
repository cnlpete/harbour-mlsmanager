#ifndef PACKAGE_H
#define PACKAGE_H

#include <QDebug>
#include <QMetaType>
#include <QString>

struct Package
{
    QString code;
    quint64 downloadSize{0};
    bool installed{false};
    QString installedVersion;
    QString latestVersion;
    bool updateAvailable{false};

    enum Action {
        Install,
        Remove,
        Update
    };

    Package() = default;

    bool isValid() const;
    QString packageId(quint8 action = Install) const;

    bool operator==(const Package &other) const;
    bool operator!=(const Package &other) const;
};
Q_DECLARE_METATYPE(Package);

QDebug operator<< (QDebug d, const Package &package);




#endif // PACKAGE_H
