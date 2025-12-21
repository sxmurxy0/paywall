#ifndef FILEUTILITY_H
#define FILEUTILITY_H

#include <QByteArray>

QByteArray readFileBytes(const QString &path);
void writeFileBytes(const QString &path, const QByteArray &data);

#endif // FILEUTILITY_H
