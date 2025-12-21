#include <QFile>
#include "fileutility.h"

QByteArray readFileBytes(const QString &path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);

    QByteArray bytes = file.readAll();

    file.close();

    return bytes;
}

void writeFileBytes(const QString &path, const QByteArray &data)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);

    file.write(data);

    file.close();
}
