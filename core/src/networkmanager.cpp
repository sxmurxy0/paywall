#include <QJsonObject>
#include "networkmanager.h"
#include "jsonutility.h"
#include "winutility.h"

NetworkManager::NetworkManager(RSAVerifier *verifier, QObject *parent) : 
    QObject(parent),
    manager(new QNetworkAccessManager(this)), 
    verifier(verifier)
{

}

void NetworkManager::activateKeyRequest(const QString &key)
{
    QJsonObject payload;
    payload["hwid"] = getDeviceHWID();
    payload["key"] = key;

    QByteArray data = jsonObjectToByteArray(payload).toBase64();

    QNetworkRequest request(QUrl(BASE_URL + "/activate-key"));
    QNetworkReply *reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, [=]() {
        int code = reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute).toInt();
        if (code == 200) {
            QString signature;
            if (!verifySignature(reply->readAll(), signature)) {
                return;
            }

            emit signatureRecieved(signature);
        } else {
            emit networkReplyErrorOccurred(getErrorString(reply));
        }
        
        emit networkRequestFinished();

        reply->deleteLater();
    });
}

void NetworkManager::getSignatureRequest()
{
    QString hwid = getDeviceHWID();

    QNetworkRequest request(QUrl(BASE_URL + "/get-signature/" + hwid));
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, [=]() {
        int code = reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute).toInt();
        if (code == 200) {
            QString signature;
            if (!verifySignature(reply->readAll(), signature)) {
                return;
            }

            emit signatureRecieved(signature);
        } else if (code != 401 && code != 404) {
            emit networkReplyErrorOccurred(getErrorString(reply));
        }

        emit networkRequestFinished();

        reply->deleteLater();
    });
}

bool NetworkManager::verifySignature(const QByteArray &replyBytes, QString &signature)
{
    QByteArray decodedReply = QByteArray::fromBase64(replyBytes);
    QJsonObject replyBody = byteArrayToJsonObject(decodedReply);
    if (!replyBody.contains("signature")) {
        emit networkReplyErrorOccurred("Invalid server reply!");
        return false;
    }

    signature = replyBody["signature"].toString();

    QByteArray hwidBytes = getDeviceHWID().toUtf8();  
    QByteArray signatureBytes = QByteArray::fromBase64(signature.toUtf8());
    
    if (!verifier->verifyData(hwidBytes, signatureBytes)) {
        emit networkReplyErrorOccurred("Security error!");
        return false;
    }

    return true;
}

QString NetworkManager::getErrorString(QNetworkReply *reply)
{
    QString error;

    QByteArray decoded = QByteArray::fromBase64(reply->readAll());
    QJsonObject replyBody = byteArrayToJsonObject(decoded);
    if (replyBody.contains("error")) {
        // error = reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute).toString();
        // error += " " + reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        // error += "\n" + replyBody["error"].toString();
        error = replyBody["error"].toString();
    } else {
        error = reply->errorString();
    }

    return error;
}
