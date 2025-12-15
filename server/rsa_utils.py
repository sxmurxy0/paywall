from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives import serialization, hashes
import base64

def read_rb(filename):
    with open(filename, 'rb') as file:
        _bytes = file.read()
    
    return _bytes

def load_private_key(filename, password=None):
    _bytes = read_rb(filename)
    
    return serialization.load_pem_private_key(_bytes, password)

def load_public_key(filename):
    _bytes = read_rb(filename)

    return serialization.load_pem_public_key(_bytes)

def sign_data(data, private_key):
    signature = private_key.sign(
        data.encode(),
        padding.PKCS1v15(),
        hashes.SHA256()
    )

    return base64.b64encode(signature).decode()