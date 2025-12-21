#include <QDir>
#include "storagemanager.h"
#include "fileutility.h"
#include "winutility.h"

StorageManager::StorageManager(RSAVerifier *verifier, QObject *parent) :
    QObject(parent),
    verifier(verifier)
{

}

bool StorageManager::hasValidSignature()
{
    if (!QFile::exists(PATH)) {
        return false;
    }

    QByteArray signature = QByteArray::fromBase64(readFileBytes(PATH));
    QByteArray hwidBytes = getDeviceHWID().toUtf8();

    return verifier->verifyData(hwidBytes, signature);
}

void StorageManager::saveSignature(const QString &signature)
{
    QDir dir;
    dir.mkpath(DIR);
    
    QByteArray signatureBytes = signature.toUtf8();
    writeFileBytes(PATH, signatureBytes);
}