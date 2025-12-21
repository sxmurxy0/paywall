#ifndef RSAVERIFIER_H
#define RSAVERIFIER_H

#include <QObject>
#include <openssl/rsa.h>

const QString PUBLIC_KEY = ":public.pem";

class RSAVerifier : public QObject
{
    Q_OBJECT

private:
    RSA *publicKey;

public:
    RSAVerifier(QObject *parent = nullptr);
    ~RSAVerifier();

    bool verifyData(const QByteArray &data, const QByteArray &signature);

private:
    void loadPublicKey(const QString &resource);
};
#endif // RSAVERIFIER_H
