#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <QObject>
#include <QStandardPaths>
#include "rsaverifier.h"

const QString DIR = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
const QString PATH = DIR + "/paywall.signature";

class StorageManager : public QObject
{
    Q_OBJECT

private:
    RSAVerifier *verifier;

public:
    StorageManager(RSAVerifier *verifier, QObject *parent = nullptr);

    bool hasValidSignature();
    void saveSignature(const QString &signature);
};
#endif // STORAGEMANAGER_H
