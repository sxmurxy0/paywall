#include <QFile>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include "rsaverifier.h"
#include "fileutility.h"

RSAVerifier::RSAVerifier(QObject *parent) :
    QObject(parent),
    publicKey(nullptr)
{
    OpenSSL_add_all_algorithms();

    loadPublicKey(PUBLIC_KEY);
}

RSAVerifier::~RSAVerifier()
{
    RSA_free(publicKey);
}

bool RSAVerifier::verifyData(const QByteArray &data, const QByteArray &signature)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data.constData(), data.size());
    SHA256_Final(hash, &sha256);
    
    int result = RSA_verify(
        NID_sha256,
        hash,
        SHA256_DIGEST_LENGTH,
        reinterpret_cast<const unsigned char*>(signature.constData()),
        signature.size(),
        publicKey
    );

    return result == 1;
}

void RSAVerifier::loadPublicKey(const QString &resource)
{
    QByteArray data = readFileBytes(resource);

    BIO *bio = BIO_new_mem_buf(data.constData(), data.size());
    publicKey = PEM_read_bio_RSA_PUBKEY(bio, nullptr, nullptr, nullptr);

    BIO_free(bio);
}
