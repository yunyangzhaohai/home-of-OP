from Cryptodome.Cipher import AES
from Cryptodome.Hash import SHA256
from Cryptodome.PublicKey import ECC
from Cryptodome.Random import get_random_bytes
from Cryptodome.Signature import DSS
def generate_ecmh_key_pair():
    key_parameters = ECC.curves.CURVES["secp256r1"]
    private_key = ECC.generate(curve=key_parameters)
    public_key = private_key.public_key()
    return private_key, public_key
def symmetric_encrypt(message, passphrase):
    cipher = AES.new(passphrase, AES.MODE_EAX)
    ciphertext, tag = cipher.encrypt_and_digest(message)
    return cipher.nonce, tag, ciphertext

def symmetric_decrypt(nonce, tag, ciphertext, passphrase):
    cipher = AES.new(passphrase, AES.MODE_EAX, nonce)
    plaintext = cipher.decrypt_and_verify(ciphertext, tag)
    return plaintext
def asymmetric_encrypt(message, public_key):
    ciphertext = public_key.encrypt(message)
    return ciphertext

def asymmetric_decrypt(ciphertext, private_key):
    plaintext = private_key.decrypt(ciphertext)
    return plaintext
def sign_message(message, private_key):
    hash_message = SHA256.new(message)
    signature_scheme = DSS.new(private_key, "fips-186-3")
    signature = signature_scheme.sign(hash_message)
    return signature

def verify_signature(message, signature, public_key):
    hash_message = SHA256.new(message)
    signature_scheme = DSS.new(public_key, "fips-186-3")
    try:
        signature_scheme.verify(hash_message, signature)
        return True
    except ValueError:
        return False
