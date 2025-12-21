#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkReply>
#include "rsaverifier.h"

const QString BASE_URL = "http://127.0.0.1:5000";

class NetworkManager : public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager *manager;
    RSAVerifier *verifier;

public:
    NetworkManager(RSAVerifier *verifier, QObject *parent = nullptr);

    void activateKeyRequest(const QString &key);
    void getSignatureRequest();

private:
    bool verifySignature(const QByteArray &replyBytes, QString &signature);
    QString getErrorString(QNetworkReply *reply);

signals:
    void signatureRecieved(const QString &signature);
    void networkRequestFinished();

    void networkReplyErrorOccurred(const QString &error);
};
#endif // NETWORKMANAGER_H
