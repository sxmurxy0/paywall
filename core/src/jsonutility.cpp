#include <QJsonDocument>
#include "jsonutility.h"

QJsonObject byteArrayToJsonObject(const QByteArray &bytes)
{
    QJsonDocument document = QJsonDocument::fromJson(bytes);
    if (document.isNull() || !document.isObject()) {
        return QJsonObject();
    }

    return document.object();
}

QByteArray jsonObjectToByteArray(const QJsonObject &json)
{
    QJsonDocument document(json);

    return document.toJson();
}
