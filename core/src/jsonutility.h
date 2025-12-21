#ifndef JSONUTILITY_H
#define JSONUTILITY_H

#include <QJsonObject>

QJsonObject byteArrayToJsonObject(const QByteArray &bytes);
QByteArray jsonObjectToByteArray(const QJsonObject &json);

#endif // JSONUTILITY_H
