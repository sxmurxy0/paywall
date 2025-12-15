from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import serialization


PUBLIC_EXPONENT = 65537
KEY_SIZE = 2048

def generate_keys():
    private_key = rsa.generate_private_key(
        public_exponent=PUBLIC_EXPONENT,
        key_size=KEY_SIZE
    )

    return private_key, private_key.public_key()

def write_wb(filename, _bytes):
    with open(filename, 'wb') as file:
        file.write(_bytes)

def save_private_key(key, filename, password):
    if len(password) == 0:
        algorithm = serialization.NoEncryption()
    else:
        algorithm = serialization.BestAvailableEncryption(password.encode())
    
    _bytes = key.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.PKCS8,
        encryption_algorithm=algorithm
    )
    write_wb(filename, _bytes)

def save_public_key(key, filename):
    _bytes = key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )
    write_wb(filename, _bytes)

def generate_and_save_keys(filebase, password):
    private_key, public_key = generate_keys()
    save_private_key(private_key, f'keys/{filebase}_private.pem', password)
    save_public_key(public_key, f'keys/{filebase}_public.pem')

if __name__ == "__main__":
    filebase = input('Enter filebase for key files: ')
    password = input('Enter password for private key: ')

    if len(filebase) == 0:
        print('Error: filebase cannot be empty!')
    else:
        generate_and_save_keys(filebase, password)
        print('Keys successfully generated!')